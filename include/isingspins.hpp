#pragma once 
 
#include "lattice.hpp"

class IsingSpins : public Lattice {
    /*
     * IsingSpins class for Ising model simulation.
     * Inherits from Lattice and adds Ising-specific properties and methods.
     */
    private:
        double J;
    public:
        IsingSpins(size_t rows, size_t cols, double J_val, std::string init_mode = "hot", int seed = -1);
        ~IsingSpins() = default;
        double getJ() const;
        double meanEnergy() const;
        double meanMagnetization() const;
};