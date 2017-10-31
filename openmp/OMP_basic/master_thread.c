// omp master
#include <stdio.h>
#include <omp.h>
int main() {
	int a[2] = {-1, -1};
	#pragma omp parallel
	{
		a[omp_get_thread_num()] = omp_get_thread_num();
		// #pragma omp master
		// default when don't have above line result 4 times You sho...
		// if have append 1 time
		printf("You should only see this once\n");
	}
	printf("a[0]=%d, a[1]=%d\n", a[0], a[1]);

	return 0;
}