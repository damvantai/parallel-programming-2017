#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

extern void draw_heat(int nx, int ny);

#define NXPROB		20	// X DIMENTION OF PROBLEM GIRD
#define NYPROB		20 	// Y DIMENTION OF PROBLEM GIRD
#define STEPS		100	// NUMBER OF TIME STEPS
#define MAXWORKER	8	// MAXIMUM NUMBER OF WORKER
#define MINWORKER	3	// MINIMUM NUMBER OF WORKER
#define BEGIN 		1
#define LTAG		2
#define RTAG		3
#define NONE		0
#define DONE		4
#define MASTER		0

struct Parms
{
	float cx;
	float cy;
} parms = {0.1, 0.1};

int main(int argc, char* argv[]) {
	void inidat();
	prtdat();
	update();

	float u[2][NXPROB][NYPROB];	// array for grid
	int taskid,
		numwokers,
		numtasks,
		averow, rows, offset, extra, // for sending rows of data
		dest, source,			// to -from for message send-receive
		left, right,			// neighbor tasks
		msgtype,				// for message types
		rc, start, end,			// misc
		i, ix, iy, iz, it;		//loop variables
	MPI_Status status;

	// First find out my taskid and how many tasks are running
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	numwokers = taskid - 1;

	if (taskid == MASTER) {
		/* master code */
		/* Check if numworkers is within range - quit if not */
		if ((numworkers > MAXWORKER) || (numworkers < MINWORKER)) {
			printf("Error: the number of tasks must be between %d and %d.\n", MINWORKER + 1, MAXWORKER + 1);
			printf("Quitting...\n");
			MPI_Abort(MPI_COMM_WORLD, rc);
			exit(1);
		}
		printf("Starting mpi_heat2D with %d worker tasks.\n", numworkers);

		// Initialize grid
		printf("Grid size: X = %d Y = %d Time steps = %d\n", NXPROB, NYPROB, STEPS);
		printf("Initializing grid and writing initial.dat file...\n");
		inidat(NXPROB, NYPROB, u);
		prtdat(NXPROB, NYPROB, u, "initial.dat");

		/* Distribute work to workers. Must first figure out how many rows to */
		/* send and what to do with extra rows. */
		averow = NXPROB / numworkers;
		extra = NXPROB % numworkers;
		offset = 0;

		for (i = 1; i <= numworkers; i++) {
			rows = (i <= extra) ? averow + 1: averow;
			/* Tell each worker who its neighbors are, since they must exchange */
			/* data with each other. */
			if (i == 1)
				left = None;
			else
				left = i - 1;
			if (i == numworkers)
				right = None;
			else
				right = i + 1;

			/* Now send startup information to each worker */
			dest = i;
			MPI_Send(&offset, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
			MPI_Send(&left, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
			MPI_Send(&right, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
			MPI_Send(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, dest, BEGIN, MPI_COMM_WORLD);
			printf("Sent to task %d: rows = %d offset = %d\n", dest, rows, offset);
			printf("left = %d, right = %d\n", left, right);
			offset += rows;
		}

		/* Now wait for results from all worker tasks */
		for (i = 1; i <= numworkers; i++) {
			source = i;
			msgtype = DONE;
			MPI_Recv(&offset, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, source, msgtype, MPI_COMM_WORLD, &status);
		}

		/* Write final output, call X graph and finalize MPI */
		printf("Writing final.dat file and generating graph...\n");
		prtdat(NXPROB, NYPROB, &u[0][0][0], "final.dat");
		printf("Click on More button to view initial/final states.\n");
		printf("Click on Exit button to quit program.\n");
		// draw_heat(NXPROB, NYPROB);
		MPI_Finalize();
	}

	/* worker code */
	if (taskid != MASTER) {
		/* Initializa everything - including the borders - to zero*/
		for (iz = 0; iz < 2; iz++)
			for (ix = 0; ix < NXPROB; ix++)
				for (iy = 0; iy < NYPROB; iy++)
					u[iz][ix][iy] = 0.0;

		/* Receive my offset, rows, neighbors and grid partition from master */
		source = MASTER;
		msgtype = BEGIN;
		MPI_Recv(&offset, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&left, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status)
		MPI_Recv(&right, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status)
		MPI_Recv(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, source, msgtype, MPI_COMM_WORLD, &status);

		/* Begin doing STEPS iterations. Must communicate border rows with */
		/* neighbors. If i have the first of last grid row, then i only need */
		/* to communicate with one neighbor */
		printf("Task %d received work. Beginning time steps...\n", taskid);
		iz = 0;
	}
}