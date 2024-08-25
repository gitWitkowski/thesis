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

	// data file headers
	data_file << "N;distribution;rounded;compression;entropy;size\n";
	
	// directory for images to be saved
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img1/";

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
        } catch (const std::exception& e) { }
	}

   	////////////////////////////////////
   	//     exponential distribution   //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_exp(N);
    std::map<unsigned char const, size_t> map;
	TH1F *hist;

	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 10.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 100.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, 1000.0);
	run_case(distr::EXP, data_exp, map, hist, data_file, IMG_DIR_PATH, BUILTIN_ROUND, [](float x){return round(x);});

   	////////////////////////////////////
   	// 	    uniform distribution      //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_uniform(N);
   
	run_case(distr::UNI, data_uniform, map, hist, data_file, IMG_DIR_PATH);
	run_case(distr::UNI, data_uniform, map, hist, data_file, IMG_DIR_PATH, 10.0);
	run_case(distr::UNI, data_uniform, map, hist, data_file, IMG_DIR_PATH, 100.0);
	run_case(distr::UNI, data_uniform, map, hist, data_file, IMG_DIR_PATH, 1000.0);
	run_case(distr::UNI, data_uniform, map, hist, data_file, IMG_DIR_PATH, BUILTIN_ROUND, [](float x){return round(x);});

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

void DrawHist(int argc, char** argv, std::vector<float> &d1, std::vector<float> &d2) {

	// TApplication for displaying graphics
   	TApplication app("app", &argc, argv);
   	auto cst1 = new TCanvas("cst1","cst1",700,400);
   	cst1->Divide(2,1);
   	cst1->Modified(); cst1->Update();
   	TRootCanvas *rc = (TRootCanvas *)cst1->GetCanvasImp();
   	// terminate program on window close
   	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

   	// first histogram
   	auto hst11 = new TH1F("hst11", "", 70, -1, 6);
   	for(size_t i=0; i<d1.size(); ++i)
		hst11->Fill(d1[i]); // fill histogram with data
 
   	// second histogram
   	auto hst12 = new TH1F("hst12", "", 70, -1, 6);
   	for(size_t i=0; i<d2.size(); ++i)
      	hst12->Fill(d2[i]); // fill histogram with data
  
   	// position and draw histograms
   	cst1->cd(1); hst12->Draw();
   	cst1->cd(2); hst11->Draw();

   	// run app
   	app.Run();
}