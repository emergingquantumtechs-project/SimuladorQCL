#include <vector>
#include <functional>
#include <cmath>

using namespace std;

//********************************************/
//*               *Euler*                 *
//********************************************/

vector<double> euler(
    double initialX,
    double initialY,
    double initialDerivative,
    double stepSize,
    double finalX,
    function<double(double, double)> equation){

    if (stepSize <= 0.0 || finalX < initialX){

            return {};
        }

    int steps = static_cast<int>(round((finalX - initialX) / stepSize));

    vector<double> y(steps + 1);

    y[0] = initialY;

    double derivative = initialDerivative;

    for (int i = 0; i < steps; i++){

        double x = initialX + i * stepSize;

        double secondDerivative = equation(x, y[i]);

        y[i + 1] = y[i] + stepSize * derivative;

        derivative += stepSize * secondDerivative;
    }

    return y;
}

//********************************************/
//*               *Leapfrog*                 *
//********************************************/

vector<double> leapfrog(
    double initialX,
    double initialY,
    double initialVelocity,
    double stepSize,
    double finalX,
    function<double(double, double)> f){

    if (stepSize <= 0.0 || finalX < initialX){
            
            return {};
        }

    int steps = static_cast<int>(round((finalX - initialX) / stepSize));

    vector<double> y(steps + 1);

    y[0] = initialY;

    double velocity = initialVelocity;

    // Aproximacion inicial de la derivada en medio paso
    velocity += 0.5 * stepSize * f(initialX, y[0]);

    for (int i = 0; i < steps; i++){

        double xNext = initialX + (i + 1) * stepSize;

        y[i + 1] = y[i] + stepSize * velocity;

        velocity += stepSize * f(xNext, y[i + 1]);

    }

    return y;
}

//********************************************/
//                *Numerov*
//********************************************/

vector<double> numerov(
    double initialX,
    double initialY,
    double nextY,
    double stepSize,
    double finalX,
    function<double(double)> f){

    if (stepSize <= 0.0 || finalX < initialX){

            return {};
        }

    int steps = static_cast<int>(round((finalX - initialX) / stepSize));

    if (steps < 1){

            return {};
        }

    vector<double> y(steps + 1);

    y[0] = initialY;
    y[1] = nextY;

    for (int i = 1; i < steps; i++){

        double xPrevious = initialX + (i - 1) * stepSize;
        double xCurrent  = initialX + i * stepSize;
        double xNext     = initialX + (i + 1) * stepSize;

        double fPrevious = f(xPrevious);
        double fCurrent  = f(xCurrent);
        double fNext     = f(xNext);

        double numerator = 2.0 * (1.0 + 5.0 * stepSize * stepSize * fCurrent / 12.0) * y[i] - (1.0 - stepSize * stepSize * fPrevious / 12.0) * y[i - 1];

        double denominator = 1.0 - stepSize * stepSize * fNext / 12.0;

        if (abs(denominator) < 1e-14){

                return {};
            }

        y[i + 1] = numerator / denominator;

    }

    return y;
}