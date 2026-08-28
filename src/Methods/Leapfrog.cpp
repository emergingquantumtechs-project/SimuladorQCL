#include "../../include/Methods/Leapfrog.hpp"

std::vector<double> Methods::leapfrog(
    double initialY,
    double initialVelocity,
    double timeStep,
    double finalTime
)

{
    int steps = static_cast<int>(finalTime / timeStep);

    std::vector<double> y(steps + 1);

    y[0] = initialY;

    double velocity = initialVelocity;
    velocity += 0.5 * timeStep * (-y[0]);

    for (int i = 0; i < steps; i++){

        y[i + 1] = y[i] + timeStep * velocity;

        velocity += timeStep * (-y[i + 1]);
    
    }

    return y;
}