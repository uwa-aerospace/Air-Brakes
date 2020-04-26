#include "air-brakes.h"

int main(int argc, char *argv[])
{
	printf("--- UWA Aerospace: Air Brakes ---\n");

	int maxTime = 100;
	int maxHeight = 10000;
	for (size_t i = 0; i <= maxTime / 2; i++)
	{
		int currentHeight = getFakeCurrentHeightQuadratic(i, maxTime, maxHeight);
		int PIDResult = PIDController(currentHeight);
		printf("%i,%i\n", currentHeight, PIDResult);
	}

	return EXIT_SUCCESS;
}
