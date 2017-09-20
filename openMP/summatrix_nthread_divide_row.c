#include <stdio.h>
#include <omp.h>

#define N 10 // so cot
#define M 6 // so hang


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
	int b[M][N];
	int c[M][N];

	int i, j;

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = i;
			b[i][j] = j;
		}
	}

	int id, Sid, Eid, Nid, T = 6;

	omp_set_num_threads(T);
	#pragma omp parallel private(i, j, id, Sid, Eid, Nid)
	{
		// tinh tung hang mot
		id = omp_get_thread_num();
		Nid = M / T;	
		// 
		Sid = Nid * id; 
		Eid = Sid + Nid;

		for (j = 0; j < N; j++) {
			for (i = Sid; i < Eid; i++) {
				c[i][j] = a[i][j] + b[i][j];
			}
		}
		printf("id = %d, Sid = %d, Eid = %d\n", id, Sid, Eid);
	}


	printf("matrix A:\n");
	printMatrix(10, 6, a);
	printf("matrix B:\n");
	printMatrix(10, 6, b);
	printf("matrix C:\n");
	printMatrix(10, 6, c);
	return 0;

}
