#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_PROMPT "shell> "

int main() {
    char input[1024];
    while (1) {
        printf("%s", SHELL_PROMPT);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        printf("You entered: %s", input);
    }
    return 0;
}
