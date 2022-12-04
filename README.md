# SetSketch: Filling the Gap between MinHash and HyperLogLog

## How to run

```
g++ -O3 -std=c++17 -fopenmp -Wall src/set-sketch-4-af-benchmark.cpp -o benchmark.out
./benchmark.out test/hw2q1_A.fastq test/short_fasta.fastq
```

This repository contains the source code to reproduce all the results and figures presented in the paper ["SetSketch: Filling the Gap between MinHash and HyperLogLog"](http://vldb.org/pvldb/vol14/p2244-ertl.pdf) which was accepted at [VLDB 2021](https://vldb.org/2021/). An [extended paper version](https://arxiv.org/abs/2101.00314) that includes mathematical proofs and additional results is available on arXiv.

## Abstract
MinHash and HyperLogLog are sketching algorithms that have become indispensable for set summaries in big data applications. While HyperLogLog allows counting different elements with very little space, MinHash is suitable for the fast comparison of sets as it allows estimating the Jaccard similarity and other joint quantities. This work presents a new data structure called SetSketch that is able to continuously fill the gap between both use cases. Its commutative and idempotent insert operation and its mergeable state make it suitable for distributed environments. Fast, robust, and easy-to-implement estimators for cardinality and joint quantities, as well as the ability to use SetSketch for similarity search, enable versatile applications. The presented joint estimator can also be applied to other data structures such as MinHash, HyperLogLog, or HyperMinHash, where it even performs better than the corresponding state-of-the-art estimators in many cases.

## Steps to reproduce the results and figures on Windows 10
1. Install Windows Subsystem for Linux (WSL) with [Ubuntu 20.04 LTS](https://www.microsoft.com/store/productId/9N6SVWS3RX71)

2. Install required packages:
   ```
   sudo apt update && sudo apt install gradle g++ libboost-dev python3-matplotlib python3-scipy texlive texlive-latex-extra texlive-fonts-extra
   ```
3. Clone repository including submodules:
   ```
   git clone --recursive https://github.com/dynatrace-research/set-sketch-paper.git
   ```
4. Switch to `set-sketch-paper` directory:
   ```
   cd set-sketch-paper
   ```
5. Run simulations (takes several hours):
   ```
   gradle runCardinalityTest runJointTest runPerformanceTest
   ```
6. Generate all figures:
   ```
   gradle pdfFigures
   ```
