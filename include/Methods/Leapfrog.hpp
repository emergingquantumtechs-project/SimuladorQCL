#ifndef LEAPFROG_HPP
#define LEAPFROG_HPP

#include <vector>

namespace Methods
{
    std::vector<double> leapfrog(
        double initialY,
        double initialVelocity,
        double timeStep,
        double finalTime
    );
}

#endif