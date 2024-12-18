#include <iostream>
#include "entropy.h"

/**
 * The purpose of this file is to evaluate how data preparation affects its compressibility. 
 * Data is generated for two distributions, and selected rounding methods are applied. 
 * The data is stored in two TTree structures, each containing multiple branches, with each 
 * branch corresponding to a specific test case. The data is written to disk and is automatically 
 * compressed by default.
 */

int main(int argc, char** argv){

    // .root file to save TTree
    TFile *file  = TFile::Open("../data/data.root","RECREATE");

    // create a TTree
    TTree *tree_exp = new TTree("tree EXP", "TTree for exponential distribution");
    TTree *tree_uni = new TTree("tree UNI", "TTree for uniform distribution");

    // txt file for output (tree data)
    FILE* log_file = freopen("../data/log_file.txt", "w", stdout);

    // canvas to draw on
    TCanvas *canvas = new TCanvas();


   	////////////////////////////////////
   	//    exponential distribution 	  //
   	////////////////////////////////////   

    std::vector<float> base_data(N);

    exp_array(base_data, N);

    std::vector<float> data_exp_1(base_data);
    tree_exp->Branch("EXP_no_round", &data_exp_1);
    
    std::vector<float> data_exp_2(base_data);
    array_apply_transform(data_exp_2, N, round_fun_1000);
	tree_exp->Branch("EXP_1000", &data_exp_2);

    std::vector<float>	data_exp_3(base_data);
    array_apply_transform(data_exp_3, N, round_fun_built_in);
	tree_exp->Branch("EXP_built_in_round", &data_exp_3);

    std::vector<float>	data_exp_4(base_data);
    array_apply_transform(data_exp_4, N, round_fun_2);
	tree_exp->Branch("EXP_2", &data_exp_4);

    std::vector<float>	data_exp_5(base_data);
    array_apply_transform(data_exp_5, N, round_fun_64);
    tree_exp->Branch("EXP_64", &data_exp_5);

    // fill TTree with data, write data to disk, print TTree info 
    tree_exp->Fill();
    tree_exp->Write();
	tree_exp->Print();

    // create labels and histogram
    std::string labels[] = {"EXP_no_round", "EXP_1000", "EXP_built_in_round", "EXP_2", "EXP_64"};
    TH1F *hist = new TH1F("hist", "Exponential distribution; branch name; compression factor", 5, 0, 5);
    hist->SetMinimum(0);

    // set bin values and add labels
    for (int i = 0; i < 5; ++i) {
        TBranch* branch = tree_exp->GetBranch(labels[i].c_str());
        double comp_fact = (double)branch->GetTotBytes() / branch->GetZipBytes();
        hist->SetBinContent(i + 1, comp_fact);
        hist->GetXaxis()->SetBinLabel(i + 1, labels[i].c_str());
    }
    hist->Draw();

    // add additional branch info
    for (int i = 0; i < 5; ++i) {
        TBranch* branch = tree_exp->GetBranch(labels[i].c_str());
        double comp_fact = (double)branch->GetTotBytes() / branch->GetZipBytes();
        Long64_t compressed_size = branch->GetZipBytes();

        double center = hist->GetXaxis()->GetBinCenter(i+1);

        TText *comprs = new TText(center, 0.6, Form("compression: %1.2f", comp_fact));
        comprs->SetTextSize(0.024);
        comprs->SetTextAlign(22); // center text
        comprs->Draw();

        TText *size = new TText(center, 0.4, Form("file size: %lld B", compressed_size));
        size->SetTextSize(0.024);
        size->SetTextAlign(22);// center text
        size->Draw();
    } 

    // save histogram image to file 
    canvas->SaveAs((DATA_DIR_PATH + "compression_factor_exp.png").c_str());
    delete hist;


    std::cout << "\n\n";


	////////////////////////////////////
   	// 		uniform distribution      //
   	////////////////////////////////////
	
    uniform_array(base_data, N);

    std::vector<float> data_uni_1(base_data);
    tree_uni->Branch("UNI_no_round", &data_uni_1);
    
    std::vector<float> data_uni_2(base_data);
    array_apply_transform(data_uni_2, N, round_fun_1000);
	tree_uni->Branch("UNI_1000", &data_uni_2);

    std::vector<float>	data_uni_3(base_data);
    array_apply_transform(data_uni_3, N, round_fun_built_in);
	tree_uni->Branch("UNI_built_in_round", &data_uni_3);

    std::vector<float>	data_uni_4(base_data);
    array_apply_transform(data_uni_4, N, round_fun_2);
	tree_uni->Branch("UNI_2", &data_uni_4);

    std::vector<float>	data_uni_5(base_data);
    array_apply_transform(data_uni_5, N, round_fun_64);
    tree_uni->Branch("UNI_64", &data_uni_5);
    
    // fill TTree with data, write data to disk, print TTree info
    tree_uni->Fill();
    tree_uni->Write();
	tree_uni->Print();

    // create labels and histogram
    std::string labels2[] = {"UNI_no_round", "UNI_1000", "UNI_built_in_round", "UNI_2", "UNI_64"};
    hist = new TH1F("hist", "Uniform distribution; branch name; compression factor", 5, 0, 5);
    hist->SetMinimum(0);

    // fill TTree with data, write data to disk, print TTree info 
    for (int i = 0; i < 5; ++i) {
        TBranch* branch = tree_uni->GetBranch(labels2[i].c_str());
        double comp_fact = (double)branch->GetTotBytes() / branch->GetZipBytes();
        hist->SetBinContent(i + 1, comp_fact);
        hist->GetXaxis()->SetBinLabel(i + 1, labels2[i].c_str());
    }
    hist->Draw();

    // add additional branch info
    for (int i = 0; i < 5; ++i) {
        TBranch* branch = tree_uni->GetBranch(labels2[i].c_str());
        double comp_fact = (double)branch->GetTotBytes() / branch->GetZipBytes();
        Long64_t compressed_size = branch->GetZipBytes();

        double center = hist->GetXaxis()->GetBinCenter(i+1);

        TText *comprs = new TText(center, 0.6, Form("compression: %1.2f", comp_fact));
        comprs->SetTextSize(0.024);
        comprs->SetTextAlign(22); // center text
        comprs->Draw();

        TText *size = new TText(center, 0.4, Form("file size: %lld B", compressed_size));
        size->SetTextSize(0.024);
        size->SetTextAlign(22);// center text
        size->Draw();
    } 

    // save histogram image to file 
    canvas->SaveAs((DATA_DIR_PATH + "compression_factor_uni.png").c_str());

    // close files
    file->Close();
    fclose(log_file);

    // reset stdout
    freopen("/dev/tty", "w", stdout);

    // free memory
    delete file, canvas, tree_exp, tree_uni, hist;

   	return 0;
}
