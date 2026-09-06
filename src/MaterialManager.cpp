#include "../include/MaterialManager.hpp"
#include "../include/CSVReader.hpp"

#include <iostream>

//carga y consulta de materiales

void MaterialManager::load(const std::string& filename)
{
    CSVReader reader;

    auto data = reader.read(filename);

    for (size_t i = 1; i < data.size(); i++)
    {
        Material material;

        material.name = data[i][0];
        material.effectiveMass = std::stod(data[i][1]);
        material.bandGap = std::stod(data[i][2]);
        material.dielectricConstant = std::stod(data[i][3]);

        materials.push_back(material);
    }
}

void MaterialManager::print() const
{
    std::cout << "Materials loaded:\n\n";

    for (const Material& material : materials)
    {
        std::cout << "- " << material.name << '\n';

        std::cout << "  Effective mass: "
                  << material.effectiveMass << '\n';

        std::cout << "  Band gap: "
                  << material.bandGap << " eV\n";

        std::cout << "  Dielectric constant: "
                  << material.dielectricConstant << "\n\n";
    }
}

Material MaterialManager::get(const std::string& name) const
{
    for (const Material& material : materials)
    {
        if (material.name == name)
        {
            return material;
        }
    }

    return Material{};
}