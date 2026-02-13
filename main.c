#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define SHELL_PROMPT "shell> "

int main() {
    char *input;
    while (1) {
        input = readline(SHELL_PROMPT);
        if (input == NULL) {
            printf("\n");
            break;
        }
        if (strlen(input) > 0) {
            add_history(input);
            printf("You entered: %s\n", input);
        }
        free(input);
    }
    return 0;
}
