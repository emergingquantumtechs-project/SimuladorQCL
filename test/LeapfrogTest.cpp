#include <iostream>
#include <cmath>
#include <vector>
#include <functional>

namespace Methods {
    std::vector<double> leapfrog(
        double initialX,
        double initialY,
        double initialVelocity,
        double stepSize,
        double finalX,
        std::function<double(double, double)> f
    );
}

int main()
{
    auto equation = [](double x, double y)
    {
        return -y;
    };

    double initialX = 0.0;
    double initialY = 1.0;
    double initialVelocity = 0.0;

    double stepSize = 0.1;
    double finalX = 1.0;

    auto result = Methods::leapfrog(
        initialX,
        initialY,
        initialVelocity,
        stepSize,
        finalX,
        equation
    );

    std::cout << "Leapfrog test\n\n";

    std::cout << "x\tNumerical\tExact\n";

    for (size_t i = 0; i < result.size(); i++)
    {
        double x = initialX + i * stepSize;

        double exact = std::cos(x);

        std::cout << x << "\t"
                  << result[i] << "\t"
                  << exact << '\n';
    }

    return 0;
}