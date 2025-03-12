// In this case, the compiler tries to alert us to our foolishness:
//
//     $ gcc -Wall -Wextra -g -o code7 code7.c
//     code7.c: In function ‘main’:
//     code7.c:54:5: warning: ‘free’ called on pointer ‘buffer’ with nonzero offset 4 [-Wfree-nonheap-object]
//        54 |     free(buffer + 1);
//           |     ^~~~~~~~~~~~~~~~
//     code7.c:50:19: note: returned from ‘malloc’
//        50 |     int *buffer = malloc(1 * sizeof *buffer);
//           |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~
//
// If we run the program normally or via GDB, it crashes because free detects
// the invalid pointer and aborts. Valgrind also catches the error and gives us
// some additional information:
//
//     $ valgrind --tool=memcheck --leak-check=full ./code7
//     ==164322== Memcheck, a memory error detector
//     ==164322== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
//     ==164322== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
//     ==164322== Command: ./code7
//     ==164322==
//     ==164322== Invalid free() / delete / delete[] / realloc()
//     ==164322==    at 0x48488EF: free (vg_replace_malloc.c:989)
//     ==164322==    by 0x10919E: main (code7.c:54)
//     ==164322==  Address 0x4a7b044 is 0 bytes after a block of size 4 alloc'd
//     ==164322==    at 0x48457A8: malloc (vg_replace_malloc.c:446)
//     ==164322==    by 0x10916A: main (code7.c:50)
//     ==164322==
//     ==164322==
//     ==164322== HEAP SUMMARY:
//     ==164322==     in use at exit: 4 bytes in 1 blocks
//     ==164322==   total heap usage: 1 allocs, 1 frees, 4 bytes allocated
//     ==164322==
//     ==164322== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
//     ==164322==    at 0x48457A8: malloc (vg_replace_malloc.c:446)
//     ==164322==    by 0x10916A: main (code7.c:50)
//     ==164322==
//     ==164322== LEAK SUMMARY:
//     ==164322==    definitely lost: 4 bytes in 1 blocks
//     ==164322==    indirectly lost: 0 bytes in 0 blocks
//     ==164322==      possibly lost: 0 bytes in 0 blocks
//     ==164322==    still reachable: 0 bytes in 0 blocks
//     ==164322==         suppressed: 0 bytes in 0 blocks
//     ==164322==
//     ==164322== For lists of detected and suppressed errors, rerun with: -s
//     ==164322== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)

#include <err.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
    int *buffer = malloc(1 * sizeof *buffer);
    if (NULL == buffer) {
        err(EXIT_FAILURE, "malloc");
    }
    free(buffer + 1);
}
