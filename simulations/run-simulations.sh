#!/bin/bash

# Compile, exit if compilation fails
make -C .. || exit -1
echo "Compiled successfully."
echo

# Run the simulations
echo "height,brake-angle" > temp_output.csv
../air-brakes -s | grep "[0-9]*\,[0-9]*" >> temp_output.csv
python3 graph-results.py temp_output.csv
rm temp_output.csv 
echo "Simulations complete."