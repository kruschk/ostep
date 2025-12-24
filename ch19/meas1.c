// According to the gettimeofday man pages, gettimeofday is deprecated in
// favour of clock_gettime; therefore, the latter is used.
//
// According to clock_getres, the CLOCK_PROCESS_CPUTIME_ID clock has a
// resolution of 1 ns, so any operation measured using clock_gettime should
// take significantly longer than 1 ns or be repeated enough times that the
// collection of operations does. In the latter case, the interval would need
// to be divided by the number of repetitions. Note that the call to
// clock_gettime and any other logic within a loop will incur some overhead,
// which should be quantified to get the most accurate possible measure.

#include <stdio.h>
#include <time.h>

#define ITERATIONS 1000

long int timespec_to_ns(struct timespec time) {
    return 1'000'000'000 * time.tv_sec + time.tv_nsec;
}

int main(void) {
    struct timespec resolution;
    struct timespec timestamp;
    struct timespec timestamps[ITERATIONS];
    // Get the clock's resolution.
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &resolution);
    printf("Resolution: %ld ns\n", timespec_to_ns(resolution));
    // Record a few timestamps.
    for (size_t i = 0; i < ITERATIONS; i++) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timestamp);
        timestamps[i] = timestamp;
    }
    // Print the timestamps.
    printf("Timestamps (ns)\n");
    for (size_t i = 0; i < ITERATIONS; i++) {
        printf("%ld\n", timespec_to_ns(timestamps[i]));
    }
    // Determine the average overhead associated with timing.
    long int average = 0;
    for (size_t i = 1; i < ITERATIONS; i++) {
        average += timespec_to_ns(timestamps[i])
                   - timespec_to_ns(timestamps[i - 1]);
    }
    printf("Average overhead: %ld ns", average / ITERATIONS);
}
