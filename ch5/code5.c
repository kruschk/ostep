// On success, wait returns the PID of the terminated child. On failure, it
// returns -1.
//
// Calling wait from the child returns -1 and sets errno to ECHILD (i.e, 10).

#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>

int main(void) {
    pid_t pid = fork();
    pid_t wait_pid;
    if (pid < 0) { // Error
        return 1;
    } else if (0 == pid) { // Child
        wait_pid = wait(NULL);
        printf("child: errno = %d, pid = %d, wait_pid = %d\n",
               errno, pid, wait_pid);
        return 0;
    } else { // Parent
        wait_pid = wait(NULL);
        printf("parent: errno = %d, pid = %d, wait_pid = %d\n",
               errno, pid, wait_pid);
        return 0;
    }
}
