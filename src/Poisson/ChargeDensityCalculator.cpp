#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

double ReadBoltzmann(){

ifstream file("../../data/universal_constants.csv"); //para leer datos (Input File Stream)

if (!file.is_open()){
        cout << "Error: No se pudo abrir el archivo .csv" << endl;
        return 0.0;
    }

string line;

    getline(file, line);

    while (getline(file, line))
    {
        stringstream lineStream(line);
        //separar los datos de la linea en columnas
        string constantName;
        string alias;
        string value;
        string unit;

        //separar con comas
        getline(lineStream, constantName, ',');
        getline(lineStream, alias, ',');
        getline(lineStream, value, ',');
        getline(lineStream, unit, ',');

        if (constantName == "boltzmann_constant")
        {
            return stod(value);
        }
    }

    cout << "Boltzmann constant not found." << endl;

    return 0.0;

}

int main()
{
    double boltzmannConstant = ReadBoltzmann();

    //para comprobar que leía correctamente (boltzmann.exe)
    //cout << "Boltzmann constant: "
    //          << boltzmannConstant
    //          << endl;

    return 0;
}