#include <iostream>
#include <vector>

#include "../include/CSVReader.hpp"
#include "../include/Material.hpp"

int main()
{
    std::cout << "==============================\n";
    std::cout << "Quantum Cascade Laser Simulator\n";
    std::cout << "==============================\n\n";

    CSVReader reader;

    auto data = reader.read("data/materials.csv");

    std::cout << "Materials loaded:\n\n";

    for (size_t i = 1; i < data.size(); i++)
    {
        Material material;

        material.name = data[i][0];
        material.effectiveMass = std::stod(data[i][1]);
        material.bandGap = std::stod(data[i][2]);
        material.dielectricConstant = std::stod(data[i][3]);

        std::cout << "- " << material.name << '\n';
    }

    return 0;
}