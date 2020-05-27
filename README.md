# Air-Brakes

Project brief at https://uniwa.sharepoint.com/sites/uwaaerospace/_layouts/15/Doc.aspx?sourcedoc=%7B0EBDC4BA-4F7B-4E6B-A414-E4BB85CCF012%7D&file=Air%20Brakes%20Briefing.docx&action=default&mobileredirect=true

## Running the simulations

To run the simulations and view an output graph:

1. Navigate to `simulations/` (`cd simulations`)
2. Ensure `run-simulations.sh` is executable (`chmod +x run-simulations.sh`)
3. Run the script with `./run-simulations.sh`


### Example simulation using data from a file

`./air-brakes -s -f simulations/data/H115DM-14A-14.csv -t 1 -c 2`

- -s: simulate
- -f: use a file
- -t: column of the csv file that is the time
- -c: column of the csv file that is the altitude
