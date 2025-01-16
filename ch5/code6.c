// One scenario in which waitpid would be useful is when a parent process
// spawns several child processes, but then needs to wait for a specific one of
// them to finish before continuing.

#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { // Error
        return 1;
    } else if (0 == pid) { // Child
        printf("child\n");
        return 0;
    } else { // Parent
        pid_t wait_pid = waitpid(pid, NULL, 0);
        if (wait_pid < 0) {
            fprintf(stderr, "error waiting for child %d", pid);
        }
        printf("parent\n");
        return 0;
    }
}
