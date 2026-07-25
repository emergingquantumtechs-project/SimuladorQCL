#include <iostream>

// CLASE QUE RECIBA LOS ARCHIVOS:
// SOLICITAR AL USUARIO DATOS DE ENTRADA COMO POTENCIAL DE ENERGIA, TEMPERATURA Y EL NUMERO DE PERIODOS 
class LaserStructure {
public:
    double potentialEnergy = 0.0;
    double temperature = 0.0;
    int numPeriods = 0;

    LaserStructure() = default;
    LaserStructure(double v, double t, int n)
        : potentialEnergy(v), temperature(t), numPeriods(n) {}
};

int main()
{
    std::cout << "Quantum Cascade Laser Simulator\n";

    return 0;
}

