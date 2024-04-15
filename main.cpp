#include "Simulation.h"
#include <random>

int main()
{
    std::srand(time(0));

    Simulation sim = Simulation(100, 800, 600);
    sim.run();

    return 0;

}