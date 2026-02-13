#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"

int is_builtin(char **argv) {
    if (argv == NULL || argv[0] == NULL) return 0;
    if (strcmp(argv[0], "exit") == 0) return 1;
    if (strcmp(argv[0], "pwd") == 0) return 1;
    if (strcmp(argv[0], "cd") == 0) return 1;
    return 0;
}

int exec_builtin(char **argv) {
    if (strcmp(argv[0], "exit") == 0) return 0;
    
    if (strcmp(argv[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd error");
        }
        return 1;
    }
    
    if (strcmp(argv[0], "cd") == 0) {
        char *path = argv[1];
        if (path == NULL) path = getenv("HOME");
        if (chdir(path) != 0) perror("cd error");
        return 1;
    }
    
    return 1;
}
