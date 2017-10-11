#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int p;
	int my_rank;
	// int ierr;

	// start up initial MPI environment
	MPI_Init(&argc, &argv);

	// get the number of PE's in the group: MPI_COMM_WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// get my rank in the group: MPI_COMM_WORLD
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// say hello
	printf("My rank: PW[%d] out of %d Total Processors \n", my_rank, p);

	MPI_Finalize();
}