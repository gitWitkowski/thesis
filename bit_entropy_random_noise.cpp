#include "entropy.h"

const std::string ROOT_DATA_PATH = "/home/witkowski/2018.ntuple.root";

union FloatBits
{
    float f;
    uint32_t i;
};

float insert_noise_at_bits(uint8_t start_pos, uint8_t end_pos, float val)
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

    // val = cast.f;
    return cast.f;
}

#define STR(x) std::to_string(x)

void run_with_noise_at(uint8_t start_pos, uint8_t end_pos, const std::vector<float> &trk_pt_all_container, bool truncate)
{
    const int bins = 2000;

    std::string suffix = std::string(truncate ? "truncate" : "noise") + "_bits_" + STR(start_pos) + "_" + STR(end_pos);

    const std::string SAVE_DIR_PATH = IMG_DIR_PATH + "bit_entropy/" + suffix +"/";
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

    
    TH1F *hist_default = new TH1F(("hist_default"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 10);
    hist_default->SetDirectory(nullptr);
    TH1F *hist_default_0_1 = new TH1F(("hist_default_range_0_1_"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 1);
    hist_default_0_1->SetDirectory(nullptr);
    TH1F *hist_default_1_2 = new TH1F(("hist_default_range_1_2_"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 1, 2);
    hist_default_1_2->SetDirectory(nullptr);
    TH1F *hist_default_2_5 = new TH1F(("hist_default_range_2_5_"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 2, 5);
    hist_default_2_5->SetDirectory(nullptr);
    TH1F *hist_default_5_10 = new TH1F(("hist_default_range_5_10_"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 5, 10);
    hist_default_5_10->SetDirectory(nullptr);
    TH1F *hist_default_0_5 = new TH1F(("hist_default_range_0_5_"+suffix).c_str(), "Rozklad trk_pt;Wartosci;Liczba zliczen", bins, 0, 5);
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
    save_histogram_to_file(hist_default, SAVE_DIR_PATH+"hist_default_"+suffix+".pdf");

    for (int i=0; i<trk_pt_all_container.size(); ++i)
    {
        if (truncate)
        {
            trk_pt_all_noise_container.push_back(
                truncateFloat(trk_pt_all_container[i], end_pos));
        }
        else
        {
            trk_pt_all_noise_container.push_back(
                insert_noise_at_bits(start_pos, end_pos, trk_pt_all_container[i]));
        }
    }

    TH1F *hist_noise = new TH1F(("hist_noise_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 0, 10);
    hist_noise->SetDirectory(nullptr);
    TH1F *hist_noise_0_1 = new TH1F(("hist_noise_0_1_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 0, 1);
    hist_noise_0_1->SetDirectory(nullptr);
    TH1F *hist_noise_1_2 = new TH1F(("hist_noise_1_2_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 1, 2);
    hist_noise_1_2->SetDirectory(nullptr);
    TH1F *hist_noise_2_5 = new TH1F(("hist_noise_2_5_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 2, 5);
    hist_noise_2_5->SetDirectory(nullptr);
    TH1F *hist_noise_5_10 = new TH1F(("hist_noise_5_10_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 5, 10);
    hist_noise_5_10->SetDirectory(nullptr);
    TH1F *hist_noise_0_5 = new TH1F(("hist_noise_0_5_"+suffix).c_str(), ("Rozklad trk_pt "+suffix+";Wartosci;Liczba zliczen").c_str(), bins, 0, 5);
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
    save_histogram_to_file(hist_noise, SAVE_DIR_PATH+"hist_noise_"+suffix+".pdf");

    TLegend leg2(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c2("c", "title", 800, 600);
    hist_default->Draw();
    hist_default->SetLineColor(kBlue);
    hist_noise->Draw("SAME");
    hist_noise->SetLineColor(kRed);
   	c2.SaveAs((SAVE_DIR_PATH+"hist_comp_"+suffix+".pdf").c_str());
    c2.SetLogy();
    c2.Update();
    leg2.AddEntry(hist_default, "default", "l");
    leg2.AddEntry(hist_noise, (truncate ? "truncated" : "noise"), "l");
    leg2.Draw();
   	c2.SaveAs((SAVE_DIR_PATH+"hist_comp_"+suffix+"_log.pdf").c_str());

    TLegend leg_0_2(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c_0_1("c_0_1", "title", 800, 600);
    hist_noise_0_1->Draw();
    hist_noise_0_1->SetLineColor(kRed);
    hist_default_0_1->Draw("SAME");
    hist_default_0_1->SetLineColor(kBlue);
    leg_0_2.AddEntry(hist_default_0_1, "default", "l");
    leg_0_2.AddEntry(hist_noise_0_1, (truncate ? "truncated" : "noise"), "l");
    leg_0_2.Draw();
    c_0_1.SaveAs((SAVE_DIR_PATH+"hist_comp_0_1_"+suffix+".pdf").c_str());

    TLegend leg_1_2(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c_1_2("c_1_2", "title", 800, 600);
    hist_noise_1_2->Draw();
    hist_noise_1_2->SetLineColor(kRed);
    hist_default_1_2->Draw("SAME");
    hist_default_1_2->SetLineColor(kBlue);
    leg_1_2.AddEntry(hist_default_1_2, "default", "l");
    leg_1_2.AddEntry(hist_noise_1_2, (truncate ? "truncated" : "noise"), "l");
    leg_1_2.Draw();
    c_1_2.SaveAs((SAVE_DIR_PATH+"hist_comp_1_2_"+suffix+".pdf").c_str());

    TLegend leg_2_5(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c_2_5("c_2_5", "title", 800, 600);
    hist_noise_2_5->Draw();
    hist_noise_2_5->SetLineColor(kRed);
    hist_default_2_5->Draw("SAME");
    hist_default_2_5->SetLineColor(kBlue);
    leg_2_5.AddEntry(hist_default_2_5, "default", "l");
    leg_2_5.AddEntry(hist_noise_2_5, (truncate ? "truncated" : "noise"), "l");
    leg_2_5.Draw();
    c_2_5.SaveAs((SAVE_DIR_PATH+"hist_comp_2_5_"+suffix+".pdf").c_str());

    TLegend leg_5_10(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c_5_10("c_5_10", "title", 800, 600);
    hist_noise_5_10->Draw();
    hist_noise_5_10->SetLineColor(kRed);
    hist_default_5_10->Draw("SAME");
    hist_default_5_10->SetLineColor(kBlue);
    leg_5_10.AddEntry(hist_default_5_10, "default", "l");
    leg_5_10.AddEntry(hist_noise_5_10, (truncate ? "truncated" : "noise"), "l");
    leg_5_10.Draw();
    c_5_10.SaveAs((SAVE_DIR_PATH+"hist_comp_5_10_"+suffix+".pdf").c_str());

    TLegend leg_0_5(0.8, 0.6, 0.9, 0.7); // (x1, y1, x2, y2) w NDC
    TCanvas c_0_5("c_0_5", "title", 800, 600);
    hist_noise_0_5->Draw();
    hist_noise_0_5->SetLineColor(kRed);
    hist_default_0_5->Draw("SAME");
    hist_default_0_5->SetLineColor(kBlue);
    leg_0_5.AddEntry(hist_default_0_5, "default", "l");
    leg_0_5.AddEntry(hist_noise_0_5, (truncate ? "truncated" : "noise"), "l");
    leg_0_5.Draw();
   	c_0_5.SaveAs((SAVE_DIR_PATH+"hist_comp_0_5_"+suffix+".pdf").c_str());


    TH1F *hist_delta = new TH1F(("hist_delta_"+suffix).c_str(), "Delta pT;#Delta pT [GeV];N", bins, 0.5, 0.5);
    hist_delta->SetDirectory(nullptr);

    for (int i=0; i<trk_pt_all_container.size(); ++i)
    {
        hist_delta->Fill(
            trk_pt_all_noise_container[i] - trk_pt_all_container[i]
        );
    }

    save_histogram_to_file(hist_delta, SAVE_DIR_PATH+"hist_diff_"+suffix+".pdf");

    gStyle->SetOptLogy();
    save_histogram_to_file(hist_delta, SAVE_DIR_PATH+"hist_diff_"+suffix+"_log.pdf");
    gStyle->SetOptLogy(0);

    TH2F *hist_delta_2d = new TH2F(("hist_delta_2d_"+suffix).c_str(), "Delta vs pT;pT [GeV];#Delta pT [GeV]",
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
   	c.SaveAs((SAVE_DIR_PATH+"hist_diff_"+suffix+"_2d.pdf").c_str());

}

int main(int argc, char** argv){

    gDirectory->Clear();
    
    ROOT::RDataFrame df("tree", ROOT_DATA_PATH.c_str());

    auto trk_pt_all = df.Take<ROOT::RVec<float>>("trk_pt");

    std::vector<float> trk_pt_all_container{};

    flatten_vector_branch(*trk_pt_all, trk_pt_all_container);

    for (auto bit : {13, 14, 15, 16, 17, 18})
    {
        run_with_noise_at(0, bit, trk_pt_all_container, false);
        run_with_noise_at(0, bit, trk_pt_all_container, true);
    }

    return 0;
}