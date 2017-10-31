#include <stdio.h>
#include <omp.h>

#define M 100
// #define T 10
#define N 100

int phantu_max(int a[], int i, int k) {
	int max = a[0];
	for (int i = 0; i < k; i++) {
		if (a[i] > max) max = a[i];
	}

	return max;
}

int main() {
	int a[100];
	int T = 10;
	int b[T];
	int i;
	for (i = 0; i < N; i++) {
		a[i] = -i * i + 100 * i + 4;
	}

	for (i = 0; i < N; i++) {
		if (i % 10 == 0) printf("\n");
		printf("%d ", a[i]);
	}

	printf("\n");
	printf("Max tuan tu la: %d\n", phantu_max(a, 0, 100));


	int id, Sid, Eid, Nid;
	omp_set_num_threads(T);
	#pragma omp parallel private(id, Sid, Nid, Eid)
	{	
		// N la kich thuoc day nhap vao
		// T la so luong
		id = omp_get_thread_num();
		// so cac so duoc so sanh cua moi luong
		Nid = N / T;	// = 10
		// 
		Sid = Nid * id; // = 10 * id
		Eid = Sid + Nid;	// = 10 * id + 10
		b[id] = phantu_max(a, Sid, Eid);
		printf("id = %d, Sid = %d, Eid = %d, b[i] = %d\n", id, Sid, Eid, b[i]);
	}

	// printf("Max song song la: %d \n", phantu_max(b, 0, T));
	// return 0;

	do {
		T = T / 2;
		omp_set_num_threads(T);
		#pragma omp parallel private(id)
		{
			id = omp_get_thread_num();
			
			b[id] = b[id] > b[id + T] ? b[id]:b[id + T];
			printf("id = %d, b[i] = %d\n", id, b[i]);

		}
	} while (T > 1);

	printf("Max song song la: %d\n", b[0]);
	return 0;
}



// do {
// 	T = T / 2;
// 	omp_set_num_threads(T);
// 	#pragma omp parallel private(id)
// 	{
// 		id = omp_get_thread_num

// 		b[id] = b[id] > b[id + T] ? b[id]:b[id + T];
// 	}

// } while (T > 1);

// printf("\nMax song song la: %d\n", b[0]);
// return 0;