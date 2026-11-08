#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// tres primeros eigenvalores (energias).
// E_n = hbar * omega * (n + 1/2)
vector<double> CalculateEigenvalues(double hbar, double omega){
    vector<double> eigenvalues;

    for (int n = 0; n < 3; n++){
        double energy = hbar * omega * (n + 0.5);
        eigenvalues.push_back(energy);
    }

    return eigenvalues;
}
 
// calc funcion de onda del estado n en la coordenada adimensional xi.
//
// Formula general:
// psi_n(xi) = [1 / sqrt(2^n * n! * sqrt(pi))] * H_n(xi) * exp(-xi^2 / 2)
//
// Polinomios de Hermite:
// H_0(xi) = 1
// H_1(xi) = 2*xi
// H_2(xi) = 4*xi^2 - 2

double CalculateWavefunction(int n, double xi){
    const double pi = 3.14159265358979323846;

    // Estado fundamental: psi_0(xi) = pi^(-1/4) * exp(-xi^2 / 2)
    double psi0 = (1.0 / sqrt(sqrt(pi))) * exp(-xi * xi / 2.0);

    if (n == 0) {
        return psi0;
    }
    // Estado n = 1: psi_1(xi) = sqrt(2) * xi * psi_0(xi)
    else if (n == 1){
        return sqrt(2.0) * xi * psi0;
    }
    // Estado n = 2: psi_2(xi) = [(2*xi^2 - 1) / sqrt(2)] * psi_0(xi)
    else if (n == 2){
        return ((2.0 * xi * xi - 1.0) / sqrt(2.0)) * psi0;
    }

    return 0.0;
}

int main() {
    // unidades adimensionales para simplificar 
    double hbar = 1.05457181e-34 ; //j
    double omega = 1.0; //rads¿

    // calcular las energias.
    vector<double> energies = CalculateEigenvalues(hbar, omega);

    cout << "Eigenvalores:" << endl;

    for (int i = 0; i < energies.size(); i++){
        cout << "E" << i << " = " << energies[i] << endl;
    }

    //malla de posiciones adimensionales xi
    double xiMin = -5.0; // Lim inferior de la malla
    double xiMax = 5.0; // Lim superior de la malla
    double step = 0.01;

    int numberOfPoints =
        static_cast<int>((xiMax - xiMin) / step) + 1;

    cout << "\nFunciones de onda en algunos puntos:" << endl;

    //calcular las funciones de onda en cada posicion.
    for (int i = 0; i < numberOfPoints; i++){
        double xi = xiMin + i * step;

        for (int n = 0; n < 3; n++){
            double psi = CalculateWavefunction(n, xi);
          
            cout << "xi = " << xi << ", Psi" << n << " = " << psi << endl;
            }
        
    }

    cout << "\nComprobacion de normalizacion:" << endl;

    for (int n = 0; n < 3; n++){
        double integral = 0.0;

        for (int i = 0; i < numberOfPoints; i++) {
        double xi = xiMin + i * step;
        double psi = CalculateWavefunction(n, xi);

        integral += psi * psi * step;
    }

    cout << "Estado n = " << n
         << ", integral = " << integral << endl;
}

// guardar en un archivo CSV.
ofstream energyFile("eigenvalues.csv");

if (energyFile.is_open()){
    energyFile << "n,energy\n";

    for (int n = 0; n < energies.size(); n++) {
        energyFile << n << "," << energies[n] << "\n";
    }

    energyFile.close();
    cout << "\nEnergias en eigenvalues.csv" << endl;
}
else {
    cout << "Error al crear eigenvalues.csv" << endl;
}

// funciones de onda archivo CSV.
ofstream wavefunctionFile("wavefunctions.csv");

if (wavefunctionFile.is_open()) {
    wavefunctionFile << "xi,psi0,psi1,psi2\n";

    for (int i = 0; i < numberOfPoints; i++){
        double xi = xiMin + i * step;

        double psi0 = CalculateWavefunction(0, xi);
        double psi1 = CalculateWavefunction(1, xi);
        double psi2 = CalculateWavefunction(2, xi);

        wavefunctionFile << xi << ","
                         << psi0 << ","
                         << psi1 << ","
                         << psi2 << "\n";
    }

    wavefunctionFile.close();
    cout << "Funciones de onda  en wavefunctions.csv" << endl;
}
else {
    cout << "Error al crear wavefunctions.csv" << endl;
}


    return 0;
}
