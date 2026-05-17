#include "entropy.h"

int main(){

	// create dir
	if (!std::filesystem::exists(IMG_DIR_PATH)) {
        try {
            std::filesystem::create_directories(IMG_DIR_PATH);
            std::filesystem::create_directories(IMG_DIR_PATH + "values/");
            std::filesystem::create_directories(IMG_DIR_PATH + "bytes/");
            std::filesystem::create_directories(DATA_DIR_PATH + "bytes/");
            std::filesystem::create_directories(IMG_DIR_PATH + "bit_entropy/");
        } catch (const std::exception& e) { }
	}

   	return 0;
}