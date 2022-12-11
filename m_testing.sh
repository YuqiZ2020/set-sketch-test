#!/bin/bash

#printf "parameter testing\n" > param_testing_ecoli/m_test/m_test.txt
#m=12
b=1.5
a=20
q=62
k=11

for m in 40 50; do 
#for m in 30 60 70 80; do 
    start=$SECONDS
    ./benchmark-param.out param_test_set/ecoli/ param_testing_ecoli/m_test/output_${m}.txt $m $b $a $q $k
    duration=$(( SECONDS - start ))
    printf "%d %d\n" $m $duration >> param_testing_ecoli/m_test/m_test.txt 
done
