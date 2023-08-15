#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include <stdbool.h>
#include "code.h"
#include "pq.h"
#include "node.h"
#include "stack.h"
#include "io.h"
//makes a tree given a histogram
//then add nodes, then dequeue and join to a parent and left/right kid

Node *build_tree(uint64_t hist[static ALPHABET]) {
    int track = 0;
    PriorityQueue *q = pq_create(ALPHABET); //size of 256 ascii
    //make nodes
    Node *node;
    Node *hold;
    Node *hold2;
    Node *hold3;
    while (track < ALPHABET) { //less than alphabet enqueue
        if (!(hist[track] == 0)) {
            node = node_create(track, hist[track]);
            enqueue(q, node);
        }

        track += 1;
    }
    while (pq_size(q) > 1) { //maybe 0?
        dequeue(q, &hold); //left side
        dequeue(q, &hold2); //right side
        hold3 = node_join(hold, hold2); //join em
        enqueue(q, hold3);
    }
    dequeue(q, &hold3); //were done so dequeue
    pq_delete(&q);
    return hold3;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code c = { 0, { 0 } };
    // Fucking 2 day ERROR RIGHT HERE
    uint8_t bits = 0;
    //if given a correct root
    //otherwise idk man
    if (root) {

        //if kids are 0 given the symbol $
        if (root->left == 0 && root->right == 0) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0); //add 0
            build_codes(root->left, table); //its left go down tree
            code_pop_bit(&c, &bits); //pop the bit
            code_push_bit(&c, 1); //push 1 cuz its right
            build_codes(root->right, table); //call itself so we build it up
            code_pop_bit(&c, &bits); //then were done
        }
    }

    return;
}

//this takes 'L's and 'I's and write the data
void dump_tree(int outfile, Node *root) {
    uint8_t hold = 'L';
    uint8_t hold2 = 'I';

    if (root) { //if were given a good tree
        dump_tree(outfile, root->left); //go down the left
        dump_tree(outfile, root->right); //go down the right
        if (root->right == 0 && root->left == 0) { //write the 'L'
            write_bytes(outfile, &hold, 1);
            write_bytes(outfile, &(root->symbol), 1); //write the symbol $

        } else {
            write_bytes(outfile, &hold2, 1); //otherwise its an interior node
        }
    }

    return;
}

//hold2 and hold are left and right values
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    Stack *stack = stack_create(ALPHABET); //use a stack
    Node *node;
    Node *hold;
    Node *hold2;
    for (uint16_t x = 0; x < nbytes; x++) { //write till number of bytes
        if (tree_dump[x] == 'L') { //if were at a good node, then write the value
            x += 1;
            node = node_create(tree_dump[x], 0);
            stack_push(stack, node);

        } else { //if were a bad interiro then pop both left and right, push the joined node
            stack_pop(stack, &hold);
            stack_pop(stack, &hold2);
            stack_push(stack, node_join(hold2, hold));
        }
    }
    stack_pop(stack, &node); //were done
    stack_delete(&stack);
    return node;
}

void delete_tree(Node **root) { //if root exists then null and delete
    if (*root) {
        delete_tree(&(*root)->right);
        delete_tree(&(*root)->left);
        node_delete(root);
        *root = NULL;
    }

    return;
}
