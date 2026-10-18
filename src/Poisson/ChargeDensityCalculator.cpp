#include <vector>
#include <cmath>
#include <numbers>

using namespace std;

//***ChargeDensityCalculator***
// Clase para cálculos relacionados con la densidad de carga
// Contiene métodos para calcular la ocupación de Fermi-Dirac y calcular la densidad de carga rho(z) 
// a partir de funciones de onda y densidades superficiales


class ChargeDensityCalculator{
private:
    double electron_Charge;
    double boltzmann_Constant;
    double temperature;
    double chemical_Potential;
    double effective_Mass;
    double hbar;

public:
    //Constantes físicas
    ChargeDensityCalculator(
        double charge,
        double kB,
        double temp,
        double effectiveMass,
        double hbarValue
    )
        : electron_Charge(charge),
          boltzmann_Constant(kB),
          temperature(temp),
          chemical_Potential(0.0), //provisional
          effective_Mass(effectiveMass),
          hbar(hbarValue)
    {
    }

    //***CalculateDensityOfStates***
    // Calcula la densidad de estados para las subbandas
    // cuantizadas en la dirección de crecimiento.
    // Densidad de estados = m* / (π ħ²)
    // Donde:
    // m*: masa efectiva
    // ħ: constante de Planck reducida

    double CalculateDensityOfStates()
{
    return effective_Mass / (numbers::pi * hbar * hbar);
}
    //***CalculateFermiDirac***
    // Calcula la ocupación de Fermi-Dirac f(E) para un vector de energías dado
    // f(E) = 1 / (1 + exp((E - mu) / (kB * T)))
    // E y mu se expresan en eV y se convierten a joules
    // mediante la carga elemental antes de dividir entre kB*T.

    // Donde:
    // E: energía de la subbanda
    // mu: potencial químico (POR CALCULAR CON MÉTODO DE RAÍCES)
    // kB: constante de Boltzmann
    // T: temperatura
    // Devuelve un vector con la ocupación correspondiente a cada energía
    // Ocupacion f(E) de cada subbanda

    vector<double> CalculateFermiDirac(vector<double> energies){
        vector<double> occupation(energies.size());

        for (int i = 0; i < energies.size(); i++){
            occupation[i] = 1.0 / (1.0 + exp(((energies[i]-chemical_Potential)*electron_Charge)/(boltzmann_Constant*temperature)));
        }

        //Si E_i << mu, la ocupación se acerca a 1.
        //Si E_i >> mu , se acerca a 0.
        //Si E_i = mu, es aproximadamente 0.5.

        return occupation;

        
    }

    //***Calcular Densidad Electronica 
    //recibe wavefunctions, subband_densities y z

    //Calcula   n(z) = SUM [ n_i |psi_i(z)|² ]
    //Donde:
    //n(z): densidad electronica
    //n_i: densidad superficial de cada subbanda
    //psi_i(z): función de onda de cada subbanda
    //z: mallado espacial
    //Devuelve un vector con la densidad electronica n(z) en cada punto del mallado espacial

    vector<double> CalculateElectronDensity(
        const vector<vector<double>>& wavefunctions, //ψ1(z), ψ2(z), ψ3(z), ...
        const vector<double>& subband_densities, //n1, n2, n3, ...
        const vector<double>& z) //mallado espacial z0​, z1​, z2​, ...
    {
        size_t gridSize = z.size(); //n(z) 
        
        vector<double> electronDensity(gridSize, 0.0); //inicializar densidad electronica n(z) a cero

        //***Sumar la contribución de cada subbanda.
        //recorrer las funciones de onda
        for (size_t i = 0; i < wavefunctions.size(); i++){

            double ni = subband_densities[i];

            for (size_t j = 0; j < gridSize; j++) {

                electronDensity[j] += ni * wavefunctions[i][j] * wavefunctions[i][j];
            }
        }

        return electronDensity;
    }

    //***Calcular Densidad de Carga rho(z)***
    //Calcula  rho(z) = e * [Nd(z) - n(z)]
    //Donde:
    //rho(z): densidad de carga
    //Nd(z): densidad de dopaje o densidad de donadores
    //n(z): densidad electronica
    //e: carga elemental
    
    //Devuelve un vector con la densidad de carga rho(z) 
    //en cada punto del mallado espacial
    // La contribucion de los donadores es positiva
    // y la contribucion de los electrones es negativa.

     vector<double> CalculateChargeDensity(
        const vector<double>& electronDensity,
        const vector<double>& donorDensity){

        size_t gridSize = donorDensity.size();

        vector<double> chargeDensity(gridSize,0.0);

        for (size_t j = 0; j < gridSize; j++){
            chargeDensity[j] = electron_Charge * (donorDensity[j] - electronDensity[j]);
        }

        return chargeDensity;
    }
};
