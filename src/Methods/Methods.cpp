#include <vector>
#include <functional>

using namespace std;

//********************************************/
//*               *Leapfrog*                 *
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