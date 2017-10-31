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

	// in class 2
	int i1, i2, k1, k2, NTc = 3, NTh = 2, idh, idc, id, Ms, Ps;
	Ms = M/NTh;
	Ps = P/NTc;
	omp_set_num_threads(6);
	#pragma omp parallel private(id, i, j, k, i1, i2, k1, k2)
	{
		id = omp_get_thread_num();
		idh = id / NTc;
		idc = id % NTc;
		i1 = Ms * idh;
		i2 = Ms * (idh + 1);
		k1 = Ps * idc;
		k2 = Ps * (idc + 1);

		for(i = i1; i < i2; i++)
			for (k = k1; k < k2; k++) {
				c[i][k] = 0.0;
				for (j = 0; j < N; j++) {
					// *(C + i*p + k) = *(C + i*p + k) + (*(A + i*n + j)) * (*(B + j*p + k));
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
				}
			}
	}

	printf("matrix C:\n");
	printMatrix(M, P, c);

	return 0;
}