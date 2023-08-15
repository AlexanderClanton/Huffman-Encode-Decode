#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include "defines.h"
#include "code.h"
#include <stdbool.h>

static uint8_t buffer_writecode[BLOCK];
uint64_t bytes_written = 0;
uint64_t bytes_read = 0;
static uint32_t idx = 0;
//the read byte checks if we have read the total number of requested bytes
// then check if it errored out
//then check if were done then return
int read_bytes(int infile, uint8_t *buf, int nbytes) { //might need a NULL terminator
    int read_tot = 0;
    int red = 1;
    while ((read_tot != nbytes) || red != 0) { //for eof
        red = read(infile, buf, nbytes - read_tot);

        if (red == -1) {
            printf("ahh!read error");
        }
        bytes_read += red;
        read_tot += red;
        if (red == 0) {
            break;
        }
    }

    return read_tot;
}

//write byte is the same concept, we check to make sure we have not written more than the size requested
//then check if theirs an error
//add up till we reach 0, EOF
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int write_tot = 0;
    int writ = 1;
    while ((write_tot != nbytes) || writ != 0) {
        writ = write(outfile, buf, nbytes - write_tot);

        if (writ == -1) {
            printf("writing error");

            break;
        }
        bytes_written += writ;
        write_tot += writ;
        if (writ == 0) { //no errors no deteched;
            break;
        }
    }
    return write_tot;
}

//checks if were leaving bytes of info
//go throughs extra bytes and prserves the bits to write
void flush_codes(int outfile) {
    int bit_val = idx / 8;
    uint8_t bit_left = (0x1 << (idx % 8)) - 1;
    if (bit_left) {
        buffer_writecode[bit_val++] &= bit_left;
    }
    write_bytes(outfile, buffer_writecode, bit_val);
}

//have a local array, checks if were at 0
//then read in the data
//pass the bit back through using getbit stuff, once again kinda local
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t arraybit[BLOCK] = { 0 };
    int block = 8 * BLOCK;
    static uint32_t once = 0;
    if (idx == 0) {
        //maybe an issue
        once = read_bytes(infile, arraybit, BLOCK) * 8;
    }
    *bit = (arraybit[idx / 8] >> idx % 8) & 0x1;

    idx = (idx + 1) % (block);

    if (idx < once) {
        return true;
    }
    return false;
}

//writes the bytes to bits into the code which is kinda a stack
//gets total size of code, and goes through checking if we can set, then if we can clr

void write_code(int outfile, Code *c) {
    uint32_t block = 8 * BLOCK;
    static uint32_t tot_bit = 0;
    tot_bit = code_size(c);
    for (uint32_t z = 0; z < tot_bit; z++) {

        if (((c->bits[z / 8] >> z % 8) & 0x1)
            == 1) { //have issues with getbit so i do it here, ah well

            buffer_writecode[idx / 8] |= (1 << idx % 8); //this sets the bit
        } else {

            buffer_writecode[idx / 8] &= ~(1 << idx % 8); //this clears the bit
        }

        idx += 1;

        if (idx == (block)) {
            write_bytes(outfile, buffer_writecode, BLOCK);
            idx = 0;
        }
    }
}
