#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
);

struct BisectionResult {
    double energy;
    double residual;
    int iterations;
    bool converged;
};

double calculateResidual(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){
    // psi(-3) = 0, psi'(-3) = 1.
    // The residual is psi(3).

    vector<double> psi = solveSchrodingerEuler(
        x, potential,
        effectiveMass, energy, hbar,
        0.0, 1.0
    );

    if (psi.size() != x.size()) {

        return NAN;
    }

    return psi.back();
}

BisectionResult bisection(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double hbar,
    double lowerEnergy,
    double upperEnergy,
    double energyTolerance,
    int maxIterations,
    ofstream& history
) {
    double fLower = calculateResidual(x, potential, effectiveMass, lowerEnergy, hbar);

    double fUpper = calculateResidual(x, potential, effectiveMass, upperEnergy, hbar);

    cout << "Initial energy interval: [" << lowerEnergy << ", " << upperEnergy << "]\n";

    cout << "F(E lower) = " << fLower << '\n';
    cout << "F(E upper) = " << fUpper << "\n\n";

    if (!isfinite(fLower) || !isfinite(fUpper) || fLower * fUpper >= 0.0) {

        cerr << "Error: The initial interval does not " << "bracket a root.\n";

        return {NAN, NAN, 0, false};

    }

    cout << "Bisection process\n";
    cout << left << setw(12) << "Iteration" << setw(22) << "Midpoint energy" << "Residual\n";

    double midpoint = lowerEnergy;
    double fMidpoint = fLower;

    for (int iteration = 1; iteration <= maxIterations; ++iteration) {

        midpoint = 0.5 * (lowerEnergy + upperEnergy);

        fMidpoint = calculateResidual(x, potential, effectiveMass, midpoint, hbar);

        if (!isfinite(fMidpoint)) {

            cerr << "Error: Non-finite residual encountered.\n";

            return {midpoint, fMidpoint, iteration, false};

        }

        cout << left << setw(12) << iteration << setw(22) << midpoint << fMidpoint << '\n';

        history << iteration << ',' << setprecision(12) << midpoint << ',' << fMidpoint << '\n';

        if (abs(fMidpoint) < 1e-10 || (upperEnergy - lowerEnergy) < energyTolerance) {

            return {midpoint, fMidpoint, iteration, true};
        }

        if (fLower * fMidpoint < 0.0) {

                upperEnergy = midpoint;
                fUpper = fMidpoint;

        } 
        else {

            lowerEnergy = midpoint;
            fLower = fMidpoint;

        }
    }

    return {midpoint, fMidpoint, maxIterations, false};
}

int main() {

    cout << fixed << setprecision(10);

    cout << "********************************************\n";
    cout << "*          SCHRODINGER SIMULATOR           *\n";
    cout << "*          Euler Method + Bisection        *\n";
    cout << "********************************************\n\n";

    // Spatial domain and discretization.
    const double initialX = -3.0;
    const double finalX = 3.0;
    const double step = 0.01;

    const int intervals = static_cast<int>(round((finalX - initialX) / step));

    const int points = intervals + 1;

    vector<double> x(points);
    vector<double> potential(points);

    for (int i = 0; i < points; ++i) {

        x[i] = initialX + i * step;
        potential[i] = x[i] * x[i];

    }

    // Normalized parameters
    const double effectiveMass = 0.5;
    const double hbar = 1.0;

    // Initial energy search interval
    const double lowerEnergy = 0.5;
    const double upperEnergy = 1.5;

    const double energyTolerance = 1e-10;
    const int maxIterations = 100;

    cout << "Potential: V(x) = x^2\n";
    cout << "Spatial domain: [" << initialX << ", " << finalX << "]\n";
    cout << "Spatial step: " << step << '\n';
    cout << "Number of spatial points: " << points << "\n\n";

    ofstream history("BisectionConvergence.csv");

    if (!history.is_open()) {

        cerr << "Error: Could not create convergence CSV.\n";

        return 1;

    }

    history << "iteration,energy,residual\n";

    BisectionResult result = bisection(x, potential, effectiveMass, hbar, lowerEnergy, upperEnergy, energyTolerance, maxIterations, history);

    history.close();

    if (!result.converged) {

        cerr << "\nThe bisection search did not converge " << "successfully. Check the interval and residual.\n";

        return 1;

    }

    // Recalculate the wavefunction at the computed energy
    vector<double> psi = solveSchrodingerEuler(x, potential, effectiveMass, result.energy, hbar, 0.0, 1.0);

    if (psi.size() != x.size()) {

        cerr << "Error: Could not calculate final wavefunction.\n";

        return 1;

    }

    ofstream output("EulerBisectionWavefunction.csv");

    if (!output.is_open()) {

        cerr << "Error: Could not create wavefunction CSV.\n";

        return 1;

    }

    output << "x,V_x,psi\n";

    for (int i = 0; i < points; ++i) {

        output << x[i] << ',' << potential[i] << ',' << psi[i] << '\n';

    }

    output.close();

    cout << "\n********************************************\n";
    cout << "*               FINAL RESULTS              *\n";
    cout << "********************************************\n";
    cout << "Computed energy: " << result.energy << '\n';
    cout << "Final residual psi(3): " << result.residual << '\n';
    cout << "Iterations: " << result.iterations << '\n';
    cout << "Wavefunction data: EulerBisectionWavefunction.csv\n";
    cout << "Convergence history: BisectionConvergence.csv\n";

    return 0;
}
