#!/usr/bin/env bash

if [ $# -ne 1 ]; then
  echo "Usage: ./runsample2.sh CPPSOURCEFILE"
  exit
fi

echo "Compiling and running the C++ file $1..."
g++ -O2 $1 && time ./a.out < sample2.in | ./judge sample2.out
echo "If user time + system time < 10 seconds, you should be fine for 
the real test."
