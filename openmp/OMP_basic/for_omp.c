// test omp for
#include <stdio.h>
// #include <omp.h>
#include <math.h>

int main() {
	float a[100], b[100];
	int i;
	for (i = 0; i < 100; i++)
		b[i] = i;

	#pragma omp parallel
	{
		#pragma omp for
		for (i = 0; i < 100; i++)
			a[i] = sqrt(b[i]);
			printf("%f\n", a[i]);
	}

	return 0;
}