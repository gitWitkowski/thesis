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
	data_file << "N;distribution;rounded;compression;entropy;size\n";

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

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_2", [](float x){return round(x * 2.0) / 2.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_4", [](float x){return round(x * 4.0) / 4.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_8", [](float x){return round(x * 8.0) / 8.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_16", [](float x){return round(x * 16.0) / 16.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_32", [](float x){return round(x * 32.0) / 32;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_64", [](float x){return round(x * 64.0) / 64;}, Z_BEST_COMPRESSION);

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_2", [](float x){return round(x * 2.0) / 2.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_4", [](float x){return round(x * 4.0) / 4.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_8", [](float x){return round(x * 8.0) / 8.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_16", [](float x){return round(x * 16.0) / 16.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_32", [](float x){return round(x * 32.0) / 32;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_64", [](float x){return round(x * 64.0) / 64;});

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_10", [](float x){return round(x * 10.0) / 10.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_100", [](float x){return round(x * 4.0) / 4.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_1000", [](float x){return round(x * 8.0) / 8.0;});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "built_in_round", [](float x){return round(x);});
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "no_round", nullptr);

	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_10", [](float x){return round(x * 10.0) / 10.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_100", [](float x){return round(x * 4.0) / 4.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_1000", [](float x){return round(x * 8.0) / 8.0;}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "built_in_round", [](float x){return round(x);}, Z_BEST_COMPRESSION);
	run_case(distr::EXP, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "no_round", nullptr, Z_BEST_COMPRESSION);


	////////////////////////////////////
   	// 		uniform distribution      //
   	////////////////////////////////////

   	// prepare variables
   	std::vector<float> data_uniform(N);
    std::vector<unsigned char> data_uniform_char;
	std::vector<unsigned char> compressed_data_uniform;

	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_2", [](float x){return round(x * 2.0) / 2.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_4", [](float x){return round(x * 4.0) / 4.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_8", [](float x){return round(x * 8.0) / 8.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_16", [](float x){return round(x * 16.0) / 16.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_32", [](float x){return round(x * 32.0) / 32;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_64", [](float x){return round(x * 64.0) / 64;}, Z_BEST_COMPRESSION);

	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_2", [](float x){return round(x * 2.0) / 2.0;});
	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_4", [](float x){return round(x * 4.0) / 4.0;});
	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_8", [](float x){return round(x * 8.0) / 8.0;});
	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_16", [](float x){return round(x * 16.0) / 16.0;});
	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_32", [](float x){return round(x * 32.0) / 32;});
	run_case(distr::UNI, data_uniform, data_exp_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_64", [](float x){return round(x * 64.0) / 64;});

	run_case(distr::UNI, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_10", [](float x){return round(x * 10.0) / 10.0;});
	run_case(distr::UNI, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_100", [](float x){return round(x * 4.0) / 4.0;});
	run_case(distr::UNI, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "round_1000", [](float x){return round(x * 8.0) / 8.0;});
	run_case(distr::UNI, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "built_in_round", [](float x){return round(x);});
	run_case(distr::UNI, data_exp, data_exp_char, compressed_data_exp, map, hist, data_file, IMG_DIR_PATH, "no_round", nullptr);

	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_10", [](float x){return round(x * 10.0) / 10.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_100", [](float x){return round(x * 4.0) / 4.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "round_1000", [](float x){return round(x * 8.0) / 8.0;}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "built_in_round", [](float x){return round(x);}, Z_BEST_COMPRESSION);
	run_case(distr::UNI, data_uniform, data_uniform_char, compressed_data_uniform, map, hist, data_file, IMG_DIR_PATH, "no_round", nullptr, Z_BEST_COMPRESSION);

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