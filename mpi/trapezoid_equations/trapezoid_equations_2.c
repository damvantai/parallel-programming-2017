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
	int my_rank, comm_sz, local_n, source;
	double a, b, h, local_a, local_b, local_int, total_int;

	// Let the system do what it needs to start up MPI
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_COMM_size(MPI_COMM_WORLD, &comm_sz);

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
	}
}