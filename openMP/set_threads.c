#include <omp.h>
#include <stdio.h>

int main(void) {
	
	// #pragma omp parallel
	// printf("hello from thread %d n hread %d\n", omp_get_thread_num(), omp_get_num_threads());
	omp_set_num_threads(10);
	#pragma omp_parallet
	for (int i = 0; i < 10; i++) {
		printf("hello %d from thread %d n hread %d\n", i, omp_get_thread_num(), omp_get_num_threads());
	}
}