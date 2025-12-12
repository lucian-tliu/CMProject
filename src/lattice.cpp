#include "lattice.hpp"

Lattice::Lattice(size_t rows, size_t cols, std::string init_mode, int seed)
    : N_rows(rows), N_cols(cols) {
    if (init_mode == "cold") {
        matx.resize(rows * cols, 1);
        return;
    }else if (init_mode == "heat") {
        matx.resize(rows * cols);
        std::mt19937 rng_;

        if (seed >= 0) {
            rng_.seed(seed);
        } else {
            std::random_device rd;
            rng_.seed(rd());
        }

        std::uniform_int_distribution<int> bit(0, 1);

        #pragma omp parallel for simd
        for (size_t i = 0; i < rows * cols; i++) {
            matx[i] = bit(rng_) * 2 - 1;
        };
    };
};

size_t Lattice::getNumRows() const {
    return N_rows;
};

size_t Lattice::getNumCols() const {
    return N_cols;
};

int Lattice::getSpin(size_t idx) const {
    return matx[idx];
};

void Lattice::flipSingleSpin(size_t row, size_t col) {
    matx[row * N_cols + col] *= -1;
};

void Lattice::flipSpins(std::unordered_set<std::pair<int, int>, PairHash>& cluster) {
    for (const auto& [row, col] : cluster) {
        flipSingleSpin(row, col);
    }
};

int Lattice::operator()(size_t row, size_t col) const {
    return matx[row * N_cols + col];
};

std::ostream& operator<<(std::ostream& os, const Lattice& lattice) {
    os << "[";
    for (size_t i = 0; i < lattice.N_rows; i++) {
        for (size_t j = 0; j < lattice.N_cols; j++) {
            if (i == 0 && j == 0)
                os << std::setw(2) << std::right << lattice.matx[i * lattice.N_cols + j];
            else if (j != lattice.N_cols - 1)
                os << std::setw(3) << std::right << lattice.matx[i * lattice.N_cols + j];
            else if (i != lattice.N_rows - 1)
                os << std::setw(3) << std::right << lattice.matx[i * lattice.N_cols + j] << std::endl;
            else
                os << std::setw(3) << std::right << lattice.matx[i * lattice.N_cols + j];
        }
    }
    os << "]";
    return os;
}