#include <vector>
#include <functional>
#include <cmath>

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

//********************************************/
//*              *Euler solver*              *
//********************************************/

vector<double> solveSchrodingerEuler(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){
    double dx = x[1] - x[0];

    auto equation =
        [&](double position, double psi)
        {
            size_t index =
                static_cast<size_t>(
                    (position - x[0]) / dx
                );

            if (index >= potential.size())
            {
                index = potential.size() - 1;
            }

            return
                (2.0 * effectiveMass /
                (hbar * hbar))
                *
                (potential[index] - energy)
                *
                psi;
        };

    return euler(
        x[0],
        1.0,
        0.0,
        dx,
        x.back(),
        equation
    );
}

//********************************************/
//*           *Leapfrog solver*              *
//********************************************/

vector<double> solveSchrodingerLeapfrog(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
){

    double dx = x[1] - x[0];

    auto equation = [&](double position, double psi){

            size_t index = static_cast<size_t>((position - x[0]) / dx);

            if (index >= potential.size()){

                index = potential.size() - 1;
            }

            return (2.0 * effectiveMass / (hbar * hbar)) * (potential[index] - energy) * psi;
        };

    return leapfrog(x[0], 1.0, 0.0, dx, x.back(), equation);
}

//********************************************/
//*            *Numerov solver*              *
//********************************************/

/*

    Numerov trabaja directamente con: ψ′′ = f(x)ψ
    por lo que definimos: f(x) = ℏ2 / 2m ∗ ​(V(x)−E)

*/

vector<double> solveSchrodingerNumerov(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
)
{
    double dx = x[1] - x[0];

    auto equation =
        [&](double position)
        {
            size_t index =
                static_cast<size_t>(
                    (position - x[0]) / dx
                );

            if (index >= potential.size())
            {
                index = potential.size() - 1;
            }

            return
                (2.0 * effectiveMass /
                (hbar * hbar))
                *
                (potential[index] - energy);
        };

    double firstValue = 1.0;

    double secondValue =
        1.0;

    return numerov(
        x[0],
        firstValue,
        secondValue,
        dx,
        x.back(),
        equation
    );
}