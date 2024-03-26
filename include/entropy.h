#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include "TRandom3.h"

/**
 * Calculates entropy based on given input values
 * 
 * @param X vector of probabilities
 * @return entropy value
 */
double calc_entropy(std::vector<double> X);

/**
 * Fills given vector with float numbers using exponential distribution
 * 
 * @param arr float array pointer
 * @param n length of an array
 */
void exp_array(std::vector<float> &arr, size_t n);

/**
 * Fills given vector with float numbers using uniform distribution
 * 
 * @param arr reference to vector for float values
 * @param n length of an array
 */
void uniform_array(std::vector<float> &arr, size_t n);

/**
 * Counts number of occurrences of each byte from given array
 * and returns vector X of probabilities of each byte appearance
 * 
 * @param arr reference to vector for float values
 * @param n length of an array
 * @return vector of probabilities
 */
std::vector<double> count_bytes(const std::vector<float> &data, size_t n);