#include <stdio.h>
#include <omp.h>

#define M 20
#define Time 20
#define dt 0.01
#define dx 0.1
#define D 0.1
#define NT 4

void daohambac2(float *T, float *dT, int start, int stop) {
	int i;
	float c,l,r;
	for (i = start; i < stop; i++) {
		c = *(T + i);
		l = (i == 0) ? 100. : *(T + i - 1);
		r = (i == M - 1) ? 25. : *(T + i + 1);
		*(dT + i) = D * (l - 2 * c + r) / (dx * dx);
	}
}

int main() {
	FILE *f;
	f = fopen("data_omp_2.txt", "w");
	float T[20];
	float dT[20];
	int i;
	float t;
	int Ntime;

	Ntime = Time / dt;

	// init value T(i) = 25C
	for (i = 0; i < 20; i++) {
		T[i] = 25;
		dT[i] = 0;
		// printf("%f\t", T[i]);
	}
	// printf("\n");

	// cai dat ham tich hip theo thoi gian

	int id, Mc;
	int start, stop;
	float c,l,r;

	
	for (t = 0; t < Ntime; t++) {
		fprintf(f, "%0.2f ", t);
		#pragma omp parallel private(id, i, c, l, r, start, stop, Mc, t)
		{
			omp_set_num_threads(NT);
			// printf("%d\n", omp_get_num_threads());
			id = omp_get_thread_num(); // id = 0, 1, 2 , 4
			// printf("%d\n", id);
			Mc = M / NT; // Mc = 20 / 5 = 4
			start = id * Mc; // start = 0, 4, 8, 12, 16
			stop = start + Mc;	// stop = 4, 8, 12, 16, 20
			// printf("%d\n", stop);
			daohambac2(&T, &dT, start, stop);
			// int i;
			// float c,l,r;
			// for (i = start; i < stop; i++) {
			// 	c = *(T + i);
			// 	l = (i == 0) ? 100. : *(T + i - 1);
			// 	r = (i == M - 1) ? 25. : *(T + i + 1);
			// 	*(dT + i) = D * (l - 2 * c + r) / (dx * dx);
			// }

			#pragma omp barrier
			// thiet lap NT threads

			for (i = start; i < stop; i++) {
				*(T + i) = *(T + i) + *(dT + i) * dt;
				// fprintf(f, "%0.4f ", *(T + i));
			}
			// #pragma omp barrier

		}
		for (i = 0; i < M; i++) {
			fprintf(f, "%0.4f ", *(T + i));
		}

		// #pragma omp barrier
		fprintf(f, "\n");
		t = t + dt;
		// #pragma omp barrier

	}


	printf("\n");

	for (i = 0; i < M; i++) {
		printf("%0.4f\t", *(dT + i));
		if ((i + 1) % 5 == 0) printf("\n");
	}

	for (i = 0; i < M; i++) {
		printf("%f\t", *(T + i));
		if ((i + 1) % 5 == 0) printf("\n");
	}

	fclose(f);
	return 0;
}