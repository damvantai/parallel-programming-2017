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

	// int id, Sid, Eid, Nid, T = 6;

	// omp_set_num_threads(T);
	// #pragma omp parallel private(i, j, id, Sid, Eid, Nid)
	// {
	// 	// tinh tung hang mot
	// 	id = omp_get_thread_num();
	// 	Nid = M / T;	
	// 	// 
	// 	Sid = Nid * id; 
	// 	Eid = Sid + Nid;
	// 	// for (i = Sid; i < Eid; i++) {
	// 	// 	for (j = 0; j < N; j++) {
	// 	// 		c[i][j] = a[i][j] + b[i][j];
	// 	// 	}
	// 	// }
	// 	// printf("id = %d, Sid = %d, Eid = %d\n", id, Sid, Eid);
	// 	for (j = 0; j < N; j++) {
	// 		for (i = Sid; i < Eid; i++) {
	// 			c[i][j] = a[i][j] + b[i][j];
	// 		}
	// 	}
	// 	printf("id = %d, Sid = %d, Eid = %d\n", id, Sid, Eid);
	// }





	// chia hinh chua nhat ra thanh co o nhu kich thuoc Tc x Th, chuyen doi dia chi cua hang cot, id -> (idh, idc)
	int id, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc, Tc = 2, Th = 3;
	omp_set_num_threads(Th * Tc);
	#pragma omp parallel private(i, j, id, idh, Sidh, Eidh, Nidh, idc, Sidc, Eidc, Nidc)
	{
		id = omp_get_thread_num();
		idh = id/Tc; idc = id%Tc;	// id luong chia cho Tc
		Nidh = M/Th; Nidc = N/Tc;	// Nidh = 2, Nidc = 5
		Sidh = Nidh * idh; Eidh = Sidh + Nidh;	// 2 * (1, 2, 3) || ++ 2  dau hang va cuoi hang
		Sidc = Nidc * idc; Eidc = Sidc + Nidc;	// 5 (0, 1) || ++ 5		  dau cot va cuoi cot

		for (i = Sidh; i < Eidh; i++)
			for (j = Sidc; j < Eidc; j++) {
				c[i][j] = a[i][j] + b[i][j];
			}

		printf("id = %d, idh = %d, Nidh = %d, Sidc = %d, Eidc = %d, Sidh = %d, Eidh = %d\n", id, idh, Nidh, Sidc, Eidc, Sidh, Eidh);
	}

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			printf("%d\t", b[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			printf("%d\t", c[i][j]);
		}
		printf("\n");
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


// bai tap 2 nhan ma tran chay song song