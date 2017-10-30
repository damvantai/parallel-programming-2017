// Purpose:	Use MPI to implement a parallel version of the trapezoidal rule
// 			This version uses collective communications and MPI derived datatypes
// 			to distribute the input data and compute the global sum

// Input:		The endpoints of the interval of integration and the number of trapezoids
// Output:		Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.

// Compile:	mpicc -g -Wall -o trapezoids_equations.compute2.c
// Run: 		mpiexec -n <number of processes> ./

// Algorithm:	
// 			1. Each process calculates "its" interval of integration
// 			2. Each process Estimate the integral of f(x) over its interval using the trapezoidal rule
// 			3. Each process != 0 sends its integral to 0.
// 			4. process 0 sums the calculations received from the individual processes and prints the result.

// NOTE: 		f(x) is all hardwired.

#include <stdio.h>
#include <mpi.h>

// Get the input values
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);

// Calculate local integral
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

// Function we're integrating
double f(double x);

int main() {
	int my_rank, comm_sz, n, local_n, source;
	double a, b, h, local_a, local_b, local_int, total_int;

	// Let the system do what it needs to start up MPI
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	Get_input(my_rank, comm_sz, &a, &b, &n);

	h = (b - a) / n;
	local_n = n / comm_sz;

	// Length of each process's interval of integration = local_n * h. So my interval starts at:
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	local_int = Trap(local_a, local_b, local_n, h);

	// Add up the integrals calculated by each process
	if (my_rank != 0) {
		printf("PE[%d] SEND: For %d trapezoids, local estimate = %f\n", my_rank, local_n, local_int);
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else {
		total_int = local_int;
		for (source = 1; source < comm_sz; source++) {
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("PE[%d] RECV from PE[%d]: local estimate=%f \n", my_rank, source, local_int);
			total_int += local_int;
		}
	}

	// Print the result
	if (my_rank == 0) {
		printf("With n = %d trapezoids, our estimate of the integral from %f to %f = %.15e\n", n, a, b, total_int);
	}

	MPI_Finalize();
	return 0;
}

// Function: 	Get_input
// Purpose:	Get the user input: the left and right endpoints
// 			and the number of trapezoids
// Input args	my_rank: process rank in MPI_COMM_WORLD
// 			comm_sz: number of processes in MPI_COMM_WORLD
// Output args a_p: pointer to left endpoints
// 			b_p: pointer to right endpoints
// 			n_p: pointer to number of trapezoids
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
	int dest;

	if (my_rank == 0) {
		printf("Enter a, b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for (dest = 1; dest < comm_sz; dest++) {
			MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
	} else { // rank != 0
		MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}




double Trap(
	double left_endpt,
	double right_endpt,
	int trap_count,
	double base_len
	) {

	double estimate, x;
	int i;

	estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
	for (i = 1; i <= trap_count; i++) {
		x = left_endpt + i * base_len;
		estimate += f(x);
	}

	estimate = estimate * base_len;

	return estimate;
}

double f(double x) {
	return x * x;
}