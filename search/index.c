#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "unordered_set.h"
#include "unordered_map.h"
#include "haffman.h"

enum {
    MAX_STRING_SIZE = 100
};

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
getFileSize(const char* file_name) {
    int _file_size = 0;
    FILE* fd = fopen(file_name, "rb");
    if (fd == NULL){
        _file_size = -1;
    } else {
        while(getc(fd) != EOF)
            _file_size++;
        fclose(fd);
    }
    return _file_size;
}

int
main(int argc, char **argv) {
    FILE *out = fopen("cur_index.txt", "w");
    fprintf(out, "%d\n", argc - 1);

    int **name_size = calloc(argc - 1, sizeof(*name_size));
    int *tmp = calloc((argc - 1) * 2, sizeof (*tmp));
    for (int i = 0; i < argc - 1; ++i) {
        name_size[i] = &tmp[i * 2];
    }

    //заполнение массива пар [размер файла][индекс в массиве argv]
    for (int i = 1; i < argc; i++) {
        //получаем размер файла
        int file_size = getFileSize(argv[i]);

        name_size[i - 1][1] = i;
        name_size[i - 1][0] = file_size;
    }

    //сортировка по убыванию размера
    qsort(*name_size, argc - 1, sizeof(*name_size), compare);

    //присваивание файлам с большим размером меньший номер
    for (int i = 0; i < argc - 1; i++) {
        fprintf(out, "%s\n", argv[name_size[i][1]]);
    }

    Trie words_in_files;
    UnorderedSet dictionary;
    us_init(&dictionary);

    //добавление слов в сет, номеров файлов к словам в бор
    for (int i = 0; i < argc - 1; i++) {
        FILE *cur_file = fopen(argv[name_size[i][1]], "r");

        char word[MAX_STRING_SIZE];
        while (fscanf(cur_file, "%s", word) == 1) {
            us_insert(&dictionary, word);
            t_push_back(&words_in_files, word, i);
        }

        fclose(cur_file);
    }

    //поиск по сету (хэш-таблице), вывод слов и файлов, в которых они содержается
    for (int value = 0; value < MAX_HASH_TABLE_SIZE; value++) {
        ListStr *cur = dictionary.arr[value];
        
        while (cur != 0 && cur->next != NULL) {
            Vector *files = t_get_ptr(&words_in_files, cur->data);

            fprintf(out, "%s %zu\n", cur->data, files->size);
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

    //сжатие полученного файла
    compress("cur_index.txt", "index.txt");

    //конец работы (победа!!!)
    us_free(&dictionary);
    t_free(&words_in_files);
    free(name_size);
    free(tmp);
    return 0;
}
