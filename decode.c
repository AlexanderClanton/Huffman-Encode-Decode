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
#include "stack.h"
#include <inttypes.h>
#define OPTIONS "hvi:o:"
int main(int argc, char **argv) {
    int track = 0;
    int tru_val[4] = { false };
    int opt = 0;
    int fp = 0;
    int outfp = 1;

    uint64_t read_byte1;

    //git man
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

    Node *huff;
    Node *held; //when a root is found, stores it
    Header head;
    uint8_t buffer[BLOCK];
    struct stat holder;

    read_bytes(fp, (uint8_t *) &head, sizeof(Header));
    fstat(fp, &holder);
    fchmod(outfp, head.permissions);
    if (head.magic != MAGIC) {
        printf("error,magic");
        exit(errno);
    }
    uint8_t tree_bytes[head.tree_size]; //using data in header make the tree the size

    //read the dumped tree
    read_bytes(fp, tree_bytes, head.tree_size);
    //rebuild that baby
    held = rebuild_tree(head.tree_size, tree_bytes);
    //swap the tree
    huff = held;
    read_byte1 = 0;
    uint64_t mem_idx = 0; //i had issues not being 0
    uint8_t bit_val;
    while ((read_byte1 < head.file_size) && (read_bit(fp, &bit_val))) {
        //this checks if we have written less than what header described
        if (mem_idx == BLOCK) { //read the bits in above, if we've written to size were done, reset
            write_bytes(outfp, buffer, BLOCK);
            mem_idx = 0;
        }
        if ((bit_val) == 1) { //if we found 1
            huff = huff->right; //set right

        } else { //otherwise its a left
            huff = huff->left;
        }

        if ((huff->left == 0) && (huff->right == 0)) { //if we found the ends
            buffer[mem_idx++] = huff->symbol; //then set the symbol
            read_byte1 += 1; //read a byte then
            huff = held; //swap roots
        }
    }

    write_bytes(outfp, buffer, mem_idx); //maybe not needed test
    float saved = 100 * (1 - (bytes_read / bytes_written));
    if (tru_val[3] == true) {
        fprintf(stderr, "compressed size: %lu bytes\n", bytes_read);
        fprintf(stderr, "uncompressed size: %lu bytes\n", bytes_written);
        fprintf(stderr, "space saved %lf bytes\n", saved);
    }
    close(fp);
    close(outfp);
    delete_tree(&held);
    //delete_tree(&huff); error?
}
