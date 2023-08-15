#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
//credit prof long for struct and constructor
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};
//we dont know how big to make struct so we use an pointer and dynamically allocate for given size aka capacity
//top of stack starts as index val 0
//return the stack

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = malloc(capacity * sizeof(Node));
    }
    return s;
}
//if stack is their then we go through and free the items pointer and stack
//no return

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}
//check if the top of stack index is 0, then its empty
//return true/false

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}
//if stack index is capacity then its full
//return true/false

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }

    return false;
}
//returns number of items in the stack, which is the top value of stack
uint32_t stack_size(Stack *s) {
    return s->top;
}
//if stack is not full, then we add to items the top value of stack which starts off as 0, so items starts by adding value at 0
//increment the top
//return true

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s) == false) {

        s->items[s->top] = n;
        s->top += 1;
        return true;
    }
    return false;
}
//if stack is not empty
//then we go the items array and check top-1, as top is 1 always above items which starts off as 0
//decrement the top
//return true/false

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s) == false) {

        *n = s->items[(s->top - 1)];
        s->top -= 1;

        return true;
    }

    return false;
}
//go through total items in the stack
//print those to the file with bits
//if i+1!top then add an arrow

void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        node_print(s->items[i]);
    }
}
