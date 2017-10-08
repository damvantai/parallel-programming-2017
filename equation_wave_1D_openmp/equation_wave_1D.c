#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define  20
#define M_PI 3.14159265358979323846
#define Time 20
#define C 0.2
#define dt 0.1
void KhoiTao(float *Phi_old, float *Phi_current)
{
	int i;
	float dx, x;
	dx = 1.0/(M-1);
	for (i = 0; i < M; i++) {
		x = i * dx;
		*(Phi_old + i) = sin(2.0*M_PI*x);
		// *(Phi_current + i) = *(Phi_old + 1);
	}
	for (i = 0; i < M; i++) {
		x = i * dx;
		*(Phi_current + i) = *(Phi_old + i + 1);
	}
}

void FD(float *Phi_current, float *dPhi)
{
	int i;
	float c, l, r;
	for (i = 0; i < M; i++) {
		c = *(Phi_current + i);
		l = (i == 0) ? -*(Phi_current + i + 1) : *(Phi_current + i - 1);
		r = (i == M - 1) ? -*(Phi_current + i - 1) : *(Phi_current + i + 1);
		*(dPhi + i) = l - 2*c + r;
	}
}

int main() {
	int i;
	float dx, t, tau;
	t = 0.0;
	float *Phi_old, *Phi_current, *Phi_new, *dPhi;

	Phi_old = (float *) malloc (M*sizeof(float));
	Phi_current = (float *) malloc (M*sizeof(float));
	Phi_new = (float *) malloc (M*sizeof(float));
	dPhi = (float *) malloc (M*sizeof(float));
	
	for (i = 0; i < M; i++) {
		*(Phi_new + i) = 0;
	}

	KhoiTao(Phi_old, Phi_current);

	// FD(Phi_current, dPhi);

	// for (i = 0; i < M; i++) {
	// 	if (i % 5 == 0) printf("\n");
	// 	printf("%f\t", Phi_old[i]);
	// }

	// for (i = 0; i < M; i++) {
	// 	if (i % 5 == 0) printf("\n");
	// 	printf("%f\t", Phi_current[i]);
	// }

	for (t = 0; t < 20; t++) {
		FD(Phi_old, Phi_current);
		for (i = 0; i < M; i++) {
			*(Phi_new + i) = 2 * *(Phi_current + i) - *(Phi_old + i) + dt * dt * C * *(dPhi + i);
		}
		for (i = 0; i < M; i++) {
			if (i % 5 == 0) printf("\n");
			printf("%.02f\t", Phi_new[i]);
		}
		printf("\n");

		for (i = 0; i < M; i++) {
			*(Phi_current + i ) = *(Phi_new + i);
			*(Phi_old + i) = *(Phi_current + i);
		}
		t = t + dt;
	}



	for (i = 0; i < M; i++) {
		if (i % 5 == 0) printf("\n");
		printf("%.02f\t", Phi_new[i]);
	}
}