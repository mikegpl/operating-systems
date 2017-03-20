#!/bin/bash

DELIMITER="\n\n\n"

printf "\nMaking static \n"
make benchmark_static
printf "Static  - basic\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt O0\n"
make benchmark_staticO0
printf "Static  - opt O0\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt O1\n"
make benchmark_staticO1
printf "Static  - opt O1\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt O2\n"
make benchmark_staticO2
printf "Static  - opt O2\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt Os\n"
make benchmark_staticOs
printf "Static  - opt Os\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt O3\n"
make benchmark_staticO3
printf "Static  - opt O3\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking static  - opt Ofast\n"
make benchmark_staticOfast
printf "Static  - opt Ofast\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

make clean

printf "\nMaking shared \n"
make benchmark_shared
printf "Shared  - basic \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking shared  - opt O0\n"
make benchmark_sharedO0
printf "Shared - opt O0\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking shared  - opt O1\n"
make benchmark_sharedO1
printf "Shared - opt O1 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dshared  - opt O2\n"
make benchmark_sharedO2
printf "Shared - opt O2 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking shared  - opt Os\n"
make benchmark_sharedOs
printf "Shared - opt Os \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking shared  - opt O3\n"
make benchmark_sharedO3
printf "Shared - opt O3 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking shared  - opt Ofast\n"
make benchmark_sharedOfast
printf "Shared - opt Ofast \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

make clean

printf "\nMaking dynamic \n"
make benchmark_dynamic
printf "Dynamic  - basic\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt O0\n"
make benchmark_dynamicO0
printf "Dynamic - opt O0\n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt O1\n"
make benchmark_dynamicO1
printf "Dynamic - opt O1 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt O2\n"
make benchmark_dynamicO2
printf "Dynamic - opt O2 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt Os\n"
make benchmark_dynamicOs
printf "Dynamic - opt Os \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt O3\n"
make benchmark_dynamicO3
printf "Dynamic - opt O3 \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

printf "\nMaking dynamic  - opt Ofast\n"
make benchmark_dynamicOfast
printf "Dynamic - opt Ofast \n" >> results.txt
./benchmark >> results.txt
printf $DELIMITER >> results.txt

make clean

clear
printf "Results saved to results.txt\n"