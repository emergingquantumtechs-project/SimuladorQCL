#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct Layer
{
    int number;
    string material;
    double thicknessNm;
    bool doped;
    double doping;
};

vector<double> solveSchrodinger(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
);

vector<Layer> loadStructure(
    const string& filename
)
{
    vector<Layer> layers;

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error opening structure file\n";
        return layers;
    }

    string line;

    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);

        string value;

        Layer layer;

        getline(ss, value, ',');
        layer.number = stoi(value);

        getline(ss, value, ',');
        layer.material = value;

        getline(ss, value, ',');
        layer.thicknessNm = stod(value);

        getline(ss, value, ',');
        layer.doped = (value == "true");

        getline(ss, value, ',');
        layer.doping = stod(value);

        layers.push_back(layer);
    }

    return layers;
}

void buildPotential(
    const vector<Layer>& layers,
    vector<double>& x,
    vector<double>& potential,
    double dx
)
{
    const double nmToMeter = 1e-9;

    const double barrierEnergy =
        0.30 * 1.60217646e-19;

    double position = 0.0;

    for (const Layer& layer : layers)
    {
        double thickness =
            layer.thicknessNm * nmToMeter;

        int points = static_cast<int>(
            thickness / dx
        );

        for (int i = 0; i < points; i++)
        {
            x.push_back(position);

            if (layer.material == "AlGaAs")
            {
                potential.push_back(barrierEnergy);
            }
            else
            {
                potential.push_back(0.0);
            }

            position += dx;
        }
    }
}

void runLaserQCL()
{
    cout << "**********************************\n";
    cout << "         *QCL SIMULATOR*\n";
    cout << "**********************************\n\n";

    auto layers = loadStructure(
        "data/laser_structure.csv"
    );

    if (layers.empty())
    {
        cerr << "No layers loaded.\n";
        return;
    }

    vector<double> x;
    vector<double> potential;

    const double dx = 0.05e-9;

    buildPotential(
        layers,
        x,
        potential,
        dx
    );

    cout << "Layers loaded: " << layers.size() << '\n';

    cout << "Grid points: " << x.size() << '\n';

    cout << "Structure length: " << x.back() * 1e9 << " nm\n\n";

    // Preliminary parameters
    const double electronMass = 9.10938188e-31;

    const double effectiveMass = 0.067 * electronMass;

    const double hbar = 1.05457168e-34;

    const double energy = 0.10 * 1.60217646e-19;

    auto wavefunction = solveSchrodinger(
        x,
        potential,
        effectiveMass,
        energy,
        hbar
    );

    if (wavefunction.empty())
    {
        cerr << "Schrodinger solver failed.\n";
        return;
    }

    ofstream output(
        "wavefunction.csv"
    );

    output << "x_nm,potential_eV,psi\n";

    for (size_t i = 0; i < x.size(); i++)
    {
        output << x[i] * 1e9 << "," << potential[i] / 1.60217646e-19 << "," << wavefunction[i] << '\n';
    }

    output.close();

    cout << "Wavefunction calculated.\n";
    cout << "Results saved to wavefunction.csv\n";
}