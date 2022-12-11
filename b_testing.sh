#!/bin/bash

printf "parameter testing\n" > param_testing_ecoli/b_test/b_test.txt
m=12
#b=1.5
a=20
q=62
k=11
#for b in  1.75 1.5 1.25 1.01 1.1 1.15 1.18 1.2 1.22 1.9 1.28 1.33 1.4 1.95; do
for b in 1.15 1.18 1.2 1.22 1.9 1.28 1.33 1.4 1.95; do
    start=$SECONDS
    ./benchmark-param.out param_test_set/ecoli/ param_testing_ecoli/b_test/output_${b}.txt $m $b $a $q $k
    duration=$(( SECONDS - start ))
    printf "%f %d\n" $b $duration >> param_testing_ecoli/b_test/b_test.txt
done
