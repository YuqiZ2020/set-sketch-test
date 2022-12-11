#!/bin/bash

printf "parameter testing\n" > param_testing_ecoli/k_test/k_test.txt
m=12
b=1.5
a=20
q=62

for k in 4 8 12 16 20 24 28 31; do
    start=$SECONDS
    ./benchmark-param.out param_test_set/ecoli/ param_testing_ecoli/k_test/output_${k}.txt $m $b $a $q $k
    duration=$(( SECONDS - start ))
    printf "%d %d\n" $k $duration >> param_testing_ecoli/k_test/k_test.txt 
done
