#!/usr/bin/env bash

# By observing the stability of results between runs with different numbers of
# trials, an approximate minimum of 100,000 trials is required to attain
# reliable measurements on this machine.

set -o nounset

function Measure () {
    declare -r Time=$(./meas2 "$1" 100000)
    printf "%*d\t%*f\n" ${#Column1} "$Pages" ${#Column2} "$Time"
}

declare -i Pages
declare -r Column1="Pages"
declare -r Column2="Time (ns)"

printf "%s\t%s\n" "$Column1" "$Column2"

for Pages in {1..31}
do
    Measure "$Pages"
done

for Pages in {32..63..8}
do
    Measure "$Pages"
done

for Pages in {64..127..16}
do
    Measure "$Pages"
done

for Pages in {128..255..32}
do
    Measure "$Pages"
done

for Pages in {256..511..64}
do
    Measure "$Pages"
done

for Pages in {512..1023..128}
do
    Measure "$Pages"
done

for Pages in {1024..2047..256}
do
    Measure "$Pages"
done

for Pages in {2048..4095..512}
do
    Measure "$Pages"
done

for Pages in {4096..8191..1024}
do
    Measure "$Pages"
done

for Pages in {8192..16383..2048}
do
    Measure "$Pages"
done
