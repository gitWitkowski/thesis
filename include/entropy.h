#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include "TRandom3.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include <string>

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
 * @param f transform function
 */
void exp_array(std::vector<float> &arr, size_t n, std::function<float(float)> f = nullptr);

/**
 * Fills given vector with float numbers using uniform distribution
 * 
 * @param arr reference to vector for float values
 * @param n length of an array
 */
void uniform_array(std::vector<float> &arr, size_t n, std::function<float(float)> f = nullptr);

/**
 * Counts number of occurrences of each byte from given array
 * and returns map of bytes occurences
 * 
 * @param arr reference to vector for float values
 * @return map with byte as key and its number of occurences as value
 */
std::map<unsigned char const, size_t> count_bytes(const std::vector<float> &data);

/**
 * Calculates probability of occurence for each byte in a map
 * and returns vector X of probabilities
 * 
 * @param occurrence_map map with byte as key and its number of occurences as value
 * @param n number of generated samples
 * @return vector of probabilities
 */
std::vector<double> calc_probability(std::map<unsigned char const, size_t> &occurrence_map, size_t n);

/**
 * Save histogram to a file with given path
 * 
 * @param hist pointer to histogram class
 * @param file_path string where to save the file
 */
void save_histogram_to_file(TH1 *hist, std::string file_path);

/**
 * Takes map and returns pointer to histogram
 * presenting number of byte occurences
 * 
 * @param occurrence_map reference to map
 * @param name string for name
 * @param title string for title
 * @return pointer to histogram TH1F *
 */
TH1F *map_to_hist(
    std::map<unsigned char const, size_t> &occurrence_map,
    std::string name = "",
    std::string title = ""
    );