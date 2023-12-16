#!/bin/bash

make clean
make
cp uccompiler ../mooshak_testing/uccompiler
cd ../mooshak_testing
./compare_files.sh
