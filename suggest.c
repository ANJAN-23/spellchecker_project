#include "suggest.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_SUGGESTIONS 20

// Recursive function to collect words from trie
void collectWords(node *curr, char *buffer, int depth,
                  char output[][MAX_LENGTH + 1], int *count)
{
    if (!curr || *count >= MAX_SUGGESTIONS) return;

    if (curr->is_word)
    {
        buffer[depth] = '\0';
        strcpy(output[*count], buffer);
        (*count)++;
    }

    for (int i = 0; i < 26; i++)
    {
        if (curr->children[i])
        {
            buffer[depth] = 'a' + i;
            collectWords(curr->children[i], buffer, depth + 1, output, count);
        }
    }
}

// Autocomplete function
int autocomplete(const char *prefix, char output[][MAX_LENGTH + 1])
{
    node *crawler = root;

    for (int i = 0; i < strlen(prefix); i++)
    {
        int idx = tolower(prefix[i]) - 'a';
        if (idx < 0 || idx >= 26) return 0;

        if (!crawler->children[idx])
            return 0;

        crawler = crawler->children[idx];
    }

    char buffer[MAX_LENGTH + 1];
    strcpy(buffer, prefix);

    int count = 0;
    collectWords(crawler, buffer, strlen(prefix), output, &count);

    return count;
}

// --- Levenshtein distance ---
int editDistance(const char *a, const char *b)
{
    int len1 = strlen(a), len2 = strlen(b);
    int dp[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++)
        dp[i][0] = i;
    for (int j = 0; j <= len2; j++)
        dp[0][j] = j;

    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
            {
                int insert = dp[i][j - 1] + 1;
                int del = dp[i - 1][j] + 1;
                int replace = dp[i - 1][j - 1] + 1;
                dp[i][j] = insert < del ? (insert < replace ? insert : replace) : (del < replace ? del : replace);
            }
        }
    }
    return dp[len1][len2];
}

// Spell suggestion function
void collectSuggestions(node *curr, char *buffer, int depth,
                        const char *word,
                        char output[][MAX_LENGTH + 1],
                        int *count)
{
    if (!curr || *count >= MAX_SUGGESTIONS)
        return;

    if (curr->is_word)
    {
        buffer[depth] = '\0';
        // Use edit distance <= 2 for suggestion
        if (editDistance(buffer, word) <= 2)
        {
            strcpy(output[*count], buffer);
            (*count)++;
        }
    }

    for (int i = 0; i < 26; i++)
    {
        if (curr->children[i])
        {
            buffer[depth] = 'a' + i;
            collectSuggestions(curr->children[i], buffer, depth + 1, word, output, count);
        }
    }
}
