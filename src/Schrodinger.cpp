
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


// Calcula psi'' para la ecuacion de Schrodinger.

static double schrodingerCoefficient(
    double position,
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar,
    double dx
){

    size_t index = getPotentialIndex(position, x, dx);

    return (2.0 * effectiveMass / (hbar * hbar)) * (potential[index] - energy);

}


//********************************************/
//*             *Euler solver*               *
//********************************************/

vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){

            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position, double psi){

        double coefficient = schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx);

        return coefficient * psi;

    };

    return euler(x.front(), 1.0, 0.0, dx, x.back(), equation);
}


//********************************************/
//*             *Leapfrog solver*            *
//********************************************/

vector<double> solveSchrodingerLeapfrog(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){

            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position, double psi){

        double coefficient = schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx);

        return coefficient * psi;
    };

    return leapfrog(x.front(), 1.0, 0.0, dx, x.back(), equation);

}

//********************************************/
//*             *Numerov solver*             *
//********************************************/

vector<double> solveSchrodingerNumerov(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){

    if (x.size() < 2 || x.size() != potential.size() || hbar == 0.0){

            return {};
        }

    double dx = x[1] - x[0];

    auto equation = [&](double position){

        return schrodingerCoefficient(position, x, potential, effectiveMass, energy, hbar, dx);
    };

    double firstValue = 1.0;

    // Taylor: psi(h) = psi(0) + h*psi'(0) + h^2*psi''(0)/2

    double secondDerivative = equation(x.front()) * firstValue;

    double secondValue = firstValue + dx * 0.0 + 0.5 * dx * dx * secondDerivative;

    return numerov(x.front(), firstValue, secondValue, dx, x.back(), equation);
    
}
