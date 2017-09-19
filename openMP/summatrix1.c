#include <stdio.h>
#include <omp.h>

#define N 10 // so cot
#define M 6 // so hang

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
		// for (i = Sid; i < Eid; i++) {
		// 	for (j = 0; j < N; j++) {
		// 		c[i][j] = a[i][j] + b[i][j];
		// 	}
		// }
		// printf("id = %d, Sid = %d, Eid = %d\n", id, Sid, Eid);
		for (j = 0; j < N; j++) {
			for (i = Sid; i < Eid; i++) {
				c[i][j] = a[i][j] + b[i][j];
			}
		}
		printf("id = %d, Sid = %d, Eid = %d\n", id, Sid, Eid);
	}

	return 0;

}
// dung luong tinh theo hang
// int id, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc, Tc = 2, Th = 3;
// omp_set_num_threads(Th * Tc);
// #pragma omp parallel private(i, j, id, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc)
// {
// 	id = omp_get_thread_num();
// 	idh = id/Tc; idc = id%Tc;
// 	idh = M/Th; Nidc = N/Tc;
// 	Sidh = Nidh; Eidh = Sidh + Nidh;
// 	Sidc = Nidc * idc; Eidc = Sidc + Nidc;
// 	for (i = Sidh; i < Eidh; i++)
// 		for (j = Sidc; j < Eidc; j++) {
// 			c[i][j] = a[i][j] + b[i][j];
// 		}
// }
