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