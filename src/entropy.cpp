#include "entropy.h"
#include <iostream>

std::function<float(float)> 
	round_fun_10 		= [](float x){ return round(x * 10.0) / 10.0; },
	round_fun_100 		= [](float x){ return round(x * 100.0) / 100.0; },
	round_fun_1000		= [](float x){ return round(x * 1000.0) / 1000.0; },
	round_fun_2 		= [](float x){ return round(x * 2.0) / 2.0; },
	round_fun_4 		= [](float x){ return round(x * 4.0) / 4.0; },
	round_fun_8 		= [](float x){ return round(x * 8.0) / 8.0; },
	round_fun_16 		= [](float x){ return round(x * 16.0) / 16.0; },
	round_fun_32 		= [](float x){ return round(x * 32.0) / 32.0; },
	round_fun_64 		= [](float x){ return round(x * 64.0) / 64.0; },
	round_fun_built_in	= [](float x){ return round(x); };

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
         	arr[i] = f(rng.Uniform(100));
      	else
         	arr[i] = rng.Uniform(100);
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

std::map<unsigned char const, size_t> count_bytes(const std::vector<unsigned char> &data){
   	// map: <byte, number of occurrences>
   	std::map<unsigned char const, size_t> occurrence_map;

   	// for each generated number
   	for(const unsigned char &byte : data){
      
         	// find byte in map
         	auto it = occurrence_map.find(byte); 

         	if (it == occurrence_map.end()) {
            	// not found:
            	// insert byte into map with count=1 
            	occurrence_map.insert(std::make_pair(byte, 1));    
         	} else {
            	// found:
            	// increase number of occurrences by 1
            	++(it->second);
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

TH1F *map_to_hist(
   	std::map<unsigned char const, size_t> &occurrence_map,
   	std::string name,
   	std::string title
){
    std::vector<float> tab(256); // 256 possible bytes
    for(auto [key, pair] : occurrence_map)
        tab[static_cast<int>(key)] = pair;

   	TH1F *hist = new TH1F(name.c_str(), title.c_str(), 256, 0, 256);
   	for(size_t i=0; i<256; ++i)
    	for(int j=0; j<tab[i]; ++j)
        	hist->Fill(i); // fill histogram with data

	return hist;
}

std::vector<unsigned char> float_to_char_vector(const std::vector<float>& float_data) {
	// prepare char vector
    std::vector<unsigned char> char_data;
    char_data.reserve(float_data.size() * sizeof(float));

	// cast values from float to unsigned char and insert into vector
    for (const auto& f : float_data) {
        unsigned char const* begin = reinterpret_cast<unsigned char const*>(&f);
        char_data.insert(char_data.end(), begin, begin + sizeof(float));
    }

    return char_data;
}

// https://www.zlib.net/manual.html and chat gpt
std::vector<unsigned char> compress_vector(const std::vector<unsigned char>& data) {
    uLongf compressed_size = compressBound(data.size());
    std::vector<unsigned char> compressed_data(compressed_size);

    if (compress(compressed_data.data(), &compressed_size, data.data(), data.size()) != Z_OK) {
        throw std::runtime_error("Compression failed!");
    }

    compressed_data.resize(compressed_size); // Adjust the size to the actual compressed size
    return compressed_data;
}

// https://www.zlib.net/manual.html and chat gpt
std::vector<unsigned char> deflate_vector(const std::vector<unsigned char>& data, int compression_level) {
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (deflateInit(&strm, compression_level) != Z_OK) {
        throw std::runtime_error("deflateInit failed!");
    }

    strm.avail_in = data.size();
    strm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data.data()));

    uLongf compressed_size = deflateBound(&strm, data.size());
    std::vector<unsigned char> compressed_data(compressed_size);

    strm.avail_out = compressed_size;
    strm.next_out = compressed_data.data();

    if (deflate(&strm, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&strm);
        throw std::runtime_error("deflate failed!");
    }

    compressed_data.resize(compressed_size - strm.avail_out);
    deflateEnd(&strm);
    return compressed_data;
}

void run_case(
	distr distribution_type,
	std::vector<float> &array,
	std::vector<unsigned char> &char_array,
	std::vector<unsigned char> &compressed_char_array,
	std::map<unsigned char const, size_t> &map,
	std::ofstream &file,
	const std::string path,
	const std::string title,
	std::function<float(float)> rounding_f,
	int compression_level
	){

	// function used to fill an array
	std::function<void(std::vector<float> &, size_t, std::function<float(float)>)> fill_array_f;

	// distribution name
	std::string distr_name;

	// compression level as string used in title/file name
	std::string compression_lvl_str = (compression_level == Z_NO_COMPRESSION ? "Z_NO_COMPRESSION" : "Z_BEST_COMPRESSION");

	// set name and function
	if(distribution_type == EXP){
		distr_name = "exponential";
		fill_array_f = exp_array;
	}
	else if(distribution_type == UNI){
		distr_name = "uniform";
		fill_array_f = uniform_array;
	}else
		throw -1;

	// fill array with choosen function
	fill_array_f(
		array, 
		N, 
		rounding_f
	);

	// size of data
	std::size_t size = 0;

	if(compression_level != Z_NO_COMPRESSION){
		// convert data from float to char
    	char_array = float_to_char_vector(array);
		// compress data
		compressed_char_array = deflate_vector(char_array, compression_level);
		map = count_bytes(compressed_char_array);
		size = compressed_char_array.size() * sizeof(char);
	}else{
		map = count_bytes(array);
		size = array.size() * sizeof(float);
	}

	// calculate probability and entropy
    std::vector<double> X = calc_probability(map, N);
	double entropy = calc_entropy(X);

	// create histogram from map
	TH1F *hist = map_to_hist(
		map,
		"entropy: " + std::to_string(entropy),
		distr_name + "_" + compression_lvl_str + "_" + title 
	);
	
	// save histogram
	save_histogram_to_file(
		hist, 
		path + distr_name + "_" + compression_lvl_str + "_" + title + ".png"
	);

	// free memory
	delete hist;

	// write data to file
	file << N << ";" << distr_name << ";" + title + ";" << compression_lvl_str << ";" << entropy << ";" << size << "\n"; 
}