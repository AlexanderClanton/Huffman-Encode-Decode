#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"
#include "defines.h"
#include "io.h"
#include "huffman.h"
#include "code.h"
#include <fcntl.h> //elmer for this ideas
#include <sys/errno.h>
#include <unistd.h>
#include <sys/stat.h>
#define OPTIONS "hvi:o:"

int main(int argc, char **argv) {
    int track = 0;
    int tru_val[4] = { false };
    int fp = 0;
    int outfp = 1;
    //git man
    //some encode error
    int std = false;
    int opt = 0;
    int temp_stuff;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\n"); //help prompt
            printf("   A Huffman encoder.\nCompresses a file using the Huffman coding algorithm\n");
            printf("USAGE\n   ./encode [-h] [-i infile]   [-o outfile]\n\n");
            printf("OPTIONS\n\n");
            printf("  -h             Program usage and help.\n");
            printf("  -v             Print compression statistics.\n");
            printf("  -i infile      Input file to compress.\n");
            printf("  -o outfile     Output of compressed data.\n");

            return 0;

        case 'i':
            tru_val[1] = true;
            ++track;
            //Elmer provided this input for file
            fp = open(optarg, O_RDONLY);
            if (fp < 0) {
                perror("cat");
                exit(errno);
            }
            std = lseek(fp, 0, SEEK_SET);
            break;
        case 'o':
            tru_val[2] = true;
            ++track; //elmer discord
            outfp = open(optarg, O_WRONLY | O_CREAT);
            if (outfp < 0) {
                perror("cat1");
                exit(errno);
            }
            break;
        case 'v': tru_val[3] = true; break;
        }
    }

    //the file stuff is kinda mine from prev assignemnts
    struct stat filez;
    uint8_t buffer[BLOCK];

    Code c[ALPHABET] = { 0 };
    Header head;
    uint64_t histo[ALPHABET] = { 0 };
    uint16_t symbol = 0;
    int val;
    histo[0] += 1; //this is cuz apprantly the default tree has this included so i added them in?
    histo[255] += 1;
    //make sure its at 0
    lseek(fp, 0, SEEK_SET);
    if (std == -1) { //means no given file
        std = 0;
        std = open("/tmp/temp", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        while ((temp_stuff = read_bytes(fp, buffer, BLOCK)) > 0) {
            write_bytes(std, buffer, temp_stuff);
        }
        fp = std;
    }
    Node *huff;
    fstat(fp, &filez); //gets the stats for the file
    fchmod(outfp, filez.st_mode);

    while ((val = read_bytes(fp, buffer, BLOCK)) > 0) {

        for (int z = 0; z < val; z++) { //reads the file and makes histogram
            histo[buffer[z]]++;
        }
    }

    //number of symbols found
    for (int z = 0; z < ALPHABET; z++) {

        if (histo[z] > 0) {
            symbol++;
        }
    }

    huff = build_tree(histo);
    build_codes(huff, c);
    //header
    head.magic = MAGIC;
    head.permissions = filez.st_mode;
    head.tree_size = (3 * symbol) - 1;
    head.file_size = filez.st_size;
    write_bytes(outfp, (uint8_t *) &head, sizeof(Header)); //write the header
    dump_tree(outfp, huff); //dump tree to header
    //make sure i'm at the beginning, had errors earlier
    lseek(fp, 0, SEEK_SET);
    while ((val = read_bytes(fp, buffer, BLOCK))
           > 0) { // read until the buffer is nothing, then go through sized data and write to fil
        //this takes the bytes in buffer aka values from tree dump and write
        for (int z = 0; z < val; z++) {
            write_code(outfp, &c[buffer[z]]);
        }
    }
    flush_codes(outfp);
    //flush_codes(outfp);
    //flush_codes(outfp);
    float saved = 100 * (1 - (bytes_written / filez.st_size));
    if (tru_val[3] == true) {
        fprintf(stderr, "compressed size: %lu bytes\n", bytes_written);
        fprintf(stderr, "uncompressed size: %lu bytes\n", filez.st_size);
        fprintf(stderr, "space saved %lf bytes\n", saved);
    }

    close(fp);
    close(outfp);
    delete_tree(&huff);
}
