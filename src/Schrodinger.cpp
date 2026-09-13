#include <vector>
#include <functional>

using namespace std;

vector<double> solveSchrodinger(
    const vector<double>& x,
    const vector<double>& potential,
    double effectiveMass,
    double energy,
    double hbar
)
{
    if (x.size() != potential.size() || x.size() < 2)
    {
        return {};
    }

    double dx = x[1] - x[0];

    vector<double> psi(x.size());

    psi[0] = 0.0;

    double velocity = 1.0;

    auto equation = [&](double position, double wavefunction)
    {
        size_t index = static_cast<size_t>(
            (position - x[0]) / dx
        );

        if (index >= potential.size())
        {
            index = potential.size() - 1;
        }

        return (2.0 * effectiveMass / (hbar * hbar)) *
               (potential[index] - energy) *
               wavefunction;
    };

    velocity += 0.5 * dx * equation(x[0], psi[0]);

    for (size_t i = 0; i < x.size() - 1; i++)
    {
        psi[i + 1] = psi[i] + dx * velocity;

        velocity += dx * equation(x[i + 1], psi[i + 1]);
    }

    return psi;
}