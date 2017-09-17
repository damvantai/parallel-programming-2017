#include <omp.h>
#include <stdio.h>

int main(void) {
	omp_get_num_threads(10)
	#pragma omp parallel
	printf("hello from thread %d n hread %d\n", omp_get_thread_num(), omp_get_num_threads());


}