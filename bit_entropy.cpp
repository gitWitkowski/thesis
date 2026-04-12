#include "entropy.h"

const std::string ROOT_DATA_PATH = "/home/witkowski/2018.ntuple.root";

float polynomial(float x)
{
    //  $P(x) = \pi x^3 + \sqrt{2}x^2 + e$
    return M_PI * x*x*x + std::sqrt(2.0f) * x*x + std::exp(1.0f);
}

void flatten_vector_branch(const std::vector<ROOT::RVec<float>> &branchData, std::vector<float> &flatVector)
{
    int sizeToReserve = 0;
    for (int i=0; i<branchData.size(); ++i)
    {
        sizeToReserve += branchData.at(i).size();
    }

    flatVector.reserve(sizeToReserve);

    for (int i=0; i<branchData.size(); ++i)
    {
        flatVector.insert(flatVector.end(), branchData.at(i).begin(), branchData.at(i).end());
    }

    // std::cout << "Size after flattening: " << flatVector.size() << "\n";
}

void calc_bits_entropy(std::vector<float> &flatVector, std::string fileName, std::string description)
{
    std::ofstream data_file_bits(DATA_DIR_PATH + "bit_entropy/" + fileName + ".txt");
    double epsilon = 1e-4;
    std::array<float, 32> H;
    std::map<unsigned char const, size_t> map = count_bytes(flatVector);
    std::vector<double> X = calc_probability(map, flatVector.size());
	double entropy = calc_entropy(X);

    data_file_bits << "Desc: " << description << "\n";
    data_file_bits << "Number of elements: " << flatVector.size() << "\n";
    data_file_bits << "Epsilon: " << epsilon << "\n";    
    data_file_bits << "Entropy of bytes: " << entropy << "\n";
    data_file_bits << "------------------------------------------\n";

    for (int n = 31; n >= 0; --n)
    {
        size_t numOfOnes = 0;
        for (int i = 0; i < flatVector.size(); ++i)
        {
            if (get_nth_bit(n, flatVector[i]))
            {
                ++numOfOnes;
            }
        }

        H.at(n) =
        calc_entropy({static_cast<double>(numOfOnes) / flatVector.size(),
                      1.0 - static_cast<double>(numOfOnes) / flatVector.size()});
    }

    for (int n = 31; n >= 0; --n)
    {
        data_file_bits << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10);
        data_file_bits << "bit " << std::setw(2) << n << ":\t" //<< std::setw(6)
        <<  (std::isnan(H.at(n)) ? 0 : H.at(n))
        << " " << std::boolalpha << (std::fabs(1.0 - H.at(n)) < epsilon) 
        << "\n";
        if (n == 31 || n == 23) data_file_bits << "---------------------\n";
    }

    data_file_bits.close();

    TH1D *h1 = new TH1D("h1", (description+";p_{T} [GeV];Counts").c_str(), 100, 0.0, 10.0);

    for (float val : flatVector)
    {
        h1->Fill(val);
    }

    save_histogram_to_file(h1, IMG_DIR_PATH + "bit_entropy/" + fileName + ".pdf");

    delete h1;
}

int main(int argc, char** argv){
    
    ROOT::RDataFrame df("tree", ROOT_DATA_PATH.c_str());

    auto h = df.Histo1D({"h_pt", "Rozklad pT;pT [GeV];N", 100, 0, 10}, "trk_pt");
    save_histogram_to_file(&(*h), IMG_DIR_PATH+"bit_entropy/raw_trk_pt.pdf");

    auto h_max = df.Filter("trk_pt.size() > 0")
               .Define("max_pt", "Max(trk_pt)")
               .Histo1D({"h_max", "Max pT;pT;Events", 200, 0, 200}, "max_pt");
    save_histogram_to_file(&(*h_max), IMG_DIR_PATH+"bit_entropy/max_trk_pt.pdf");

    auto trk_pt_all = df.Take<ROOT::RVec<float>>("trk_pt");

    auto trk_pt_1 = df
        .Define("df_range_1", "trk_pt[trk_pt > 0.0 && trk_pt < 3.0]")
        .Take<ROOT::RVec<float>>("df_range_1");

    auto trk_pt_2 = df
        .Define("df_range_2", "trk_pt[trk_pt > 3.0 && trk_pt < 5.0]")
        .Take<ROOT::RVec<float>>("df_range_2");

    auto trk_pt_3 = df
        .Define("df_range_3", "trk_pt[trk_pt > 5.0 && trk_pt < 10.0]")
        .Take<ROOT::RVec<float>>("df_range_3");

    auto trk_pt_4 = df
        .Define("df_range_4", "trk_pt[trk_pt > 10.0]")
        .Take<ROOT::RVec<float>>("df_range_4");

    std::vector<float> 
        trk_pt_all_container{},     trk_pt_all_poly_container{},
        trk_pt_1_container{},       trk_pt_1_poly_container{},
        trk_pt_2_container{},       trk_pt_2_poly_container{},
        trk_pt_3_container{},       trk_pt_3_poly_container{},
        trk_pt_4_container{},       trk_pt_4_poly_container{},
        generated_gaus_container{},
        generated_exp_container{};

    size_t n = 100'000;
    generated_gaus_container.reserve(n);
    generated_exp_container.reserve(n);
    TRandom3 rng(0);

    for (int i=0; i<n; ++i)
    {
        generated_gaus_container.insert(generated_gaus_container.end(), rng.Gaus(-0.02177, 1.355));
        generated_exp_container.insert(generated_exp_container.end(), rng.Exp(1.5));
    }

    flatten_vector_branch(*trk_pt_all, trk_pt_all_container);
    std::transform(trk_pt_all_container.cbegin(), trk_pt_all_container.cend(), std::back_inserter(trk_pt_all_poly_container), polynomial);

    // flatten_vector_branch(*trk_pt_1, trk_pt_1_container);
    // std::transform(trk_pt_1_container.cbegin(), trk_pt_1_container.cend(), std::back_inserter(trk_pt_1_poly_container), polynomial);

    // flatten_vector_branch(*trk_pt_2, trk_pt_2_container);
    // std::transform(trk_pt_2_container.cbegin(), trk_pt_2_container.cend(), std::back_inserter(trk_pt_2_poly_container), polynomial);

    // flatten_vector_branch(*trk_pt_3, trk_pt_3_container);
    // std::transform(trk_pt_3_container.cbegin(), trk_pt_3_container.cend(), std::back_inserter(trk_pt_3_poly_container), polynomial);

    // flatten_vector_branch(*trk_pt_4, trk_pt_4_container);
    // std::transform(trk_pt_4_container.cbegin(), trk_pt_4_container.cend(), std::back_inserter(trk_pt_4_poly_container), polynomial);


    calc_bits_entropy(trk_pt_all_container, "trk_pt_all", "trk_pt");
    calc_bits_entropy(trk_pt_all_poly_container, "trk_pt_all_poly", "trk_pt_poly");

    // calc_bits_entropy(trk_pt_1_container, "trk_pt_1", "trk_pt[trk_pt > 0.0 && trk_pt < 3.0]");
    // calc_bits_entropy(trk_pt_1_poly_container, "trk_pt_1_poly", "trk_pt[trk_pt > 0.0 && trk_pt < 3.0]");
    
    // calc_bits_entropy(trk_pt_2_container, "trk_pt_2", "trk_pt[trk_pt > 3.0 && trk_pt < 5.0]");
    // calc_bits_entropy(trk_pt_2_poly_container, "trk_pt_2_poly", "trk_pt[trk_pt > 3.0 && trk_pt < 5.0]");
    
    // calc_bits_entropy(trk_pt_3_container, "trk_pt_3", "trk_pt[trk_pt > 5.0 && trk_pt < 10.0]");
    // calc_bits_entropy(trk_pt_3_poly_container, "trk_pt_3_poly", "trk_pt[trk_pt > 5.0 && trk_pt < 10.0]");
    
    // calc_bits_entropy(trk_pt_4_container, "trk_pt_4", "trk_pt[trk_pt > 10.0]");
    // calc_bits_entropy(trk_pt_4_poly_container, "trk_pt_4_poly", "trk_pt[trk_pt > 10.0]");
    
    calc_bits_entropy(generated_gaus_container, "generated_gaus", "rng.Gaus(-0.02177, 1.355)");
    calc_bits_entropy(generated_exp_container, "generated_exp", "rng.Exp(1.5)");

   	return 0;
}