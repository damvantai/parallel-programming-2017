#include <omp.h>
#include <stdio.h>

int main() {
	int id, x, nthreads;
	omp_set_num_threads(10);
	#pragma omp parallel// private(id, x)
	{
		id = omp_get_thread_num();
		x = 10 * id;
		printf("\n");
		printf("Hello from thread %d, x = %d", id, x);
		// printf("Hello from thread %d, x = %d", omp_get_thread_num(), 10 * omp_get_thread_num());
		printf("\n");
		// #pragma omp barrier
		if (id == 0) {
			nthreads = omp_get_num_threads();
			printf("There are %d thread\n", nthreads);
		}

	}
	return 0;
}