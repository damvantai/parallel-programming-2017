#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;
int main() {
	char greeting[MAX_STRING];	// String storing message
	int comm_sz;	// Number of processer
	int my_rank;	// MY process rank
	int q;

	// start up MPI
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	// Get my rank among all the processes
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank != 0) {
		// Create message
		sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
		// Send message to process 0
		MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else {
		// Print my message
		printf("Greetings from Master process %d of %d!\n", my_rank, comm_sz);
		for (q = 1; q < comm_sz; q++) {
			// Receive message from process q
			MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%s\n", greeting);
		}
	}

	// Shut down MPI
	MPI_Finalize();

	return 0;
}