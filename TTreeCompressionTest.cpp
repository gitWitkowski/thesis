#include <iostream>
#include "entropy.h"

/**
 * TThe purpose of this file is to evaluate how data preparation affects its compressibility. 
 * Data is generated for two distributions, and selected rounding methods are applied. 
 * The data is stored in two TTree structures, each containing multiple branches, with each 
 * branch corresponding to a specific test case. The data is written to disk and is automatically 
 * compressed by default.
 */

int main(int argc, char** argv){

    // dummy file
    std::ofstream data_file("", std::ios::app);

    // .root file to save TTree
    TFile *file  = TFile::Open("../data/data.root","RECREATE");

    // create a TTree
    TTree *tree_exp = new TTree("tree EXP", "TTree for exponential distribution");
    TTree *tree_uni = new TTree("tree UNI", "TTree for uniform distribution");

    // txt file for output (tree data)
    FILE* log_file = freopen("../data/log_file.txt", "w", stdout);

    // prepare variables
	std::map<unsigned char const, size_t> map;
    std::vector<unsigned char> data_exp_char, compressed_data_exp, data_uniform_char, compressed_data_uniform;


   	////////////////////////////////////
   	//    exponential distribution 	  //
   	////////////////////////////////////   

    std::vector<float>	data_exp_1(N);
	run_case(distr::EXP, data_exp_1, data_exp_char, compressed_data_exp, map, data_file, "", "", nullptr);
    tree_exp->Branch("EXP no round", &data_exp_1);
    
    std::vector<float>	data_exp_2(N);
	run_case(distr::EXP, data_exp_2, data_exp_char, compressed_data_exp, map, data_file, "", "", round_fun_1000);
	tree_exp->Branch("EXP 1000", &data_exp_2);

    std::vector<float>	data_exp_3(N);
    run_case(distr::EXP, data_exp_1, data_exp_char, compressed_data_exp, map, data_file, "", "", round_fun_built_in);
	tree_exp->Branch("EXP round()", &data_exp_3);

    std::vector<float>	data_exp_4(N);
    run_case(distr::EXP, data_exp_4, data_exp_char, compressed_data_exp, map, data_file, "", "", round_fun_2);
	tree_exp->Branch("EXP 2", &data_exp_4);

    std::vector<float>	data_exp_5(N);
    run_case(distr::EXP, data_exp_5, data_exp_char, compressed_data_exp, map, data_file, "", "", round_fun_64);
    tree_exp->Branch("EXP 64", &data_exp_5);

    // fill TTree with data, write data to disk, print TTree info 
    tree_exp->Fill();
    tree_exp->Write();
	tree_exp->Print();


    std::cout << "\n\n";


	////////////////////////////////////
   	// 		uniform distribution      //
   	////////////////////////////////////
	
    std::vector<float>	data_uni_1(N);
	run_case(distr::UNI, data_uni_1, data_uniform_char, compressed_data_uniform, map, data_file, "", "", nullptr);
    tree_uni->Branch("UNI no round", &data_uni_1);

    std::vector<float>	data_uni_2(N);
	run_case(distr::UNI, data_uni_2, data_uniform_char, compressed_data_uniform, map, data_file, "", "", round_fun_1000);
    tree_uni->Branch("UNI 1000", &data_uni_2);

    std::vector<float>	data_uni_3(N);
	run_case(distr::UNI, data_uni_3, data_uniform_char, compressed_data_uniform, map, data_file, "", "", round_fun_built_in);
    tree_uni->Branch("UNI round()", &data_uni_3);

    std::vector<float>	data_uni_4(N);
	run_case(distr::UNI, data_uni_4, data_uniform_char, compressed_data_uniform, map, data_file, "", "", round_fun_2);
    tree_uni->Branch("UNI 2", &data_uni_4);

    std::vector<float>	data_uni_5(N);
	run_case(distr::UNI, data_uni_5, data_uniform_char, compressed_data_uniform, map, data_file, "", "", round_fun_64);
    tree_uni->Branch("UNI 64", &data_uni_5);
    
    // fill TTree with data, write data to disk, print TTree info
    tree_uni->Fill();
    tree_uni->Write();
	tree_uni->Print();


    // close files
    file->Close();
	data_file.close();
    fclose(log_file);

    // reset stdout
    freopen("/dev/tty", "w", stdout);

    // free memory
    delete file;

   	return 0;
}
