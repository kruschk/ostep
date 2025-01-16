// Having several variants of exec increases the flexibility of the API
// (improving user-friendliness), and certain variants may offer performance
// benefits. For example, the "l" variants free the caller from having to
// construct an array (unlike the "v" variants). The "p" variants perform a
// search for the desired executable file, which may be convenient, but also
// expensive. Finally, the "e" variants allow the child process's environment
// to be explicitly specified in an array, which may be convenient if multiple
// child processes are to share identical initial environments.

#define _GNU_SOURCE // Required for execvpe

#include <unistd.h>

#include <stdio.h>
#include <stddef.h>

int main(void) {
    // execl
    pid_t pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        if (-1 == execl("/bin/ls", "/bin/ls", (char *)NULL)) {
            fprintf(stderr, "execl failed\n");
            return 2;
        }
    }

    // execle
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        char *const envp[] = {NULL};
        if (-1 == execle("/bin/ls", "/bin/ls", (char *)NULL, envp)) {
            fprintf(stderr, "execle failed\n");
            return 3;
        }
    }

    // execlp
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        if (-1 == execlp("ls", "ls", (char *)NULL)) {
            fprintf(stderr, "execlp failed\n");
            return 4;
        }
    }

    // execv
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        char *const argv[] = {"/bin/ls", NULL};
        if (-1 == execv("/bin/ls", argv)) {
            fprintf(stderr, "execv failed\n");
            return 5;
        }
    }

    // execve
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        char *const argv[] = {NULL};
        char *const envp[] = {NULL};
        if (-1 == execve("/bin/ls", argv, envp)) {
            fprintf(stderr, "execve failed\n");
            return 6;
        }
    }

    // execvp
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        char *const argv[] = {"ls", NULL};
        if (-1 == execvp("ls", argv)) {
            fprintf(stderr, "execvp failed\n");
            return 7;
        }
    }

    // execvpe
    pid = fork();
    if (pid < 0) { // Error
        return 1;
    }
    if (0 == pid) { // Child
        char *const argv[] = {"ls", NULL};
        char *const envp[] = {NULL};
        if (-1 == execvpe("ls", argv, envp)) {
            fprintf(stderr, "execvpe failed\n");
            return 8;
        }
    }
}
