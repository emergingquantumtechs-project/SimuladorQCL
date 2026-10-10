
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

    //********************************************/
    //*       Configuracion de la malla          *
    //********************************************/

    double initialX = 0.0;
    double finalX = 1.0;
    double step = 0.01;

    int intervals = static_cast<int>(round((finalX - initialX) / step));

    int points = intervals + 1;

    vector<double> x(points);
    vector<double> potential(points);

    //****************************************************************/
    //*        Construir la posicion y el potencial V(x) = x^2       *
    //****************************************************************/

    for (int i = 0; i < points; i++){

        x[i] = initialX + i * step;
        potential[i] = x[i] * x[i];

    }

    //***************************************************/
    //*       Parametros normalizados de la prueba      *
    //***************************************************/

    double effectiveMass = 0.5;
    double energy = 1.0;
    double hbar = 1.0;

    //***************************************************/
    //*          Resolver con cada metodo               *
    //***************************************************/

    auto eulerResult = solveSchrodingerEuler(x, potential, effectiveMass, energy, hbar);

    auto leapfrogResult = solveSchrodingerLeapfrog(x, potential, effectiveMass, energy, hbar);

    auto numerovResult = solveSchrodingerNumerov(x, potential, effectiveMass, energy, hbar);

    //**********************************************************/
    //*       Comprobar que los resultados sean validos        *
    //**********************************************************/

    if (eulerResult.size() != x.size() || leapfrogResult.size() != x.size() || numerovResult.size() != x.size()){

        cerr << "Error: no se pudieron calcular " << "todas las soluciones.\n";

        return 1;
    }

    //*************************************************/
    //*          Preparar el archivo CSV              *
    //*************************************************/

    ofstream output("SchrodingerComparison.csv");

    if (!output.is_open()){

        cerr << "Error: no se pudo crear el archivo CSV.\n";
        return 1;

    }

    output << "x,V_x,Euler,Leapfrog,Numerov," << "Error_Euler_Leapfrog," << "Error_Euler_Numerov," << "Error_Leapfrog_Numerov\n";

    //******************************************************************/
    //*         Mostrar los resultados y calcular diferencias          *
    //******************************************************************/

    double maxEulerLeapfrog = 0.0;
    double maxEulerNumerov = 0.0;
    double maxLeapfrogNumerov = 0.0;

    cout << fixed << setprecision(8);

    cout << "/************************************************************/\n";
    cout << "/*         COMPARACION DE METODOS DE SCHRODINGER            *\n";
    cout << "/************************************************************/\n\n";

    cout << "Potencial: V(x) = x^2\n";
    cout << "Intervalo: [" << initialX << ", " << finalX << "]\n";
    cout << "Paso: " << step << '\n';
    cout << "Numero de puntos: " << points << "\n\n";
    cout << "x\tV(x)\tEuler\tLeapfrog\tNumerov\n";

    for (size_t i = 0; i < x.size(); i++){

        double errorEulerLeapfrog = abs(eulerResult[i] - leapfrogResult[i]);

        double errorEulerNumerov = abs(eulerResult[i] - numerovResult[i]);

        double errorLeapfrogNumerov = abs(leapfrogResult[i] - numerovResult[i]);

        if (errorEulerLeapfrog > maxEulerLeapfrog){

                maxEulerLeapfrog = errorEulerLeapfrog;
            }

        if (errorEulerNumerov > maxEulerNumerov){

                maxEulerNumerov = errorEulerNumerov;
            }

        if (errorLeapfrogNumerov > maxLeapfrogNumerov){

                maxLeapfrogNumerov = errorLeapfrogNumerov;
            }

        cout << x[i] << '\t'  << potential[i] << '\t'  << eulerResult[i] << '\t' << leapfrogResult[i] << '\t' << numerovResult[i] << '\n';

        output << x[i] << ',' << potential[i] << ',' << eulerResult[i] << ',' << leapfrogResult[i] << ',' << numerovResult[i] << ',' << errorEulerLeapfrog << ','
            << errorEulerNumerov << ',' << errorLeapfrogNumerov << '\n';

    }

    output.close();

    //************************************************************/
    //*           Resumen de las diferencias maximas             *
    //************************************************************/

    cout << "\n/************************************************************/\n";
    cout << "/*            DIFERENCIAS MAXIMAS ENTRE METODOS             *\n";
    cout << "/************************************************************/\n";

    cout << "Euler - Leapfrog: " << maxEulerLeapfrog << '\n';

    cout << "Euler - Numerov: " << maxEulerNumerov << '\n';

    cout << "Leapfrog - Numerov: " << maxLeapfrogNumerov << '\n';

    cout << "\nResultados guardados en " << "SchrodingerComparison.csv\n";

    return 0;
}
