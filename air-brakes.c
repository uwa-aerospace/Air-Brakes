#include "air-brakes.h"

#define OPTLIST ":sf:t:a:"

void runQuadraticSimulations()
{
	int maxTime = 100;
	int maxHeight = 10000;
	for (size_t i = 0; i <= maxTime / 2; i++)
	{
		int currentHeight = getFakeCurrentHeightQuadratic(i, maxTime, maxHeight);
		int PIDResult = PIDController(currentHeight, maxHeight);
		printf("%i,%i\n", currentHeight, PIDResult);
	}
}

void runFileSimulations(int maxTime)
{
	for (float i = 0.0; i <= maxTime; i += 0.1)
	{
		int currentHeight = getFakeCurrentHeightFromFile(i);
		int PIDResult = PIDController(currentHeight, 2400);
		printf("%i,%i\n", currentHeight, PIDResult);
	}
}

int main(int argc, char *argv[])
{
	printf("--- UWA Aerospace: Air Brakes ---\n");
	int option = -1;
	bool simulate = false;
	bool useFile = false;
	int csvAltitudeColumn = 2;
	int csvTimeColumn = 1;

	char *fileName = NULL;
	while ((option = getopt(argc, argv, OPTLIST)) != -1)
	{
		switch (option)
		{
		case 's':
		{
			// Whether to simulate or not
			simulate = true;
			break;
		}
		case 'f':
		{
			// Simulation data file to use (with -s flag)
			useFile = true;
			fileName = strdup(optarg);
			break;
		}
		case 'a':
		{
			// What column of the CSV to use for altitude (with -f flag)
			csvAltitudeColumn = atoi(strdup(optarg));
			break;
		}
		case 't':
		{
			// What column of the CSV to use for time (with -f flag)
			csvTimeColumn = atoi(strdup(optarg));
			break;
		}
		default:
		{
			break;
		}
		}
	}

	if (simulate)
	{
		if (useFile)
		{
			runFileSimulations(initialiseFile(fileName, csvTimeColumn, csvAltitudeColumn));
		}
		else
		{
			runQuadraticSimulations();
		}
	}

	return EXIT_SUCCESS;
}
