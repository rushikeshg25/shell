#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "execute.h"
#include "signals.h"

#define SHELL_PROMPT "jarvis> "

void start_repl() {
    char *input;
    while (1) {
        input = readline(SHELL_PROMPT);
        
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
