#!/bin/bash

# delete build dir and contents of data dir
rm -rf build/*
rm -rf data/*

# create build dir and move into it
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

# generate makefile etc.
cmake ..

# compile
make

# run
make run
make run_tree

# exec python script
cd ../PYTH
python3 main.py