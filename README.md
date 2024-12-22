## Technologies used
- C++17
- [ROOT v6.30.04](https://root.cern/install/)
- [zlib](https://www.zlib.net/) (zlib1g-dev)
- cmake
- Python
- [Matplotlib](https://matplotlib.org/)
- [seaborn](https://seaborn.pydata.org/)
- [pandas](https://pandas.pydata.org/)
 
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
- **rounded** - rounding method applied to every element of the array (e.g "round_10 means that such operation `round(x * 10.0) / 10.0;` was applied, where `x` is an element of the array)
- **compression** - compression level in zlib (*Z_NO_COMPRESSION*, *Z_BEST_SPEED*, *Z_BEST_COMPRESSION*, *Z_DEFAULT_COMPRESSION*)
- **entropy** - Shannon entropy in bits
- **size** - size of data in bytes (number of elements in `std::vector<unsigned char>`)

Below you can see a bar chart that compares the obtained entropy values based on the rounding method, the distribution used, and whether the data was compressed.

![entropy comparison](https://raw.githubusercontent.com/gitWitkowski/thesis/main/data/img/entropy_values_comparison.png)
