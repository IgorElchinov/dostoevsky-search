#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "unordered_set.h"
#include "unordered_map.h"
#include "haffman.h"

int
compare(const void *x1, const void * x2) {
    const int *tmp1 = x1, *tmp2 = x2;
    const int a = tmp1[0] - tmp2[0], b = tmp1[1] - tmp2[1];
    if (a > 0 || (a == 0 && b < 0)) {
        return -1;
    } else if (a == 0 && b == 0) {
        return 0;
    } else {
        return 1;
    }
}

int
main(int argc, char **argv) {
    FILE *out = fopen("cur_index.txt", "w");
    fprintf(out, "%d\n", argc - 1);

    int **name_size = calloc(argc - 1, sizeof(*name_size)); //[размер][имя]
    int *tmp = calloc((argc - 1) * 2, sizeof (*tmp));
    for (int i = 0; i < argc - 1; ++i) {
        name_size[i] = &tmp[i * 2];
    }

    for (int i = 1; i < argc; i++) {
        FILE *cur_file = fopen(argv[i], "r"); //файл с названием argv[i]

        int file_size = 0;
        char c;
        while ((c = getc(cur_file)) != EOF)// количество элементов
            file_size++;
        fclose(cur_file);

        name_size[i - 1][1] = i;
        name_size[i - 1][0] = file_size;
    }

    qsort(*name_size, argc - 1, sizeof(*name_size), compare); //сортируем по убыванию размера

    for (int i = 0; i < argc - 1; i++) { //присваивание файлам с большим размером меньший номер
        fprintf(out, "%s %d\n", argv[name_size[i][1]], i);
    }

    Trie words_in_files;
    UnorderedSet dictionary;
    us_init(&dictionary);
    for (int i = 0; i < argc - 1; i++) {
        FILE *cur_file = fopen(argv[name_size[i][1]], "r");
        char word[100];
        while (fscanf(cur_file, "%s", word) == 1) { //добавление слов в бор и сет
            t_push_back(&words_in_files, word, i);
            us_insert(&dictionary, word);
        }
        fclose(cur_file);
    }
    for (int value = 0; value < MAX_HASH_TABLE_SIZE; value++) {
        ListStr *cur = dictionary.arr[value];
        while (cur != 0 && cur->next != NULL) {
            Vector *files = t_get_ptr(&words_in_files, cur->data);
            fprintf(out, "%s %llu\n", cur->data, files->size);
            for (int i = 0; i < files->size; i++) {
                fprintf(out, "%d ", v_get(files, i));
            }
            fprintf(out, "\n");
            cur = cur->next;
            v_free(files);
        }
        ls_free(cur);
    }
    fclose(out);
    compress("cur_index.txt", "index.txt");
    us_free(&dictionary);
    t_free(&words_in_files);
    free(name_size);
    free(tmp);
}
