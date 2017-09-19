#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

int main() {
	int a[10][10], mx = -1, lmx = -1, i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			a[i][j] = i + j;
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	#pragma omp parallel private(i, j) firstprivate(lmx)
		{
			#pragma omp for 
			for (i = 0; i < 10; i++)
				for (j = 0; j < 10; j++)
					lmx = (lmx > a[i][j]) ? lmx : a[i][j];
			#pragma omp critical
				mx = (mx > lmx) ? mx : lmx;
		}
		printf("max value of a is %d\n", mx);

	return 0;
}