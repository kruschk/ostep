#include <unistd.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long int timespec_to_ns(struct timespec time) {
    return 1000000000 * time.tv_sec + time.tv_nsec;
}

int main(int argc, char *argv[]) {
    // Parse arguments.
    if (argc < 3) {
        fprintf(stderr, "usage: %s PAGES TRIALS\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const long int pages = strtol(argv[1], NULL, 10);
    if (EINVAL == errno || ERANGE == errno) {
        err(EXIT_FAILURE, NULL);
    }
    if (pages < 1) {
        fprintf(stderr, "PAGES argument must be positive");
        exit(EXIT_FAILURE);
    }
    const long int trials = strtol(argv[2], NULL, 10);
    if (EINVAL == errno || ERANGE == errno) {
        err(EXIT_FAILURE, NULL);
    }
    if (trials < 1) {
        fprintf(stderr, "TRIALS argument must be positive");
        exit(EXIT_FAILURE);
    }

    // Get the system's page size.
    const long int page_size = sysconf(_SC_PAGE_SIZE);

    // Allocate memory.
    int *array = calloc(pages, page_size);
    if (NULL == array) {
        err(EXIT_FAILURE, NULL);
    }

    // Measure the average time, in nanoseconds, required to access each page,
    // which can reveal some information about the TLB size(s).
    const long int jump = page_size / sizeof *array;
    const long int limit = jump * pages;
    struct timespec time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (long int trial = 0; trial < trials; trial++) {
        for (long int offset = 0; offset < limit; offset += jump) {
            array[offset] += 1;
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    const double average = (double)(timespec_to_ns(time2)
                                    - timespec_to_ns(time1))
                           / pages / trials;
    printf("%f\n", average);

    // Free memory.
    free(array);
}
