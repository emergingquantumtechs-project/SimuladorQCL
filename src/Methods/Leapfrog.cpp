#include "../../include/Methods/Leapfrog.hpp"

std::vector<double> Methods::leapfrog(
    double initialX,
    double initialY,
    double initialVelocity,
    double stepSize,
    double finalX,
    std::function<double(double, double)> f
)
{
    int steps = static_cast<int>((finalX - initialX) / stepSize);

    std::vector<double> y(steps + 1);

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