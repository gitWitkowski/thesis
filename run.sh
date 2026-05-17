#!/bin/bash

# delete build dir and contents of data dir
# rm -rf build/*
rm -rf data/*

# create build dir and move into it
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

# generate makefile etc.
cmake ..

# compile
make || exit 1

# run
make run_init_structure || exit 1
# make run || exit 1
# make run_tree || exit 1
# make run_vector || exit 1
# make run_bit_entropy || exit 1
make run_bit_entropy_random_noise || exit 1

# exec python script
cd ../PYTH
# python3 main.py
# python3 bit_entropy.py