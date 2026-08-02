#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

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

    //cout << "Boltzmann constant: " << boltzmannConstant << endl;
    //cout << "Planck constant: " << h << endl;
    //cout << "Reduced Planck constant: " << hbar << endl;

    return 0;
}
