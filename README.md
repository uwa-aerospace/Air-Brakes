# Air-Brakes

Project brief at <https://uniwa.sharepoint.com/sites/uwaaerospace/_layouts/15/Doc.aspx?sourcedoc=%7B0EBDC4BA-4F7B-4E6B-A414-E4BB85CCF012%7D&file=Air%20Brakes%20Briefing.docx&action=default&mobileredirect=true>

## Project description

This project aims to develop a PID based approach for use in Air Brakes in a rocket, designed to ensure that the rocket reaches apogee (its highest point), as close as possible to the desired height (usually 10,000 ft or 30,000 ft).

### Directory structure

In the main directory, the main C files are located, including the downloaded files for the BMX055 sensor API. The `simulations` directory includes scripts for simulating the data on a computer, for testing purposes. Some CSVs are provided as a source of real data from previous launches.

## Running the simulations

To run the simulations and view an output graph:

1. Navigate to `simulations/` (`cd simulations`)
2. Ensure that the Python3 packages `wheel` and `pandas` are installed: `python3 -m pip install wheel`, `python3 -m pip install pandas`
3. Ensure `run-simulations.sh` is executable (`chmod +x run-simulations.sh`)
4. Run the script with `./run-simulations.sh`

### Example simulation using data from a file

`./air-brakes -s -f simulations/data/G72-10.csv -t 1 -a 2 -r 3 -c 4`

- `-s`: simulate
- `-f`: use a file
- `-t`: column of the csv file that is the time
- `-a`: column of the csv file that is the altitude
- `-r`: column of the csv file that is the speed
- `-c`: column of the csv file that is the acceleration

## BMX055 example code

Check Bosch's GitHub repositories and example code:
<https://github.com/BoschSensortec/BMA2x2_driver> for the accelerometer
<https://github.com/BoschSensortec/BMM150-Sensor-API> for the magnetometer
<https://github.com/BoschSensortec/BMG160_driver> for the gyroscope
