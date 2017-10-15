// programing parallel hust
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NRA 62
#define NCA 15
#define NCB 7
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[]) {
	int numtasks,	// number of tasks in partition
		taskid,		// a task identifier
		numworkers,	// number of worker tasks
		source,		// task id of message source
		dest,		// task id of mesage destination
		mtype,
		rows,
		averow,
		extra,
		offset,
		i, j, k, rc;	// result matrix C

	double a[NRA][NCA],
			b[NCA][NCB],
			c[NRA][NCB];

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	if (numtasks < 2) {
		printf("Need at least two MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}

	numworkers = numtasks - 1;

/********************** master task ***********************/

	if (taskid == MASTER) {
		printf("mpi_matrix_hust has started with %d task.\n", numtasks);
		printf("Initializing arrays....\n");
		for (i = 0; i < NRA; i++)
			for (j = 0; j < NCA; j++)
				a[i][j] = i + j;
		for (i = 0; i < NCA; i++)
			for (j = 0; j < NCB; j++)
				b[i][j] = j * i;

		mtype = FROM_MASTER;
		offset = 0;

		for (dest = 1; dest <= numworkers; dest++) {
			printf("Sending 1 rows to task %d offset = %d\n", dest, offset);
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset = offset + 1;
		}

		
		// MPI_Send(&c[62][0], NCB, MPI_DOUBLE, 62, mtype, MPI_COMM_WORLD);

		// Receive results from worker tasks
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++) {
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], NCB, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			printf("Received results from task %d\n", source);
		}

		for (k = 0; k < NCB; k++) {
			c[61][k] = 0.0;
			for (j = 0; j < NCA; j++) {
				c[61][k] = c[61][k] + a[61][j] * b[j][k];
			}
		}

		// Print results
		printf("Resuls Matrix:\n");
		for (i = 0; i < NRA; i++) {
			printf("\n");
			for (j = 0; j < NCB; j++) {
				printf("%6.2f    ", c[i][j]);
			}
		}
		printf("\n*********************************************\n");
		printf("Done\n");
	}
/**************************worker task****************************/
	if (taskid > MASTER) {
		mtype = FROM_MASTER;
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

		for (k = 0; k < NCB; k++) {
			for (i = 0; i < 1; i++) {
				c[i][k] = 0.0;
				for (j = 0; j < NCA; j++) {
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
				}
			}
		}

		mtype = FROM_WORKER;
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&c, NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}