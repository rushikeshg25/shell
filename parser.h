#ifndef PARSER_H
#define PARSER_H

char **parse_args(char *line, int *argc);
char **parse_pipeline(char *line, int *num_cmds);
void free_tokens(char **tokens);
int parse_background(char *line);

#endif
