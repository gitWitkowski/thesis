#pragma once
#include <vector>
#include <algorithm>
#include <cmath>

/**
 * Calculates entropy based on given input values
 * 
 * @param X vector of probabilities
 * @return entropy value
 */
double calc_entropy(std::vector<double> X);