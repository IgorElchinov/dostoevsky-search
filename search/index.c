#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "unordered_set.h"
#include "unordered_map.h"
#include "Haffman.h"

int compare(const void * x1, const void * x2) {
    int *tmp1 = (int*)x1, *tmp2 = (int*)x2;
    int a = tmp1[0] - tmp2[0], b = tmp1[1] - tmp2[1];
    return a > 0 || (a == 0 && b < 0);
}

int
main(int argc, char **argv) {
    FILE *out = fopen("index.txt", "w");
    fprintf(out, "%d\n", argc - 1);

    int **name_size = calloc(argc - 1, sizeof(*name_size)); //[размер][индекс имениъ
    int *tmp = calloc((argc - 1) * 2, sizeof (*tmp));
    for (int i = 0; i < argc - 1; ++i) {
        name_size[i] = &tmp[i * 2];
    }

    for (int i = 1; i < argc; i++) {
        FILE *cur_file = fopen(argv[i], "r"); //файл с названием argv[i]

        int file_size = 0;
        while(getc(cur_file) != EOF) // количество элементов
            file_size++;
        fclose(cur_file);

        name_size[i - 1][1] = i;
        name_size[i - 1][0] = file_size;
    }

    qsort(name_size, argc - 1, sizeof(*name_size), compare); //сортируем по убыванию размера

    Trie words_in_files = t_init();
    for (int i = 0; i < argc - 1; i++) { //присваивание файлам с большим размером меньший номер
        fprintf(out, "%s\n", argv[name_size[i][1]]);
    }

    UnorderedSet dictionary;
    us_init(&dictionary);
    for (int i = 0; i < argc - 1; i++) {
        FILE *cur_file = fopen(argv[name_size[i][1]], "r");
        char *word;
        while (fscanf(cur_file, "%s", &word) == 1) { //добавление слов в бор и сет
            t_push_back(&words_in_files, word, i);
            us_insert(&dictionary, word);
        }

        fclose(cur_file);
    }
    
    for (int value = 0; value < MAX_HASH_TABLE_SIZE; value++) { //все добавленные в сет (хеш-таблицу) слова
        ListStr *cur = dictionary.arr[value];
        while (cur->next != NULL) {
            Vector *files = t_get_ptr(&words_in_files, cur->data);
            fprintf(out, "%s %llu\n", cur->data, files->size); // вывод слова и количество файлов, в которых оно присутствует
            for (int i = 0; i < files->size; i++) {
                fprintf(out, "%d ", v_get(files, i)); //вывод номера файла
            }
            fprintf(out, "\n");
            
            cur = cur->next;
            
            v_free(files);
        }
        
        ls_free(cur);
    }
    
    us_free(&dictionary);
    t_free(&words_in_files);
    free(name_size);
    free(tmp);
    fclose(out);
    free(name_size);
    return 0;
}
