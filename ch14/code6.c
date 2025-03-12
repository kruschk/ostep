// GCC tries to warn us that we are attempting a use-after-free:
//
//     $ gcc -Wall -Wextra -g -o code6 code6.c
//     code6.c: In function ‘main’:
//     code6.c:51:5: warning: pointer ‘buffer’ used after ‘free’ [-Wuse-after-free]
//        51 |     printf("%d\n", buffer[0]);
//           |     ^~~~~~~~~~~~~~~~~~~~~~~~~
//     code6.c:50:5: note: call to ‘free’ here
//        50 |     free(buffer);
//           |     ^~~~~~~~~~~~
//
// No issues are apparent when running this program normally or via GDB;
// however, Valgrind flags the error:
//
//     $ valgrind --tool=memcheck --leak-check=full ./code6
//     ==163776== Memcheck, a memory error detector
//     ==163776== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
//     ==163776== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
//     ==163776== Command: ./code6
//     ==163776==
//     ==163776== Invalid read of size 4
//     ==163776==    at 0x1091AF: main (code6.c:51)
//     ==163776==  Address 0x4a7b040 is 0 bytes inside a block of size 4 free'd
//     ==163776==    at 0x48488EF: free (vg_replace_malloc.c:989)
//     ==163776==    by 0x1091AA: main (code6.c:50)
//     ==163776==  Block was alloc'd at
//     ==163776==    at 0x48457A8: malloc (vg_replace_malloc.c:446)
//     ==163776==    by 0x10917A: main (code6.c:46)
//     ==163776==
//     ==163776==
//     ==163776== HEAP SUMMARY:
//     ==163776==     in use at exit: 0 bytes in 0 blocks
//     ==163776==   total heap usage: 2 allocs, 2 frees, 1,028 bytes allocated
//     ==163776==
//     ==163776== All heap blocks were freed -- no leaks are possible
//     ==163776==
//     ==163776== For lists of detected and suppressed errors, rerun with: -s
//     ==163776== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *buffer = malloc(1 * sizeof *buffer);
    if (NULL == buffer) {
        err(EXIT_FAILURE, "malloc");
    }
    free(buffer);
    printf("%d\n", buffer[0]);
}
