#include <omp.h>
#include <stdio.h>

int main() {
	int nthreads, id;
	#pragma omp parallel
	{
		nthreads = omp_get_num_threads();
		id = omp_get_thread_num();
		printf("hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
		printf("hello form id = %d\n", id);
		#pragma omp barrier
		if (id == 0) {
			nthreads = omp_get_num_threads();
			printf("There are %d thread, id = %d\n", nthreads, id);
		}
	}
	return 0;
}