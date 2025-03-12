#include <stddef.h>
#include <stdio.h>

int main(void) {
    int *pointer = NULL;
    // Dereferencing this null pointer triggers a segmentation fault!
    *pointer = 0;
}
