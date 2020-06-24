#include "air-brakes.h"

#define OPTLIST ":sf:t:a:r:c:"

#define FLIGHT_DESIRED_HEIGHT 10000

void runQuadraticSimulations()
{
	int maxTime = 100;
	int maxHeight = 10000;
	DATA_POINT currentData;
	for (size_t i = 0; i <= maxTime / 2; i++)
	{
		int currentHeight = getFakeCurrentHeightQuadratic(i, maxTime, maxHeight);
		currentData.height = currentHeight;
		currentData.speed = 10;
		currentData.vertical_acceleration = 5;
		float PIDResult = PIDController(maxHeight, &currentData);
		printf("%i,%f\n", currentHeight, PIDResult);
	}
}

void runFileSimulations(int apogeeTime)
{
	for (float i = 0.0; i <= apogeeTime; i += 0.1)
	{
		DATA_POINT *currentData = getDataFromFile(i);
		// Max height 2376.0. Simulate a slight overshoot below.
		float PIDResult = PIDController(2200, currentData);
		printf("%f,%f\n", currentData->height, PIDResult);
	}
}

int main(int argc, char *argv[])
{
	printf("--- UWA Aerospace: Air Brakes ---\n");
	int option = -1;

	bool simulate = false;
	bool useFile = false;

	// Default CSV columns
	int csvTimeColumn = 1;		   // time passed (s)
	int csvAltitudeColumn = 2;	   // height (ft)
	int csvSpeedColumn = 3;		   // vertical speed (m/s)
	int csvAccelerationColumn = 4; // vertical acceleration (m/s/s)

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
		case 'r':
		{
			// What column of the CSV to use for speed (with -f flag)
			csvSpeedColumn = atoi(strdup(optarg));
			break;
		}
		case 'c':
		{
			// What column of the CSV to use for acceleration (with -f flag)
			csvAccelerationColumn = atoi(strdup(optarg));
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
			runFileSimulations(initialiseFile(fileName, csvTimeColumn, csvAltitudeColumn, csvSpeedColumn, csvAccelerationColumn));
		}
		else
		{
			runQuadraticSimulations();
		}
	}
	else
	{
		initialiseBMX055();
		DATA_POINT currentData;
		while (true)
		{
			currentData = getCurrentBMX055Data();
			float brakePercentage = PIDController(FLIGHT_DESIRED_HEIGHT, &currentData);
			printf("Brake percentage: %f\n", brakePercentage);
			// TODO: set the brake percentage on the stepper motor
		}
	}

	return EXIT_SUCCESS;
}
