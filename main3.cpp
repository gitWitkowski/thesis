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
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img3/";

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

    std::vector<unsigned char> data_exp_char = float_to_char_vector(data_exp);

	std::vector<unsigned char> compressed_data_exp = deflate_vector(data_exp_char, Z_BEST_COMPRESSION);

    std::map<unsigned char const, size_t> map_before = count_bytes(data_exp_char);
    std::vector<double> X_before = calc_probability(map_before, N);
	double entropy_before = calc_entropy(X_before);

	std::map<unsigned char const, size_t> map_after = count_bytes(compressed_data_exp);
    std::vector<double> X_after = calc_probability(map_after, N);
	double entropy_after = calc_entropy(X_after);

	auto hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"exponential_entropy_in_chars_before_compression");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "exponential_entropy_in_chars_before_compression.png");
	delete hist_before;

	auto hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"exponential_entropy_in_chars_after_compression");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "exponential_entropy_in_chars_after_compression.png");
	delete hist_after;

	data_file << N << ";" << "exponential;NO;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	exp_array(data_exp, N, [](float x){return round(x * 10.0) / 10.0;});

    data_exp_char = float_to_char_vector(data_exp);

	compressed_data_exp = deflate_vector(data_exp_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_exp_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_exp);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"exponential_entropy_in_chars_before_compression_rounded_10");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "exponential_entropy_in_chars_before_compression_rounded_10.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"exponential_entropy_in_chars_after_compression_rounded_10");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "exponential_entropy_in_chars_after_compression_rounded_10.png");
	delete hist_after;

	data_file << N << ";" << "exponential;10.0/10.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	exp_array(data_exp, N, [](float x){return round(x * 100.0) / 100.0;});

    data_exp_char = float_to_char_vector(data_exp);

	compressed_data_exp = deflate_vector(data_exp_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_exp_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_exp);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"exponential_entropy_in_chars_before_compression_rounded_100");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "exponential_entropy_in_chars_before_compression_rounded_100.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"exponential_entropy_in_chars_after_compression_rounded_100");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "exponential_entropy_in_chars_after_compression_rounded_100.png");
	delete hist_after;

	data_file << N << ";" << "exponential;100.0/100.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	exp_array(data_exp, N, [](float x){return round(x * 1000.0) / 1000.0;});

    data_exp_char = float_to_char_vector(data_exp);

	compressed_data_exp = deflate_vector(data_exp_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_exp_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_exp);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"exponential_entropy_in_chars_before_compression_rounded_1000");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "exponential_entropy_in_chars_before_compression_rounded_1000.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"exponential_entropy_in_chars_after_compression_rounded_1000");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "exponential_entropy_in_chars_after_compression_rounded_1000.png");
	delete hist_after;

	data_file << N << ";" << "exponential;1000.0/1000.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	exp_array(data_exp, N, [](float x){return round(x);});

    data_exp_char = float_to_char_vector(data_exp);

	compressed_data_exp = deflate_vector(data_exp_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_exp_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_exp);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"exponential_entropy_in_chars_before_compression_round");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "exponential_entropy_in_chars_before_compression_round.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"exponential_entropy_in_chars_after_compression_round");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "exponential_entropy_in_chars_after_compression_round.png");
	delete hist_after;

	data_file << N << ";" << "exponential;round().0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

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
	"exponential");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform.png");
	delete hist;

    std::vector<unsigned char> data_uniform_char = float_to_char_vector(data_uniform);

	std::vector<unsigned char> compressed_data_uniform = deflate_vector(data_uniform_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_uniform_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_uniform);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"uniform_entropy_in_chars_before_compression");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "uniform_entropy_in_chars_before_compression.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"uniform_entropy_in_chars_after_compression");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "uniform_entropy_in_chars_after_compression.png");
	delete hist_after;

	data_file << N << ";" << "uniform;NO;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	uniform_array(data_uniform, N, [](float x){return round(x * 10.0) / 10.0;});

    data_uniform_char = float_to_char_vector(data_uniform);

	compressed_data_uniform = deflate_vector(data_uniform_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_uniform_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_uniform);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"uniform_entropy_in_chars_before_compression_rounded_10");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "uniform_entropy_in_chars_before_compression_rounded_10.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"uniform_entropy_in_chars_after_compression_rounded_10");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "uniform_entropy_in_chars_after_compression_rounded_10.png");
	delete hist_after;

	data_file << N << ";" << "uniform;10.0/10.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	uniform_array(data_uniform, N, [](float x){return round(x * 100.0) / 100.0;});

    data_uniform_char = float_to_char_vector(data_uniform);

	compressed_data_uniform = deflate_vector(data_uniform_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_uniform_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_uniform);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"uniform_entropy_in_chars_before_compression_rounded_100");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "uniform_entropy_in_chars_before_compression_rounded_100.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"uniform_entropy_in_chars_after_compression_rounded_100");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "uniform_entropy_in_chars_after_compression_rounded_100.png");
	delete hist_after;

	data_file << N << ";" << "uniform;100.0/100.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	uniform_array(data_uniform, N, [](float x){return round(x * 1000.0) / 1000.0;});

    data_uniform_char = float_to_char_vector(data_uniform);

	compressed_data_uniform = deflate_vector(data_uniform_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_uniform_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_uniform);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"uniform_entropy_in_chars_before_compression_rounded_1000");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "uniform_entropy_in_chars_before_compression_rounded_1000.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"uniform_entropy_in_chars_after_compression_rounded_1000");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "uniform_entropy_in_chars_after_compression_rounded_1000.png");
	delete hist_after;

	data_file << N << ";" << "uniform;1000.0/1000.0;Z_BEST_COMPRESSION;" << entropy_after << "\n";

	/////////////////////////////////

	uniform_array(data_uniform, N, [](float x){return round(x);});

    data_uniform_char = float_to_char_vector(data_uniform);

	compressed_data_uniform = deflate_vector(data_uniform_char, Z_BEST_COMPRESSION);

    map_before = count_bytes(data_uniform_char);
    X_before = calc_probability(map_before, N);
	entropy_before = calc_entropy(X_before);

	map_after = count_bytes(compressed_data_uniform);
    X_after = calc_probability(map_after, N);
	entropy_after = calc_entropy(X_after);

	hist_before = map_to_hist(map_before, "entropy: " + std::to_string(entropy_before),
	"uniform_entropy_in_chars_before_compression_round");
	save_histogram_to_file(hist_before, IMG_DIR_PATH + "uniform_entropy_in_chars_before_compression_round.png");
	delete hist_before;

	hist_after = map_to_hist(map_after, "entropy: " + std::to_string(entropy_after),
	"uniform_entropy_in_chars_after_compression_round");
	save_histogram_to_file(hist_after, IMG_DIR_PATH + "uniform_entropy_in_chars_after_compression_round.png");
	delete hist_after;

	data_file << N << ";" << "uniform;round();Z_BEST_COMPRESSION;" << entropy_after << "\n";

	data_file.close();

   	return 0;
}