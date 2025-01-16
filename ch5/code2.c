// Both processes are able to use the file descriptor returned by open without
// issue, because file descriptors are shared across the system.
//
// When both processes write to the file concurrently, the data written may
// appear in either child-first or parent-first order (i.e., a race condition
// occurs). The order of the writes may be enforced by having the parent wait
// for the child process to complete.

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

int main(void) {
    int fd = open("code2.txt",
                  O_APPEND | O_CREAT | O_WRONLY,
                  S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
    if (fd < 0) {
        fprintf(stderr, "unable to open file\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid < 0) { // Error
        fprintf(stderr, "some forkin' error occurred\n");
        return 2;
    } else if (0 == pid) { // Child
        write(fd, "child\n", 6);
        return 0;
    } else { // Parent
        write(fd, "parent\n", 7);
        return 0;
    }
}
