#!/bin/bash

# Author: Akeel Ali
# Date: Dec. 2015
# Script testing the rounds program by printing the solutions
# using both implementations and comparing their results.

CARDS_IN_DECK=150

make clean > /dev/null
make all MACROS=-DBASH_RUN=1 > /dev/null

rm naive_results.txt &> /dev/null
rm rotation_results.txt &> /dev/null

echo Testing naive implementation...
for ((i=1;i<="$CARDS_IN_DECK";i++))
do
./rounds -n $i >> naive_results.txt
done

echo Testing rotation implementation...
for ((i=1;i<="$CARDS_IN_DECK";i++))
do
./rounds -r -n $i >> rotation_results.txt
done

echo Comparing results...
echo 
RESULTS_DIFF=$(diff naive_results.txt rotation_results.txt) 

if [ "$RESULTS_DIFF" != "" ] 
then 
	echo Test Failed; 
	echo "DIFF BETWEEN BOTH IMPLEMENTATIONS"
	echo "================================="
	echo $RESULTS_DIFF
	echo "================================="
	echo Test Failed
else 
	echo Test Passed!;
fi

#Cleanup
make clean &> /dev/null
rm naive_results.txt rotation_results.txt
