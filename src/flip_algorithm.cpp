#include "flip_algorithm.hpp"

std::random_device rd;
std::mt19937 rng_(rd());

void metropolisStep(IsingSpins& s, double kBT) {
    /*
     * Metropolis step for Ising model simulation.
     * Randomly flip a spin based on Metropolis criterion.
     */
    size_t N_rows = s.getNumRows();
    size_t N_cols = s.getNumCols();
    double J = s.getJ();

    std::uniform_int_distribution<int> uni(0, N_rows * N_cols - 1);
    std::uniform_real_distribution<double> uni_real(0.0, 1.0);
    int random_index = uni(rng_);
    size_t i = random_index / N_cols;
    size_t j = random_index % N_cols;

    int S = s(i, j);
    int neighbor_right = s(i, (j + 1) % N_cols);
    int neighbor_down = s((i + 1) % N_rows, j);
    int neighbor_left = s(i, (j - 1 + N_cols) % N_cols);
    int neighbor_up = s((i - 1 + N_rows) % N_rows, j);
    
    // calculate energy change and apply Metropolis criterion
    double dE = 2.0 * J * S * (neighbor_right + neighbor_down + neighbor_left + neighbor_up);
    if (dE < 0 || uni_real(rng_) < std::exp(-dE / kBT)) {
        s.flipSingleSpin(i, j);
    }
}

size_t wolffStep(IsingSpins& s, double kBT) {
    /*
     * Wolff step for Ising model simulation.
     * Grow a cluster of spins and flip them together.
     */
    size_t N_rows = s.getNumRows();
    size_t N_cols = s.getNumCols();
    double J = s.getJ();

    // register random number generators
    std::uniform_int_distribution<int> uni(0, N_rows * N_cols - 1);
    std::uniform_real_distribution<double> uni_real(0.0, 1.0);

    int random_index = uni(rng_);
    size_t i = random_index / N_cols;
    size_t j = random_index % N_cols;
    std::pair<int, int> idx(i, j);

    int S = 0;
    double P_add = 1.0 - std::exp(-2.0 * fabs(J) / kBT);

    // deque is used for storing the cluster growth front
    std::deque<std::pair<int, int>> cluster_queue;

    // unordered_set is used for storing the cluster members
    std::unordered_set<std::pair<int, int>, PairHash> cluster;

    cluster_queue.push_back({i, j});
    cluster.insert({i, j});

    // Grow the cluster
    while (!cluster_queue.empty()) {
        auto [x, y] = cluster_queue.front();
        S = s(x, y);
        cluster_queue.pop_front();

        std::vector<std::pair<int, int>> neighbors = {
            { (x + 1) % N_rows, y },
            { (x - 1 + N_rows) % N_rows, y },
            { x, (y + 1) % N_cols },
            { x, (y - 1 + N_cols) % N_cols }
        };

        for (const auto& [nx, ny] : neighbors) {
            if (J > 0.0 && s(nx, ny) == S && cluster.find({nx, ny}) == cluster.end()) {
                if (uni_real(rng_) < P_add) {
                    cluster.insert({nx, ny});
                    cluster_queue.push_back({nx, ny});
                }
            } else if (J < 0.0 && s(nx, ny) == -S && cluster.find({nx, ny}) == cluster.end()) {
                if (uni_real(rng_) < P_add) {
                    cluster.insert({nx, ny});
                    cluster_queue.push_back({nx, ny});
                }
            }
        }
    }
    s.flipSpins(cluster);
    return cluster.size();
}

std::optional<std::tuple<std::vector<double>, std::vector<double>, size_t>>
multipleSteps(IsingSpins& s, double kBT, size_t n_steps, const std::string& algorithm, bool record, size_t lag = 1) {
    /*
     * Perform multiple steps of the specified algorithm (Metropolis or Wolff).
     * Optionally record mean energy and magnetization after each step.
     */
    std::vector<double> energies;
    std::vector<double> magnetizations;
    size_t mean_cluster_size = 0;

    // optionally reserve memory to save time
    if (record){
        energies.resize(n_steps / lag + 1);
        magnetizations.resize(n_steps / lag + 1);
    }

    if (algorithm == "metropolis") {
        for (size_t step = 0; step < n_steps; step++) {
            metropolisStep(s, kBT);
            if (record && step % lag == 0) {
                energies[step / lag] = s.meanEnergy();
                magnetizations[step / lag] = s.meanMagnetization();
            }
        }
    } else if (algorithm == "wolff") {
        size_t cluster_size = 0;
        for (size_t step = 0; step < n_steps; step++) {
            cluster_size += wolffStep(s, kBT);
            if (record && step % lag == 0) {
                energies[step / lag] = s.meanEnergy();
                magnetizations[step / lag] = s.meanMagnetization();
            }
        }
        mean_cluster_size = cluster_size / n_steps;
    } else {
        throw std::invalid_argument("Invalid algorithm specified. Use 'metropolis' or 'wolff'.");
    }

    if (record) {
        return std::make_tuple(energies, magnetizations, mean_cluster_size);
    } else {
        return std::nullopt;
    }
}