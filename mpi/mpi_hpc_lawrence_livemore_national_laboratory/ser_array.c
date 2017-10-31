#include <stdio.h>
#include <stdlib.h>
#define ARRAYSIZE		1600000

int main(int argc, char *argv[]) {
	int i;
	float data[ARRAYSIZE];

	printf("Starting serial array example...\n");
	printf("Using array of %d floats. Requires %ld bytes\n", ARRAYSIZE, sizeof(data));

	// Initializa the array
	printf("Initializing array ....\n");
	for (i = 0; i < ARRAYSIZE; i++) {
		data[i] = i * 1.0;
	}

	// Do a simple value assignment to each of the array elements
	printf("Performing computation on array elements...\n");
	for (i = 1; i < ARRAYSIZE; i++) {
		data[i] = data[i] + i * 1.0;
	}

	// Print a few sample results
	printf("Sample results\n");
	printf("   data[1]=%e\n", data[1]);
	printf("   data[100]=%e\n", data[100]);
	printf("   data[1000]=%e\n", data[1000]);
	printf("   data[10000]=%e\n", data[10000]);
	printf("   data[100000]=%e\n", data[100000]);
	// printf("   data[1000000]=%e\n", data[1000000]);
	printf("\nAll Done!\n");
}