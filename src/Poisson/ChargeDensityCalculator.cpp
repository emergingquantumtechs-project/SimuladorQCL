#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class ChargeDensityCalculator{

private:
    double electron_Charge;
    double boltzmann_Constant;
    double temperature;
    double chemical_Potential;

public:
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
  VectorXd CalculateFermiDirac(VectorXd energies)
    {

        VectorXd occupation(energies.size());


        // 
        // f(E) = 1/(1+exp((E-mu)/(kB*T)))


        return occupation;
    }
};

//double ReadBoltzmann(){
double ReadConstant(string FindConstant){

ifstream file("../../data/universal_constants.csv"); //para leer datos (Input File Stream)

if (!file.is_open()){
        cout << "Error: No se pudo abrir el archivo .csv" << endl;
        return 0.0;
    }

string line;
    getline(file, line);
    while (getline(file, line)){

        stringstream lineStream(line); //separar los datos de la linea en columnas
        string constantName;
        string alias;
        string value;
        string unit;

        //separar con comas
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

int main()
{
    double boltzmannConstant = ReadConstant("boltzmann_constant");
    double h  = ReadConstant("planck_constant");
    double hbar = ReadConstant("reduced_planck_constant");

    double e = 1.60217646e-19;
    double kB = 1.3806503e-23;
    double T = 300;
    double mu = 0.1;



    //cout << "Boltzmann constant: " << boltzmannConstant << endl;
    //cout << "Planck constant: " << h << endl;
    //cout << "Reduced Planck constant: " << hbar << endl;

    return 0;
}
