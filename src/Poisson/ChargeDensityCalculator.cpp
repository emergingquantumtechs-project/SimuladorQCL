#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//***ChargeDensityCalculator***
// Clase para cálculos relacionados con la densidad de carga
// Contiene métodos para calcular la ocupación de Fermi-Dirac y calcular la densidad de carga rho(z)
// a partir de funciones de onda y densidades superficiales

class ChargeDensityCalculator{
private:
    double electron_Charge;
    double boltzmann_Constant;
    double temperature;
    double chemical_Potential;
    double effective_Mass;
    double hbar;

public:
    //Constantes físicas
    ChargeDensityCalculator(
        double charge,
        double kB,
        double temp,
        double effectiveMass,
        double hbarValue
    )
        : electron_Charge(charge),
          boltzmann_Constant(kB),
          temperature(temp),
          chemical_Potential(0.0), //provisional
          effective_Mass(effectiveMass),
          hbar(hbarValue)
    {
    }

    //***CalculateDensityOfStates***
    // Calcula la densidad de estados para las subbandas
    // cuantizadas en la dirección de crecimiento.
    // Densidad de estados = m* / (π ħ²)
    // Donde:
    // m*: masa efectiva
    // ħ: constante de Planck reducida

    double CalculateDensityOfStates()
    {
        return effective_Mass / (numbers::pi * hbar * hbar);
    }


    //***CalculateSubbandDensity***
    // Calcula la densidad superficial de electrones
    // correspondiente a una subbanda, utilizando su energia,
    // el potencial quimico y la temperatura del sistema.

    // Ecuacion utilizada:
    // ni = DOS * kB * T * ln(1 + exp((mu - Ei) / (kB * T)))

    // Donde:
    // ni: densidad superficial de electrones de la subbanda.
    // DOS: densidad de estados bidimensional.
    // kB: constante de Boltzmann.
    // T: temperatura.
    // mu: potencial quimico.
    // Ei: energia de la subbanda.

    double CalculateSubbandDensity(double energy, double chemicalPotential){

        //kB * T
        double kBT = boltzmann_Constant * temperature;

        //pasar energia y potencial quimico de eV a joules
        double energyJ = energy * electron_Charge;
        double chemicalPotentialJ = chemicalPotential * electron_Charge;

        //valor que se utilizara en la funcion exponencial
        double exponent = (chemicalPotentialJ - energyJ) / kBT;

        // Calcular la densidad superficial de electrones
        double subbandDensity = CalculateDensityOfStates() * kBT *
                                log(1.0 + exp(exponent));

        return subbandDensity;
    }


    //***CalculateChemicalPotentialFunction***
    // Calcula la diferencia entre la densidad superficial
    // total de electrones y la densidad superficial objetivo.

    // Ecuacion utilizada:
    // f(mu) = SUM(ni) - Ns

    // Donde:
    // ni: densidad superficial de cada subbanda.
    // Ns: densidad superficial objetivo de electrones.

    double CalculateChemicalPotentialFunction(
        const vector<double>& energies,
        double chemicalPotential,
        double targetDensity
    ){

        // Inicializar la densidad total de electrones
        double totalDensity = 0.0;

        // Sumar la densidad superficial de todas las subbandas
        for (size_t i = 0; i < energies.size(); i++){

            totalDensity += CalculateSubbandDensity(
                energies[i], chemicalPotential
            );
        }

        // Calcular la diferencia con la densidad objetivo
        return totalDensity - targetDensity;
    }


    //***CalculateChemicalPotential***
    // Calcula el potencial quimico mediante el metodo de biseccion
    // middlePoint = (lowerLimit + upperLimit) / 2
    // Se busca el valor de mu donde:
    // f(mu) = SUM(ni) - Ns = 0

    double CalculateChemicalPotential(
        const vector<double>& energies,
        double targetDensity,
        double lowerLimit,
        double upperLimit,
        double tolerance,
        int maxIterations
    ){

        double functionAtLowerLimit = CalculateChemicalPotentialFunction(
            energies, lowerLimit, targetDensity
        );

        double functionAtUpperLimit = CalculateChemicalPotentialFunction(
            energies, upperLimit, targetDensity
        );

        if (functionAtLowerLimit * functionAtUpperLimit >= 0.0){

            cout << "Error: Los limites no encierran una raiz." << endl;
            return 0.0;
        }

        double middlePoint = 0.0;

        for (int i = 0; i < maxIterations; i++){

            //punto medio del intervalo
            middlePoint = (lowerLimit + upperLimit) / 2.0;

            //evaluala funcion en el punto medio
            double functionAtMiddlePoint = CalculateChemicalPotentialFunction(
                energies, middlePoint, targetDensity
            );

            if (abs(upperLimit - lowerLimit) / 2.0 < tolerance){
                chemical_Potential = middlePoint;

                return chemical_Potential;
            }

            //mitad del intervalo que contiene la raiz
            if (functionAtLowerLimit * functionAtMiddlePoint < 0.0){

                upperLimit = middlePoint;
                functionAtUpperLimit = functionAtMiddlePoint;

            }
            else
            {

                lowerLimit = middlePoint;
                functionAtLowerLimit = functionAtMiddlePoint;

            }
        }
        cout << "No se alcanzo la convergencia." << endl;

        return 0.0;
    }


    //***CalculateFermiDirac***
    // Calcula la ocupación de Fermi-Dirac f(E) para un vector de energías dado
    // f(E) = 1 / (1 + exp((E - mu) / (kB * T)))
    // E y mu se expresan en eV y se convierten a joules
    // mediante la carga elemental antes de dividir entre kB*T.

    // Donde:
    // E: energía de la subbanda
    // mu: potencial químico (POR CALCULAR CON MÉTODO DE RAÍCES)
    // kB: constante de Boltzmann
    // T: temperatura
    // Devuelve un vector con la ocupación correspondiente a cada energía
    // Ocupacion f(E) de cada subbanda

    vector<double> CalculateFermiDirac(vector<double> energies){
        vector<double> occupation(energies.size());

        for (int i = 0; i < energies.size(); i++){
            occupation[i] = 1.0 / (1.0 + exp(((energies[i]-chemical_Potential)*electron_Charge)/(boltzmann_Constant*temperature)));
        }

        //Si E_i << mu, la ocupación se acerca a 1.
        //Si E_i >> mu , se acerca a 0.
        //Si E_i = mu, es aproximadamente 0.5.

        return occupation;
    }

    //***Calcular Densidad Electronica
    //recibe wavefunctions, subband_densities y z

    //Calcula n(z) = SUM [ n_i |psi_i(z)|²]
    //Donde:
    //n(z): densidad electronica
    //n_i: densidad superficial de cada subbanda
    //psi_i(z): función de onda de cada subbanda
    //z: mallado espacial

    vector<double> CalculateElectronDensity(
        const vector<vector<double>>& wavefunctions,
        const vector<double>& subband_densities,
        const vector<double>& z)
    {
        size_t gridSize = z.size();

        vector<double> electronDensity(gridSize, 0.0);

        //***Sumar la contribución de cada subbanda.
        for (size_t i = 0; i < wavefunctions.size(); i++){

            double ni = subband_densities[i];

            for (size_t j = 0; j < gridSize; j++) {

                electronDensity[j] += ni * wavefunctions[i][j] * wavefunctions[i][j];
            }
        }

        return electronDensity;
    }

    //***Calcular Densidad de Carga rho(z)***
    //Calcula rho(z) = e * [Nd(z) - n(z)]

    vector<double> CalculateChargeDensity(
        const vector<double>& electronDensity,
        const vector<double>& donorDensity){

        size_t gridSize = donorDensity.size();

        vector<double> chargeDensity(gridSize,0.0);

        for (size_t j = 0; j < gridSize; j++){
            chargeDensity[j] = electron_Charge * (donorDensity[j] - electronDensity[j]);
        }

        return chargeDensity;
    }
};


//***ReadEigenvalues***
// Lee las energias almacenadas en eigenvalues.csv

vector<double> ReadEigenvalues(string filename){

    vector<double> energies;

    ifstream file(filename);

    if (!file.is_open()){
        cout << "Error al abrir " << filename << endl;
        return energies;
    }

    string line;

    // Saltar el encabezado del archivo
    getline(file, line);

    // Leer las energias de cada estado
    while (getline(file, line)){

        stringstream data(line);

        string state;
        string energy;

        getline(data, state, ',');
        getline(data, energy, ',');

        energies.push_back(stod(energy));
    }

    file.close();

    return energies;
}


//***ReadWavefunctions***
// Lee las funciones de onda almacenadas en wavefunctions.csv

void ReadWavefunctions(
    string filename,
    vector<double>& xi,
    vector<vector<double>>& wavefunctions
){

    ifstream file(filename);

    if (!file.is_open()){
        cout << "Error al abrir " << filename << endl;
        return;
    }

    string line;

    // Saltar el encabezado del archivo
    getline(file, line);

    // Crear un vector para cada estado
    wavefunctions.resize(3);

    // Leer las coordenadas y funciones de onda
    while (getline(file, line)){

        stringstream data(line);

        string value;

        // Leer la coordenada xi
        getline(data, value, ',');
        xi.push_back(stod(value));

        // Leer las tres funciones de onda
        for (int i = 0; i < 3; i++){

            getline(data, value, ',');

            wavefunctions[i].push_back(stod(value));
        }
    }

    file.close();
}


//***main***
// Comprobar que los archivos CSV se leen correctamente

int main(){

    // Leer las energias
    vector<double> energies = ReadEigenvalues("eigenvalues.csv");

    // Carga elemental en coulombs
    double electronCharge = 1.602176634e-19;

    // Convertir las energias de joules a electronvolts
    vector<double> energies_eV(energies.size());

    for (size_t i = 0; i < energies.size(); i++){

        energies_eV[i] = energies[i] / electronCharge;
}

    // Leer las funciones de onda y sus coordenadas
    vector<double> xi;
    vector<vector<double>> wavefunctions;

    ReadWavefunctions(
        "wavefunctions.csv",
        xi,
        wavefunctions
    );

    // Mostrar la cantidad de datos leidos
    cout << "Cantidad de energias: "
         << energies.size() << endl;

    cout << "Cantidad de puntos: "
         << xi.size() << endl;

    cout << "Cantidad de funciones de onda: "
         << wavefunctions.size() << endl;

    // Mostrar las energias
    cout << "\nEnergias leidas:" << endl;
        for (size_t i = 0; i < energies.size(); i++){
            cout << "E" << i << " = " << energies[i] << endl;
    }

    cout << "\nEnergias convertidas a eV:" << endl;
        for (size_t i = 0; i < energies_eV.size(); i++){
            cout << "E" << i << " = "
                << energies_eV[i] << " eV" << endl;
        }

    // Mostrar el primer valor de cada funcion de onda
    cout << "\nPrimer punto de las funciones de onda:" << endl;
    for (size_t i = 0; i < wavefunctions.size(); i++){
        cout << "Psi" << i << " = "
             << wavefunctions[i][0] << endl;
    }

    // Constantes para la prueba
    double boltzmannConstant = 1.380649e-23;
    double temperature = 300.0;
    double effectiveMass = 0.067 * 9.1093837015e-31;
    double hbar = 1.05457181e-34;

    // Crear el objeto ChargeDensityCalculator
    ChargeDensityCalculator calculator(
    electronCharge,
    boltzmannConstant,
    temperature,
    effectiveMass,
    hbar
);

    // Densidad superficial provisional para la prueba
    double targetDensity = 1e16; // m^-2

    double lowerLimit = -0.1; // eV
    double upperLimit = 0.2;  // eV
    double tolerance = 1e-8;  // eV
    int maxIterations = 100;

    double calculatedChemicalPotential = calculator.CalculateChemicalPotential(
        energies_eV,
        targetDensity,
        lowerLimit,
        upperLimit,
        tolerance,
        maxIterations
    );

    cout << "Chemical potential: " << calculatedChemicalPotential << " eV" << endl;
    cout << "\nPotencial quimico calculado:" << endl;
    cout << calculatedChemicalPotential << " eV" << endl;

        //ocupaciones de Fermi-Dirac
    vector<double> occupations = calculator.CalculateFermiDirac(energies_eV);

    cout << "\nOcupaciones de Fermi-Dirac:" << endl;

    for (size_t i = 0; i < occupations.size(); i++){
        cout << "Subbanda " << i << ": "
             << occupations[i] << endl;
    }

    return 0;
}
