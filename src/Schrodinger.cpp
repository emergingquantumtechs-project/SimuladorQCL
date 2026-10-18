
#include <vector>
#include <functional>
#include <cmath>
#include <cstddef>

using namespace std;

vector<double> euler(
    double initialX,
    double initialY,
    double initialDerivative,
    double stepSize,
    double finalX,
    function<double(double, double)> equation
);

vector<double> leapfrog(
    double initialX,
    double initialY,
    double initialVelocity,
    double stepSize,
    double finalX,
    function<double(double, double)> f
);

vector<double> numerov(
    double initialX,
    double initialY,
    double nextY,
    double stepSize,
    double finalX,
    function<double(double)> f
);

// Coeficiente de la ecuacion de Schrodinger:
// psi''(x) = [2m/hbar^2] * [V(x) - E] * psi(x)

static double schrodingerCoefficient(
    double position,
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double dx
){

    long index = lround((position - x.front()) / dx);

    if (index < 0){

            index = 0;
        }

    if (index >= static_cast<long>(potential.size())){

            index = static_cast<long>(potential.size()) - 1;
        }

    return (2.0 * effectiveMass / (hbar * hbar)) * (potential[static_cast<size_t>(index)] - energy);
}

//indice del potencial correspondiente a x.

static size_t getPotentialIndex(double position, const vector<double>& x, double dx){

    long index = lround((position - x[0]) / dx);

    if (index < 0){

            index = 0;
        }

    if (index >= static_cast<long>(x.size())){
        
            index = static_cast<long>(x.size()) - 1;
        }

    return static_cast<size_t>(index);

}

//********************************************/
//*             *Euler solver*               *
//********************************************/

vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){
        
            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position, double psi){

        return schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx) * psi;
    };

    return euler(x.front(), initialPsi, initialDerivative, dx, x.back(), equation);

}


//********************************************/
//*             *Leapfrog solver*            *
//********************************************/

vector<double> solveSchrodingerLeapfrog(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){

            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position, double psi){

        return schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx) * psi;
    };

    return leapfrog(x.front(), initialPsi, initialDerivative, dx, x.back(), equation);

}

//********************************************/
//*             *Numerov solver*             *
//********************************************/

vector<double> solveSchrodingerNumerov(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double initialPsi,
    double initialDerivative
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){

            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position){

        return schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx);
    };

    // Valor inicial
    double firstValue = initialPsi;

    // Aproximacion de Taylor para psi(dx).
    // Para V(x) = x^2, la derivada del coeficiente
    // es cero en x = 0.
    
    double coefficientAtZero = equation(x.front());

    double secondValue = initialPsi + dx * initialDerivative + 0.5 * dx * dx * coefficientAtZero * initialPsi + (dx * dx * dx / 6.0) * coefficientAtZero * initialDerivative;

    return numerov(x.front(), firstValue, secondValue, dx, x.back(), equation);

}
