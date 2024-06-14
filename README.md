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
2.  Generate a project buildsystem
```bash
cd build
cmake ../
```
3.  Build and run the project
```bash
make
make run
``` 
