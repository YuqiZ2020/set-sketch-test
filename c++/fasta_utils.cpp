#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <cstring>

using namespace std;

void read_fasta(string filename, )

void build_kmer_table(vector<pair<string, string>> &reads, int K, unordered_map<string, set<size_t>> &table){
    for (size_t idx=0; idx<reads.size(); idx++){
        string id = reads[idx].first;
        string seq = reads[idx].second;

        for (size_t i=0; i<seq.length()-K+1; i++){
            string kmer = seq.substr(i, K);
            table[kmer].insert(idx);
        }
    }
}