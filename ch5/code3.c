// It should be possible to enforce the print order without wait (or waitpid or
// the like), but this would require some form of inter-process communication
// (IPC). The parent process would need to hold off from printing until it
// receives a message from the child process that it has finished printing.
// This could likely be achieved via signals.

#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { // Error
        return 1;
    } else if (0 == pid) { // Child
        printf("hello\n");
        return 0;
    } else { // Parent
        int wait_status;
        pid_t wait_pid = wait(&wait_status);
        if (wait_pid < 0) {
            return 2;
        }
        if (!WIFEXITED(wait_status)) {
            return 3;
        }
        printf("goodbye\n");
        return 0;
    }
}
