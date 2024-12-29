#include "entropy.h"

/**
 * Based on the analysis conducted using DeltaRMatchingTool from IDTPM package, the truncateFloat function with a 14-bit mask was chosen, 
 * as the optimal rounding function, which enhances the compression capacity while preserving efficiency scores for track matching. 
 * In this file pT, eta and phi values were randomly generated, using probability distributions similar to these from the analysis data 
 * used in a previous track matching study. The tracking parameters are stored in LorentzVector, which uses the cylindrical coordinate 
 * system PtEtaPhiM4D. Entropy is calculated before and after conversion to px, py, pz parameters, checking whether coordinate system impacts
 * the entropy and compression factor. The data is stored in the TTree branch and saved to a disk with compression enabled by default.
 * Program saves data to file, to be later used for bar plot generation, comparing entropy and compression factor values for default
 * and rounded data.
 */

void lorentzVectorRoundCompress(bool round, std::ofstream &data_file_PxPyPz, std::ofstream &data_file_PtEtaPhi);

int main(int argc, char** argv){

    // text file for obtained data
    std::ofstream data_file_PxPyPz(DATA_DIR_PATH + "vector_data_PxPyPz.txt");
    std::ofstream data_file_PtEtaPhi(DATA_DIR_PATH + "vector_data_PtEtaPhi.txt");

    // headers line
    data_file_PxPyPz << "entropy;comp_fact;round_indicator\n";
    data_file_PtEtaPhi << "entropy;comp_fact;round_indicator\n";
    
    // generate data and calculate metrics
    lorentzVectorRoundCompress(true, data_file_PxPyPz, data_file_PtEtaPhi);
    lorentzVectorRoundCompress(false, data_file_PxPyPz, data_file_PtEtaPhi);

    // cleanup
    data_file_PxPyPz.close();
    data_file_PtEtaPhi.close();

   	return 0;
}

void lorentzVectorRoundCompress(bool round, std::ofstream &data_file_PxPyPz, std::ofstream &data_file_PtEtaPhi){
    using ROOT::Math::LorentzVector, ROOT::Math::PtEtaPhiM4D;
    using namespace std::string_literals;

    TRandom3 rng(0);
    std::vector<LorentzVector<PtEtaPhiM4D<float>>> lorentzVectorArray(0); 

    std::string round_indicator = round ? "rounded" : "default";

    // cylindrical coordinate system parameters histograms
    std::unique_ptr<TH1F> ptDistr(new TH1F("pT", ("Distribution of pT values "s + round_indicator).c_str(), 100, 0, 100));
    ptDistr->SetYTitle("Entries");
    ptDistr->SetXTitle("p_{T}");

    std::unique_ptr<TH1F> etaDistr(new TH1F("eta", ("Distribution of eta values "s + round_indicator).c_str(), 100, -2.5, 2.5));
    etaDistr->SetYTitle("Entries");
    etaDistr->SetXTitle("\\eta");

    std::unique_ptr<TH1F> phiDistr(new TH1F("phi", ("Distribution of phi values "s + round_indicator).c_str(), 100, -M_PI, M_PI));
    phiDistr->SetYTitle("Entries");
    phiDistr->SetXTitle("\\Phi");

    // cartesian coordinate system parameters histograms
    std::unique_ptr<TH1F> pxDistr(new TH1F("px", ("Distribution of px values "s + round_indicator).c_str(), 100, 0, 100));
    phiDistr->SetYTitle("Entries");
    phiDistr->SetXTitle("p_{x}");
    
    std::unique_ptr<TH1F> pyDistr(new TH1F("py", ("Distribution of py values "s + round_indicator).c_str(), 100, 0, 100));
    phiDistr->SetYTitle("Entries");
    phiDistr->SetXTitle("p_{y}");

    std::unique_ptr<TH1F> pzDistr(new TH1F("pz", ("Distribution of pz values "s + round_indicator).c_str(), 100, 0, 100));
    phiDistr->SetYTitle("Entries");
    phiDistr->SetXTitle("p_{z}");

    // loop for data generation and histograms filling
    for(int i=0; i<N; i++){

        // original parameters
        float pT = round ? truncateFloat_14(rng.Exp(0.7)*10) : rng.Exp(0.7)*10; 
        float eta = round ? truncateFloat_14(rng.Uniform(-2.5, 2.5)) : rng.Uniform(-2.5, 2.5);
        float phi = round ? truncateFloat_14(rng.Uniform(-M_PI, M_PI)) : rng.Uniform(-M_PI, M_PI);

        // lorentz vector storing parameters
        auto vector = LorentzVector<PtEtaPhiM4D<float>>(PtEtaPhiM4D<float>(pT, eta, phi, 0));

        // fill histograms
        ptDistr->Fill(vector.Pt());
        etaDistr->Fill(vector.Eta());
        phiDistr->Fill(vector.Phi());

        // convert parameters and fill histograms
        pxDistr->Fill(vector.Px());
        pyDistr->Fill(vector.Py());
        pzDistr->Fill(vector.Pz());

        // save lorentz vector to an array
        lorentzVectorArray.push_back(vector);
    }

    // save parameter histograms
    save_histogram_to_file(ptDistr.get(), IMG_DIR_PATH + "ptDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(etaDistr.get(), IMG_DIR_PATH + "etaDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(phiDistr.get(), IMG_DIR_PATH + "phiDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pxDistr.get(), IMG_DIR_PATH + "pxDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pyDistr.get(), IMG_DIR_PATH + "pyDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pzDistr.get(), IMG_DIR_PATH + "pzDistribution_"s + round_indicator +  ".png");

    // vector for storing all generated float values
    // later used to calculate entropy
    std::vector<float> 
        allValuesPtEtaPhi(0), // cylindrical coordinate system 
        allValuesPxPyPz(0); // cartesian coordinate system

    // fill float vector for generated parameters, converted to px, py, pz
    for(LorentzVector<PtEtaPhiM4D<float>> x : lorentzVectorArray){
        allValuesPtEtaPhi.push_back(x.Pt());
        allValuesPtEtaPhi.push_back(x.Eta());
        allValuesPtEtaPhi.push_back(x.Phi());
        allValuesPxPyPz.push_back(x.Px());
        allValuesPxPyPz.push_back(x.Py());
        allValuesPxPyPz.push_back(x.Pz());
    }

    // .root file to save TTree
    TFile *file  = TFile::Open(("../data/vector_data"s + round_indicator + ".root").c_str(),"RECREATE");

    // create a TTree
    TTree *tree = new TTree(round_indicator.c_str(), ("TTree storing values from LorentzVector, "s + round_indicator).c_str());

    // add tree branches for float data
    auto branchPxPyPz = tree->Branch("PxPyPz", &allValuesPxPyPz);
    auto branchPtEtaPhi = tree->Branch("PtEtaPhi", &allValuesPtEtaPhi);

    // fill TTree with data, write data to disk 
    tree->Fill();
    tree->Write();


    //////////////////////
    //      PxPyPz      //
    //////////////////////

    // map with byte occurrence frequency
    std::map<const unsigned char, size_t> map = count_bytes(allValuesPxPyPz);

    // calculate probability and entropy for generated data
    std::vector<double> X = calc_probability(map, 3*N);
	double entropy = calc_entropy(X);

    // calculate compression factor
    double comp_fact = (double)branchPxPyPz->GetTotBytes() / branchPxPyPz->GetZipBytes();

    // save data entry to text file
    data_file_PxPyPz << entropy << ";" << comp_fact << ";" << round_indicator << "\n";


    ////////////////////////
    //      PtEtaPhi      //
    ////////////////////////

    // map with byte occurrence frequency
    map = count_bytes(allValuesPtEtaPhi);

    // calculate probability and entropy for generated data
    X = calc_probability(map, 3*N);
	entropy = calc_entropy(X);

    // calculate compression factor
    comp_fact = (double)branchPtEtaPhi->GetTotBytes() / branchPtEtaPhi->GetZipBytes();

    // save data entry to text file
    data_file_PtEtaPhi << entropy << ";" << comp_fact << ";" << round_indicator << "\n";


    // free memory
    delete file, tree;
}