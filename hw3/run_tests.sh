#!/bin/bash
make

cp hw3 hw3-tests/hw3

cd hw3-tests

./run_tests

echo "finished"
