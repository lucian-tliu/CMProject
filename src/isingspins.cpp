#include "isingspins.hpp"

IsingSpins::IsingSpins(size_t rows, size_t cols, double J_val, std::string init_mode, int seed)
    : Lattice(rows, cols, init_mode, seed), J(J_val) {
};

double IsingSpins::getJ() const {
    return J;
}

double IsingSpins::meanEnergy() const {
    double energy = 0.0;
    size_t N_rows = getNumRows();
    size_t N_cols = getNumCols();

    #pragma omp parallel for reduction(+:energy)
    for (size_t n = 0; n < N_rows*N_cols; n++) {
        size_t i = n / N_cols;
        size_t j = n % N_cols;
        int S = (*this).getSpin(n);
        int neighbor_right = (*this)(i, (j + 1) % N_cols);
        int neighbor_down = (*this)((i + 1) % N_rows, j);
        energy += -J * S * (neighbor_right + neighbor_down);
    }
    return energy/(N_rows*N_cols);
};

double IsingSpins::meanMagnetization() const {
    double magnetization = 0.0;
    size_t N_rows = getNumRows();
    size_t N_cols = getNumCols();

    // different calculation for ferromagnetic and antiferromagnetic cases
    if (J > 0.0){
        #pragma omp parallel for reduction(+:magnetization)
        for (size_t n = 0; n < N_rows*N_cols; n++) {
            magnetization += ((*this).getSpin(n));
        }
    } else {
        // divide the lattice into two sublattices like a checkerboard pattern
        #pragma omp parallel for reduction(+:magnetization)
        for (size_t n = 0; n < N_rows*N_cols; n++) {
            size_t i = n / N_cols;
            size_t j = n % N_cols;
            int S = (*this).getSpin(n);
            int checkerboard_factor = ((i + j) % 2 == 0) ? 1 : -1;
            magnetization += S * checkerboard_factor;
        }
    }
    
    return fabs(magnetization)/(N_rows*N_cols);
};