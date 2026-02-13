#ifndef PARSER_H
#define PARSER_H

char **parse_args(char *line, int *argc);
void free_tokens(char **tokens);

#endif
