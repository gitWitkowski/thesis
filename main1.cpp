#include <iostream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "THStack.h"
#include "entropy.h"

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

int main(int argc, char** argv){

	// directory for images to be saved
	const std::string IMG_DIR_PATH = DATA_DIR_PATH + "img1/";

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

	exp_array(data_exp, N, [](float x){return round(x * 10.0) / 10.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_to_1dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_to_1dm.png");
	delete hist;

	exp_array(data_exp, N, [](float x){return round(x * 100.0) / 100.0;});
    map = count_bytes(data_exp);
	X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_to_2dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_to_2dm.png");
	delete hist;

	exp_array(data_exp, N, [](float x){return round(x * 1000.0) / 1000.0;});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_to_3dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_to_3dm.png");
	delete hist;

	exp_array(data_exp, N, [](float x){return round(x);});
    map = count_bytes(data_exp);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"exponential_rounded_to_0dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "exponential_rounded_to_0dm.png");
	delete hist;

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

	uniform_array(data_uniform, N, [](float x){return round(x * 10.0) / 10.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_to_1dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_to_1dm.png");
	delete hist;

	uniform_array(data_uniform, N, [](float x){return round(x * 100.0) / 100.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_to_2dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_to_2dm.png");
	delete hist;

	uniform_array(data_uniform, N, [](float x){return round(x * 1000.0) / 1000.0;});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_to_3dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_to_3dm.png");
	delete hist;

	uniform_array(data_uniform, N, [](float x){return round(x);});
    map = count_bytes(data_uniform);
    X = calc_probability(map, N);
	entropy = calc_entropy(X);
	hist = map_to_hist(map, "entropy: " + std::to_string(entropy),
	"uniform_rounded_to_0dm");
	save_histogram_to_file(hist, IMG_DIR_PATH + "uniform_rounded_to_0dm.png");
	delete hist;

   	return 0;
}