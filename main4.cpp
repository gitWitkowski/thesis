#include <iostream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include "entropy.h"

void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::vector<unsigned char> &char_array,
	std::vector<unsigned char> &compressed_char_array,
	std::map<unsigned char const, size_t> &map,
	int compression_level,
	double rounding_num,
	TH1F *hist,
	std::ofstream &file,
	const std::string path
	);

int main(int argc, char** argv){

	// txt file for generated data
	std::ofstream data_file(DATA_DIR_PATH + "data.txt", std::ios::app);

	// directory for images to be saved
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img4/";

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
        } catch (const std::exception& e) { }
	}

   	////////////////////////////////////
   	//    exponential distribution 	  //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float>	data_exp(N);
	std::map<unsigned char const, size_t> map, map_before, map_after;
	double entropy, entropy_before, entropy_after;
	TH1F *hist, *hist_before, *hist_after;
    std::vector<unsigned char> data_exp_char, compressed_data_exp;
    std::vector<double> X, X_before, X_after;

	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 2.0, hist, data_file, IMG_DIR_PATH);
	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 4.0, hist, data_file, IMG_DIR_PATH);
	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 8.0, hist, data_file, IMG_DIR_PATH);
	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 16.0, hist, data_file, IMG_DIR_PATH);
	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 32.0, hist, data_file, IMG_DIR_PATH);
	run_case(EXP, data_exp, data_exp_char, compressed_data_exp, map, Z_BEST_COMPRESSION, 64.0, hist, data_file, IMG_DIR_PATH);


	////////////////////////////////////
   	// 		uniform distribution      //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_uniform(N);
    std::vector<unsigned char> data_uniform_char;
	std::vector<unsigned char> compressed_data_uniform;

	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 2.0, hist, data_file, IMG_DIR_PATH);
	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 4.0, hist, data_file, IMG_DIR_PATH);
	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 8.0, hist, data_file, IMG_DIR_PATH);
	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 16.0, hist, data_file, IMG_DIR_PATH);
	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 32.0, hist, data_file, IMG_DIR_PATH);
	run_case(UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, Z_BEST_COMPRESSION, 64.0, hist, data_file, IMG_DIR_PATH);

	data_file.close();

   	return 0;
}

void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::vector<unsigned char> &char_array,
	std::vector<unsigned char> &compressed_char_array,
	std::map<unsigned char const, size_t> &map,
	int compression_level,
	double rounding_num,
	TH1F *hist,
	std::ofstream &file,
	const std::string path
	){

	// function used to fill an array
	std::function<void(std::vector<float> &, size_t, std::function<float(float)>)> fill_array_f;

	if(distribution_type == EXP)
		fill_array_f = exp_array;
	else if(distribution_type == UNI)
		fill_array_f = uniform_array;
	else
		throw -1;

	// format string with precision (1 decimal place)
	std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << rounding_num;
    std::string rounding_num_str =  oss.str();

	std::string distr_name =  distribution_type == EXP ? "exponential" : "uniform";

	// fill array with choosen function
	fill_array_f(
		array, 
		N, 
		[rounding_num](float x){return round(x * rounding_num) / rounding_num;}
	);

    char_array = float_to_char_vector(array);

	compressed_char_array = deflate_vector(char_array, compression_level);

	map = count_bytes(compressed_char_array);

    auto X = calc_probability(map, N);
	
	double entropy = calc_entropy(X);

	hist = map_to_hist(
		map,
		"entropy: " + std::to_string(entropy),
		distr_name + "_entropy_in_chars_after_compression_rounded_" + rounding_num_str
	);
	
	save_histogram_to_file(
		hist, 
		path + distr_name +	"exponential_entropy_in_chars_after_compression_rounded_" + rounding_num_str + ".png"
	);

	delete hist;

	file << N << ";" << "exponential;"+ rounding_num_str+"/"+ rounding_num_str +";Z_BEST_COMPRESSION;" << entropy << ";" << compressed_char_array.size() << "\n"; 
}