// Based on consecutive calls, gettimeofday does not have sufficient resolution
// to measure a single system call, so we must perform many iterations in our
// benchmarks to get an idea of the average overhead; this is a good
// benchmarking practice in general. Note that gettimeofday is also deprecated
// in favour of clock_gettime.
//
// The first benchmark shows that the read system call completes in
// approximately 0.5 us on my machine. Similarly, the second demonstrates that
// the write system call completes in approximately the same period of time.
//
// The third benchmark demands slightly more analysis. From it, we can see that
// each iteration completes in approximately 2.5 us (keeping in mind that twice
// as many iterations are being performed because there are now two processes).
// Each iteration comprises a read and a write. The context switch overhead is
// therefore approximately 2.5 us - 0.5 us - 0.5 us = 1.5 us.
//
// Note that these benchmarks do not consider the overhead of loops and
// procedure calls (and probably a bunch of other things, too). A more complete
// analysis would require studying the assembly code emitted by the compiler
// (e.g., it may have unrolled the loops automatically as an optimisation).

#define _GNU_SOURCE

#include <fcntl.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void report(const char name[],
            const unsigned int iterations,
            const struct timeval time1,
            const struct timeval time2) {
    const uintmax_t elapsed = (time2.tv_usec - time1.tv_usec)
                              + 1000000 * (time2.tv_sec - time1.tv_sec);
    if (printf("%s:\n"
               "iterations = %d\n"
               "time1 = %ld.%06ld s\n"
               "time2 = %ld.%06ld s\n"
               "elapsed = %lu us\n"
               "overhead = %g us\n\n",
               name,
               iterations,
               time1.tv_sec, time1.tv_usec,
               time2.tv_sec, time2.tv_usec,
               elapsed,
               (double)elapsed / iterations) < 0) {
        perror("printf");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    char buffer = '\0';
    const unsigned int iterations = 100000;
    cpu_set_t cpu_set;
    int pipe1[2];
    int pipe2[2];
    pid_t pid;
    struct timeval time1;
    struct timeval time2;

    // Call gettimeofday back-to-back to get an idea of its resolution.
    gettimeofday(&time1, NULL);
    gettimeofday(&time2, NULL);

    if (printf("gettimeofday:\n"
               "time1 = %ld.%06ld s\n"
               "time2 = %ld.%06ld s\n\n",
               time1.tv_sec, time1.tv_usec,
               time2.tv_sec, time2.tv_usec) < 0) {
        perror("printf");
        exit(EXIT_FAILURE);
    }

    // Benchmark 1: read
    gettimeofday(&time1, NULL);
    for (unsigned int i = 0; i < iterations; i++) {
        // NOTE: we don't check for errors within the benchmark loops to
        // minimise overhead.
        read(STDIN_FILENO, NULL, 0);
    }
    gettimeofday(&time2, NULL);
    report("read", iterations, time1, time2);

    // Benchmark 2: write
    gettimeofday(&time1, NULL);
    for (unsigned int i = 0; i < iterations; i++) {
        write(STDOUT_FILENO, NULL, 0);
    }
    gettimeofday(&time2, NULL);
    report("write", iterations, time1, time2);

    // Benchmark 3: context switch
    // Create the pipes.
    if (pipe(pipe1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    // Fork into two processes which read and write to these pipes.
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // Ensure both processes run on CPU 0.
    CPU_ZERO(&cpu_set);
    CPU_SET(0, &cpu_set);
    if (sched_setaffinity(0, sizeof cpu_set, &cpu_set) == -1) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
    // Run the benchmark.
    if (pid == 0) {
        for (unsigned int i = 0; i < iterations; i++) {
            read(pipe1[0], &buffer, 1);
            write(pipe2[1], &buffer, 1);
        }
        if (close(pipe1[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        if (close(pipe1[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else {
        gettimeofday(&time1, NULL);
        for (unsigned int i = 0; i < iterations; i++) {
            write(pipe1[1], &buffer, 1);
            read(pipe2[0], &buffer, 1);
        }
        gettimeofday(&time2, NULL);
        if (close(pipe2[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        if (close(pipe2[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        // There are twice as many iterations because of the two processes.
        report("context switch", 2 * iterations, time1, time2);
        exit(EXIT_SUCCESS);
    }
}
