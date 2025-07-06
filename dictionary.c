// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// ───────────────────────────────────────────────────────────────
// 1.  Параметри хеш-таблиці
//    2^17 = 131 072  — достатньо, аби середній бакет мав ≤1 елемент
#define N 131072U

// 2.  Вузол
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// 3.  Таблиця й лічильник
static node *table[N];
static unsigned int words_in_dict = 0;

// 4.  djb2-хеш
static inline unsigned int hash(const char *word)
{
    unsigned long h = 5381;
    for (unsigned char c; (c = (unsigned char)*word++);)
    {
        if (c == '\'') continue;        // апостроф не враховуємо
        h = ((h << 5) + h) + tolower(c); // h * 33 + c
    }
    return h % N;
}

// ───────────────────────────────────────────────────────────────
// LOAD
bool load(const char *dictionary)
{
    // Відкриваємо файл
    FILE *fp = fopen(dictionary, "r");
    if (!fp) return false;

    char w[LENGTH + 1];

    // Читаємо слово за словом
    while (fscanf(fp, "%45s", w) == 1)
    {
        // Виділяємо ноду
        node *n = malloc(sizeof *n);
        if (!n)
        {
            fclose(fp);
            return false;               // брак памʼяті
        }
        strcpy(n->word, w);

        // Вставляємо у відповідний бакет (голова списку)
        unsigned int idx = hash(w);
        n->next        = table[idx];
        table[idx]     = n;

        words_in_dict++;
    }
    fclose(fp);
    return true;
}

// ───────────────────────────────────────────────────────────────
// SIZE
unsigned int size(void)
{
    return words_in_dict;
}

// ───────────────────────────────────────────────────────────────
// CHECK
bool check(const char *word)
{
    char lower[LENGTH + 1];
    int len = 0;
    for (; word[len] && len < LENGTH; len++)
        lower[len] = tolower((unsigned char)word[len]);
    lower[len] = '\0';

    for (node *n = table[hash(lower)]; n; n = n->next)
    {
        if (strcmp(n->word, lower) == 0)
            return true;
    }
    return false;
}

// ───────────────────────────────────────────────────────────────
// UNLOAD
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
    return true;
}
