#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int rank, size;
	int *A, *B, *C, i;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Status thongbao;
	A = (int *) malloc (20 * sizeof(int));
	B = (int *) malloc (20 * sizeof(int));
	C = (int *) malloc (20 * sizeof(int));
	if (rank == 0) {
		for (i = 0; i < 20; i++) {
			*(A + i) = i;
			*(B + i) = 10 - i;
			*(C + i) = 0;
		}
		MPI_Send(A, 20, MPI_INT, 1, 100, MPI_COMM_WORLD);
	} else {
		MPI_Recv(B, 20, MPI_INT, 0, 100, MPI_COMM_WORLD, &thongbao);
		for (i = 0; i < 20; i++) {
			*(C + i) = 2 * *(B + i);
		}
	}

	for (i = 0; i < 20; i++) {
		printf("%d\t", *(C + i));
		if (i % 4 == 0)
			printf("\n");
	}
	printf("\n");
	// printf("Hello world from process %d of %d\n", rank, size);
	MPI_Finalize();
	return 0;
}