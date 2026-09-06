#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

class ChargeDensityCalculator{
//para cálculos relacionados con la densidad de carga

private:
    double electron_Charge;
    double boltzmann_Constant;
    double temperature;
    double chemical_Potential;

public:
    //cons
    ChargeDensityCalculator(
        double charge,
        double kB,
        double temp,
        double chemicalPotencial
    )
        : electron_Charge(charge),
          boltzmann_Constant(kB),
          temperature(temp),
          chemical_Potential(chemicalPotencial){
    }

    // ocupacion f(E) de cada subbanda
    vector<double> CalculateFermiDirac(vector<double> energies){
        vector<double> occupation(energies.size());

        for (int i = 0; i < energies.size(); i++){
            occupation[i] = 1.0 / (1.0 + exp(((energies[i]-chemical_Potential)*electron_Charge)/(boltzmann_Constant*temperature)));
        }
        return occupation;
    }

    // Normaliza una funcion de onda: integral |psi|^2 dz = 1
    vector<double> Normalizar(vector<double> psi, double dz){
        double norma = 0.0;
        int n = psi.size();

        //1.calcular la suma cuadrada discretizada
        for (int i = 0; i < n; i++) {
            norma = norma + (psi[i]*psi[i]*dz);
        }
        norma = sqrt(norma);

        // 2.dividir cada elemento entre la norma total
        vector<double> psi_normalizada(n);
        if (norma > 0.0){
            for (int i = 0; i < n; i++){
                psi_normalizada[i] = psi[i] / norma;
            }
        }
        return psi_normalizada;
    }

    //calcularDensidad
    //Calcula rho(z) = e * [ Nd(z) - sum( ni * |psi_i(z)|^2 ) ]
    vector<double> CalcularDensidad(
        vector<vector<double>> wavefunctions, 
        vector<double> subband_densities, 
        vector<double> Nd, 
        double dz
    ) {
        int grid_size = Nd.size();
        vector<double> n_electronico(grid_size, 0.0);
        vector<double> rho(grid_size, 0.0);

        //1.sumar la contribucion electronica de cada subbanda normalizada
        for (int i = 0; i < wavefunctions.size(); i++){
            vector<double> psi_norm = Normalizar(wavefunctions[i], dz);
            double ni = subband_densities[i];

            for (int j = 0; j < grid_size; j++){
                n_electronico[j] = n_electronico[j] + (ni * psi_norm[j] * psi_norm[j]);
            }
        }

        //2.calcular la densidad de carga neta rho(z)
        for (int j = 0; j < grid_size; j++){
            rho[j] = electron_Charge * (Nd[j] - n_electronico[j]);
        }
        return rho;
    }
};

//lectura de constantes CSV
double ReadConstant(string FindConstant){
    ifstream file("../../data/universal_constants.csv");

    if (!file.is_open()){
        cout << "error: no se pudo abrir el archivo .csv" << endl;
        return 0.0;
    }

    string line;
    getline(file, line);//salta cabecera
    while (getline(file, line)){
        stringstream lineStream(line);
        string constantName;
        string alias;
        string value;
        string unit;

        getline(lineStream, constantName, ',');
        getline(lineStream, alias, ',');
        getline(lineStream, value, ',');
        getline(lineStream, unit, ',');

        if (constantName == FindConstant){
            return stod(value);
        }
    }

    cout << "Constant not found: " << FindConstant << endl;
    return 0.0;
}

int main(){
    
    //constantes fisicas
    double boltzmannConstant = ReadConstant("boltzmann_constant");
    double h  = ReadConstant("planck_constant");
    double hbar = ReadConstant("reduced_planck_constant");
    double e = 1.60217646e-19;
    double kB = 1.3806503e-23;
    double T = 300;
    double mu = 0.1; // en eV

    //objeto de la clase
    ChargeDensityCalculator calculator(e, kB, T, mu);

    //Fermi-Dirac
    vector<double> energies = {0.05, 0.1, 0.15}; //energias de prueba en eV
    vector<double> results = calculator.CalculateFermiDirac(energies);

    cout << "Resultados de Fermi-Dirac" << endl;
    for (int i = 0; i < results.size(); i++) {
        cout << "Energia: " << energies[i] << " eV -> Ocupacion: " << results[i] << endl;
    }

    //mallado espacial z
    int N = 100;
    double dz = 1.0e-9; // 1 nm en metros
    double ns_total = 1.0e16; //densidad superficial de prueba (1/m^2)

    //dopaje (uniforme, Nd(z))
    vector<double> Nd(N, ns_total / (N * dz));

    vector<double> psi1(N), psi2(N), psi3(N);
    for (int j = 0; j < N; j++) {
        psi1[j] = sin(1.0 * 3.14159265 * j / (N - 1));
        psi2[j] = sin(2.0 * 3.14159265 * j / (N - 1));
        psi3[j] = sin(3.0 * 3.14159265 * j / (N - 1));
    }
    vector<vector<double>> wavefunctions = {psi1, psi2, psi3};

    //poblaciones superficiales proporcionales a la ocupacion
    vector<double> subband_densities = {results[0] * 1e15, results[1] * 1e15, results[2] * 1e15};

    //calculo densidad de carga rho(z)
    vector<double> rho = calculator.CalcularDensidad(wavefunctions, subband_densities, Nd, dz);

    cout << "\ndensidad de carga rho(z) - prueba" << endl;
    cout << "rho en el borde (z = 0): " << rho[0] << " C/m^3" << endl;
    cout << "rho en el centro (z = " << N / 2 << "): " << rho[N / 2] << " C/m^3" << endl;
    cout << "rho en el otro borde (z = " << N - 1 << "): " << rho[N - 1] << " C/m^3" << endl;

    return 0;
}