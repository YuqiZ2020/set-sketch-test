#!/bin/bash

#printf "parameter testing\n" > m_test.txt
b=1.5
a=20
q=62
k=11
#for m in 5 15 25 30 60 70 80; do 
for m in 30 60 70 80; do 
    start=$SECONDS
    ./benchmark-param.out param_test_set/fish/ m_test/output_${m}.txt $m $b $a $q $k  
    duration=$(( SECONDS - start ))
    printf "%d %d\n" $m $duration >> m_test.txt
done
