#!/bin/bash

make clean
make
cp lexer ../mooshak_testing/uccompiler
cd ../mooshak_testing
./compare_files.sh
