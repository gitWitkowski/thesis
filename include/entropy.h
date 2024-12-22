#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <zlib.h>
#include <filesystem>
#include <fstream>

#include "TRandom3.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TError.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include <TText.h>

/// @brief enum for distribution type (exponential/uniform)
enum distr {EXP, UNI};

/// @brief path to data directory
const std::string DATA_DIR_PATH = std::filesystem::current_path().string() + "/../data/";

/// @brief directory for images to be saved
const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img/";

/// @brief number of generated numbers
constexpr size_t N = 4'000;

/// @brief custom rounding functions
extern std::function<float(float)> 
	round_fun_10,
	round_fun_100,
	round_fun_1000,
	round_fun_2,
	round_fun_4,
	round_fun_8,
	round_fun_16,
	round_fun_32,
	round_fun_64,
	round_fun_built_in;

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
 * @param f transform function
 */
void uniform_array(std::vector<float> &arr, size_t n, std::function<float(float)> f = nullptr);

/**
 * Apply transformation defined by f function to every element of the std::vector
 * 
 * @param arr reference to vector
 * @param n length of an array
 * @param f transform function
 */
void array_apply_transform(std::vector<float> &arr, size_t n, std::function<float(float)> f);

/**
 * Counts number of occurrences of each byte from given array
 * and returns map of bytes occurrences
 * 
 * @param arr reference to vector for float values
 * @return map with byte as key and its number of occurrences as value
 */
std::map<unsigned char const, size_t> count_bytes(const std::vector<float> &data);

/**
 * Counts number of occurrences of each byte from given array
 * and returns map of bytes occurrences
 * 
 * @param arr reference to vector with data stored as chars
 * @return map with byte as key and its number of occurrences as value
 */
std::map<unsigned char const, size_t> count_bytes(const std::vector<unsigned char> &data);

/**
 * Calculates probability of occurrence for each byte in a map
 * and returns vector X of probabilities
 * 
 * @param occurrence_map map with byte as key and its number of occurrences as value
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
 * presenting number of byte occurrences
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

/**
 * Takes vector of floats and converts 
 * data into vector of chars
 * 
 * @param floats vector of data to be converted
 * @return vector of unsigned chars
 */
std::vector<unsigned char> float_to_char_vector(const std::vector<float>& floats);

/**
 * Takes vector of chars and compresses data according to choose compression level
 * 
 * @param data vector of data to be compressed
 * @param compression_level available options: Z_NO_COMPRESSION, Z_BEST_SPEED, Z_BEST_COMPRESSION, Z_DEFAULT_COMPRESSION
 * @return vector of unsigned chars with compressed data
 */
std::vector<unsigned char> compress_vector(const std::vector<unsigned char>& data, int compression_level);

/**
 * 
 * 
 * @param distribution_type enum specifying distribution used to generate data  
 * @param array vector of generated data
 * @param char_array vector of data stored in unsigned char
 * @param compressed_char_array vector of data stored in unsigned char after compression
 * @param map map with byte as key and its number of occurrences as value 
 * @param file file for generated data
 * @param path location for images to be saved in
 * @param title text included in file names and histogram title
 * @param rounding_f function applied to every element of array
 * @param save_byte_map flag indicating whether to save byte values to file
 * @param compression_level compression level
 */
void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::vector<unsigned char> &char_array,
	std::vector<unsigned char> &compressed_char_array,
	std::map<unsigned char const, size_t> &map,
	std::ofstream &file,
	const std::string path,
	const std::string title,
	std::function<float(float)> rounding_f,
	bool save_byte_map,
	int compression_level = Z_NO_COMPRESSION
	);