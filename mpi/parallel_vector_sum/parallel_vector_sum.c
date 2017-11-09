#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(void) {
	int n, local_n;
	int comm_sz, my_rank;
	double *local_x, *local_y, *local_z;
	MPI_Comm comm;

	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);

	Read_n(&n, &local_n, my_rank, comm_sz, comm);

	printf("Proc %d > n = %d, local_n = %d\n", my_rank, n, local_n);
	Allocate_vectors(&local_x, &local_y, &local_z, local_n, comm);

	Read_vector(local_x, local_n, n, "x", my_rank, comm);
	Print_vector(local_x, local_n, n, "x is", my_rank, comm);
	Read_vector(local_y, local_n, n, "y", my_rank, comm);
	Print_vector(local_y, local_n, n, "y is", my_rank, comm);

	Parallel_vector_sum(local_x, local_y, local_z, local_n);
	Print_vector(local_z, local_n, n, "The sum is", my_rank, comm);

	free(local_x);
	free(local_y);
	free(local_z);

	MPI_Finalize();

	return 0;
}


/*
Function: 	Read_n
Purpose:	Get the order of the vectors from stdin on proc 0 and broadcast to other processes.
In args:	my_rank:	process rank in communicator
			comm_sz: 	number of processes in communicator
			comm:		commmunicator containing all the processes calling Read_n

Out args:	n_p:		global value of n
			local_n_p:	local value of n = n/comm_sz

Errors: 	n should be positive and evenly divisible by comm_sz
*/

void Read_n(
	int* n_p,
	int* local_n_p,
	int my_rank,
	int comm_sz,
	MPI_Comm comm
	) {
	int local_ok = 1;
	char *fname = "Read_n";

	if (my_rank == 0) {
		printf("What's the order of the vectors?\n");
		scanf("%d", n_p);
	}

	MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
	if ((*n_p <= 0) || (*n_p % comm_sz != 0)) local_ok = 0;
	Check_for_error(local_ok, fname, "n should be > 0 and evenly divisible by comm_sz", comm);
	*local_n_p = *n_p/comm_sz;
}	// Read_n

/*
Function:	Allocate_vectors
Purpose:	Allocate storage for x, y, and z
In args:	local_n: 	the size of the local vectors
			comm:		the communicator containing the calling processes

Out args:	local_x_pp, local_y_pp, local_z_pp: pointers to memory blocks to be allocated for local vectors

Errors: 	One or more of the calls to malloc fails
*/

void Allocate_vectors(
	double** local_x_pp,
	double** local_y_pp,
	double** local_z_pp,
	int local_n,
	MPI_Comm comm) {

	int local_ok = 1;
	char* fname = "Allocate_vectors";

	*local_x_pp = malloc(local_n*sizeof(double));
	*local_y_pp = malloc(local_n*sizeof(double));
	*local_z_pp = malloc(local_n*sizeof(double));

	if (*local_x_pp == NULL || *local_y_pp == NULL || *local_z_pp == NULL) local_ok = 0;
	Check_for_error(local_ok, fname, "Can't Allocate local vector(s)", comm);
}

/*
Function:	Read_vector
Purpose:	Read a vector from stdin on process 0 and distribute among the processes using a block distribution
Errors:		If the malloc on process 0 for temporary storage fails the program terminates
Note:		This function assumes a block distribution and the order of the vector evenly divisible by comm_sz 
*/
void Read_vector(
	double local_a[],
	int local_n,
	int n,
	char vec_name[],
	int my_rank,
	MPI_Comm comm
	) {
	double* a = NULL;
	int i;
	int local_ok = 1;
	char* fname = "Read_vector";

	if (my_rank == 0) {
		a = malloc(n*sizeof(double));
		if (a == NULL) local_ok = 0;
		Check_for_error(local_ok, fname, "Can't Allocate temporary vector", comm);
		printf("Enter the vector %s\n", vec_name);
		for (i = 0; i < n; i++) {
			scanf("%1f", &a[i]);
		}
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
		free(a);
	} else {
		Check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
	}
}


/*
Function: 	Print_vector
Purpose:	Print a vector that has a block distribution to stdout assumes order of vector is evenly divisible by the nume of processes
*/
void Print_vector(
	double local_b[],
	int local_n,
	int n,
	char title[],
	int my_rank,
	MPI_Comm comm) {
	double* b = NULL;
	int i;
	int local_ok = 1;
	char* fname = "Print_vector";

	if (my_rank == 0) {
		b = malloc(n*sizeof(double));
		if (b == NULL) local_ok = 0;
		Check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
		MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
		printf("%s\n", title);
		for (i = 0; i < n; i++)
			printf("%f ", b[i]);
		printf("\n");
		free(b);
	} else {
		Check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
		MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
	}
}

/*
Function: 	Parallel_vector_sum
Purpose:	Add a vector that's been distributed among the processes
In args:	local_x:	local storage of one of the vectors begin added
			local_y:	local storage for the second vector being added 
			local_n:	the number of components in local_x, local_y, and local_z
Out arg:	local_z:	local storage for the sum of the two vectors
*/
void Parallel_vector_sum(
	double local_x[],
	double local_y[],
	double local_z[],
	int local_n) {
	int local_i;

	for (local_i = 0; local_i < local_n; local_i++)
		local_z[local_i] = local_x[local_i] + local_y[local_i];
}

/*
function:		Check_for_error
Purpose:		Check whether any process has found an error. If so, print message and terminal
				Otherwise, continue execution.
In args:		local_ok:	1 if calling Check_for_error
				fname:		name of function calling Check_for_error
				message:	message to print if there's an error
				comm:		communicator containing processes calling Check_for_error: should be MPI_COMM_WORLD

*/
void Check_for_error(
	int local_ok,
	char fname[],
	char message[],
	MPI_Comm comm) {

	int ok;

	MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
	if (ok == 0) {
		int my_rank;
		MPI_Comm_rank(comm, &my_rank);
		if (my_rank == 0) {
			fprintf(stderr, "Proc %d > In %s,%s\n", my_rank, fname, message);
			fflush(stderr);
		}
		MPI_Finalize();
		exit(-1);
	}
}
