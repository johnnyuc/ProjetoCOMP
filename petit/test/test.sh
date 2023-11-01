#!/bin/sh

rm -f fibonacci gcd primes factorial

echo "[fibonacci]"
../source/petit < fibonacci.pt > fibonacci.ll
llc fibonacci.ll -o fibonacci.s
clang fibonacci.s ../source/io.c -o fibonacci
./fibonacci

echo "[gcd]"
../source/petit < gcd.pt > gcd.ll
llc gcd.ll -o gcd.s
clang gcd.s ../source/io.c -o gcd
./gcd

echo "[primes]"
../source/petit < primes.pt > primes.ll
llc primes.ll -o primes.s
clang primes.s ../source/io.c -o primes
./primes

echo "[factorial]"
../source/petit < factorial.pt > factorial.ll
llc factorial.ll -o factorial.s
clang factorial.s ../source/io.c -o factorial
#./factorial
