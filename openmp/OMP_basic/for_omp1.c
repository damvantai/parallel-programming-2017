#include <stdio.h>
#include <omp.h>

int main() {
	#pragma omp parallel num_threads(10)
	{
		int i;
		printf("Hello world\n");
		// #pragma omp for private(i)
		#pragma omp for
		for (i = 1; i <= 4; i++) {
			printf("Iteration %d\n", i);
		}
	
		printf("Goodbye world\n");
	}
	return 0;
}