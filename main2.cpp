#include <iostream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include "entropy.h"

int main(int argc, char** argv){

	// txt file for generated data
	std::ofstream data_file(DATA_DIR_PATH + "data.txt", std::ios::app);

	// directory for images to be saved
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img2/";

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
        } catch (const std::exception& e) { }
	}

   	////////////////////////////////////
   	// exponential distribution
   	////////////////////////////////////

   	// vector for generated data
   	std::vector<float> data_exp(N);
   
	exp_array(data_exp, N);
    std::map<unsigned char const, size_t> map = count_bytes(data_exp);
    std::vector<double> X = calc_probability(map, N);
	double entropy = calc_entropy(X);
	auto hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential.png");
	delete hist;

	// data_file << N << ";" << "exponential;NO;NO;" << entropy << "\n"; 

	exp_array(data_exp, N, [](float x){return round(x * 8.0) / 8.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_8");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_8.png");
	delete hist;

	data_file << N << ";" << "exponential;8.0/8.0;NO;" << entropy << "\n"; 

	exp_array(data_exp, N, [](float x){return round(x * 10.0) / 10.0;});
    map = count_bytes(data_exp);
	X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_10");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_10.png");
	delete hist;

	// data_file << N << ";" << "exponential;10.0/10.0;NO;" << entropy << "\n";

	exp_array(data_exp, N, [](float x){return round(x * 16.0) / 16.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_16");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_16.png");
	delete hist;

	data_file << N << ";" << "exponential;16.0/16.0;NO;" << entropy << "\n";

	exp_array(data_exp, N, [](float x){return round(x * 32.0) / 32.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_32");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_32.png");
	delete hist;

	data_file << N << ";" << "exponential;32.0/32.0;NO;" << entropy << "\n";

   	////////////////////////////////////
   	// uniform distribution
   	////////////////////////////////////

   	// vector for generated data
   	std::vector<float> data_uniform(N);
   
	uniform_array(data_uniform, N);
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform.png");
	delete hist;

	uniform_array(data_uniform, N, [](float x){return round(x * 8.0) / 8.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_8");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_8.png");
	delete hist;

	data_file << N << ";" << "uniform;8.0/8.0;NO;" << entropy << "\n";

	uniform_array(data_uniform, N, [](float x){return round(x * 16.0) / 16.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_16");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_16.png");
	delete hist;

	data_file << N << ";" << "uniform;16.0/16.0;NO;" << entropy << "\n";

	uniform_array(data_uniform, N, [](float x){return round(x * 32.0) / 32.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_32");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_32.png");
	delete hist;

	data_file << N << ";" << "uniform;32.0/32.0;NO;" << entropy << "\n";

	uniform_array(data_uniform, N, [](float x){return round(x * 64.0) / 64.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_64");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_64.png");
	delete hist;

	data_file << N << ";" << "uniform;64.0/64.0;NO;" << entropy << "\n";

	data_file.close();

   	return 0;
}