#include <stdio.h>
#include <stdlib.h>
#include "code.h"
#include "defines.h"
#include <stdbool.h>
//credit to prof long for the code struct
//
//sets the top and bits array to 0
Code code_init(void) {
    Code c = { 0, { 0 } };
    return c;
}
//top is the top bit like a stack, returns the value
uint32_t code_size(Code *c) {
    return c->top;
}
//if the returned size is 0
bool code_empty(Code *c) {
    if ((code_size(c)) == 0) {
        return true;
    }
    return false;
}
//if returned size is an alphabet
bool code_full(Code *c) {
    if ((code_size(c)) == ALPHABET) {
        return true;
    }
    return false;
}
//being honest these functions barely work so i kinda redid them the math to double check in io.c
// :/, credit to professor long for the below functions on Bit-vector git
//anyway supposed to find a bit given a byte index and set to 1 that bit only
bool code_set_bit(Code *c, uint32_t i) { //professor code helped
    if ((0 <= i) && (i < ALPHABET)) { //ehhhh? not sure

        c->bits[i / 8] |= ((uint8_t) 0x1 << i % 8);
        return true;
    }
    return false;
}
//being honest these functions barely work so i kinda redid them the math to double check in io.c
// :/, credit to professor long for the below functions on Bit-vector git
//anway it find a bit given a byte index and sets to 0
bool code_clr_bit(Code *c, uint32_t i) {
    if ((0 <= i) && (i < ALPHABET)) {
        c->bits[i / 8] &= ~((uint8_t) 0x1 << i % 8);
        return true;
    }
    return false;
}
//being honest these functions barely work so i kinda redid them the math to double check in io.c
// :/, credit to professor long for the below functions on Bit-vector git
//grabs the bit value, check out io.c for the stuff ;/
bool code_get_bit(Code *c, uint32_t i) {
    if ((0 <= i) && (i < ALPHABET)) {
        c->top += 0;
        return true;
    }

    return false;
}
//checks if we can push, aka full
//then it sees if a bit is 1, then call set_bit, make it 1
//if not 1 then call clr and make it 0
bool code_push_bit(Code *c, uint8_t bit) {
    if ((code_full(c)) == true) {
        return false;
    }
    if (bit) {
        if (code_set_bit(c, c->top++)) {
            //c->top+=1;
            return true;
        }

    } else {
        if (code_clr_bit(c, c->top++)) {
            //c->top+=1;
            return true;
        }
    }

    return false;
}
//checks if its not empty
//then if we can grab a bit, if so then pass the bit back using code of get_bit
bool code_pop_bit(Code *c, uint8_t *bit) {

    if ((code_size(c)) == 0) {

        return false;
    }

    if (code_get_bit(c, --c->top)) {
        uint32_t hold = c->top;
        *bit = ((c->bits[hold / 8] >> hold % 8) & 0x1);
        return true;
    }

    return false;
}
