g++ -O3 -std=c++17 -fopenmp -Wall src/set-sketch-4-af-w-param.cpp -o benchmark-param.out
for i in {4,8,12,16,20,24,28,32}
{
time ./benchmark-param.out test/ fish-output-k-$i.txt 12 1.5 20 62 $i
}
