#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <stdbool.h>
//credit to prof long for the struct Node
//creates node ADT, as its size is unkown
//set the values in nodes pointers to passed values of freq and symbol
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) { //
        n->frequency = frequency;
        n->symbol = symbol;
        n->left = 0; //
        n->right = 0;
    }
    return n;
}

//check if node exists then we free it
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

//call node create and pass the '$' for our interal symbol
//then the right and left get each other
//then return a node
Node *node_join(Node *left, Node *right) { //thanks TA Ben

    Node *n = node_create('$', left->frequency + right->frequency);
    n->right = right;

    n->left = left;
    return n;
}
//prints it out
//symbol and right/left
void node_print(Node *n) {
    if (n) {
        printf("%d", n->symbol);
        node_print(n->left);
        node_print(n->right);
    }
}
