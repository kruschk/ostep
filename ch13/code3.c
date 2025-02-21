#include <unistd.h>

#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char * const argv[]) {
    // Check for correct usage.
    // NOTE: overflowing and underflowing arguments will wrap.
    if (argc != 3) {
        fprintf(stderr, "usage: %s SPACE TIME\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the SPACE argument (given in MiB).
    errno = 0;
    size_t space = strtoul(argv[1], NULL, 10);
    if (errno != 0) {
        err(EXIT_FAILURE, "failed to parse SPACE argument");
    }
    space *= 1024 * 1024;

    // Parse the TIME argument (given in s).
    size_t time = strtoul(argv[2], NULL, 10);
    if (errno != 0) {
        err(EXIT_FAILURE, "failed to parse TIME argument");
    }

    // Take up the desired SPACE.
    char * const buffer = malloc(space);
    if (buffer == NULL) {
        err(EXIT_FAILURE, "malloc");
    }
    for (size_t i = 0; i < space; i++) {
        buffer[i]++;
    }

    // Sleep for the desired TIME.
    sleep(time);
}
