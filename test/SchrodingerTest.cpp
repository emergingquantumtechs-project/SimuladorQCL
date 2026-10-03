#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
);

vector<double> solveSchrodingerLeapfrog(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
);

vector<double> solveSchrodingerNumerov(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
);


int main(){

    double step = 0.1;
    double finalX = 1.0;

    int points = static_cast<int>(finalX / step) + 1;

    vector<double> x(points);
    vector<double> potential(points);


    for (int i = 0; i < points; i++){

        x[i] = i * step;
        potential[i] = 0.0;

    }


    /*
        Queremos obtener: ψ'' = -ψ

        Para ello: (2m* / hbar²)(V-E) = -1

        Usamos: m* = 0.5, E  = 1, hbar = 1

    */

    double effectiveMass = 0.5;

    double energy = 1.0;

    double hbar = 1.0;


    auto eulerResult = solveSchrodingerEuler(
            x,
            potential,
            effectiveMass,
            energy,
            hbar
        );


    auto leapfrogResult = solveSchrodingerLeapfrog(
            x,
            potential,
            effectiveMass,
            energy,
            hbar
        );


    auto numerovResult =  solveSchrodingerNumerov(
            x,
            potential,
            effectiveMass,
            energy,
            hbar
        );


    cout << "x\tExact\tEuler\tLeapfrog\tNumerov\n\n";


    for (size_t i = 0; i < x.size(); i++){

        double exact = cos(x[i]);

        cout << x[i] << "\t" << exact << "\t" << eulerResult[i] << "\t" << leapfrogResult[i] << "\t" << numerovResult[i] << '\n';
        
    }


    return 0;
}