#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
	int N = 100;
	int *a, *s, *b;
	a = (int*) malloc(sizeof(int)*N);
	b = (int*) malloc(sizeof(int)*N);
	s = (int*) malloc(sizeof(int)*N);
	int i;
	for(i = 0; i < N; i++){
		a[i] = 2 * i;
		b[i] = i;
		// printf("%d %d", a[i], b[i]);
	}
	int Nid, id, Sid, Eid, sum = 0, T=10;
	omp_set_num_threads(10);
	#pragma omp parallel private(id, Sid, Eid)
	{
		id = omp_get_thread_num();
		Nid = N/T;
		Sid = id * Nid;
		Eid = Sid + Nid;
		for(i = Sid; i < Eid; i++){
			s[i] = a[i] + b[i];
			printf("%d\n", s[i]);
			sum += a[i] + b[i];
		}
	}
	for(int i = 0; i < N; i++){
		printf("%d ",s[i]);
	}
	printf("\n");
	return 0;
}