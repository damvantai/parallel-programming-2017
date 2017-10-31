#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NRA 62
#define NCA 15
#define NCB 7
#define MASTER 		0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[]) {
	int numtasks,
		taskid,
		numworkers,
		source, 
		dest,
		mtype,
		rows,
		averow,
		extra,
		offset,
		i,
		j,
		k,
		rc;
	double a[NRA][NCA],
			b[NCA][NCB],
			c[NRA][NCB];
	MPI_Status status;

	// numworkers = 4;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	if (numtasks < 2) {
		printf("Need at least two MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	numworkers = numtasks - 1;

	/************************** master task ********************************/
	if (taskid == MASTER) {
		printf("mpi_mm has started with %d tasks.\n", numtasks);
		printf("Initializing arrays...\n");
		for (i = 0; i < NRA; i++) 
			for (j = 0; j < NCA; j++) 
				a[i][j] = i + j;
		for (i = 0; i < NCA; i++)
			for (j = 0; j < NCB; j++) 
				b[i][j] = i * j;

		// Send matrix data to the worker tasks
		averow = NRA/numworkers;	// 62/4=15
		extra = NRA%numworkers;		// 62%4=2
		offset = 0;
		mtype = FROM_MASTER;
		for (dest = 1; dest <= numworkers; dest++) {
			rows = (dest <= extra) ? averow+1 : averow;
			printf("Sending %d rows to task %d offset=%d\n", rows, dest, offset);
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, MPI_COMM_WORLD);
			MPI_Send(&b, NCA * NCB, MPI_DOUBLE, dest, type, MPI_COMM_WORLD);
			offset = offset + rows;
		}

		// receive results from worker tasks
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++) {
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows*NCB; MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			printf("Received results from task %d\n", source);

		}

		// Print results
		printf("Result Matrix:\n");
		for (i = 0; i < NRA; i++) {
			printf("\n");
			for (j = 0; j < NCB; j++) {
				printf("%6.2f\t", c[i][j]);
			}
		}
		printf("\n************************************\n");
		printf("Done.\n");
	}
}