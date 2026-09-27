#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

using namespace std;

vector<double> numerov(
    double initialX,
    double initialY,
    double nextY,
    double stepSize,
    double finalX,
    function<double(double)> f
);

int main()
{
    double step = 0.1;
    double finalX = 1.0;

    double initialY = 1.0;

    double nextY = 1.0 - 0.5 * step * step;

    auto equation = [](double x)
    {
        return -1.0;
    };

    auto result = numerov(
        0.0,
        initialY,
        nextY,
        step,
        finalX,
        equation
    );

    cout << "x\tNumerov\tExact\tError\n\n";

    for (size_t i = 0; i < result.size(); i++)
    {
        double x = i * step;

        double exact = cos(x);

        double error = abs(result[i] - exact);

        cout << x << "\t" << result[i] << "\t" << exact << "\t" << error << '\n';
    }

    return 0;
}