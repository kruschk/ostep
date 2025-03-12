// Everything appears to be fine when running this program normally, via GDB,
// or through valgrind, but, in fact, the program is not correct. It writes to
// an invalid memory location due to an off-by-one error: we've allocated an
// array of 100 integers, but we try to modify the 101st element (remember that
// only indices 0 through 99 inclusive are valid). This invalid memory access,
// where data is written beyond the buffer's allocated memory, is known as
// buffer overflow or buffer overrun.

#include <stdio.h>

int main(void) {
    int data[100] = {0};
    data[100] = 0; // Oops, off by one!
}
