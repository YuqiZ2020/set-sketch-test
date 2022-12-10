#include "c++/sketch.hpp"
#include "c++/util.hpp"
#include "c++/wyhash/wyhash.h"
#include "fasta_utils.hpp"

#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <set>
#include <unordered_set>
#include <filesystem>
#include <math.h>

namespace fs = std::filesystem;
using namespace std;

static const uint64_t wyhashSecret[4] = {UINT64_C(0xbbc3be7c929be0ca), UINT64_C(0x2cfbaea4f1028efe), UINT64_C(0xc04f8e039a014db9), UINT64_C(0x28b6e9976c77fe03)};


template<typename C>
float test(uint64_t seed, C&& config, unordered_set<uint64_t>&kmer_set1, unordered_set<uint64_t>& kmer_set2) {
	
	auto sketch1 = config.create();
	for(uint64_t i : kmer_set1) {
        sketch1.add(i);
    }
	
	auto sketch2 = config.create();
	for(uint64_t i : kmer_set2) {
        sketch2.add(i);
    }

	typedef std::remove_reference_t<decltype(config.getEstimator())> estimator_type;
    const estimator_type& estimator = config.getEstimator();
    
    JointEstimationResult res = estimator.estimateJointNew(sketch1.getState(), sketch2.getState());
    return res.getJaccard(); 
}

void getFilePaths(char* fp, vector<string>& filepaths){
    // generate pairs of FASTA files
    const fs::path folderpath = fp;

    for (const auto& entry : fs::directory_iterator(folderpath)) {
        const auto filename = entry.path().filename().string();
        if (entry.is_regular_file()){
            
            //TODO: add .fastq check

            filepaths.push_back(filename);
        }
    }
}

vector<pair<string, string>> getFilePairs(char* fp){
    vector<string> filepaths;
    vector<pair<string, string>> pairs;

    getFilePaths(fp, filepaths);

    for (size_t i = 0; i < filepaths.size() - 1; i++){
        for (size_t j = i+1; j < filepaths.size(); j++){
            pairs.push_back(make_pair(filepaths[i], filepaths[j]));
        }
    }

    return pairs;
}

float jaccardToDistance (float k, float j){
    return -1/k * log(2 * j / (1 + j));
}

int main(int argc, char* argv[]) {
    // parameters from the original paper
    mt19937_64 dataSeedRng(UINT64_C(0x291be5007a3d06fc));
    const uint64_t registerSizeExponent = 8;
    int k = 30;

    ofstream outputfile; 
    outputfile.open(argv[2]);

    vector<pair<string, string>> pairs = getFilePairs(argv[1]);

    const uint32_t numRegisters = UINT32_C(1) << registerSizeExponent;

    for (auto& x : pairs){
        string fileA, fileB, seqA, seqB, folder;
        folder = argv[1];
        fileA = x.first;
        fileB = x.second;
        seqA = read_fasta_seq(folder + fileA);
        seqB = read_fasta_seq(folder + fileB);

        unordered_set<uint64_t> kmerA = build_kmer_set(seqA, k);
        unordered_set<uint64_t> kmerB = build_kmer_set(seqB, k);

        float result = test(dataSeedRng(), SetSketchConfig1<RegistersWithLowerBound<uint8_t>>(numRegisters, 2., 20, 62), kmerA, kmerB);
    
        // cout << "jcard idx " << result << endl;

        /*
        Output:
        Simple simple text file with three tab-separated columns: first two columns store identifiers of two sequences being compared, and third column has a numerical distance value of this comparison.

        Example of Text File Format 
        A   B   8.876
        A   C   6.
        */

        float dist = jaccardToDistance(k, result);
        outputfile << fileA << '\t' << fileB << '\t' << dist << endl;
    }
    outputfile.close();

    return 0;
}
