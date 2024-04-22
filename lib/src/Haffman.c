//#include "Haffman.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

enum {
    BUFFER_SIZE = 32768,
    MAX_FILE_SIZE = 2000000,
    UNIQUE_SYMBOLS = 255
};

typedef struct Pair {
    unsigned char symbol;
    int count;
    char bit;
    struct Pair *left;
    struct Pair *right;
} Pair;

typedef struct HaffmanCode {
    unsigned char symbol;
    int code;
    char code_length;
} HaffmanCode;

void
swap_pairs(Pair *a, Pair *b) {
    Pair tmp = *a;
    *a = *b;
    *b = tmp;
}

void
quick_step(int start, int end, Pair *arr) {
    int i = start, j = end;
    int pivot = arr[(start + end) / 2].count;
    while (i <= j) {
        while (arr[i].count < pivot) {
            i++;
        }
        while (arr[j].count > pivot) {
            j--;
        }
        if (i <= j) {
            swap_pairs(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (start < j)
        quick_step(start, j, arr);
    if (i < end)
        quick_step(i, end, arr);
}

void
quick_sort(Pair *array) {
    quick_step(0, UNIQUE_SYMBOLS, array);
}

void
build_haffman_table(Pair *pair, unsigned int code, char iteration, HaffmanCode *code_table) {
    HaffmanCode haffman_code, swapper;
    if (iteration == -1) {
        build_haffman_table(pair->left, code, iteration + 1, code_table);
        build_haffman_table(pair->right, code, iteration + 1, code_table);
        return;
    }
    code += pair->bit << (31 - iteration);
    if (pair->symbol != 255) {
        code /= 1 << (31 - iteration);
        haffman_code.symbol = pair->symbol;
        haffman_code.code = code;
        haffman_code.code_length = iteration + 1;
        code_table[UNIQUE_SYMBOLS - 1] = haffman_code;
        for (int i = UNIQUE_SYMBOLS - 1; i >= 1; i--) {
            if (code_table[i].code_length < code_table[i - 1].code_length) {
                swapper = code_table[i];
                code_table[i] = code_table[i - 1];
                code_table[i - 1] = swapper;
            } else {
                break;
            }
        }
        return;
    }
    build_haffman_table(pair->left, code, iteration + 1, code_table);
    build_haffman_table(pair->right, code, iteration + 1, code_table);
}

HaffmanCode *
build_haffman_code(unsigned char start, Pair *arr) {
    int new, sum, i = 0;
    Pair new_pair, swap_pair;
    HaffmanCode *code_table = calloc(UNIQUE_SYMBOLS + 1, sizeof(*code_table));
    HaffmanCode filler;
    filler.code_length = CHAR_MAX;
    for (int i = 0; i <= UNIQUE_SYMBOLS; i++) {
        code_table[i] = filler;
    }
    while (arr[i].count != INT_MAX) {
        sum += arr[i].count;
        i++;
    }
    i = UNIQUE_SYMBOLS - 1;
    while (arr[start].count != sum && start < UNIQUE_SYMBOLS) {
        arr[start].bit = 0;
        arr[start + 1].bit = 1;
        new_pair.symbol = 255;
        new_pair.count = arr[start].count + arr[start + 1].count;
        new_pair.left = &arr[start];
        new_pair.right = &arr[start + 1];
        arr[UNIQUE_SYMBOLS - 1] = new_pair;
        while (arr[i].count < arr[i - 1].count && i > 0) {
            swap_pair = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = swap_pair;
            i--;
        }
        start += 2;
        if (arr[start].count == INT_MAX) {
            start--;
            break;
        } else if (arr[start + 1].count == INT_MAX) {
            break;
        }
        i = UNIQUE_SYMBOLS - 1;
    }
    build_haffman_table(&new_pair, 0, -1, code_table);
    for (int i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (code_table[i].code_length == CHAR_MAX) {
            break;
        }
    }
    return code_table;
}

char *
compress(char *filename, char *compressed_filename, char *decompressed_file) {
    FILE *file = fopen(filename, "r");
    FILE *compressed_file = fopen(compressed_filename, "wb");
    unsigned char k, min_code_length, max_code_length, same_lengths_count = 2, flag = 0;
    uint64_t cur = 0;
    char c, iter = 0;
    int sum = 0;
    Pair *symbol_count = calloc(UNIQUE_SYMBOLS + 1, sizeof(*symbol_count));
    HaffmanCode *unsorted_code_table = calloc(UNIQUE_SYMBOLS + 1, sizeof(*unsorted_code_table));
    char *compressed_str = calloc(MAX_FILE_SIZE, sizeof(*compressed_str));;
    for (unsigned char i = 0; i < UNIQUE_SYMBOLS; i++) {
        symbol_count[i].count = 0;
        symbol_count[i].symbol = i;
    }
    while ((c = getc(file)) != EOF) {
        symbol_count[c].count++;
    }
    for (unsigned char i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (symbol_count[i].count == 0) {
            symbol_count[i].count = INT_MAX;
        }
    }
    symbol_count[UNIQUE_SYMBOLS].count = INT_MAX;
    quick_sort(symbol_count);
    HaffmanCode *sorted_code_table = build_haffman_code(0, symbol_count);
    min_code_length = sorted_code_table[0].code_length;
    for (int i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (sorted_code_table[i].code_length == CHAR_MAX) {
            break;
        }
        unsorted_code_table[sorted_code_table[i].symbol] = sorted_code_table[i];
    }
    for (int i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (sorted_code_table[i].code_length == CHAR_MAX) {
            break;
        }
    }
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (sorted_code_table[i].code_length == min_code_length) {
            compressed_str[same_lengths_count] = sorted_code_table[i].symbol;
            same_lengths_count++;
        } else {
            cur = (same_lengths_count) * 8;
            compressed_str[0] = same_lengths_count - 2;
            compressed_str[1] = min_code_length;
            for (int j = same_lengths_count - 2; j > 0; j--) {
                for (int k = min_code_length - 1; k >= 0; k--) {
                    compressed_str[cur / 8] |= ((sorted_code_table[i - j].code >> k) & 1u) << (cur % 8);
                    cur++;
                }
            }
            min_code_length = sorted_code_table[i].code_length;
            if (cur % 8 != 0) {
                for (int k = 7 - cur % 8; k >= 0; k--) {
                    compressed_str[cur / 8] |= 0 << (7 - cur % 8);
                    cur++;
                }
            }
            same_lengths_count = 2;
            for (int j = 0; j < cur / 8; j++) {
                fprintf(compressed_file, "%c", compressed_str[j]);
            }
            for (int j = 0; j < cur / 8; j++) {
                compressed_str[j] = 0;
            }
            compressed_str[same_lengths_count] = sorted_code_table[i].symbol;
            same_lengths_count++;
            if (min_code_length == CHAR_MAX) {
                break;
            }
        }
    }
    for (int j = 0; j < 256; j++) {
        compressed_str[j] = 0;
    }
    fprintf(compressed_file, "%c", 255);
    cur = 0;
    while ((c = getc(file)) != EOF) {
        for (int i = unsorted_code_table[c].code_length - 1; i >= 0; i--) {
            compressed_str[cur / 8] |= ((unsorted_code_table[c].code >> i) & 1u) << (7 - cur % 8);
            cur++;
            if (cur % 8 == 0 && compressed_str[cur / 8 - 1] == 0) {
                fprintf(compressed_file, "%s", compressed_str);
                fprintf(compressed_file, "%c", 0);
                for (int j = 0; j < cur / 8; j++) {
                    compressed_str[j] = 0;
                }
                sum += cur / 8;
                cur = 0;
            } else if (cur % 2048 == 0) {
                fprintf(compressed_file, "%s", compressed_str);
                sum += 256;
                cur = 0;
                for (int j = 0; j < 256; j++) {
                    compressed_str[j] = 0;
                }
            }
        }
    }
    if (cur % 8 != 0) {
        for (int j = 7 - cur % 8; j >= 0; j--) {
            compressed_str[cur / 8] |= 0 << (cur % 8);
            cur++;
        }
        sum += cur / 8;
    }
    fprintf(compressed_file, "%s", compressed_str);
    fclose(file);
    fclose(compressed_file);
    free(compressed_str);
    free(symbol_count);
    free(sorted_code_table);
    free(unsorted_code_table);
}

char *
add(char *str1, char *str2) {
    char *tmp = calloc(100, sizeof(*tmp));
    for (int i = 0; i < strlen(str1); i++) {
        tmp[i] = str1[i];
    }
    for (int j = 0; j < strlen(str2); j++) {
        tmp[j + strlen(str1)] = str2[j];
    }
    return tmp;
}

void
decompress(char *compressed_filename, char *decompressed_filename) {
    FILE *compressed_file = fopen("compressed_war_and_peace2.txt", "rb");
    FILE *decompressed_file = fopen(decompressed_filename, "wb");
    fseek(compressed_file, 0, SEEK_END);
    int size = ftell(compressed_file) - 1;
    fseek(compressed_file, 0, SEEK_SET);
    char *decompressed_str = calloc(MAX_FILE_SIZE, sizeof(*decompressed_str));
    HaffmanCode *sorted_code_table = calloc(UNIQUE_SYMBOLS, sizeof(*sorted_code_table)); 
    uint8_t c, same_length_symbols_count = 0, cur = 0, code_length = 0, unique_symbols = 0, read_symbols = 0;
    uint32_t symbol_code = 0, symbols_to_write = 0, index = 0;
    while (1) {
        same_length_symbols_count = getc(compressed_file);
        size--;
        if (same_length_symbols_count == 255) {
            break;
        }
        code_length = getc(compressed_file);
        size--;
        for (int i = unique_symbols; i < unique_symbols + same_length_symbols_count; i++) {
            c = getc(compressed_file);
            size--;
            sorted_code_table[i].symbol = c;
        }
        for (int i = unique_symbols; i < unique_symbols + same_length_symbols_count; i++) {
            sorted_code_table[i].code_length = code_length;
            for (char j = code_length - 1; j >= 0; j--) {
                if (cur % 8 == 0) {
                    c = getc(compressed_file);
                    size--;
                }
                sorted_code_table[i].code |= ((c >> (cur % 8)) & 1u) << j;
                cur++;
            }
            read_symbols++;
        }
        unique_symbols += read_symbols;
        cur = 0;
        read_symbols = 0;
    }
    cur = 0;
    for (int i = 0; i < size; i++) {
        c = getc(compressed_file);
        for (int j = 7; j >= 0; j--) {
            symbol_code *= 2;
            symbol_code |= ((c >> j) & 1u) << 0;
            cur++;
            while (sorted_code_table[index].code_length == cur) {
                if (symbol_code == sorted_code_table[index].code) {
                    decompressed_str[symbols_to_write] = sorted_code_table[index].symbol;
                    symbols_to_write++;
                    cur = 0;
                    symbol_code = 0;
                    index = 0;
                    break;
                }
                index++;
            }
        }
        if (symbols_to_write >= 2048) {
            fprintf(decompressed_file, "%s", decompressed_str);
            for (int j = 2048; j <= symbols_to_write; j++) {
                decompressed_str[j] = 0;
            }
            symbols_to_write = 0;
        }
    }
    if (symbols_to_write != 0) {
        for (int j = symbols_to_write; j <= 2048; j++) {
            decompressed_str[j] = 0;
        }
    fprintf(decompressed_file, "%s", decompressed_str);
    fclose(compressed_file);
    fclose(decompressed_file);
    free(decompressed_str);
    }
}

int
main() {
    // for (int i = 1; i <= 154; i++) {
    //     char *tmp = int_to_str(i);
    //     char *tmp2 = add("sonnets/c", tmp);
    //     tmp = add("sonnets/", tmp);
    //     //printf("%s ", tmp2);
    //     compress(tmp, tmp2, "decompressed_war_and_peace.txt");
    //     free(tmp);
    //     free(tmp2);
    // }
    compress("war_and_peace.txt", "compressed_war_and_peace2.txt", "decompressed_war_and_peace.txt");
    decompress("compressed_war_and_peace2.txt", "decompressed_war_and_peace.txt");
}