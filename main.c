#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"
#include "suggest.h"

#define MAX_SUGGESTIONS 20

int main()
{
    char str[200];
    printf("Enter a line: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0;

    if (!loadDictionary())
    {
        printf("Dictionary load error\n");
        return 1;
    }

    printf("\nCorrected sentence:\n");

    char word[50];
    int idx = 0;

    for (int i = 0; i <= strlen(str); i++)
    {
        if (isalpha(str[i]))
            word[idx++] = tolower(str[i]);
        else
        {
            if (idx > 0)
            {
                word[idx] = '\0';
                idx = 0;

                if (check(word))
                    printf("%s ", word);
                else
                {
                    printf("[Incorrect: %s] ", word);

                    char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1];
                    int count = 0;
                    char buffer[MAX_LENGTH + 1];
                    collectSuggestions(root, buffer, 0, word, suggestions, &count);

                    if (count > 0)
                    {
                        printf("(Suggestions: ");
                        for (int j = 0; j < count; j++)
                        {
                            printf("%s", suggestions[j]);
                            if (j < count - 1) printf(", ");
                        }
                        printf(") ");
                    }
                }
            }
            if (str[i] != '\0')
                printf("%c", str[i]);
        }
    }

    printf("\n\nAUTOCOMPLETE\nEnter prefix: ");
    char prefix[50];
    scanf("%s", prefix);

    char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1];
    int count = autocomplete(prefix, suggestions);

    if (count == 0)
        printf("No suggestions.\n");
    else
    {
        printf("Suggestions:\n");
        for (int i = 0; i < count; i++)
            printf("%d. %s\n", i + 1, suggestions[i]);
    }

    unload();
    return 0;
}
