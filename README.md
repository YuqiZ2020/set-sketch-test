# Adapting SetSketch for alignment-free sequence comparison

This repository adapts the **SetSketch** algorithm to perform alignment free genomic sequence comparison following the specifications set by the [AFproject](https://afproject.org/app/).

## How to run
1. Clone repository including submodules:
   ```
   git clone --recursive https://github.com/YuqiZ2020/set-sketch-test.git
   ```
2. Switch to `set-sketch-test` directory:
   ```
   cd set-sketch-paper
   ```
3. Upload your FASTA files into the test folder using the following structure
   ```
   src\
      c++\
      fasta_utils.hpp
      set-sketch-4-af-benchmark.cpp
   test\
      A.fasta
      B.fasta
      ...
   ```
4. Build the benchmark executable
   ```
   g++ -O3 -std=c++17 -fopenmp -Wall src/set-sketch-4-af-benchmark.cpp -o benchmark.out
   ```
   
5. Running the benchmark will generate all-pairs distance between FASTA sequences in the test folder.
   ```
   ./benchmark.out test/ output.txt 
   ```

6. If you want to change different parameters, you can run:
   ```
   g++ -O3 -std=c++17 -fopenmp -Wall src/set-sketch-4-af-w-param.cpp -o benchmark-param.out
   ./benchmark-param.out data_folder/ output_file.txt num-register base a q k
   ```
   for example, 
   ```
   ./benchmark-param.out test/ output.txt 12 1.5 20 62 11
   ```
## SetSketch
The original algorithm by Otmar Ertl was presented in the paper ["SetSketch: Filling the Gap between MinHash and HyperLogLog"](http://vldb.org/pvldb/vol14/p2244-ertl.pdf) which was accepted at [VLDB 2021](https://vldb.org/2021/). An [extended paper version](https://arxiv.org/abs/2101.00314) that includes mathematical proofs and additional results is available on arXiv. The author's [original implementation](https://github.com/dynatrace-research/set-sketch-paper) in C++ and additional Python visualization tools is available on Github.
