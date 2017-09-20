//
// dot matrix use number thread equals number element in matrix result
//
#include <stdio.h>
#include <omp.h>

#define M 4 // row a and col c
#define N 6 // col a and row b
#define P 8 // col b and col c


void printMatrix(int row, int column, int matrix[row][column]) {
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}



int main() {
	int a[M][N];
	int b[N][P];
	int c[M][P];

	int i, j, k;

	// create matrix A zimple
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = i + j;
		}
	}

	// create matrix b is matrix diagonal
	for (i = 0; i < N; i++) {
		for (j = 0; j < P; j++) {
			if (i == j) 
				b[i][j] = 1;
			else 
				b[i][j] = 0;
			// b[i][j] = i - j;
		}
	}

	// create matrix c with all elements equals zero
	for (i = 0; i < M; i++) {
		for (j = 0; j < P; j++) {
			c[i][j] = 0.0;
		}
	}

	printf("matrix A:\n");
	printMatrix(M, N, a);
	printf("matrix B\n");
	printMatrix(N, P, b);

	int sum, starti, id, mc, nthreads;
	omp_set_num_threads(2);
	#pragma omp parallel private(id, starti, i, j, k, sum)
	{
		nthreads = omp_get_num_threads();
		id = omp_get_thread_num();
		sum = 0;
		mc = M / nthreads;
		starti = id * mc;
		for (i = starti; i < starti + mc; i++)
			for (j = 0; j < P; j++)
				for (k = 0; k < N; k++)
					c[i][j] = c[i][j] + a[i][k] * b[k][j];
	}

	printf("matrix C:\n");
	printMatrix(M, P, c);

	return 0;
}

