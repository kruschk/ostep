// This code is adapted from the example in the pipe(2) man pages. I'm not sure
// how to complete the problem without dup2.

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int pipefd[2];
    pid_t pid;

    if (-1 == pipe(pipefd)) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Reading child
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Close the unused write end of the pipe.
        if (close(pipefd[1]) == -1) {
            perror("reading child: close");
            exit(EXIT_FAILURE);
        }
        // Redirect standard input from the read end of the pipe.
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("reading child: dup2");
            exit(EXIT_FAILURE);
        }
        // Execute wc.
        if (execl("/bin/wc", "/bin/wc", (char *)NULL) == -1) {
            perror("reading child: execl");
            exit(EXIT_FAILURE);
        }
    }

    // Writing child
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Close the unused read end of the pipe.
        if (close(pipefd[0]) == -1) {
            perror("writing child: close");
            exit(EXIT_FAILURE);
        }
        // Redirect standard output to the write end of the pipe.
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("writing child: dup2");
            exit(EXIT_FAILURE);
        }
        // Execute ls.
        if (execl("/bin/ls", "/bin/ls", "-1", (char *)NULL) == -1) {
            perror("writing child: execl");
            exit(EXIT_FAILURE);
        }
    }
}
