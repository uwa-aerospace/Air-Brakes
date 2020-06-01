#!/bin/bash

# Compile, exit if compilation fails
make -C .. || exit -1
echo "Compiled successfully."
echo

# Run the simulations
echo "height,brake-percentage" >temp_output.csv

# ../air-brakes -s | grep "[0-9]*\,[0-9]*" >> temp_output.csv # Quadratic
../air-brakes -s -f data/G72-10.csv -t 1 -a 2 -r 3 -c 4 | grep "[0-9]*\,[0-9]*" >>temp_output.csv # From file

python3 graph-results.py temp_output.csv
rm temp_output.csv
echo "Simulations complete."
