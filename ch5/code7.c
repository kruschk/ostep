// When printf is called after closing standard output, the call succeeds, but
// nothing appears in the terminal. Apparently, this is due to buffering. See
// https://stackoverflow.com/questions/59094054 for details.

#include <unistd.h>

#include <stdio.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (0 == pid) {
        close(STDOUT_FILENO);
        printf("what happens now?");
    }
}
