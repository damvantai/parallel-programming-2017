#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define LENGTH_WIRE 1.1
#define GAMMA 0.1
#define TIME_STOP 5
#define DELTA_TIME 0.1
#define NUMBER_POINT 55

int main() {
	int i;
	float l;
	float C;
	float DELTA_X;
	float t;
	float x;
	float *Phi_current, *Phi_new, *Phi_old;

	l = TIME_STOP / DELTA_TIME;	// 5/0.1 = 50
	DELTA_X = (float)LENGTH_WIRE / (NUMBER_POINT - 1); // 1/10 = 0.1
	C = GAMMA * DELTA_TIME / DELTA_X; // C = 0.1 * 0.1 / 0.1 = 0.1

	// printf("%f C, %f l, %f DELTA_X\n", C, DELTA_X, l);
	Phi_old = (float *) malloc(NUMBER_POINT * sizeof(float));
	Phi_current = (float *) malloc(NUMBER_POINT * sizeof(float));
	Phi_new = (float *) malloc(NUMBER_POINT * sizeof(float));

	t = 0;
	printf("%f\n", DELTA_X);
	printf("%f\n", DELTA_TIME);
	printf("%f\n", C);
	printf("%f\n", l);
	for(i = 1; i <= NUMBER_POINT; i++) {
		x = (i - 1) * DELTA_X;
		// printf("%f\n", x);
		*(Phi_current + i) = sin(2.0 * M_PI * x / LENGTH_WIRE);
		// printf("%0.2f\n", *(Phi_current + i));
	}

	printf("\nsau khi khoi tao Phi_current\n");
	for (i = 1; i <= NUMBER_POINT; i++) {
		printf("%0.6f\t", Phi_current[i]);
		if (i % 5 == 0) printf("\n");
	}

	for(i = 2; i < NUMBER_POINT; i++) {
		*(Phi_old + i) = *(Phi_current + i) + 1.0/2 * C * C * (*(Phi_current + i +1) - 2 * *(Phi_current + i) + *(Phi_current + i - 1));
	}

	*(Phi_old + 1) = 0;
	*(Phi_old + NUMBER_POINT) = 0;

	printf("\nsau khi khoi tao Phi_old\n");
	for (i = 1; i <= NUMBER_POINT; i++) {		
		printf("%0.6f\t", Phi_old[i]);
		if (i % 5 == 0) printf("\n");
	}
	*(Phi_new + 45) = 0;
	for (i = 2; i < NUMBER_POINT; i++) {
		*(Phi_new + i) = 2 * *(Phi_current + i) - *(Phi_old + i) + C * C * (*(Phi_current + i +1) - 2 * *(Phi_current +  i) + *(Phi_current + i - 1));
	}
	// float a, b, c, d, e, f;
	// a = *(Phi_current + 45);
	// b = *(Phi_current + 46);
	// c = *(Phi_current + 44);
	// d = *(Phi_old + 45);
	// e = C * C;
	// printf("%f c square \n", e);
	// printf("%f a:\n", *(Phi_current + 45));
	// printf("%f b:\n", *(Phi_current + 46));
	// printf("%f c:\n", *(Phi_current + 44));
	// printf("%f d:\n", *(Phi_old + 45));
	// printf("%f f:\n", *(Phi_new + 45));

	// f = 2 * a - d + c*(b + c - 2*a);
	// printf("%f\n", f);

	while (t < TIME_STOP) {
		t = t + DELTA_TIME;

		for (i = 2; i < NUMBER_POINT; i++) {
			*(Phi_new + i) = 2 * *(Phi_current + i) - *(Phi_old + i) + C * C * (*(Phi_current + i +1) - 2 * *(Phi_current +  i) + *(Phi_current + i - 1));
		}

		*(Phi_new + 1) = 0;
		*(Phi_new + NUMBER_POINT) = 0;

		for (i = 2; i < NUMBER_POINT; i++) {
			*(Phi_old + i) = *(Phi_current + i);
			*(Phi_current + i) = *(Phi_new + i);
		}
		printf("\n");
		for (i = 1; i <= NUMBER_POINT; i++) {
			printf("%0.6f\t", Phi_new[i]);
			if (i % 5 == 0) printf("\n");
		}
	}

	printf("\n");
	for (i = 1; i <= NUMBER_POINT; i++) {
		
		printf("%0.6f\t", Phi_new[i]);
		if (i % 5 == 0) printf("\n");
	}

	return 0;
}