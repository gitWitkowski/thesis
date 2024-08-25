#include <iostream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include "entropy.h"

#define NO_ROUNDING -1
#define BUILTIN_ROUND -2

void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::map<unsigned char const, size_t> &map,
	TH1F *hist,
	std::ofstream &file,
	const std::string path,
	double rounding_num = NO_ROUNDING,
	std::function<float(float)> rounding_f = nullptr
	);

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
   	//    exponential distribution    //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_exp(N);
    std::map<unsigned char const, size_t> map;
	TH1F *hist;

	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 2.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 4.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 8.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 16.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 32.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 64.0);

   	////////////////////////////////////
   	//      uniform distribution      //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_uniform(N);
   
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 2.0);
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 4.0);
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 8.0);
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 16.0);
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 32.0);
	run_case(distr::UNI, data_exp, map, hist, data_file, IMG_DIR_PATH, 64.0);

	data_file.close();

   	return 0;
}

void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::map<unsigned char const, size_t> &map,
	TH1F *hist,
	std::ofstream &file,
	const std::string path,
	double rounding_num,
	std::function<float(float)> rounding_f
	){

	std::string distr_name =  distribution_type == EXP ? "exponential" : "uniform";

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
    std::string rounding_num_str = rounding_num == BUILTIN_ROUND ? "0" : oss.str();

	// set rounding type string
	std::string rounding_type;
	if(rounding_num == BUILTIN_ROUND)
		rounding_type = "round()";
	else if(rounding_num == NO_ROUNDING)
		rounding_type = "NO";
	else
		rounding_type = rounding_num_str;

	// set rounding function
	if(rounding_num == NO_ROUNDING)
		rounding_f = nullptr;
	else if(rounding_num != BUILTIN_ROUND)
		rounding_f = [rounding_num](float x){ return round(x * rounding_num) / rounding_num; };

	// fill array with choosen function
	fill_array_f(
		array, 
		N, 
		rounding_f
	);

    map = count_bytes(array);

    auto X = calc_probability(map, N);
	
	double entropy = calc_entropy(X);

	hist = map_to_hist(
		map,
		"entropy: " + std::to_string(entropy),
		distr_name + (rounding_f ? "_rounded_to_" + rounding_type : "")
	);
	
	save_histogram_to_file(
		hist, 
		path + distr_name + (rounding_f ? "_rounded_to_" + rounding_type : "") + ".png"
	);

	delete hist;

	file << N << ";" << distr_name << ";" << rounding_type << ";NO;" << entropy << ";" << array.size() * sizeof(float) << "\n"; 
}