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

    // save histogram image to file 
    TCanvas *canvas = new TCanvas();
    hist->Draw();
    canvas->SaveAs((DATA_DIR_PATH + "compression_factor_exp.png").c_str());
    delete hist;


    std::cout << "\n\n";


    std::vector<float> *vec;
    TH1F *hist1 = new TH1F(labels[0].c_str(), "Byte occurrence frequency", 256, 0, 256);
    TH1F *hist2 = new TH1F(labels[1].c_str(), "Byte occurrence frequency", 256, 0, 256);
    TH1F *hist3 = new TH1F(labels[2].c_str(), "Byte occurrence frequency", 256, 0, 256);
    TH1F *hist4 = new TH1F(labels[3].c_str(), "Byte occurrence frequency", 256, 0, 256);
    TH1F *hist5 = new TH1F(labels[4].c_str(), "Byte occurrence frequency", 256, 0, 256);

    std::vector<TH1F*> histograms{
        hist1, hist2, hist3, hist4, hist5
    };

    auto hs = new THStack("hs","Stacked histograms EXP");

    hs->Add(hist1);
    hs->Add(hist2);
    hs->Add(hist3);
    hs->Add(hist4);
    hs->Add(hist5);

    for(int i=0; i<5; i++){
        tree_exp->SetBranchAddress(labels[i].c_str(), &vec);
        tree_exp->GetEntry(0);
        for(float val : *vec){
            unsigned char *bytes = reinterpret_cast<unsigned char*>(&val);
            for (size_t j = 0; j < sizeof(float); ++j)
                histograms[i]->Fill(bytes[j]);
            }
    }

    hist1->Draw();
    canvas->SaveAs((DATA_DIR_PATH + labels[0] + "_tree.png").c_str());

    hist2->Draw();
    canvas->SaveAs((DATA_DIR_PATH + labels[1] + "_tree.png").c_str());

    hist3->Draw();
    canvas->SaveAs((DATA_DIR_PATH + labels[2] + "_tree.png").c_str());

    hist4->Draw();
    canvas->SaveAs((DATA_DIR_PATH + labels[3] + "_tree.png").c_str());

    hist5->Draw();
    canvas->SaveAs((DATA_DIR_PATH + labels[4] + "_tree.png").c_str());

    hs->Draw("pfc nostack");
    canvas->SaveAs((DATA_DIR_PATH + "combined_hist_EXP.png").c_str());

    delete hist1, hist2, hist3, hist4, hist5, hs; 


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
    
    // // fill TTree with data, write data to disk, print TTree info
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

    // save histogram image to file 
    hist->Draw();
    canvas->SaveAs((DATA_DIR_PATH + "compression_factor_uni.png").c_str());
    delete hist;


    // std::vector<float> *vec2;
    // hist1 = new TH1F(labels2[0].c_str(), "Byte occurrence frequency", 256, 0, 256);
    // hist2 = new TH1F(labels2[1].c_str(), "Byte occurrence frequency", 256, 0, 256);
    // hist3 = new TH1F(labels2[2].c_str(), "Byte occurrence frequency", 256, 0, 256);
    // hist4 = new TH1F(labels2[3].c_str(), "Byte occurrence frequency", 256, 0, 256);
    // hist5 = new TH1F(labels2[4].c_str(), "Byte occurrence frequency", 256, 0, 256);

    // std::vector<TH1F*> histograms2{
    //     hist1, hist2, hist3, hist4, hist5
    // };

    // hs = new THStack("hs","Stacked histograms UNI");

    // hs->Add(hist1);
    // hs->Add(hist2);
    // hs->Add(hist3);
    // hs->Add(hist4);
    // hs->Add(hist5);

    // for(int i=0; i<5; i++){
    //     tree_uni->SetBranchAddress(labels2[i].c_str(), &vec2);
    //     tree_uni->GetEntry(0);
    //     for(float val : *vec2){
    //         unsigned char *bytes = reinterpret_cast<unsigned char*>(&val);
    //         for (size_t j = 0; j < sizeof(float); ++j)
    //             histograms2[i]->Fill(bytes[j]);
    //         }
    // }

    // hist1->Draw();
    // canvas->SaveAs((DATA_DIR_PATH + labels2[0] + "_tree.png").c_str());

    // hist2->Draw();
    // canvas->SaveAs((DATA_DIR_PATH + labels2[1] + "_tree.png").c_str());

    // hist3->Draw();
    // canvas->SaveAs((DATA_DIR_PATH + labels2[2] + "_tree.png").c_str());

    // hist4->Draw();
    // canvas->SaveAs((DATA_DIR_PATH + labels2[3] + "_tree.png").c_str());

    // hist5->Draw();
    // canvas->SaveAs((DATA_DIR_PATH + labels2[4] + "_tree.png").c_str());

    // hs->Draw("pfc nostack");
    // canvas->SaveAs((DATA_DIR_PATH + "combined_hist_UNI.png").c_str());

    // delete hist1, hist2, hist3, hist4, hist5, hs; 


    // close files
    file->Close();
	data_file.close();
    fclose(log_file);

    // reset stdout
    freopen("/dev/tty", "w", stdout);

    // free memory
    delete file, canvas, tree_exp, tree_uni;

   	return 0;
}
