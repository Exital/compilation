#!/bin/bash

flex scanner.lex
g++ -std=c++17 lex.yy.c hw1.cpp -o hw1.out

for i in {1..17}
do
   echo "testing test ${i}"

./hw1.out < tests/t${i}.in > tests/t${i}_mine.out

diff tests/t${i}.out tests/t${i}_mine.out
done

./pytest2/tester.py -c -e

./pytest2/tester.py -c
echo "finished"
