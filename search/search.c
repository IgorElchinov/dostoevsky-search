#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "haffman.h"
#include "trie.h"
#include "vector.h"

enum {
    MAX_LEN_NAME = 105,
    MAX_LEN_WORD = 85,
    MAX_LEN_REQUEST = 10005,
};

int
main(int argc, char **argv) {  
    // 1. ^ Берём файл с индексом -> его название - argv[1]
    // 2. Разжимаем его
    decompress(argv[1], "index_dec.txt");

    // 3. Проходимся по разжатому файлу, создаём книгу (массив) с именами файлов
    FILE *in = fopen("index_dec.txt", "r");
    int number_of_files;
    fscanf(in, "%d", &number_of_files);
    char **book = calloc(number_of_files, sizeof(*book));
    book[0] = calloc(number_of_files * MAX_LEN_NAME, sizeof(**book));
    for (int i = 1; i < number_of_files; ++i) {
        book[i] = book[i - 1] + MAX_LEN_NAME;
    }
    
    for (int i = 0; i < number_of_files; ++i) {
        for (int j = 0; j < MAX_LEN_NAME; ++j) {
            book[i][j] = '\n';
        }
    }
    for (int i = 0; i < number_of_files; ++i) {
        char cur;
        int lenname = 0;
        while((cur = fgetc(in)) != '\n') {
            book[i][lenname] = cur;
            ++lenname;
        }
    }

    // 4. Создаём словарь, добавляя слова в бор и номера доков, в которых они встречаются
    int number_of_words;
    fscanf(in, "%d", &number_of_words);
    Trie dictionary = t_init();
    for (int i = 0; i < number_of_words; ++i) {
        char *word = calloc(MAX_LEN_WORD, sizeof(*word));
        fscanf(in, "%s", word);
        int number_of_relevant_files;
        fscanf(in, "%d", &number_of_relevant_files);
        for (int j = 0; j < number_of_relevant_files; ++j) {
            int doc;
            fscanf(in, "%d", &doc);
            t_push_back(&dictionary, word, doc);
        }
        free(word);
    }
    fclose(in);

    // 5. Считываем кол-во запросов и по-отдельности каждый запрос
    int n;
    scanf("%d", &n);

    int *alldocs = calloc(number_of_files, sizeof(*alldocs));
    for (int i = 0 ; i < number_of_files; ++i) {
        alldocs[i] = 0;
    }

    for (int o = 0; o < n; ++o) {
        char *request = calloc(MAX_LEN_REQUEST, sizeof(*request));
        fgets(request, MAX_LEN_REQUEST - 1, stdin);
        size_t n = strlen(request) - 1;
        int index = 0;
        char *wordnow = calloc(MAX_LEN_WORD, sizeof(*wordnow));
        int lenwordnow = 0;
        int wordcount = 0;
        while (index != n) {
            if (request[index] == ' ') {
                ++wordcount;
                char *word = calloc(lenwordnow, sizeof(*word));
                for (int i = 0; i < lenwordnow; ++i) {
                    word[i] = wordnow[i];
                }
                // 6. Для каждого слова в запросе идём в бор и смотрим, в каких доках оно встречается
                Vector docs = t_get(&dictionary, word);
                for (int i = 0; i < docs.size; ++i) {
                    ++alldocs[v_get(&docs, i)];
                }
                lenwordnow = 0;
                free(word);
            } else {
                wordnow[lenwordnow] = request[index];
                ++lenwordnow;
            }
            ++index;
        }
        ++wordcount;
        free(wordnow);
        // 7. Находим пересечение
        for (int i = 0; i < number_of_files; ++i) {
            if (alldocs[i] == wordcount) {
                // 8. Выводим ответ на экран
                int j = 0;
                while (book[i][j] != '\n') {
                    printf("%c", book[i][j]);
                }
                printf("\n");
            }
        }
    }

    // 9. Побочная ересь
    t_free(&dictionary);
    free(*book);
    free(book);
    return 0;
}
