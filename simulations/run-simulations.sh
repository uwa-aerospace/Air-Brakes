#!/bin/bash

# Compile, exit if compilation fails
make -C .. || exit 1

# Run the simulations
echo "height,brake-angle" > temp_output.csv
../air-brakes | grep -vi "Aerospace" >> temp_output.csv
python3 graph-results.py temp_output.csv
rm temp_output.csv 
echo "Run complete"