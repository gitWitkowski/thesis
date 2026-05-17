#include "entropy.h"

const std::string ROOT_DATA_PATH = "/home/witkowski/2018.ntuple.root";

union FloatBits
{
    float f;
    uint32_t i;
};

void insert_noise_at_bits(uint8_t start_pos, uint8_t end_pos, float &val)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    FloatBits cast;
    cast.f = val;

    uint32_t random_32_bits = gen();

    for (int pos=start_pos; pos<=end_pos; ++pos)
    {
        // extract bit from random_32_bits at pos and check if it is 0/1
        bool noise_bit = ((random_32_bits & (1u << pos)) != 0);

        uint32_t mask = (1u << pos); // mask for bit at position pos
        if (noise_bit) {
            cast.i |= mask; // set bit high
        } else {
            cast.i &= ~mask; // set bit low
        }
    }

    val = cast.f;
}

#define STR(x) std::to_string(x)

void run_with_noise_at(uint8_t start_pos, uint8_t end_pos, const std::vector<float> &trk_pt_all_container/*, TH1F *hist_default, const int bins*/)
{
    const int bins = 2000;

    const std::string SAVE_DIR_PATH = IMG_DIR_PATH + "bit_entropy/noise_at_bits_"+STR(start_pos)+"_"+STR(end_pos)+"/";
    if (!std::filesystem::exists(SAVE_DIR_PATH)) {
        try {
            std::filesystem::create_directories(SAVE_DIR_PATH);
        } catch (const std::exception& e) { std::terminate(); }
	}
    std::vector<float> 
        trk_pt_all_noise_container{},
        trk_pt_all_delta_container{};

    trk_pt_all_noise_container.reserve(trk_pt_all_container.size());
    trk_pt_all_delta_container.reserve(trk_pt_all_container.size());

    std::string suffix = "_noise_bits_" + STR(start_pos) + "_" + STR(end_pos);
    
    TH1F *hist_default = new TH1F(("hist_default"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 10);
    hist_default->SetDirectory(nullptr);
    TH1F *hist_default_0_1 = new TH1F(("hist_default_range_0_1"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 1);
    hist_default_0_1->SetDirectory(nullptr);
    TH1F *hist_default_1_2 = new TH1F(("hist_default_range_1_2"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 1, 2);
    hist_default_1_2->SetDirectory(nullptr);
    TH1F *hist_default_2_5 = new TH1F(("hist_default_range_2_5"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 2, 5);
    hist_default_2_5->SetDirectory(nullptr);
    TH1F *hist_default_5_10 = new TH1F(("hist_default_range_5_10"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 5, 10);
    hist_default_5_10->SetDirectory(nullptr);
    TH1F *hist_default_0_5 = new TH1F(("hist_default_range_0_5"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 5);
    hist_default_0_5->SetDirectory(nullptr);

    for (const float& val : trk_pt_all_container)
    {
        hist_default->Fill(val);
        if (val >= 0 && val <= 1) hist_default_0_1->Fill(val);
        if (val >= 1 && val <= 2) hist_default_1_2->Fill(val);
        if (val >= 2 && val <= 5) hist_default_2_5->Fill(val);
        if (val >= 5 && val <= 10) hist_default_5_10->Fill(val);
        if (val >= 0 && val <= 5) hist_default_0_5->Fill(val);
    }
    save_histogram_to_file(hist_default, SAVE_DIR_PATH+"hist_default"+suffix+".pdf");

    for (int i=0; i<trk_pt_all_container.size(); ++i)
    {
        trk_pt_all_noise_container.push_back(trk_pt_all_container[i]);
        insert_noise_at_bits(start_pos, end_pos, trk_pt_all_noise_container[i]);
    }

    TH1F *hist_noise = new TH1F(("hist_noise"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 10);
    hist_noise->SetDirectory(nullptr);
    TH1F *hist_noise_0_1 = new TH1F(("hist_noise_0_1"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 1);
    hist_noise_0_1->SetDirectory(nullptr);
    TH1F *hist_noise_1_2 = new TH1F(("hist_noise_1_2"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 1, 2);
    hist_noise_1_2->SetDirectory(nullptr);
    TH1F *hist_noise_2_5 = new TH1F(("hist_noise_2_5"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 2, 5);
    hist_noise_2_5->SetDirectory(nullptr);
    TH1F *hist_noise_5_10 = new TH1F(("hist_noise_5_10"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 5, 10);
    hist_noise_5_10->SetDirectory(nullptr);
    TH1F *hist_noise_0_5 = new TH1F(("hist_noise_0_5"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 5);
    hist_noise_0_5->SetDirectory(nullptr);

    for (const float& val : trk_pt_all_noise_container)
    {
        hist_noise->Fill(val);
        if (val >= 0 && val <= 1) hist_noise_0_1->Fill(val);
        if (val >= 1 && val <= 2) hist_noise_1_2->Fill(val);
        if (val >= 2 && val <= 5) hist_noise_2_5->Fill(val);
        if (val >= 5 && val <= 10) hist_noise_5_10->Fill(val);
        if (val >= 0 && val <= 5) hist_noise_0_5->Fill(val);
    }
    save_histogram_to_file(hist_noise, SAVE_DIR_PATH+"hist_noise"+suffix+".pdf");

    TCanvas c2("c", "title", 800, 600);
    hist_default->Draw();
    hist_default->SetLineColor(kBlue);
    hist_noise->Draw("SAME");
    hist_noise->SetLineColor(kRed);
   	c2.SaveAs((SAVE_DIR_PATH+"hist_comp"+suffix+".pdf").c_str());
    c2.SetLogy();
    c2.Update();
   	c2.SaveAs((SAVE_DIR_PATH+"hist_comp"+suffix+"_log.pdf").c_str());

    TCanvas c_0_1("c_0_1", "title", 800, 600);
    hist_default_0_1->Draw();
    hist_default_0_1->SetLineColor(kBlue);
    hist_noise_0_1->Draw("SAME");
    hist_noise_0_1->SetLineColor(kRed);
    c_0_1.SaveAs((SAVE_DIR_PATH+"hist_comp_0_1"+suffix+".pdf").c_str());

    TCanvas c_1_2("c_1_2", "title", 800, 600);
    hist_default_1_2->Draw();
    hist_default_1_2->SetLineColor(kBlue);
    hist_noise_1_2->Draw("SAME");
    hist_noise_1_2->SetLineColor(kRed);
   	c_1_2.SaveAs((SAVE_DIR_PATH+"hist_comp_1_2"+suffix+".pdf").c_str());

    TCanvas c_2_5("c_2_5", "title", 800, 600);
    hist_default_2_5->Draw();
    hist_default_2_5->SetLineColor(kBlue);
    hist_noise_2_5->Draw("SAME");
    hist_noise_2_5->SetLineColor(kRed);
   	c_2_5.SaveAs((SAVE_DIR_PATH+"hist_comp_2_5"+suffix+".pdf").c_str());

    TCanvas c_5_10("c_5_10", "title", 800, 600);
    hist_default_5_10->Draw();
    hist_default_5_10->SetLineColor(kBlue);
    hist_noise_5_10->Draw("SAME");
    hist_noise_5_10->SetLineColor(kRed);
   	c_5_10.SaveAs((SAVE_DIR_PATH+"hist_comp_5_10"+suffix+".pdf").c_str());

    TCanvas c_0_5("c_0_5", "title", 800, 600);
    hist_default_0_5->Draw();
    hist_default_0_5->SetLineColor(kBlue);
    hist_noise_0_5->Draw("SAME");
    hist_noise_0_5->SetLineColor(kRed);
   	c_0_5.SaveAs((SAVE_DIR_PATH+"hist_comp_0_5"+suffix+".pdf").c_str());

    // TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9); // (x1, y1, x2, y2) w NDC
    // leg->AddEntry(hist_default_0_5, "opis 1", "l");
    // leg->AddEntry(hist_noise_0_5, "opis 2", "l");
    // leg->Draw();

    TH1F *hist_delta = new TH1F(("hist_delta"+suffix).c_str(), "Delta pT;#Delta pT [GeV];N", bins, 0.5, 0.5);
    hist_delta->SetDirectory(nullptr);

    for (int i=0; i<trk_pt_all_container.size(); ++i)
    {
        hist_delta->Fill(
            trk_pt_all_noise_container[i] - trk_pt_all_container[i]
        );
    }

    save_histogram_to_file(hist_delta, SAVE_DIR_PATH+"hist_diff"+suffix+".pdf");

    gStyle->SetOptLogy();
    save_histogram_to_file(hist_delta, SAVE_DIR_PATH+"hist_diff"+suffix+"_log.pdf");
    gStyle->SetOptLogy(0);

    TH2F *hist_delta_2d = new TH2F(("hist_delta_2d"+suffix).c_str(), "Delta vs pT;pT [GeV];#Delta pT [GeV]",
                                   bins, 0, 10,      // X axis: val
                                   bins, -0.3, 0.3); // Y axis: delta
    hist_delta_2d->SetDirectory(nullptr);

    for (int i = 0; i < trk_pt_all_container.size(); ++i) {
        float delta = trk_pt_all_noise_container[i] - trk_pt_all_container[i];
        hist_delta_2d->Fill(trk_pt_all_container[i], delta);
    }

	gPad = nullptr;
   	TCanvas c;
	c.cd();
	c.SetLogz();
    hist_delta_2d->Draw("COLZ"); 
    c.Update();

    TPaletteAxis *palette = (TPaletteAxis*)hist_delta_2d->GetListOfFunctions()->FindObject("palette");
    if (palette)
    {
        palette->SetX1NDC(0.91);
        palette->SetX2NDC(0.95);
        palette->SetY1NDC(0.1);
        palette->SetY2NDC(0.9);
    }

    TPaveStats *stats = (TPaveStats*)hist_delta_2d->GetListOfFunctions()->FindObject("stats");
    if (stats)
    {
        stats->SetX1NDC(0.1);   // left (NDC = 0-1)
        stats->SetX2NDC(0.35);  // right
        stats->SetY1NDC(0.65);  // bottom
        stats->SetY2NDC(0.9);   // top
        stats->Draw();
    }

    c.Modified();
    c.Update();
   	c.SaveAs((SAVE_DIR_PATH+"hist_diff"+suffix+"_2d.pdf").c_str());

}

int main(int argc, char** argv){

    gDirectory->Clear();
    
    ROOT::RDataFrame df("tree", ROOT_DATA_PATH.c_str());

    auto h = df.Histo1D({"h_pt", "Rozklad pT;pT [GeV];N", 500, 0, 10}, "trk_pt");

    auto trk_pt_all = df.Take<ROOT::RVec<float>>("trk_pt");

    std::vector<float> trk_pt_all_container{};
        // trk_pt_all_noise_container{},
        // trk_pt_all_delta_container{};

    flatten_vector_branch(*trk_pt_all, trk_pt_all_container);

    // int bins = 2000;

    run_with_noise_at(0, 13, trk_pt_all_container/*, hist_default, bins*/);
    run_with_noise_at(0, 15, trk_pt_all_container/*, hist_default, bins*/);
    run_with_noise_at(0, 16, trk_pt_all_container/*, hist_default, bins*/);
    run_with_noise_at(0, 17, trk_pt_all_container/*, hist_default, bins*/);
    // run_with_noise_at(0, 18, trk_pt_all_container/*, hist_default, bins*/);
    // run_with_noise_at(0, 20, trk_pt_all_container/*, hist_default, bins*/);

   	return 0;
}