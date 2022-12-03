#include "fasta_utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cstring>
#include <assert.h>
using namespace std;

int main(int argc, char* argv[])
{
    assert(argc == 2);
    string seq = read_fasta_seq(argv[1]);
    
    unordered_map<uint64_t, int> table = build_kmer_table(seq, 10);

    // debug
    for (auto const &pair: table) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
    
    return 0;
}