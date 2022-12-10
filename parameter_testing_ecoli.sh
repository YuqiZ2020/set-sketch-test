#!/bin/bash

printf "parameter testing\n" > file_ecoli.txt

counter = 1
for m in 1 10 20 40; do
    for b in  1.5 1.2 1.01; do 
        for a in 0.1 5 10 20; do
            for q in 0.1 10 20; do 
                for k in 1 10 15 20 30; do 
		    start=$SECONDS
		    ./benchmark-param.out test_ecoli/ output_ecoli/output_${counter}.txt $m $b $a $q $k  
		    duration=$(( SECONDS - start ))
		    printf "%d %d %f %f %f %d %d\n" $counter $m $b $a $q $k $duration >> file_ecoli.txt
		    counter=$[$counter +1]
		    #counter=$((counter+1))
done
done
done
done
done

