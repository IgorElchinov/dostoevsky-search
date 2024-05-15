#include "haffman.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

enum {
    UNIQUE_SYMBOLS = 255,
    LAST_SYMBOL_CODE = 255,
    BITS_IN_BYTE = 8,
    BITS_IN_INTEGER = 31,
    SYMBOLS_TO_WRITE = 2048
};

void
swap_pairs(HaffmanNode *a, HaffmanNode *b) {
    HaffmanNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void
quick_step(int start, int end, HaffmanNode *arr) {
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
quick_sort(HaffmanNode *array) {
    quick_step(0, UNIQUE_SYMBOLS, array);
}

void
build_haffman_table(HaffmanNode *node, unsigned int code, char iteration, HaffmanCode *code_table) {
    HaffmanCode haffman_code, swapper;
    if (iteration == -1) {
        build_haffman_table(node->left, code, iteration + 1, code_table);
        build_haffman_table(node->right, code, iteration + 1, code_table);
        return;
    }
    code += node->bit << (BITS_IN_INTEGER - iteration);
    if (node->symbol != LAST_SYMBOL_CODE) {
        code /= 1 << (BITS_IN_INTEGER - iteration);
        haffman_code.symbol = node->symbol;
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
    build_haffman_table(node->left, code, iteration + 1, code_table);
    build_haffman_table(node->right, code, iteration + 1, code_table);
}

HaffmanCode *
build_haffman_code(unsigned char start, HaffmanNode *arr) {
    int sum = 0, i = 0;
    HaffmanNode new_node, swap_node;
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
        new_node.symbol = LAST_SYMBOL_CODE;
        new_node.count = arr[start].count + arr[start + 1].count;
        new_node.left = &arr[start];
        new_node.right = &arr[start + 1];
        arr[UNIQUE_SYMBOLS - 1] = new_node;
        while (arr[i].count < arr[i - 1].count && i > 0) {
            swap_node = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = swap_node;
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
    build_haffman_table(&new_node, 0, -1, code_table);
    for (int i = 0; i < UNIQUE_SYMBOLS; i++) {
        if (code_table[i].code_length == CHAR_MAX) {
            break;
        }
    }
    return code_table;
}

void
compress(char *filename, char *compressed_filename) {
    FILE *file = fopen(filename, "r");
    FILE *compressed_file = fopen(compressed_filename, "wb");
    unsigned char min_code_length, same_lengths_count = 2;
    uint64_t cur = 0;
    int c;
    HaffmanNode *symbol_count = calloc(UNIQUE_SYMBOLS + 1, sizeof(*symbol_count));
    HaffmanCode *unsorted_code_table = calloc(UNIQUE_SYMBOLS + 1, sizeof(*unsorted_code_table));
    char *compressed_str = calloc(SYMBOLS_TO_WRITE + BITS_IN_BYTE, sizeof(*compressed_str));;
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
                    compressed_str[cur / BITS_IN_BYTE] |= ((sorted_code_table[i - j].code >> k) & 1u) << (cur % BITS_IN_BYTE);
                    cur++;
                }
            }
            min_code_length = sorted_code_table[i].code_length;
            if (cur % BITS_IN_BYTE != 0) {
                for (int k = (BITS_IN_BYTE - 1) - cur % BITS_IN_BYTE; k >= 0; k--) {
                    compressed_str[cur / BITS_IN_BYTE] |= 0 << ((BITS_IN_BYTE - 1) - cur % BITS_IN_BYTE);
                    cur++;
                }
            }
            same_lengths_count = 2;
            for (int j = 0; j < cur / BITS_IN_BYTE; j++) {
                fprintf(compressed_file, "%c", compressed_str[j]);
            }
            for (int j = 0; j < cur / BITS_IN_BYTE; j++) {
                compressed_str[j] = 0;
            }
            compressed_str[same_lengths_count] = sorted_code_table[i].symbol;
            same_lengths_count++;
            if (min_code_length == CHAR_MAX) {
                break;
            }
        }
    }
    for (int j = 0; j <= UNIQUE_SYMBOLS; j++) {
        compressed_str[j] = 0;
    }
    fprintf(compressed_file, "%c", LAST_SYMBOL_CODE);
    cur = 0;
    while ((c = getc(file)) != EOF) {
        for (int i = unsorted_code_table[c].code_length - 1; i >= 0; i--) {
            compressed_str[cur / BITS_IN_BYTE] |= ((unsorted_code_table[c].code >> i) & 1u) << ((BITS_IN_BYTE - 1) - cur % BITS_IN_BYTE);
            cur++;
            if (cur % BITS_IN_BYTE == 0 && compressed_str[cur / BITS_IN_BYTE - 1] == 0) {
                fprintf(compressed_file, "%s", compressed_str);
                fprintf(compressed_file, "%c", 0);
                for (int j = 0; j < cur / BITS_IN_BYTE; j++) {
                    compressed_str[j] = 0;
                }
                cur = 0;
            } else if (cur % (SYMBOLS_TO_WRITE * BITS_IN_BYTE) == 0) {
                fprintf(compressed_file, "%s", compressed_str);
                cur = 0;
                for (int j = 0; j < SYMBOLS_TO_WRITE; j++) {
                    compressed_str[j] = 0;
                }
            }
        }
    }
    if (cur % BITS_IN_BYTE != 0) {
        for (int j = (BITS_IN_BYTE - 1) - cur % BITS_IN_BYTE; j >= 0; j--) {
            compressed_str[cur / BITS_IN_BYTE] |= 0 << (cur % BITS_IN_BYTE);
            cur++;
        }
    }
    fprintf(compressed_file, "%s", compressed_str);
    fclose(file);
    fclose(compressed_file);
    free(compressed_str);
    free(symbol_count);
    free(sorted_code_table);
    free(unsorted_code_table);
}

void
decompress(char *compressed_filename, char *decompressed_filename) {
    FILE *compressed_file = fopen(compressed_filename, "rb");
    FILE *decompressed_file = fopen(decompressed_filename, "wb");
    fseek(compressed_file, 0, SEEK_END);
    int size = ftell(compressed_file) - 1;
    fseek(compressed_file, 0, SEEK_SET);
    char *decompressed_str = calloc(BITS_IN_BYTE + SYMBOLS_TO_WRITE, sizeof(*decompressed_str));
    HaffmanCode *sorted_code_table = calloc(UNIQUE_SYMBOLS, sizeof(*sorted_code_table));
    uint8_t same_length_symbols_count = 0, cur = 0, code_length = 0, unique_symbols = 0, read_symbols = 0, different_lengths = 0;
    uint32_t c, symbol_code = 0, symbols_to_write = 0, index = 0, max_code = 0;
    while (1) {
        same_length_symbols_count = getc(compressed_file);
        size--;
        if (same_length_symbols_count == LAST_SYMBOL_CODE) {
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
                if (cur % BITS_IN_BYTE == 0) {
                    c = getc(compressed_file);
                    size--;
                }
                sorted_code_table[i].code |= ((c >> (cur % 8)) & 1u) << j;
                cur++;
            }
            if (sorted_code_table[i].code > max_code) {
                max_code = sorted_code_table[i].code;
            }
            read_symbols++;
        }
        unique_symbols += read_symbols;
        different_lengths++;
        cur = 0;
        read_symbols = 0;
    }
    char **unsorted_code_table = calloc(UNIQUE_SYMBOLS, sizeof(*unsorted_code_table));
    unsorted_code_table[0] = calloc(UNIQUE_SYMBOLS * (max_code + 1), sizeof(**unsorted_code_table));
    for (int i = 1; i < UNIQUE_SYMBOLS; ++i) {
        unsorted_code_table[i] = unsorted_code_table[i - 1] + (max_code + 1);
    }
    for (int i = 0; i < UNIQUE_SYMBOLS; ++i) {
        unsorted_code_table[sorted_code_table[i].code_length][sorted_code_table[i].code] = sorted_code_table[i].symbol;
    }
    cur = 0;
    for (int i = 0; i < size; i++) {
        c = getc(compressed_file);
        for (int j = BITS_IN_BYTE - 1; j >= 0; j--) {
            symbol_code *= 2;
            symbol_code |= ((c >> j) & 1u) << 0;
            cur++;
            if (unsorted_code_table[cur][symbol_code] != 0) {
                decompressed_str[symbols_to_write] = unsorted_code_table[cur][symbol_code];
                symbols_to_write++;
                cur = 0;
                symbol_code = 0;
            }
        }
        if (symbols_to_write >= SYMBOLS_TO_WRITE) {
            fprintf(decompressed_file, "%s", decompressed_str);
            for (int j = SYMBOLS_TO_WRITE; j <= symbols_to_write; j++) {
                decompressed_str[j] = 0;
            }
            symbols_to_write = 0;
        }
    }
    if (symbols_to_write != 0) {
        for (int j = symbols_to_write; j <= SYMBOLS_TO_WRITE; j++) {
            decompressed_str[j] = 0;
        }
        fprintf(decompressed_file, "%s", decompressed_str);
    }
    fclose(compressed_file);
    fclose(decompressed_file);
    free(*unsorted_code_table);
    free(unsorted_code_table);
    free(decompressed_str);
    free(sorted_code_table);
}