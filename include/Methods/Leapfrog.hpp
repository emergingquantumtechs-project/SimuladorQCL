#ifndef LEAPFROG_HPP
#define LEAPFROG_HPP

#include <vector>
#include <functional>

namespace Methods
{
    std::vector<double> leapfrog(
        double initialX,
        double initialY,
        double initialVelocity,
        double stepSize,
        double finalX,
        std::function<double(double, double)> f
    );
}

#endif