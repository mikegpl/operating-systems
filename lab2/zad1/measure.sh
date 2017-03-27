#!/bin/bash
FILESIZES=(4 512 4096 8192)
FILES=""

for FILE in ${FILESIZES[@]}
do
	FILES="$FILES test$FILE"
done

I=0
for FILE in $FILES
do
	printf "Generating: record size ${FILESIZES[I]} \n"
	./main default generate /dev/urandom "${FILE}a1.bin" 5000 ${FILESIZES[I]}
	./main default generate /dev/urandom "${FILE}b1.bin" 2000 ${FILESIZES[I]}
	cp "${FILE}a1.bin" "${FILE}a2.bin"
	cp "${FILE}b1.bin" "${FILE}b2.bin"

	I=$(expr $I + 1)
done

I=0
for FILE in $FILES
do
	printf "Shuffling: record size ${FILESIZES[I]} \n"
	./benchmark lib shuffle "${FILE}a1.bin" 5000 ${FILESIZES[I]} >> res.txt
	./benchmark sys shuffle "${FILE}a2.bin" 5000 ${FILESIZES[I]} >> res.txt
	./benchmark lib shuffle "${FILE}b1.bin" 2000 ${FILESIZES[I]} >> res.txt
	./benchmark sys shuffle "${FILE}b2.bin" 2000 ${FILESIZES[I]} >> res.txt
	rm "${FILE}a2.bin" "${FILE}b2.bin"
	cp "${FILE}a1.bin" "${FILE}a2.bin"
	cp "${FILE}b1.bin" "${FILE}b2.bin"
	I=$(expr $I + 1)
done

I=0
for FILE in $FILES
do
	printf "Sorting: record size ${FILESIZES[I]} \n"
	./benchmark lib sort "${FILE}a1.bin" 5000 ${FILESIZES[I]} >> res.txt
	./benchmark sys sort "${FILE}a2.bin" 5000 ${FILESIZES[I]} >> res.txt
	./benchmark lib sort "${FILE}b1.bin" 2000 ${FILESIZES[I]} >> res.txt
	./benchmark sys sort "${FILE}b2.bin" 2000 ${FILESIZES[I]} >> res.txt
	I=$(expr $I + 1)
done

make clean
rm *.bin