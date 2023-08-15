#include <stdio.h>
#include <stdlib.h>
#include "pq.h"
#include "defines.h"
#include "stdbool.h"
#include "node.h"
//min heap ideas from a C++ example with explanation below:
//https://www.techiedelight.com/introduction-priority-queues-using-binary-heaps/
//https://www.techiedelight.com/min-heap-max-heap-implementation-c/
//its C++ with vectors, but helped me with my implentation with nodes queue and for looping it

//struct carries a capacity for how large the queue is, and num for the number of items
struct PriorityQueue {
    Node **nodes;
    uint32_t cap;
    uint32_t num;
};
//this function fixs the heap, defined here
void heap_down(PriorityQueue *q, uint32_t index);

//my queue has a capacity amd number of items starting at 0
//the nodes poiunter has to be alloc as we don't know the size
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->cap = capacity;
        q->num = 0;
        q->nodes = calloc(capacity, capacity * sizeof(Node));
    }
    return q;
}
// the pq_delete checks if their are nodes
//then if they are, free the pointer to those nodes
//free the pointer itself, set stuff to null
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->nodes) { //maybe remove nodes?
        free((*q)->nodes);
        (*q)->nodes = NULL;
        free(*q);
        *q = NULL;
    }
    return;
}

//if num in queue is 0 its empty
bool pq_empty(PriorityQueue *q) {
    if (q->num == 0) {
        return true;
    }
    return false;
}

//of number in the queue is capacity then its full
bool pq_full(PriorityQueue *q) {
    if ((q->num) == (q->cap)) {
        return true;
    }
    return false;
}
//return number in queue
uint32_t pq_size(PriorityQueue *q) {
    return q->num;
}

//store q-> dont wanna accidently change it
bool enqueue(PriorityQueue *q, Node *n) {
    //uint64_t track=0;
    //uint64_t hold;//
    uint64_t hold2 = q->num;
    //Node  *hold3;
    if (q->num == 0) { //if we start at 0, then just add it
        q->nodes[0] = n;
        q->num += 1;
        return true;
    }
    if (!(pq_full(q))) { //if its not full

        q->nodes[hold2] = n; //pass the node to the queue
        q->num += 1; //make queue bigger
        hold2 = q->num;
        for (int x = (hold2 / 2) - 1; x >= 0; x--) { //go through parents nodes and fix
            heap_down(q, x);
        }
        return true;
    } else {
        return false;
    }
}
void heap_down(PriorityQueue *q, uint32_t index) {
    //since were starting at 0, we gotta do 2x+1 and 2x+2
    uint32_t left_kid = (2 * index) + 1;
    uint32_t right_kid = (2 * index) + 2;
    uint32_t small = index;
    Node *hold3;
    //if its 1 in queue return, exit
    if (q->num == 1) {
        return;
    }
    if ((left_kid < q->num) && ((q->nodes[left_kid]->frequency) < (q->nodes[small]->frequency))) {
        small = left_kid;
        //if queue left is less num, dont do this, if left is smaller than smallest then store
    }

    if ((right_kid < q->num) && ((q->nodes[right_kid]->frequency) < (q->nodes[small]->frequency))) {
        //if queue right is less num, dont do this, if right is smaller than smallest then store
        small = right_kid;
    }
    //if the smallest is not index (parent) then swap
    if (small != index) {
        hold3 = q->nodes[index];
        q->nodes[index] = q->nodes[small];
        q->nodes[small] = hold3;
        heap_down(q, small);
    }
}

//take the value at number store it dont wanna mess with it
bool dequeue(PriorityQueue *q, Node **n) {
    //double check heap
    if (!(pq_empty(q))) {
        uint32_t hold2 = q->num;
        *n = q->nodes[0]; //pass back the 0th nodes
        q->nodes[0] = q->nodes[hold2 - 1]; //add the back to front
        q->num -= 1; //make queue smaller
        hold2 = q->num;
        for (int x = ((hold2 / 2) - 1); x >= 0; x--) { //then fix queue
            heap_down(q, x);
        }
        return true;
    }
    return false;
}
//ya i didn't need this

void pq_print(PriorityQueue *q) {
    q->cap = q->cap;
    return;
}
