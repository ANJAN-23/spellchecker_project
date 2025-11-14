#ifndef SUGGEST_H
#define SUGGEST_H

#include "dictionary.h"

#define MAX_SUGGESTIONS 20

int autocomplete(const char *prefix, char output[][MAX_LENGTH + 1]);

void collectSuggestions(node *curr, char *buffer, int depth,
                        const char *word,
                        char output[][MAX_LENGTH + 1],
                        int *count);

#endif
