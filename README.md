## Technologies used
- C++17
- [ROOT v6.30.04](https://root.cern/install/)
- [zlib](https://www.zlib.net/) (zlib1g-dev)
- cmake
 
## How to run
1. Source the appropriate script (if needed)
```bash
source /path/to/root/install_dir/bin/thisroot.sh
```
2.  Make run.sh executable
```bash
chmod +x run.sh
```
3.  Run shell script
```bash
./ruh.sh
``` 

## Results

File [`data.txt`](https://github.com/gitWitkowski/thesis/blob/main/data/data.txt) contains all the data obtain in the process for various cases of different parameters such as: 
- **N** - quantity of floating point numbers in an array
- **distribution** - distribution used to generate sample numbers (*exponential*, *uniform*)
- **rounded** - rounding method applied to every element of the array (e.g "10.0/10.0" means that such opeartion `round(x * 10.0) / 10.0;` was applied, where `x` is an element of the array)
- **compression** - compression level in zlib (*Z_NO_COMPRESSION*, *Z_BEST_SPEED*, *Z_BEST_COMPRESSION*, *Z_DEFAULT_COMPRESSION*)
- **entropy** - Shannon entropy in bits
- **size** - size of data in bytes (number of elements in `std::vector<unsigned char>`)
