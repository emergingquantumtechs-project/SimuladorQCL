#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Material
{
    string name;
    double effectiveMass;
    double bandGap;
    double dielectricConstant;
};


// Cargar los materiales desde un archivo CSV
vector<Material> loadMaterials(const string& filename)
{
    vector<Material> materials;

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: could not open materials file: "
                  << filename << '\n';

        return materials;
    }

    string line;

    // Saltar encabezado
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string value;

        Material material;

        // Name
        getline(ss, value, ',');
        material.name = value;

        // Effective mass
        getline(ss, value, ',');
        material.effectiveMass = stod(value);

        // Band gap
        getline(ss, value, ',');
        material.bandGap = stod(value);

        // Dielectric constant
        getline(ss, value, ',');
        material.dielectricConstant = stod(value);

        materials.push_back(material);
    }

    return materials;
}

void printMaterials(const vector<Material>& materials)
{
    cout << "Materials loaded:\n\n";

    for (const Material& material : materials)
    {
        cout << "- " << material.name << '\n';

        cout << "  Effective mass: "
                  << material.effectiveMass << '\n';

        cout << "  Band gap: "
                  << material.bandGap << " eV\n";

        cout << "  Dielectric constant: "
                  << material.dielectricConstant << "\n\n";
    }
}

Material getMaterial(
    const vector<Material>& materials,
    const string& name
)
{
    for (const Material& material : materials)
    {
        if (material.name == name)
        {
            return material;
        }
    }

    // Material vacío si no se encuentra
    return Material{};
}