#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "execute.h"
#include "parser.h"
#include "redirect.h"
#include "builtins.h"

static pid_t fork_and_execute(char *command, int in_fd, int out_fd) {
    int argc = 0;
    char **argv = parse_args(command, &argc);
    if (argc == 0) {
        free_tokens(argv);
        return -1;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (in_fd != STDIN_FILENO) { dup2(in_fd, STDIN_FILENO); close(in_fd); }
        if (out_fd != STDOUT_FILENO) { dup2(out_fd, STDOUT_FILENO); close(out_fd); }
        
        if (apply_redirections(argv) == -1) exit(EXIT_FAILURE);
        if (is_builtin(argv)) exit(exec_builtin(argv) == 0 ? 0 : 1);
        if (execvp(argv[0], argv) == -1) perror("shell execution error");
        exit(EXIT_FAILURE);
    }
    
    free_tokens(argv);
    return pid;
}

static int handle_parent_builtin(char *command) {
    int argc = 0;
    char **argv = parse_args(command, &argc);
    int status = 1;
    if (argc > 0 && is_builtin(argv)) {
        if (strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "exit") == 0) {
            status = exec_builtin(argv);
            free_tokens(argv);
            return status;
        }
    }
    free_tokens(argv);
    return -1;
}

int execute(char *input) {
    int bg = parse_background(input);
    int num_cmds = 0;
    char **cmds = parse_pipeline(input, &num_cmds);
    if (num_cmds == 0) { free_tokens(cmds); return 1; }

    if (num_cmds == 1) {
        int status = handle_parent_builtin(cmds[0]);
        if (status != -1) { free_tokens(cmds); return status; }
    }

    pid_t *pids = malloc(num_cmds * sizeof(pid_t));
    int in_fd = STDIN_FILENO;
    int fd[2];
    
    for (int i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1) {
            if (pipe(fd) == -1) { perror("shell pipe error"); break; }
            pids[i] = fork_and_execute(cmds[i], in_fd, fd[1]);
            close(fd[1]);
            if (in_fd != STDIN_FILENO) close(in_fd);
            in_fd = fd[0];
        } else {
            pids[i] = fork_and_execute(cmds[i], in_fd, STDOUT_FILENO);
            if (in_fd != STDIN_FILENO) close(in_fd);
        }
    }
    
    if (!bg) {
        for (int i = 0; i < num_cmds; i++) {
            if (pids[i] > 0) waitpid(pids[i], NULL, WUNTRACED);
        }
    } else if (num_cmds > 0 && pids[num_cmds-1] > 0) {
        printf("[%d]\n", pids[num_cmds-1]);
    }
    
    free(pids);
    free_tokens(cmds);
    return 1;
}
