#include "entropy.h"

/**
 * Based on the analysis conducted using DeltaRMatchingTool from IDTPM package, the truncateFloat function with a 14-bit mask was chosen, 
 * as the optimal rounding function, which enhances the compression capacity while preserving efficiency scores for track matching. 
 * In this file pT, eta and phi values were randomly generated, using probability distributions similar to these from the analysis data 
 * used in a previous track matching study. The tracking parameters are stored in LorentzVector, which uses the cylindrical coordinate 
 * system PtEtaPhiM4D; entropy is calculated after conversion to px, py, pz parameters. Additionally, data are stored in the TTree branch
 * and saved to a disk with compression enabled by default. Program saves data to file, to be later used for bar plot generation, comparing
 * entropy and compression factor values for default and rounded data.
 */

void lorentzVectorRoundCompress(bool round, std::ofstream &data_file);

int main(int argc, char** argv){

    // text file for obtained data
    std::ofstream data_file(DATA_DIR_PATH + "vector_data.txt");

    // headers line
    data_file << "entropy;comp_fact;round_indicator\n";
    
    // generate data and calculate metrics
    lorentzVectorRoundCompress(true, data_file);
    lorentzVectorRoundCompress(false, data_file);

    // cleanup
    data_file.close();

   	return 0;
}

void lorentzVectorRoundCompress(bool round, std::ofstream &data_file){
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
        float pT = rng.Exp(0.7)*10;
        float eta = rng.Uniform(-2.5, 2.5);
        float phi = rng.Uniform(-M_PI, M_PI);

        // fill histograms
        ptDistr->Fill(pT);
        etaDistr->Fill(eta);
        phiDistr->Fill(phi);

        // lorentz vector storing parameters
        auto vector = LorentzVector<PtEtaPhiM4D<float>>(PtEtaPhiM4D<float>(pT, eta, phi, 0));

        // convert parameters and fill histograms
        pxDistr->Fill(vector.Px());
        pyDistr->Fill(vector.Py());
        pzDistr->Fill(vector.Pz());

        // save lorentz vector to an array
        lorentzVectorArray.push_back(vector);
    }

    // vector for storing all generated float values
    // later used to calculate entropy
    std::vector<float> allValues(0);

    // fill float vector for generated parameters, converted to px, py, pz
    for(LorentzVector<PtEtaPhiM4D<float>> x : lorentzVectorArray){
        allValues.push_back(
            round ?
            truncateFloat_14(x.Px()) : 
            x.Px()
        );
        allValues.push_back(
            round ? truncateFloat_14(x.Py()) : 
            x.Py()
        );
        allValues.push_back(
            round ? truncateFloat_14(x.Pz()) : 
            x.Pz()
        );
    }

    // map with byte occurrence frequency
    std::map<const unsigned char, size_t> map = count_bytes(allValues);

    // calculate probability and entropy for generated data
    std::vector<double> X = calc_probability(map, 3*N);
	double entropy = calc_entropy(X);

    // save parameter histograms
    save_histogram_to_file(ptDistr.get(), IMG_DIR_PATH + "ptDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(etaDistr.get(), IMG_DIR_PATH + "etaDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(phiDistr.get(), IMG_DIR_PATH + "phiDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pxDistr.get(), IMG_DIR_PATH + "pxDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pyDistr.get(), IMG_DIR_PATH + "pyDistribution_"s + round_indicator +  ".png");
    save_histogram_to_file(pzDistr.get(), IMG_DIR_PATH + "pzDistribution_"s + round_indicator +  ".png");


    // .root file to save TTree
    TFile *file  = TFile::Open(("../data/vector_data"s + round_indicator + ".root").c_str(),"RECREATE");

    // create a TTree
    TTree *tree = new TTree(round_indicator.c_str(), ("TTree storing values from LorentzVector, "s + round_indicator).c_str());

    // add tree branch for float data
    auto branch = tree->Branch("pxpypz_values", &allValues);

    // fill TTree with data, write data to disk 
    tree->Fill();
    tree->Write();

    // calculate compression factor
    double comp_fact = (double)branch->GetTotBytes() / branch->GetZipBytes();

    // save data entry to text file
    data_file << entropy << ";" << comp_fact << ";" << round_indicator << "\n"; 

    // free memory
    delete file, tree;
}