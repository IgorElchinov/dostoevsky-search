#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "haffman.h"
// #include "trie.h"
// #include "vector.h"

typedef struct Vector {
    int *data;
    size_t size;
    size_t capacity;
} Vector;



Vector
v_init(size_t size) {
    Vector vector;
    vector.size = size;
    if (size == 0) {
        size = 1;
    }
    vector.data = calloc(size, sizeof(*vector.data));
    if (vector.data == NULL) {
        fprintf(stderr, "%s: memory allocation error\n", __func__);
        fflush(stderr);
        exit(1);
    }
    vector.capacity = size;
    return vector;
}

void
v_free(Vector *vector) {
    if (vector->capacity != 0) {
        free(vector->data);
        vector->size = 0;
        vector->capacity = 0;
    }
    return;
}

void
v_check_pos(Vector *vector, size_t pos, const char func_name[]) {
    if (pos < 0 || pos >= vector->size) {
        fprintf(stderr, "%s: list index is out of range\n", func_name);
        fflush(stderr);
        exit(1);
    }
    return;
}

int
v_get(Vector *vector, size_t pos) {
    v_check_pos(vector, pos, __func__);
    return vector->data[pos];
}

void
v_set(Vector *vector, int elem, size_t pos) {
    v_check_pos(vector, pos, __func__);
    vector->data[pos] = elem;
    return;
}

inline void
v_update(Vector *vector, int elem, size_t pos) {
    v_set(vector, elem, pos);
    return;
}

void
v_push_back(Vector *vector, int elem) {
    if (vector->capacity == 0) {
        vector->capacity = 1;
        vector->size = 1;
        vector->data = calloc(1, sizeof(*vector->data));
        return;
    }
    if (vector->size >= vector->capacity) {
        vector->data = realloc(vector->data, 2 * vector->capacity * sizeof(*vector->data));
        if (vector->data == NULL) {
            fprintf(stderr, "%s: memory allocation error\n", __func__);
            fflush(stderr);
            exit(1);
        }
        vector->capacity *= 2;
    }
    vector->data[vector->size] = elem;
    vector->size++;
    return;
}


void
v_insert(Vector *vector, int elem, size_t pos) {
    v_push_back(vector, elem);
    v_check_pos(vector, pos, __func__);
    for (size_t i = vector->size - 1; i > pos; --i) {
        v_set(vector, v_get(vector, i - 1), i);
    }
    v_set(vector, elem, pos);
    return;
}

void
v_erase(Vector *vector, size_t pos) {
    v_check_pos(vector, pos, __func__);
    for (size_t i = pos; i < vector->size - 1; ++i) {
        v_set(vector, v_get(vector, i + 1), i);
    }
    vector->size--;
    return;
}

void
v_print(Vector *vector) {
    if (vector->size == 0) {
        printf("Empty vector");
    }
    for (size_t i = 0; i < vector->size; ++i) {
        printf("%d ", v_get(vector, i));
    }
    printf("\n");
    return;
}

void
v_copy(const Vector *src, Vector *dest) {
    if (src == NULL) {
        fprintf(stderr, "%s: source pointer is NULL\n", __func__);
        fflush(stderr);
        exit(1);
    }
    if (dest == NULL) {
        fprintf(stderr, "%s: destination pointer is NULL\n", __func__);
        fflush(stderr);
        exit(1);
    }
    if (dest->data != NULL) {
        free(dest->data);
    }
    *dest = v_init(src->capacity);
    dest->size = src->size;
    memcpy(dest->data, src->data, src->size * sizeof(*src->data));
    return;
}

enum {
    MAX_NEXT_NUM = 65,
};

typedef struct TrieNode {
    struct TrieNode *next[MAX_NEXT_NUM];
    int is_terminal;
    Vector data;
} TrieNode;

typedef struct Trie
{
    TrieNode *root;
} Trie;

const static char *__valid_chars = "`,',-,a-z,A-Z,0-9,А-Я,а-я";             // can be changed

int 
char_to_ind(char c) {
    int n = strlen(__valid_chars);
    int cur_ind = 0;
    signed char prev_c = -1, cur_c = -1;
    int range = 0;
    for (int i = 0; i < n; ++i) {
        if (i & 1) {
            if (__valid_chars[i] == '-') {
                range = 1;
            } else {
                range = 0;
            }
        } else {
            prev_c = cur_c;
            cur_c = __valid_chars[i];
            if (range) {
                if (cur_c <= prev_c) {
                    fprintf(stderr, "%s: incorrect valid chars string format\n", __func__);
                    fflush(stderr);
                    exit(1);
                }
                if (prev_c <= c && c <= cur_c) {
                    return cur_ind + c - prev_c - 1;
                }
                cur_ind += cur_c - prev_c;
            } else {
                if (cur_c == c) {
                    return cur_ind;
                }
                cur_ind++;
            }
        }
    }
    fprintf(stderr, "%s: invalid symbol '%c'\n", __func__, c);
    fflush(stderr);
    exit(1);
    return -1;
}

void
print_trie_node(TrieNode *t) {
    printf("\nTrie node %p\n", t);
    printf("is terminal: %d\n", t->is_terminal);
    printf("data: ");
    if (t->is_terminal) {
        //v_print(v);
        v_print(&t->data);
    } else {
        printf("no data\n");
    }
    printf("Next:\n");
    for (int i = 0; i < MAX_NEXT_NUM; ++i) {
        if (t->next[i] != 0) {
            printf("%d %p\n", i, t->next[i]);
        }
    }
    for (int i = 0; i < MAX_NEXT_NUM; ++i) {
        if (t->next[i] != NULL) {
            //v_push_back(v, i);
            print_trie_node(t->next[i]);
            //v_erase(v, v->size);
        }
    }
}

void
print_trie(Trie t) {
    printf("Printing trie...\n");
    //Vector v = v_init(0);
    print_trie_node(t.root);
    printf("End\n");
}

static TrieNode *
add_node() {
    TrieNode *res = calloc(1, sizeof(*res));
    for (size_t i = 0; i < MAX_NEXT_NUM; ++i) {
        res->next[i] = NULL;
    }
    res->is_terminal = 0;
    res->data = v_init(0);
    return res;
}

Trie
t_init() {
    Trie trie;
    trie.root = add_node();
    return trie;
}

static void
free_node(TrieNode *node) {
    if (node == NULL) {
        fprintf(stderr, "%s: trie node ptr is NULL", __func__);
        fflush(stderr);
        exit(1);
    }
    for (int i = 0; i < MAX_NEXT_NUM; ++i) {
        if (node->next[i] != NULL) {
            free_node(node->next[i]);
        }
    }
    v_free(&node->data);
    free(node);
}

void
t_free(Trie *trie) {
    if (trie == NULL) {
        fprintf(stderr, "%s: trie ptr is NULL", __func__);
        fflush(stderr);
        exit(1);
    }
    if (trie->root != NULL) {
        free_node(trie->root);
    }
    return;
}

void 
t_add(Trie *trie, char *word, Vector docs) {
    if (trie->root == NULL) {
        *trie = t_init();
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            cur->next[char_to_ind(word[i])] = add_node();
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    cur->is_terminal = 1;
    v_copy(&docs, &cur->data);
    return;
}

Vector
t_get(Trie *trie, char *word) {
    Vector res = v_init(0);
    if (trie->root == NULL) {
        return res;
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            return res;
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    if (cur->is_terminal) {
        v_copy(&cur->data, &res);
    }
    return res;
}

Vector *
t_get_ptr(Trie *trie, char *word) {
    if (trie->root == NULL) {
        return NULL;
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            return NULL;
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    if (cur->is_terminal) {
        return &cur->data;
    }
    return NULL;
}

void
t_push_back(Trie *trie, char *word, int doc) {
    Vector *v = t_get_ptr(trie, word);
    if (v == NULL) {
        t_add(trie, word, v_init(0));
        v = t_get_ptr(trie, word);
    }
    for (int i = 0; i < v->size; ++i) {
        if (v_get(v, i) == doc) {
            return;
        }
    }
    v_push_back(v, doc);
    return;
}

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
    char cur;
    fscanf(in, "%d", &number_of_files);
    cur = fgetc(in);
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
        int lenname = 0;
        while((cur = fgetc(in)) != '\n') {
            book[i][lenname] = cur;
            ++lenname;
        }
    }
    // 4. Создаём словарь, добавляя слова в бор и номера доков, в которых они встречаются
    int number_of_words;
    fscanf(in, "%d", &number_of_words);
    char *word = calloc(MAX_LEN_WORD, sizeof(*word));
    Trie dictionary = t_init();
    for (int i = 0; i < number_of_words; ++i) {
        fscanf(in, "%s", word);
        int number_of_relevant_files;
        fscanf(in, "%d", &number_of_relevant_files);
        for (int j = 0; j < number_of_relevant_files; ++j) {
            int doc;
            fscanf(in, "%d", &doc);
            t_push_back(&dictionary, word, doc);
        }
        for (int j = 0; j < MAX_LEN_WORD; j++) {
            word[j] = 0;
        }
    }
    fclose(in);
    // 5. Считываем кол-во запросов и по-отдельности каждый запрос
    int n;
    scanf("%d", &n);
    cur = getc(stdin);
    char *request = calloc(MAX_LEN_REQUEST, sizeof(*request));
    char *wordnow = calloc(MAX_LEN_WORD, sizeof(*wordnow));
    int *alldocs = calloc(number_of_files, sizeof(*alldocs));
    for (int o = 0; o < n; ++o) {
        for (int i = 0 ; i < number_of_files; ++i) {
            alldocs[i] = 0;
        }
        for (int i = 0 ; i < MAX_LEN_REQUEST; ++i) {
            request[i] = 0;
        }
        for (int i = 0 ; i < MAX_LEN_WORD; ++i) {
            wordnow[i] = 0;
        }
        fgets(request, MAX_LEN_REQUEST - 1, stdin);
        size_t n = strlen(request);
        int index = 0;
        int lenwordnow = 0;
        int wordcount = 0;
        while (index != n) {
            if (request[index] == ' ' || request[index] == '\n') {
                ++wordcount;
                for (int i = 0; i < lenwordnow; ++i) {
                    word[i] = wordnow[i];
                }
                word[lenwordnow] = 0;
                // 6. Для каждого слова в запросе идём в бор и смотрим, в каких доках оно встречается
                Vector docs = t_get(&dictionary, word);
                for (int i = 0; i < docs.size; ++i) {
                    ++alldocs[v_get(&docs, i)];
                }
                lenwordnow = 0;
                v_free(&docs);
            } else {
                wordnow[lenwordnow] = request[index];
                ++lenwordnow;
            }
            ++index;
        }
        // 7. Находим пересечение
        int flag = 0;
        for (int i = 0; i < number_of_files; ++i) {
            if (alldocs[i] == wordcount) {
                flag = 1;
                // 8. Выводим ответ на экран
                int j = 0;
                while (book[i][j] != '\n') {
                    printf("%c", book[i][j]);
                    j++;
                }
                printf("\n");
            }
        }
        if (flag == 0) {
            printf("No suitable files were found.\n");
        }
    }
    // 9. Освобождаем память
    t_free(&dictionary);
    free(*book);
    free(book);
    free(alldocs);
    free(request);
    free(wordnow);
    free(word);
    return 0;
}
