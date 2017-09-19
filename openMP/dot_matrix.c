#include <stdio.h>
#include <omp.h>

#define M 2
#define N 3
#define P 4


void printMatrix(int m, int n, int a[m][n]) {
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int a[M][N];
	int b[N][P];
	int c[M][P];

	int i, j, k;

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = i + j;
		}
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < P; j++) {
			b[i][j] = i - j;
		}
	}

	printMatrix(M, N, a);
	printMatrix(N, P, b);


	int id, sum, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc;
	omp_set_num_threads(8);
	#pragma omp parallel private(i, sum, id, j, k, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc)
	{	
		sum = 0;
		id = omp_get_thread_num();
		// printf("%d\n", id);
		idh = id / 4; 
		idc = id % 4;

		Nidh = 1; Nidc = 1;
		Sidh = Nidh * idh; Eidh = Sidh + Nidh;
		Sidc = Nidc * idc; Eidc = Sidc + Nidc;

		// #pragma parallel for
		// {
		for (i = Sidc; i < Eidc; i++) {
			for (j = Sidh; j < Eidh; j++) {
				for (k = 0; k < 3; k++) {
					sum = sum + a[j][k] * b[k][i];
				}
				c[j][i] = sum;
				sum = 0;
				// printf("\nsum = %d", sum);
				printf("\nc[%d][%d] = %d", j, i, c[j][i]);
			}
			// printf("\nc[%d][%d] = %d", j, i, c[i][j]);
		}
		printf("\nid = %d, idh = %d, idc = %d, Sidc = %d, Eidc = %d, Sidh = %d, Eidh = %d\n", id, idh, idc, Sidc, Eidc, Sidh, Eidh);
		// }
	}

	// for (i = 0; i < M; i++) {
	// 	for (j = 0; j < P; j++) {
	// 		printf("%d ", c[i][j]);
	// 	}
	// 	printf("\n");
	// }
	
	printMatrix(M, P, c);

	return 0;
}