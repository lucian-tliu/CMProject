#pragma once

#include "isingspins.hpp"
#include <omp.h>
#include <random>
#include <cmath>
#include <deque>
#include <unordered_set>
#include <random>
#include <string>
#include <vector>
#include <tuple>
#include <optional>

void metropolisStep(IsingSpins& s, double kBT);
size_t wolffStep(IsingSpins& s, double kBT);

std::optional<std::tuple<std::vector<double>, std::vector<double>, size_t>>
multipleSteps(IsingSpins& s, double kBT, size_t n_steps, 
              const std::string& algorithm, bool record);