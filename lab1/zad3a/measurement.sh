#!/bin/bash

DELIMITER="\n\n\n"

printf "\nMaking static version\n"
make benchmark_static
printf "Static version\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt
make clean

make benchmark_shared
printf "\nMaking shared version\n"
printf "Shared version\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt
make clean

make benchmark_dynamic
printf "\nMaking dynamic version\n"
printf "Dynamic version\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt
make clean

printf "Results saved to results.txt"