#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

// Function implemented in src/Schrodinger.cpp
vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
);

int main() {

    cout << "**********************************\n";
    cout << "         *ENERGY SCANNER*\n";
    cout << "**********************************\n\n";

    // 1. Spatial domain
    const double initialX = -6.0;
    const double finalX = 6.0;
    const double h = 0.001;

    // 2. Physical parameters
    const double effectiveMass = 0.5;
    const double hbar = 1.0;

    // 3. Initial conditions
    const double initialPsi = 0.0;
    const double initialDerivative = 1.0;

    // 4. Request trial energy
    double energy;

    cout << "Enter a trial energy: ";
    cin >> energy;

    if (!cin || !isfinite(energy)) {
        cerr << "Invalid energy value.\n";
        return 1;
    }

    // 5. Generate spatial grid and potential
    vector<double> x;
    vector<double> potential;

    const int numberOfSteps =
        static_cast<int>(round((finalX - initialX) / h));

    for (int i = 0; i <= numberOfSteps; i++) {

        double position = initialX + i * h;

        x.push_back(position);
        potential.push_back(position * position);
    }

    cout << "\nSpatial points: " << x.size() << endl;
    cout << "Domain: [" << initialX << ", " << finalX << "]\n";
    cout << "Step size: " << h << endl;
    cout << "Trial energy: " << energy << endl;

    // 6. Solve Schrodinger equation using Euler
    cout << "\nCalculating wavefunction...\n";

    vector<double> psi = solveSchrodingerEuler(
        x,
        potential,
        effectiveMass,
        energy,
        hbar,
        initialPsi,
        initialDerivative
    );

    // 7. Validate result
    if (psi.size() != x.size()) {
        cerr << "Error: invalid wavefunction size.\n";
        return 1;
    }

    for (double value : psi) {
        if (!isfinite(value)) {
            cerr << "Numerical overflow detected.\n";
            return 1;
        }
    }

    // 8. Generate filename using trial energy
    ostringstream filename;

    filename << fixed << setprecision(4)
             << "EnergyScan_E_" << energy << ".csv";

    // 9. Save wavefunction
    ofstream outputFile(filename.str());

    if (!outputFile.is_open()) {
        cerr << "Error creating " << filename.str() << endl;
        return 1;
    }

    outputFile << "x,V_x,psi\n";
    outputFile << setprecision(12);

    for (size_t i = 0; i < x.size(); i++) {

        outputFile << x[i] << ","
                   << potential[i] << ","
                   << psi[i] << "\n";
    }

    outputFile.close();

    // 10. Display results
    cout << "\nWavefunction calculation completed.\n";

    cout << "Initial value psi(-6): "
         << psi.front() << endl;

    cout << "Final value psi(6): "
         << psi.back() << endl;

    cout << "Saved: " << filename.str() << endl;

    cout << "\n**********************************\n";
    cout << "         *CALCULATION DONE*\n";
    cout << "**********************************\n";

    return 0;
}
