	int sum, starti, id, mc, nthreads;
	omp_set_num_threads(2);
	#pragma omp parallel private(id, starti, i, j, k, sum)
	{
		nthreads = omp_get_num_threads();
		id = omp_get_thread_num();
		sum = 0;
		mc = 2 / nthreads;
		starti = id * mc;
		for (i = starti; i < starti + mc; i++)
			for (j = 0; j < P; j++)
				for (k = 0; k < N; k++)
					sum = sum + a[i][k] * b[k][j];
					printf("sum=%d\n", sum);
				c[i][j] = sum;
				sum = 0;
				printf("\nc[%d][%d] = %d", i, j, c[i][j]);
	}


	// kahi bao ma tran A, B, C
	int *A, *B;
	A = (int *) malloc ((m*n)*sizeof(int));
	B 

	displaymatrix(int *A, int row, int col) {
		*(A + i * col + j)
	}


	// chia theo hang va cot cua ma tran 
	#pragma (id, i, j, k, i1, i2, k1, k2)
	{
		id = omp_get_thread_num();
		idh = id / NTc;
		idc = id % NTc;
		i1 = Ms * idh;
		i2 = Ms * (idh + 1);
		k1 = Ps * idc;
		k2 = Ps * (idc + 1)

		for(i = i1; i < i2; i++)
			for (k = k1; k < k2; k++) {
				*(C + i*p + k) = 0.0;
				for (j = 0; j < n; j++) {
					*(C + i*p + k) = *(C + i*p + k) + (*(A + i*n + j)) * (*(B + j*p + k));
				}
			}
	}