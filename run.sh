#!/bin/bash

# delete build dir and contents of data dir
rm -rf build/*
rm -rf data/*

# create build dir and move into it
mkdir build
cd build

# generate makefile etc.
cmake ..

# compile
make

# run
make run1
make run2
make run3