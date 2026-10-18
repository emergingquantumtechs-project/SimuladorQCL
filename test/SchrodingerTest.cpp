

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

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

vector<double> solveSchrodingerLeapfrog(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
);

vector<double> solveSchrodingerNumerov(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
);


int main()
{
    //************************************************************/
    //*     Configuracion: resolver desde x = 0 hasta x = 3      *
    //************************************************************/

    const double initialX = 0.0;
    const double finalX = 3.0;
    const double step = 0.01;

    const int intervals = static_cast<int>(
        round((finalX - initialX) / step)
    );

    const int points = intervals + 1;

    vector<double> x(points);
    vector<double> potential(points);

    for (int i = 0; i < points; i++)
    {
        x[i] = initialX + i * step;
        potential[i] = x[i] * x[i];
    }

    //************************************************************/
    //*                Parametros normalizados                   *
    //************************************************************/

    const double effectiveMass = 0.5;
    const double hbar = 1.0;

    // Energias conocidas del oscilador armonico normalizado
    const double energy0 = 1.0;
    const double energy1 = 3.0;

    //************************************************************/
    //*               Estado basal: par, sin nodos               *
    //*               psi(0) = 1, psi'(0) = 0                    *
    //************************************************************/

    auto euler0 = solveSchrodingerEuler(x, potential, effectiveMass, energy0, hbar, 1.0, 0.0);

    auto leapfrog0 = solveSchrodingerLeapfrog(x, potential, effectiveMass, energy0, hbar, 1.0, 0.0);

    auto numerov0 = solveSchrodingerNumerov(x, potential, effectiveMass, energy0, hbar, 1.0, 0.0);

    //************************************************************/
    //*     Primer estado excitado: impar, un nodo en x = 0      *
    //*     psi(0) = 0, psi'(0) = 1                              *
    //************************************************************/

    auto euler1 = solveSchrodingerEuler(x, potential, effectiveMass, energy1, hbar, 0.0, 1.0);

    auto leapfrog1 = solveSchrodingerLeapfrog(x, potential, effectiveMass, energy1, hbar, 0.0, 1.0);

    auto numerov1 = solveSchrodingerNumerov(x, potential, effectiveMass, energy1, hbar, 0.0, 1.0);

    //************************************************************/
    //*                  Verificar resultados                    *
    //************************************************************/

    if (euler0.size() != x.size() || leapfrog0.size() != x.size() || numerov0.size() != x.size() || euler1.size() != x.size() || leapfrog1.size() != x.size() || numerov1.size() != x.size()){

        cerr << "Error al calcular las funciones de onda.\n";
        return 1;
    }

    //************************************************************/
    //*      Crear CSV con el intervalo completo [-3, 3]         *
    //************************************************************/

    ofstream output("SchrodingerComparison.csv");

    if (!output.is_open()){

        cerr << "No se pudo crear el archivo CSV.\n";
        return 1;

    }

    output << "x,V_x," << "Euler_psi0,Leapfrog_psi0,Numerov_psi0," << "Euler_psi1,Leapfrog_psi1,Numerov_psi1\n";

    cout << fixed << setprecision(6);

    cout << "COMPARACION DE FUNCIONES DE ONDA\n";
    cout << "Potencial: V(x) = x^2\n";
    cout << "Intervalo: [-3, 3]\n";
    cout << "Paso: " << step << '\n';
    cout << "Puntos en el intervalo completo: " << 2 * intervals + 1 << "\n\n";

    cout << "x\tV(x)\tEuler_psi0\tLeapfrog_psi0\tNumerov_psi0" << "\tEuler_psi1\tLeapfrog_psi1\tNumerov_psi1\n";

    // Recorrer de -3 a 3
    for (int i = -intervals; i <= intervals; i++){

        double position = i * step;
        double positivePosition = abs(position);

        int j = static_cast<int>(round(positivePosition / step));

        // Estado basal: par, psi(-x) = psi(x)
        double e0 = euler0[j];
        double l0 = leapfrog0[j];
        double n0 = numerov0[j];

        // Primer excitado: impar, psi(-x) = -psi(x)
        double sign = (position < 0.0) ? -1.0 : 1.0;

        double e1 = sign * euler1[j];
        double l1 = sign * leapfrog1[j];
        double n1 = sign * numerov1[j];

        double potentialValue = position * position;

        output << position << ',' << potentialValue << ',' << e0 << ',' << l0 << ',' << n0 << ',' << e1 << ',' << l1 << ',' << n1 << '\n';

        // Imprimir cada 0.5 unidades para no saturar la terminal
        if (i % 50 == 0 || i == intervals){
            
            cout << position << '\t' << potentialValue << '\t' << e0 << '\t' << l0 << '\t' << n0 << '\t' << e1 << '\t' << l1 << '\t' << n1 << '\n';
        }
    }

    output.close();

    cout << "\nArchivo generado: SchrodingerComparison.csv\n";

    return 0;
}
