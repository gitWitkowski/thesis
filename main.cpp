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
   
   // number of generated numbers
   constexpr size_t n = 4'000;

   // number of times to repeat experiment 
   size_t repeat_num = 10;

   // arrays for scores from different tries
   double scores_for_exp[repeat_num];
   double scores_for_uniform[repeat_num];

   ////////////////////////////////////
   // exponential distribution
   ////////////////////////////////////

   // vector for generated data
   std::vector<float> data_exp(n);
   
   // repeat for more reliable result 
   for(int i=0; i<repeat_num; ++i){
      exp_array(data_exp, n);
      std::map<unsigned char const, size_t> map = count_bytes(data_exp, n);
      std::vector<double> X = calc_probability(map, n);
      scores_for_exp[i] = calc_entropy(X);
   }

   // calculate average
   double sum = 0.0;
   for(int i=0; i<repeat_num; ++i)
      sum += scores_for_exp[i];

   std::cout << "Average entropy exponential distribution: " << sum / repeat_num << "\n";

   ////////////////////////////////////
   // uniform distribution
   ////////////////////////////////////

   // vector for generated data
   std::vector<float> data_uniform(n);
   
   // repeat for more reliable result 
   for(int i=0; i<repeat_num; ++i){
      uniform_array(data_uniform, n);
      std::map<unsigned char const, size_t> map = count_bytes(data_exp, n);
      std::vector<double> X = calc_probability(map, n);
      scores_for_exp[i] = calc_entropy(X);
   }

   // calculate average
   sum = 0.0;
   for(int i=0; i<repeat_num; ++i)
      sum += scores_for_exp[i];

   std::cout << "Average entropy uniform distribution: " << sum / repeat_num << "\n";

   // draw hist to screen
   DrawHist(argc, argv, data_exp, data_uniform);

   return 0;
}