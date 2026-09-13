#include <iostream>
#include <cmath>
#include <vector>
#include <functional>

using namespace std;

vector<double> leapfrog(
    double initialX,
    double initialY,
    double initialVelocity,
    double stepSize,
    double finalX,
    function<double(double, double)> f
    );

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

    auto result = leapfrog(
        initialX,
        initialY,
        initialVelocity,
        stepSize,
        finalX,
        equation
    );

    cout << "Leapfrog test\n\n";

    cout << "x\tNumerical\tExact\n";

    for (size_t i = 0; i < result.size(); i++)
    {
        double x = initialX + i * stepSize;

        double exact = cos(x);

        cout << x << "\t"
                  << result[i] << "\t"
                  << exact << '\n';
    }

    return 0;
}