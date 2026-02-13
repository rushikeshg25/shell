#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "signals.h"

static void handle_sigchld(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void setup_signals() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) perror("sigaction error");
}
