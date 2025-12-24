#!/usr/bin/env bash

set -o nounset

# The program can be pinned to a given CPU with the help of the taskset
# command. With this change, the low-page-count behaviour becomes more
# consistent, with access times around 6 ns.
#
# If the task isn't pinned, the scheduler might move the process to another
# CPU, invalidating the per-core L1 and L2 caches and requiring them to warm up
# on the new core, thereby skewing the measurements.
taskset --cpu-list 0 ./meas3.sh

