#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define ARG_BUFSIZE 64
#define ARG_TOK_DELIM " \t\r\n\a"

char **parse_args(char *line, int *argc) {
    int bufsize = ARG_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    char *line_copy = strdup(line);

    token = strtok(line_copy, ARG_TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = strdup(token);
        if (position >= bufsize) {
            bufsize += ARG_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
        }
        token = strtok(NULL, ARG_TOK_DELIM);
    }
    tokens[position] = NULL;
    if (argc) *argc = position;
    free(line_copy);
    return tokens;
}

void free_tokens(char **tokens) {
    if (tokens == NULL) return;
    for (int i = 0; tokens[i] != NULL; i++) free(tokens[i]);
    free(tokens);
}
