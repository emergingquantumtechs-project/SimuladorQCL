#include <vector>
#include <functional>
#include <cmath>

using namespace std;

//********************************************/
//                *Leapfrog*
//********************************************/

vector<double> leapfrog(
    double initialX,
    double initialY,
    double initialVelocity,
    double stepSize,
    double finalX,
    function<double(double, double)> f
)
{
    int steps = static_cast<int>((finalX - initialX) / stepSize);

    vector<double> y(steps + 1);

    y[0] = initialY;

    double velocity = initialVelocity;
    double x = initialX;

    velocity += 0.5 * stepSize * f(x, y[0]);

    for (int i = 0; i < steps; i++){

        y[i + 1] = y[i] + stepSize * velocity;

        x += stepSize;

        velocity += stepSize * f(x, y[i + 1]);
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
    function<double(double)> f
)
{
    int steps = static_cast<int>((finalX - initialX) / stepSize
    );

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

        double numerator = 2.0 * (1.0 + (stepSize * stepSize / 12.0) * fCurrent) * y[i] - (1.0 - (stepSize * stepSize / 12.0) * fPrevious) * y[i - 1];
        double denominator = 1.0 - (stepSize * stepSize / 12.0) * fNext;

        y[i + 1] = numerator / denominator;

    }

    return y;
}