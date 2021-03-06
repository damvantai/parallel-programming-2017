#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define LENGTH_WIRE 1.1 //1.1
#define GAMMA 0.1
#define TIME_STOP 10
#define DELTA_TIME 0.1
#define NUMBER_POINT 56//56

int main() {
	int i;
	float l;
	float C;
	float DELTA_X;
	float t;
	float x;
	float *Phi_current, *Phi_new, *Phi_old;

	FILE *f;
	f = fopen("data_equation_1D.txt", "w");


	l = TIME_STOP / DELTA_TIME;	// 5/0.1 = 50
	DELTA_X = (float)LENGTH_WIRE / (NUMBER_POINT - 1); // 1/10 = 0.1
	C = GAMMA * DELTA_TIME / DELTA_X; // C = 0.1 * 0.1 / 0.1 = 0.1

	Phi_old = (float *) malloc(NUMBER_POINT * sizeof(float));
	Phi_current = (float *) malloc(NUMBER_POINT * sizeof(float));
	Phi_new = (float *) malloc(NUMBER_POINT * sizeof(float));

	t = 0;

	for(i = 1; i <= NUMBER_POINT; i++) {
		x = (i - 1) * DELTA_X;
		*(Phi_current + i) = sin(2.0 * M_PI * x / LENGTH_WIRE);
	}



	for(i = 2; i < NUMBER_POINT; i++) {
		*(Phi_old + i) = *(Phi_current + i) + 1.0/2 * C * C * (*(Phi_current + i +1) - 2 * *(Phi_current + i) + *(Phi_current + i - 1));
	}

	*(Phi_old + 1) = 0;
	*(Phi_old + NUMBER_POINT) = 0;


	*(Phi_new + 45) = 0;
	for (i = 2; i < NUMBER_POINT; i++) {
		*(Phi_new + i) = 2 * *(Phi_current + i) - *(Phi_old + i) + C * C * (*(Phi_current + i +1) - 2 * *(Phi_current +  i) + *(Phi_current + i - 1));
	}
	

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
		for (i = 1; i <= NUMBER_POINT; i++) {
			fprintf(f, "%0.6f ", *(Phi_new +i));
		}
		fprintf(f, "\n");
	}

	printf("\n");
	for (i = 1; i <= NUMBER_POINT; i++) {
		
		printf("%0.6f\t", Phi_new[i]);
		if (i % 5 == 0) printf("\n");
	}

	fclose(f);
	return 0;
}