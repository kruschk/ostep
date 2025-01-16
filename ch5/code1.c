// Before the fork, the variable x is initialised to 0. After the fork, the
// child receives a copy of the parent's memory space, thus inheriting a
// distinct copy of x, so changes to the variable in either the child or the
// parent process has no impact on the other process's x.

#include <unistd.h>

#include <stdio.h>

int main(void) {
    int x = 0;
    printf("parent x = %d\n", x);
    pid_t pid = fork();
    if (pid < 0) { // Error
        return 1;
    } else if (0 == pid) { // Child
        printf("child before assignment: x = %d\n", x);
        x = 1;
        printf("child after assignment: x = %d\n", x);
        return 0;
    } else { // Parent
        printf("parent before assignment: x = %d\n", x);
        x = 2;
        printf("parent after assignment: x = %d\n", x);
        return 0;
    }
}
