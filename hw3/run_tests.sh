#!/bin/bash
make

for i in {1..4}
do
   echo "testing test ${i}"

./hw3.out < hw3-tests/t${i}.in > hw3-tests/t${i}.res

diff hw3-tests/t${i}.out hw3-tests/t${i}.res
done

echo "finished"
