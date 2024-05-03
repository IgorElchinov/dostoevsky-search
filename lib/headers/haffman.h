#ifndef HAFFMAN_H
#define HAFFMAN_H

typedef struct HaffmanNode {
    unsigned char symbol;
    int count;
    char bit;
    struct HaffmanNode *left;
    struct HaffmanNode *right;
} HaffmanNode;

typedef struct HaffmanCode {
    unsigned char symbol;
    int code;
    char code_length;
} HaffmanCode;

extern void compress(char *filename, char *compressed_filename);
extern void decompress(char *compressed_filename, char *decompressed_filename);

#endif
