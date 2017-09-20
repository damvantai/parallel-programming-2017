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

	int sum_threads;
	sum_threads = M * P;
	int id, sum, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc;
	omp_set_num_threads(sum_threads);
	#pragma omp parallel private(i, sum, id, j, k, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc)
	{	
		sum = 0;
		id = omp_get_thread_num();
		// printf("%d\n", id);
		idh = id / P; 
		idc = id % P;

		Nidh = 1; Nidc = 1;
		Sidh = Nidh * idh; Eidh = Sidh + Nidh;
		Sidc = Nidc * idc; Eidc = Sidc + Nidc;

		// #pragma parallel for
		// {
		for (i = Sidc; i < Eidc; i++) {
			for (j = Sidh; j < Eidh; j++) {
				for (k = 0; k < N; k++) {
					sum = sum + a[j][k] * b[k][i];
				}
				c[j][i] = sum;
				sum = 0;
				// printf("\nc[%d][%d] = %d", j, i, c[j][i]);
			}
		}
		// printf("\nid = %d, idh = %d, idc = %d, Sidc = %d, Eidc = %d, Sidh = %d, Eidh = %d\n", id, idh, idc, Sidc, Eidc, Sidh, Eidh);
	}

	printf("matrix C:\n");
	printMatrix(M, P, c);

	return 0;
}