#include <iomanip>
#include <iostream>
#include <cmath>

#include "../include/Methods/Leapfrog.hpp"
#include "../include/MaterialManager.hpp"

int main()
{
    double initialY = 1.0;
    double initialVelocity = 0.0;

    double timeStep = 0.1;
    double finalTime = 1.0;

    auto result = Methods::leapfrog(
        initialY,
        initialVelocity,
        timeStep,
        finalTime
    );

    std::cout << "Leapfrog test\n\n";
    std::cout << "t\tNumerical\tExact\n";

    for (size_t i = 0; i < result.size(); i++){

        double t = i * timeStep;

        double exact = std::cos(t);

        std::cout << t << "\t" << result[i] << "\t" << exact << '\n';
    }

    return 0;
}