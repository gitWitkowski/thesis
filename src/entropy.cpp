#include "entropy.h"

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
	round_fun_built_in	= [](float x){ return round(x); },
	truncateFloat_14	= [](float x){ return truncateFloat(x, 14); };

float truncateFloat(float x, int n){
    union {
        float f;
        uint32_t i;
    } u;
    u.f = x;
    uint32_t mask = ~((1U << n) - 1);
    u.i &= mask;
    return u.f;
}

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
        	arr[i] = f(rng.Exp(1));
      	else
        	arr[i] = rng.Exp(1);
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
         	arr[i] = f(rng.Uniform(10));
      	else
         	arr[i] = rng.Uniform(10);
    }
}

void array_apply_transform(std::vector<float> &arr, size_t n, std::function<float(float)> f){
    // apply transform to every element
    for(size_t i=0; i<n; ++i)
    	if(f)
        	arr[i] = f(arr[i]);
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

// https://www.zlib.net/manual.html
std::vector<unsigned char> compress_vector(const std::vector<unsigned char>& data, int compression_level) {
    // zalloc, zfree and opaque must be initialized before by the call
	z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	// initialize internal stream state for compression
	assert(deflateInit(&strm, compression_level) == Z_OK);

	strm.avail_in = data.size();
    strm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data.data()));

	// upper bound on the compressed size after deflation
	uLongf compressed_size = deflateBound(&strm, data.size());
    std::vector<unsigned char> compressed_data(compressed_size);

    strm.avail_out = compressed_size;
    strm.next_out = compressed_data.data();

	// all compression done in 1 step, avail_out set to the value returned by deflateBound
	// deflate is guaranteed to return Z_STREAM_END
    deflate(&strm, Z_FINISH);

    compressed_data.resize(compressed_size - strm.avail_out);
    // free all dynamically allocated data structures for this stream
	assert(deflateEnd(&strm) == Z_OK);

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
	bool save_byte_map,
	int compression_level
	){

	// function used to fill an array
	std::function<void(std::vector<float> &, size_t, std::function<float(float)>)> fill_array_f;

	// distribution name
	std::string distr_name;

	// compression level as string used in title/file name
	std::string compression_lvl_str = (compression_level == Z_NO_COMPRESSION ? "NO" : "Z_BEST_COMPRESSION");

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
		compressed_char_array = compress_vector(char_array, compression_level);
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
	TH1F *hist_bytes = map_to_hist(
		map,
		"entropy: " + std::to_string(entropy),
		distr_name + "_" + compression_lvl_str + "_" + title + "_bytes"
	);
	
	// save histogram
	if(path != "")
		save_histogram_to_file(
			hist_bytes, 
			path + "bytes/" + distr_name + "_" + compression_lvl_str + "_" + title + "_bytes.pdf"
		);

	// create and save histogram with floating point numbers
	TH1F *hist_values = new TH1F(
		"values",
		(distr_name + "_" + compression_lvl_str + "_" + title + "_values").c_str(),
		30, 0, 10);
	for(auto &val : array){
		hist_values->Fill(val);
	}
	if(path != "")
		save_histogram_to_file(
			hist_values, 
			path + "values/" + distr_name + "_" + compression_lvl_str + "_" + title + "_values.pdf"
		);

	// write data to file
	if(file)
		file << N << ";" << distr_name << ";" + title + ";" << compression_lvl_str << ";" << entropy << ";" << size << "\n";

	// save byte map to a file if requested
	if(save_byte_map){
		std::ofstream f(DATA_DIR_PATH + "bytes/" + distr_name + "_" + compression_lvl_str + "_" + title + "_bytes.txt", std::ios::app);
		for(const auto [key, val] : map){
			f << "0x" << std::uppercase << std::hex << (int)key << " : " << std::dec << val << "\n";
		}
		f.close();
	}

	// free memory
	delete hist_bytes, hist_values;
}