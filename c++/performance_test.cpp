//#######################################
//# Copyright (C) 2019-2020 Otmar Ertl. #
//# All rights reserved.                #
//#######################################

#include "./minhash.hpp"
#include "./bitstream_random.hpp"

#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <cassert>
#include <limits>

using namespace std;

struct ExtractFunction {
    uint64_t operator()(const uint64_t& d) const {
        return d;
    }
    uint64_t operator()(const std::tuple<uint64_t,double>& d) const {
        return std::get<0>(d);
    }
};

class RNGFunction {
    const uint64_t seed;
public:

    RNGFunction(uint64_t seed) : seed(seed) {}
    
    WyrandBitStream operator()(uint64_t x) const {
        return WyrandBitStream(x, seed);
    }
};

class RNGFunctionForSignatureComponents {
    const uint64_t seed;
public:

    RNGFunctionForSignatureComponents(uint64_t seed) : seed(seed) {}
    
    WyrandBitStream operator()(uint32_t x) const {
        return WyrandBitStream(x, seed);
    }
};

struct WeightFunction {
    double operator()(const std::tuple<uint64_t,double>& d) const {
        return std::get<1>(d);
    }
};

template <typename H, typename D>
void testCase(H& h, uint64_t dataSize, uint32_t hashSize, uint64_t numCycles, const D& testData, const string& algorithmLabel, const string& distributionLabel) {

    assert(numCycles = testData.size());

    uint64_t consumer = 0;
    chrono::steady_clock::time_point tStart = chrono::steady_clock::now();
    for(int i=0; i<32; i++){
        for (const auto& data :  testData) {
            vector<uint64_t> result = h(data);
            for(uint64_t r : result) consumer ^= r;
        }
    }
    chrono::steady_clock::time_point tEnd = chrono::steady_clock::now();
    double avgHashTime = chrono::duration_cast<chrono::duration<double>>(tEnd - tStart).count() / 32;

    ofstream resultsFile("results.txt", ios::app);
    if (!resultsFile) {
        cerr << "Error: Unable to open results.txt for writing." << endl;
        return;
    }

    // Configure the file stream for scientific notation with maximum precision
    resultsFile << setprecision(numeric_limits<double>::max_digits10) << scientific;

    // Write results to the file
    resultsFile << "Algoritmo: "
                << algorithmLabel << " || Número de Genomas: "
                << numCycles << " || Tamaño de Hash: "
                << hashSize << " || Tamaño de data: "
                << dataSize << " || Tiempo de ejecución: "
                << avgHashTime << " || Nombre de la distribución de los pesos: "
                << distributionLabel << " || Consumer: "
                << consumer << endl;
}

template<template<typename, typename, typename, typename> typename H, typename D, typename GEN>
void testWeightedCase(GEN& rng, uint64_t dataSize, uint32_t hashSize, uint64_t numCycles, const D& testData, const string& algorithmLabel, const string& distributionLabel) {
    H<uint64_t, ExtractFunction, RNGFunction, WeightFunction> h(hashSize, ExtractFunction(), RNGFunction(rng()));
    testCase(h, dataSize, hashSize, numCycles, testData, algorithmLabel, distributionLabel);
}

template<template<typename, typename, typename> typename H, typename D, typename GEN>
void testUnweightedCase(GEN& rng, uint64_t dataSize, uint32_t hashSize, uint64_t numCycles, const D& testData, const string& algorithmLabel) {
    H<uint64_t, ExtractFunction, RNGFunction> h(hashSize, ExtractFunction(), RNGFunction(rng()));
    testCase(h, dataSize, hashSize, numCycles, testData, algorithmLabel, "unweighted");
}

template<typename D, typename GEN>
void testUnweightedCaseOnePermutationHashingWithOptimalDensification(GEN& rng, uint64_t dataSize, uint32_t hashSize, uint64_t numCycles, const D& testData, const string& algorithmLabel) {
    OnePermutationHashingWithOptimalDensification<uint64_t, ExtractFunction, RNGFunction, RNGFunctionForSignatureComponents> h(hashSize, ExtractFunction(), RNGFunction(rng()), RNGFunctionForSignatureComponents(rng()));
    testCase(h, dataSize, hashSize, numCycles, testData, algorithmLabel, "unweighted");
}
 
template <typename GEN> double generatePareto(GEN& rng, double scale, double shape) {
    std::uniform_real_distribution<double> distributionUniform(0., 1.); 
    return scale * pow(1 - distributionUniform(rng), -1./shape);
}

template <typename GEN> void testPMinHash(GEN& rng, uint32_t hashSize,uint64_t dataSize, uint64_t numCycles) {
    {
        const string distributionLabel = "empirical";
        // generate test data
        std::uint64_t code;
        double freq;
        std::vector<std::vector<std::tuple<std::uint64_t, double> > > testData;
        std::string root_path = "/mnt/c/projects/ProyectoSemestralTMGVD/reduced_Genomes";

        for (const auto& genome: std::filesystem::directory_iterator(root_path)) {
            if (!std::filesystem::is_directory(genome))
                continue;

            testData.push_back({});
            for (const auto& g: std::filesystem::directory_iterator(genome)) {
                std::ifstream gen(g.path().string());
                std::string buffer;
                while (std::getline(gen, buffer)) {
                    std::stringstream ss(buffer);
                    ss >> code;
                    ss >> freq;
                    testData.back().push_back(std::make_tuple(code, freq));
                }
            }
        }
    
        testWeightedCase<PMinHash>(rng, dataSize, hashSize, numCycles, testData, "P-MinHash", distributionLabel);
    }
    /* { // unweighted case (weights constant 1)
        // generate test data
        vector<vector<uint64_t>> testData(numCycles);
        for (uint64_t i = 0; i < numCycles; ++i) {
            vector<uint64_t> d(dataSize);
            for (uint64_t j = 0; j < dataSize; ++j) {
                uint64_t data = rng();
                d[j] = data;
            }
            testData[i] = d;
        }


        testUnweightedCase<PMinHash>(rng, dataSize, hashSize, numCycles, testData, "P-MinHash");
        if(hashSize > 1) testUnweightedCase<ProbMinHash3a>(rng, dataSize, hashSize, numCycles, testData, "ProbMinHash3a");
        if(hashSize > 1) testUnweightedCase<ProbMinHash4>(rng, dataSize, hashSize, numCycles, testData, "ProbMinHash4");
        testUnweightedCase<MinHash>(rng, dataSize, hashSize, numCycles, testData, "MinHash");
    }
*/
}

template <typename GEN> void testProbMinHash3a(GEN& rng, uint32_t hashSize,uint64_t dataSize, uint64_t numCycles) {
    {
        const string distributionLabel = "empirical";
        // generate test data
        std::uint64_t code;
        double freq;
        std::vector<std::vector<std::tuple<std::uint64_t, double> > > testData;
        std::string root_path = "/mnt/c/projects/ProyectoSemestralTMGVD/reduced_Genomes";

        for (const auto& genome: std::filesystem::directory_iterator(root_path)) {
            if (!std::filesystem::is_directory(genome))
                continue;

            testData.push_back({});
            for (const auto& g: std::filesystem::directory_iterator(genome)) {
                std::ifstream gen(g.path().string());
                std::string buffer;
                while (std::getline(gen, buffer)) {
                    std::stringstream ss(buffer);
                    ss >> code;
                    ss >> freq;
                    testData.back().push_back(std::make_tuple(code, freq));
                }
            }
        }
    
        if(hashSize > 1) testWeightedCase<ProbMinHash3a>(rng, dataSize, hashSize, numCycles, testData, "ProbMinHash3a", distributionLabel);
    }
}

template <typename GEN> void testProbMinHash4(GEN& rng, uint32_t hashSize,uint64_t dataSize, uint64_t numCycles) {
    {
        const string distributionLabel = "empirical";
        // generate test data
        std::uint64_t code;
        double freq;
        std::vector<std::vector<std::tuple<std::uint64_t, double> > > testData;
        std::string root_path = "/mnt/c/projects/ProyectoSemestralTMGVD/reduced_Genomes";

        for (const auto& genome: std::filesystem::directory_iterator(root_path)) {
            if (!std::filesystem::is_directory(genome))
                continue;

            testData.push_back({});
            for (const auto& g: std::filesystem::directory_iterator(genome)) {
                std::ifstream gen(g.path().string());
                std::string buffer;
                while (std::getline(gen, buffer)) {
                    std::stringstream ss(buffer);
                    ss >> code;
                    ss >> freq;
                    testData.back().push_back(std::make_tuple(code, freq));
                }
            }
        }
    
        if(hashSize > 1) testWeightedCase<ProbMinHash4>(rng, dataSize, hashSize, numCycles, testData, "ProbMinHash4", distributionLabel);
    }
}