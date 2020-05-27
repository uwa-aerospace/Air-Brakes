#include "air-brakes.h"

#define OPTLIST ":sf:t:c:"

void runSimulations()
{
	int maxTime = 100;
	int maxHeight = 10000;
	for (size_t i = 0; i <= maxTime / 2; i++)
	{
		int currentHeight = getFakeCurrentHeightQuadratic(i, maxTime, maxHeight);
		int PIDResult = PIDController(currentHeight);
		printf("%i,%i\n", currentHeight, PIDResult);
	}
}

const char *getfield(char *line, int num)
{
	const char *tok;
	for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

bool startsWith(const char *pre, const char *str)
{
	size_t lenpre = strlen(pre),
		   lenstr = strlen(str);
	return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
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
		case 'c':
		{
			// What column of the CSV to use (with -f flag)
			csvAltitudeColumn = atoi(strdup(optarg));
			break;
		}
		case 't':
		{
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
			FILE *stream = fopen(fileName, "r");
			char line[3000];
			while (fgets(line, 3000, stream))
			{
				if (startsWith("#", line))
				{
					continue;
				}
				char *tmp = strdup(line);
				const char* height = getfield(tmp, csvAltitudeColumn);
				tmp = strdup(line);
				const char* time = getfield(tmp, csvTimeColumn);
				printf("At time %s, altitude: %s\n", time, height);
				free(tmp);
			}
		}
		else
		{
			runSimulations();
		}
	}

	return EXIT_SUCCESS;
}
