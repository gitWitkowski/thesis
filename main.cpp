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
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img/";

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
        } catch (const std::exception& e) { }
	}

	// data file headers
	data_file << "N;distribution;rounding_type;compression;entropy;size\n";

	// prepare variables
   	std::vector<float>	data_exp(N), data_uniform(N);
	std::map<unsigned char const, size_t> map;
    std::vector<unsigned char> data_exp_char, compressed_data_exp, data_uniform_char, compressed_data_uniform;
	double entropy;


   	////////////////////////////////////
   	//    exponential distribution 	  //
   	////////////////////////////////////

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "no_round", nullptr);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_10", round_fun_10);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_100", round_fun_100);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_1000", round_fun_1000);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "built_in_round", round_fun_built_in);

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "no_round", nullptr, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_10", round_fun_10, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_100", round_fun_100, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_1000", round_fun_1000, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "built_in_round", round_fun_built_in, Z_BEST_COMPRESSION);

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_2", round_fun_2);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_4", round_fun_4);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_8", round_fun_8);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_16", round_fun_16);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_32", round_fun_32);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_64", round_fun_64);

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_2", round_fun_2, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_4", round_fun_4, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_8", round_fun_8, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_16", round_fun_16, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_32", round_fun_32, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, data_file, IMG_DIR_PATH, "round_64", round_fun_64, Z_BEST_COMPRESSION);


	////////////////////////////////////
   	// 		uniform distribution      //
   	////////////////////////////////////
	
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "no_round", nullptr);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_10", round_fun_10);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_100", round_fun_100);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_1000", round_fun_1000);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "built_in_round", round_fun_built_in);

	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "no_round", nullptr, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_10", round_fun_10, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_100", round_fun_100, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_1000", round_fun_1000, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "built_in_round", round_fun_built_in, Z_BEST_COMPRESSION);

	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_2", round_fun_2);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_4", round_fun_4);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_8", round_fun_8);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_16", round_fun_16);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_32", round_fun_32);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_64", round_fun_64);

	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_2", round_fun_2, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_4", round_fun_4, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_8", round_fun_8, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_16", round_fun_16, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_32", round_fun_32, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, data_file, IMG_DIR_PATH, "round_64", round_fun_64, Z_BEST_COMPRESSION);


	data_file.close();

   	return 0;
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