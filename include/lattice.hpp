#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>
#include <omp.h>
#include <string>
#include <unordered_set>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

class Lattice {
    /*
     * Lattice class for Ising model simulation.
     * Only used to store spin configurations internally and provide basic flip operations.
     */

    private:
        size_t N_rows;
        size_t N_cols;
        std::vector<int> matx;

    public:
        Lattice(size_t rows, size_t cols, std::string init_mode = "hot", int seed = -1);
        ~Lattice() = default;
        size_t getNumRows() const;
        size_t getNumCols() const;
        void flipSingleSpin(size_t row, size_t col);
        void flipSpins(std::unordered_set<std::pair<int, int>, PairHash>& cluster);
        int getSpin(size_t idx) const;
        int operator()(size_t row, size_t col) const;
        friend std::ostream& operator<<(std::ostream& os, const Lattice& lattice);
};