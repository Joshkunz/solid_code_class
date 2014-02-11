/*
 * Huffman compressor/decompressor
 *
 * Jonathon McDonald, Lynn Gao, Taylor Stapleton
 */
#include "huff_tree.h"
#include "huff_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CHAR_COUNT = 257 // number of characters
#define CHAR_EOF = 256 // represents end of file


int main (int argc, char *argv[]) 
{
    char *file = "";
    struct frequency table[256];

    if (argc != 3) {
        printf("Incorrect amount of arguments supplied.\n");
        return -1;
    }

    switch (parse_args(argv, &file)) {
        case COMPRESS:
            compress(file, table);
            break;
        case DECOMPRESS:
            build_table(file, table);
            decompress(file, table);
            break;
        case TABLE:
            build_table(file, table); 	
            dump_table(file, table);
            break;
        case UNHANDLED:
            break;
    }

    return 0;
}
