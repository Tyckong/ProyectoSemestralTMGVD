#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

int main(void)
{
    std::uint64_t code;
    double freq;
    std::vector<std::vector<std::tuple<std::uint64_t, double> > > testData;
    std::string root_path = "./reduced_Genomes";

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

    return 0;
}
