// Copyright (c) 2012-2021 Dynatrace LLC. All rights reserved.
//
// This software and associated documentation files (the "Software")
// are being made available by Dynatrace LLC for purposes of
// illustrating the implementation of certain algorithms which have
// been published by Dynatrace LLC. Permission is hereby granted,
// free of charge, to any person obtaining a copy of the Software,
// to view and use the Software for internal, non-productive,
// non-commercial purposes only 鈥?the Software may not be used to
// process live data or distributed, sublicensed, modified and/or
// sold either alone or as part of or in combination with any other
// software.
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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

using namespace std;

class DummySketch {
    uint64_t state = 0;
public:
    void add(uint64_t l) {
        state += l;
    }

    uint64_t getState() const {
        return state;
    }
};

class DummyConfig {
public:
    DummySketch create() const {
        return DummySketch();
    }

    uint32_t getNumRegisters() const {return 0;}

    std::string getName() const {return "DummyConfig";}
};

template<typename C>
static string getFileName(const C& config, const std::string& aggregationModeDescription) {
    stringstream ss;
    ss << "data/performance_test(";
    appendInfo(ss, config);
    ss << "aggregationMode=" << aggregationModeDescription << ";";
    ss << ").csv";
    return ss.str();
}

static const uint64_t wyhashSecret[4] = {UINT64_C(0xbbc3be7c929be0ca), UINT64_C(0x2cfbaea4f1028efe), UINT64_C(0xc04f8e039a014db9), UINT64_C(0x28b6e9976c77fe03)};

class RandomNumbers {
    uint64_t seed;
    uint64_t mSize;
public:

    class RandomNumberIterator {
        uint64_t seed;
        uint64_t index;
    public:

        RandomNumberIterator(uint64_t seed, uint64_t index): seed(seed), index(index) {}

        uint64_t operator*() const {
            return wyhash(&index, 8, seed, wyhashSecret);
        }

        RandomNumberIterator operator++() {
            index += 1;
            return *this;
        }

        bool operator!=(RandomNumberIterator& it) const {
            return index != it.index;
        }
    };

    RandomNumbers(uint64_t seed, uint64_t size) : seed(seed), mSize(size) {}

    RandomNumberIterator begin() const {
        return RandomNumberIterator(seed, UINT64_C(0));
    }

    RandomNumberIterator end() const {
        return RandomNumberIterator(seed, mSize);
    }

    uint64_t size() const {return mSize;}
};


struct StreamAggregation {

    template<typename S, typename D>
    void aggregate(S& sketch, const D& data) const {
        for(uint64_t d : data) {
            sketch.add(d);
        }
    }

    string getDescription() const {
        return "stream";
    }

};

struct BulkAggregation {

    template<typename S, typename D>
    void aggregate(S& sketch, const D& data) const {
        sketch.addAll(data);
    }

    string getDescription() const {
        return "bulk";
    }

};

template<typename S> uint64_t consumeState(const S& sketch) {
    uint64_t sum = 0;
    for(auto r : sketch.getState()) sum += r;
    return sum;
}

uint64_t consumeState(const DummySketch& sketch) {
    return sketch.getState();
}

template<typename C>
std::vector<uint64_t> getCardinalities(C&& config) {

    vector<uint64_t> cardinalities;
    cardinalities.push_back(1);
    cardinalities.push_back(2);
    cardinalities.push_back(5);
    cardinalities.push_back(10);
    cardinalities.push_back(20);
    cardinalities.push_back(50);
    cardinalities.push_back(100);
    cardinalities.push_back(200);
    cardinalities.push_back(500);
    cardinalities.push_back(1000);
    cardinalities.push_back(2000);
    cardinalities.push_back(5000);
    cardinalities.push_back(10000);
    cardinalities.push_back(20000);
    cardinalities.push_back(50000);
    cardinalities.push_back(100000);
    if (!std::is_same<typename std::remove_reference<C>::type, MinHashConfig>::value) {
        // do not add large cardinalities for minhash, test would take too much time otherwise
        cardinalities.push_back(200000);
        cardinalities.push_back(500000);
        cardinalities.push_back(1000000);
        cardinalities.push_back(2000000);
        cardinalities.push_back(5000000);
        cardinalities.push_back(10000000);
    }

    return cardinalities;
}

// uint64_t kmerToNum(string s) {
// 	int l = s.length();
// 	uint64_t d = 0;
// 	std::map<char, int> baseToInt = {
//         {'A', 1},
//         {'T', 2},
//         {'C', 3},
//         {'G', 4},
// };
// 	if (l < 64) {
// 		for (int i = 0; i < l; ++i) {
// 			int t = baseToInt(toupper(s[i]));
// 			d += (t * i * 10);
// 		}
// 		return d;
// 	}
// 	else {
// 		cerr << "Kmer too long" << endl;
// 		return 0;
// 	}
// }

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
    
    // const vector<string> estimatorLabels = estimator.getJointEstimateLabels();
    // const size_t numEstimates = estimatorLabels.size();
    // vector<vector<vector<JointEstimationResult>>> esti+mates(numEstimates, vector<vector<JointEstimationResult>>(numExamples));
    
    JointEstimationResult res = estimator.estimateJointNew(sketch1.getState(), sketch2.getState());
    return res.getJaccard(); 
}

void appendInfo(std::ostream& os, const DummyConfig& config)
{
    os << "dummy;";
}

int main(int argc, char* argv[]) {

    mt19937_64 dataSeedRng(UINT64_C(0x291be5007a3d06fc));

    // test(dataSeedRng(), DummyConfig(), StreamAggregation());

    std::vector<uint64_t> registerSizeExponents = {8, 12};

    ofstream outputfile; 
    outputfile.open(argv[3]);

    for(uint64_t registerSizeExponent : registerSizeExponents) {
        const uint32_t numRegisters = UINT32_C(1) << registerSizeExponent;

        // TODO: wrapper for pairs of fasta
        // pairwise files from directory
        // run in loop

        string seqA, seqB;

        // FIXME!
        // fileA = argv[1];
        // fileB = argv[2];

        seqA = read_fasta_seq(argv[1]);
        seqB = read_fasta_seq(argv[2]);

	int k = 10;
        unordered_set<uint64_t> kmerA = build_kmer_set(seqA, k);
        unordered_set<uint64_t> kmerB = build_kmer_set(seqB, k);

        float result = test(dataSeedRng(), SetSketchConfig1<RegistersWithLowerBound<uint8_t>>(numRegisters, 2., 20, 62), kmerA, kmerB);
	cout << "jcard idx " << result << endl;


        // Output:
        // Simple simple text file with three tab-separated columns: first two columns store identifiers of two sequences being compared, and third column has a numerical distance value of this comparison.

        // Example of Text File Format 

        // A   B   8.876
        // A   C   6.120

        // outputfile << fileA << '\t' << fileB << '\t' << result << endl;
    }

    outputfile.close();

    return 0;
}
