#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "redirect.h"

static int perform_dup(char *filename, int flags, int target_fd) {
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("shell open error");
        return -1;
    }
    if (dup2(fd, target_fd) < 0) {
        perror("shell dup2 error");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static void remove_args(char **argv, int index, int count) {
    int j = index + count;
    int k = index;
    while (argv[j] != NULL) argv[k++] = argv[j++];
    argv[k] = NULL;
}

int apply_redirections(char **argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        int flags = -1;
        int target_fd = -1;

        if (strcmp(argv[i], ">") == 0) {
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            target_fd = STDOUT_FILENO;
        } else if (strcmp(argv[i], ">>") == 0) {
            flags = O_WRONLY | O_CREAT | O_APPEND;
            target_fd = STDOUT_FILENO;
        } else if (strcmp(argv[i], "<") == 0) {
            flags = O_RDONLY;
            target_fd = STDIN_FILENO;
        }

        if (target_fd != -1) {
            if (argv[i + 1] == NULL) {
                fprintf(stderr, "shell: expected file after operator\n");
                return -1;
            }
            if (perform_dup(argv[i + 1], flags, target_fd) == -1) return -1;
            remove_args(argv, i, 2);
            i--;
        }
    }
    return 0;
}
