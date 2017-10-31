#include <stdio.h>
#include <omp.h>

int main() {
	int threads, id;
	threads = 10;
	id = 10;
	omp_set_num_threads(10);

	#pragma omp parallel
	printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());

	
	return 0;

}