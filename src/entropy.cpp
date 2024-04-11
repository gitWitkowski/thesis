#include "entropy.h"
#include <iostream>

double calc_entropy(std::vector<double> X){
    // formula:
    // https://en.wikipedia.org/wiki/Entropy_(information_theory)

    double sum = 0.0;

    std::for_each(X.begin(), X.end(), [&sum](double p_x){
        sum += (p_x * std::log2f(p_x)); 
    });

    return -sum;
}

void exp_array(std::vector<float> &arr, size_t n, std::function<float(float)> f){
    // random number generator
    TRandom3 rng(0);

    // fit to given size
    arr.resize(n);

    // fill with values
    for(size_t i=0; i<n; ++i){
      if(f)
         arr[i] = f(rng.Exp(1.0));
      else
         arr[i] = rng.Exp(1.0);
    }
}

void uniform_array(std::vector<float> &arr, size_t n, std::function<float(float)> f){
    // random number generator
    TRandom3 rng(0);

    // fit to given size
    arr.resize(n);

    // fill with values
    for(size_t i=0; i<n; ++i){
      if(f)
         arr[i] = f(rng.Uniform());
      else
         arr[i] = rng.Uniform();
    }
}

std::map<unsigned char const, size_t> count_bytes(const std::vector<float> &data){
   // map: <byte, number of occurrences>
   std::map<unsigned char const, size_t> occurrence_map;

   // for each generated number
   for(const float &num : data){

      // reading float number as sequence of 4 bytes
      // https://www.h-schmidt.net/FloatConverter/IEEE754.html
      unsigned char const *bytes = reinterpret_cast<unsigned char const *>(&num);

      // for each byte (4 byte float)
      for(std::size_t i = 0; i != sizeof(float); ++i){
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

   return occurrence_map;
}

std::vector<double> calc_probability(std::map<unsigned char const, size_t> &occurrence_map, size_t n){
   // vector of probabilities
   std::vector<double> X(0);

   // fill the vector
   for(auto el : occurrence_map){
      // number of occurrences / (4 bytes per float * number of elements)
      X.push_back(double(el.second) / (sizeof(float) * n));
   }
   
   return X;
}

void save_histogram_to_file(TH1 *hist, std::string file_path){
   TCanvas *c = new TCanvas();
   hist->Draw();
   c->SaveAs(file_path.c_str());
   delete c;
}