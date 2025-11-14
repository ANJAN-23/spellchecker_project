#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

node *root = NULL;
static int word_count = 0;

node *createNode()
{
    node *n = malloc(sizeof(node));
    n->is_word = false;
    for (int i = 0; i < N; i++)
        n->children[i] = NULL;
    return n;
}

bool loadDictionary()
{
    FILE *file = fopen("dictionary.txt", "r");
    if (!file) return false;

    root = createNode();
    char word[MAX_LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *crawler = root;
        for (int i = 0; i < strlen(word); i++)
        {
            int idx = tolower(word[i]) - 'a';
            if (idx < 0 || idx >= N) continue;

            if (!crawler->children[idx])
                crawler->children[idx] = createNode();

            crawler = crawler->children[idx];
        }
        crawler->is_word = true;
        word_count++;
    }
    fclose(file);
    return true;
}

bool check(const char *word)
{
    node *crawler = root;
    if (!word || strlen(word) == 0) return false;

    for (int i = 0; i < strlen(word); i++)
    {
        int idx = tolower(word[i]) - 'a';
        if (idx < 0 || idx >= N) return false;

        if (!crawler->children[idx]) return false;
        crawler = crawler->children[idx];
    }
    return crawler->is_word;
}

void unloadNode(node *curr)
{
    if (!curr) return;

    for (int i = 0; i < N; i++)
        unloadNode(curr->children[i]);

    free(curr);
}

void unload()
{
    unloadNode(root);
}

int size()
{
    return word_count;
}
