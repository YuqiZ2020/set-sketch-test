#!/bin/bash

printf "parameter testing\n" > param_testing_ecoli/a_test/a_test.txt
m=12
b=1.5
#a=20
q=62
k=11
for a in 0.1 10 20 30 40 60; do 
    start=$SECONDS
    ./benchmark-param.out param_test_set/ecoli/ param_testing_ecoli/a_test/output_${a}.txt $m $b $a $q $k  
    duration=$(( SECONDS - start ))
    printf "%f %d\n" $a $duration >> param_testing_ecoli/a_test/a_test.txt
done
