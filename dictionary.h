#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

#define N 26
#define MAX_LENGTH 45

typedef struct node
{
    bool is_word;
    struct node *children[N];
} node;

bool loadDictionary();
bool check(const char *word);
void unload();
int size();

extern node *root;

#endif
