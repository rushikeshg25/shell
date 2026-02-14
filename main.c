#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "execute.h"
#include "signals.h"

void start_repl() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strncpy(hostname, "localhost", sizeof(hostname));
    }
    hostname[sizeof(hostname) - 1] = '\0';
    char *dot = strchr(hostname, '.');
    if (dot) *dot = '\0';

    char prompt[512];
    snprintf(prompt, sizeof(prompt), "jarvis@%s> ", hostname);

    char *input;
    while (1) {
        input = readline(prompt);
        
        if (input == NULL) {
            printf("\n");
            break;
        }
        
        if (strlen(input) > 0) {
            add_history(input);
            if (execute(input) == 0) {
                free(input);
                break;
            }
        }
        
        free(input);
    }
}

int main() {
    setup_signals();
    start_repl();
    return 0;
}
