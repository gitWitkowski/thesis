#include <iostream>
#include <map>
#include "TRandom3.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "entropy.h"

void DrawHist(int argc, char** argv, std::vector<Float_t> &d) {

   // TApplication for displaying graphics
   TApplication app("app", &argc, argv);
   TCanvas* c = new TCanvas("c", "Title", 0, 0, 800, 600);
   c->Modified(); c->Update();
   TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
   // terminate program on window close
   rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

   // histogram from -1 to 6 with 70 bins
   TH1F h1("legend","Histogram",70,-1,6);
   for(int i=0; i<d.size(); ++i)
      h1.Fill(d[i]); // fill histogram with data
   
   // draw window
   h1.Draw();
   app.Run();
}

int main(int argc, char** argv){
   
   // number of generated numbers
   constexpr Int_t n = 4'000;

   std::vector<Float_t> data(n);
   
   // map: <byte, number of occurrences>
   std::map<unsigned char const, size_t> occurrence_map;

   // random number generator
   TRandom3 rng(0);

   for(int i=0; i<n; ++i)
      data[i] = rng.Exp(1.0);
      // data[i] = rng.Uniform();

   // for each generated number
   for(const Float_t num : data){

      // reading float number as sequence of 4 bytes
      // https://www.h-schmidt.net/FloatConverter/IEEE754.html
      unsigned char const *bytes = reinterpret_cast<unsigned char const *>(&num);

      // for each byte (4 byte float)
      for(std::size_t i = 0; i != sizeof(Float_t); ++i){
         // std::printf("0x%02X\n", p[i]);
      
         // find byte in map
         auto it = occurrence_map.find(bytes[i]); 

         if (it == occurrence_map.end()) {
            // not found:
            // insert byte into map with count=1 
            occurrence_map.insert(std::make_pair(bytes[i], 1));    
         } else {
            // found:
            // increase number of occurrences by 1
            ++(it->second);
         }           
      }
   }
   
   // vector of probabilities
   std::vector<double> X(0);

   // fill the vector
   for(auto el : occurrence_map){
      // number of occurrences / (number of numbers * 4 bytes)
      X.push_back(double(el.second) / (4*n));
   }
   
   std::cout << "Entropy: " << calc_entropy(X) << "\n";

   // draw hist to screen
   DrawHist(argc, argv, data);

   return 0;
}