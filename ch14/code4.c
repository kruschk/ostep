// When running this program normally or via GDB, no errors occur, lulling us
// into a false sense of security. Valgrind cures us of that misapprehension by
// pointing out the memory leak that occurs due to our failure to free the
// memory we allocated.
//
//     $ valgrind --tool=memcheck --leak-check=full ./code4
//     ==163327== Memcheck, a memory error detector
//     ==163327== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
//     ==163327== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
//     ==163327== Command: ./code4
//     ==163327==
//     ==163327==
//     ==163327== HEAP SUMMARY:
//     ==163327==     in use at exit: 1 bytes in 1 blocks
//     ==163327==   total heap usage: 1 allocs, 0 frees, 1 bytes allocated
//     ==163327==
//     ==163327== 1 bytes in 1 blocks are definitely lost in loss record 1 of 1
//     ==163327==    at 0x48457A8: malloc (vg_replace_malloc.c:446)
//     ==163327==    by 0x10915A: main (code4.c:36)
//     ==163327==
//     ==163327== LEAK SUMMARY:
//     ==163327==    definitely lost: 1 bytes in 1 blocks
//     ==163327==    indirectly lost: 0 bytes in 0 blocks
//     ==163327==      possibly lost: 0 bytes in 0 blocks
//     ==163327==    still reachable: 0 bytes in 0 blocks
//     ==163327==         suppressed: 0 bytes in 0 blocks
//     ==163327==
//     ==163327== For lists of detected and suppressed errors, rerun with: -s
//     ==163327== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *buffer = malloc(1);
    if (NULL == buffer) {
        err(EXIT_FAILURE, "malloc");
    }
    // No free? That's a memory leak!
}
