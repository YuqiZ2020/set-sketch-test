#!/bin/bash

#printf "parameter testing\n" > q_test.txt
m=12
b=1.5
a=20
#q=62
k=11
#for q in 0.1 10 20 40 60 80; do 
#for q in 90 100 110; do
for q in 30; do
    start=$SECONDS
    ./benchmark-param.out param_test_set/fish/ q_test/output_${q}.txt $m $b $a $q $k  
    duration=$(( SECONDS - start ))
    printf "%f %d\n" $q $duration >> q_test.txt
done
