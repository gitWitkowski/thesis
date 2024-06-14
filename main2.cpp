#include <iostream>
#include <filesystem>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include "entropy.h"

int main(int argc, char** argv){

	// directory for images to be saved
	const std::string IMG_DIR_PATH = std::filesystem::current_path().string() + "/../img2/";

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
        } catch (const std::exception& e) { }
	}

   	// number of generated numbers
	constexpr size_t n = 4'000;

   	////////////////////////////////////
   	// exponential distribution
   	////////////////////////////////////

   	// vector for generated data
   	std::vector<float> data_exp(n);
   
	exp_array(data_exp, n);
    std::map<unsigned char const, size_t> map = count_bytes(data_exp);
    std::vector<double> X = calc_probability(map, n);
	double entropy = calc_entropy(X);
	auto hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential.png");
	delete hist;

	exp_array(data_exp, n, [](float x){return round(x * 8.0) / 8.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_8");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_8.png");
	delete hist;

	exp_array(data_exp, n, [](float x){return round(x * 10.0) / 10.0;});
    map = count_bytes(data_exp);
	X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_10");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_10.png");
	delete hist;

	exp_array(data_exp, n, [](float x){return round(x * 16.0) / 16.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_16");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_16.png");
	delete hist;

	exp_array(data_exp, n, [](float x){return round(x * 32.0) / 32.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_32");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_32.png");
	delete hist;

   	////////////////////////////////////
   	// uniform distribution
   	////////////////////////////////////

   	// vector for generated data
   	std::vector<float> data_uniform(n);
   
	uniform_array(data_uniform, n);
    map = count_bytes(data_uniform);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform.png");
	delete hist;

	uniform_array(data_uniform, n, [](float x){return round(x * 8.0) / 8.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_8");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_8.png");
	delete hist;

	uniform_array(data_uniform, n, [](float x){return round(x * 16.0) / 16.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_16");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_16.png");
	delete hist;

	uniform_array(data_uniform, n, [](float x){return round(x * 32.0) / 32.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_32");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_32.png");
	delete hist;

	uniform_array(data_uniform, n, [](float x){return round(x * 64.0) / 64.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, n);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_64");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_64.png");
	delete hist;

   	return 0;
}