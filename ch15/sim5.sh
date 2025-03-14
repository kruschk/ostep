#!/usr/bin/env bash

set -o nounset

declare -ir Addresses=100;

printf 'Seed\tLimit\tRatio\n'
for seed in {0..7}
do
    for limit in {0..1024..8}
    do
        declare -i Violations
        Violations=$(./relocation.py --addresses=$Addresses \
                                     --asize=1k \
                                     --compute \
                                     --l="$limit" \
                                     --physmem=16k \
                                     --seed="$seed" \
                     | grep --count --fixed-strings VIOLATION)
        printf '%b\t%b\t%b\n' \
               "$seed" \
               "$limit" \
               "$(python -c "print(($Addresses - $Violations) / $Addresses)")"
    done
done
