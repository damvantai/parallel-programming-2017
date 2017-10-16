// File: mpi_trap1.c
// Purpose: Use MPI to implement a parallel version of the trapezoidal rule.
// 	This version uses collective communications and MPI derived datatupes
// 	to distribute the input data and compute the global sum.

// Input: The endpoints of the interval of integration and the number of trapezoids
// Output: Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.

// Compile: mpicc -g -Wall -o mpi_trap1 mpi_trap1.c
// Run: mpirun -np <number of processes> ./mpi_trap1

// Algorithm:
// 	1.	Each process calculates 'its' interval of integration
// 	2.	Each process Estimate the integral of f(x) over its interval using the trapezoidal rule.
// 	3a.	Each process != 0 sends its integral to 0.
// 	3b. Process 0 sums the calculations received from the individual processes and prints the result.


// Note:	f(x) is all hardwired.
// IPP: Section 3.2.2 (PP.96 and ff.)

#include <stdio.h>
#include <mpi.h>

// Calculate local integral
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

// Function we're integrating
double f(double x);

int main() {
	int my_rank, comm_sz, n = 1024, local_n;
	double a = 4.0, b = 4.001, h, local_a, local_b;
	double local_int, total_int;
	int source;

	// Let the system do what it needs to start up MPI
	MPI_Init(NULL, NULL);

	// Get my process rank
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	h = (b-a) / n;	// h is the same for all processes
	local_n = n / comm_sz;	// So is the number of trapezoids / process

	// length of each process' interval of integration = local_n * h
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	local_int = Trap(local_a, local_b, local_n, h);

	// Add up the integrals calculated by each process
	if (my_rank != 0) {
		MPI_Send(&local_int, 1 ,MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	} else {
		total_int = local_int;
		for (source = 1; source < comm_sz; source++) {
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}

	// Print the result
	if (my_rank == 0) {
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15f\n", a, b, total_int);
	}
	MPI_Finalize();	// Shut down MPI
}


// Function:	Trap
// Purpose:	Serial function for estimating a definite integral using the trapezoidal rule
// Input args:	left_endpt
// 			right_endpt
// 			trap_count
// 			base_len
// Return val:	trapezoidal rule estimate of integral from
// 			left_endpt to right_endpt using trap_count
// 			trapezoids
double Trap(
	double left_endpt,
	double right_endpt,
	int trap_count,
	double base_len
	) {
	double estimate, x;
	int i;

	estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
	for (i = 1; i <= trap_count - 1; i++) {
		x = left_endpt + i * base_len;
		estimate += f(x);
	}
	estimate = estimate * base_len;

	return estimate;
}

// Fucntion:	f
// Purpose:	Compute value of function to be integrated
// Input args: x

double f(double x) {
	return x * x;
}