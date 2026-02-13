#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define ARG_BUFSIZE 64
#define ARG_TOK_DELIM " \t\r\n\a"

static char **tokenize(char *line, const char *delim, int *count) {
    int bufsize = ARG_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    char *line_copy = strdup(line);

    if (!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line_copy, delim);
    while (token != NULL) {
        tokens[position++] = strdup(token);

        if (position >= bufsize) {
            bufsize += ARG_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    if (count) *count = position;
    free(line_copy);
    return tokens;
}

char **parse_args(char *line, int *argc) {
    return tokenize(line, ARG_TOK_DELIM, argc);
}

char **parse_pipeline(char *line, int *num_cmds) {
    return tokenize(line, "|", num_cmds);
}

void free_tokens(char **tokens) {
    if (tokens == NULL) return;
    for (int i = 0; tokens[i] != NULL; i++) free(tokens[i]);
    free(tokens);
}

int parse_background(char *line) {
    int len = strlen(line);
    if (len == 0) return 0;
    
    int i = len - 1;
    while (i >= 0 && (line[i] == ' ' || line[i] == '\t')) i--;
    
    if (i >= 0 && line[i] == '&') {
        line[i] = ' ';
        return 1;
    }
    return 0;
}
