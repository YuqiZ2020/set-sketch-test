#ifndef _FASTA_UTILS_HPP_
#define _FASTA_UTILS_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cstring>
#include <assert.h>
#include <unordered_set>
using namespace std;


// TODO: wrapper for pairwise files

string read_fasta_seq(string filename) {
    string seq;
    ifstream inputfile;
    inputfile.open(filename.c_str());

    string line;
    while (getline(inputfile, line)){
        // ignore name
        getline(inputfile, line);
        seq += line;
        getline(inputfile, line); // ignore +
        getline(inputfile, line); // ignore quality

    } 
    inputfile.close();
    return seq;
}

uint64_t kmer_to_int (string kmer){
    // first bit is a placeholder bit
    // 2 bits per nucleotide, so 64 bit unsigned integer accept k <= 31
    assert(kmer.length() < 32);
    uint64_t token = 1;

    // 00000000000001
    // 00000000000100
    // A: 00
    // C: 01
    // G: 10
    // T: 11

    for (char c : kmer){
        token <<= 2;
        // 'A' assigned to 0
        if (c == 'C') token += 1;
        if (c == 'G') token += 2;
        if (c == 'T') token += 3;
    }
    return token;
}

unordered_map<uint64_t, int> build_kmer_table(string seq, int k){
    unordered_map<uint64_t, int> table;

    for (size_t i = 0; i < seq.length() - k + 1; i++)
    {
        string substring = seq.substr(i, k);        
        int kmer = kmer_to_int(substring);
        if (table.find(kmer) != table.end()){
            table[kmer]++;
        }
        else table[kmer] = 1;
    }
    return table;
}


// TODO: BUILD KMER SET
unordered_set<uint64_t> build_kmer_set(string seq, int k){
    unordered_set<uint64_t> set;

    for (size_t i = 0; i < seq.length() - k + 1; i++)
    {
        string substring = seq.substr(i, k);        
        int kmer = kmer_to_int(substring);
        set.insert(kmer);
    }
    return set;
}
#endif //fasta_utils.hpp