#include "./c++/performance_test.cpp"

int main(int argc, char* argv[]) {

    uint64_t numCycles = 100;

    assert(argc==4);
    uint64_t seed = atol(argv[1]);
    uint32_t hashSize = atoi(argv[2]);
    uint64_t dataSize = atol(argv[3]);

    mt19937_64 rng(seed);

    testPMinHash(rng, hashSize, dataSize, numCycles);
    return 0;
}
