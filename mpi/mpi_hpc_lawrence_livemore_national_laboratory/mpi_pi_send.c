/*
DESCRIPTION:
	This program calculates pi using a "dartboard" algorithm
	All processes contribute to the calculation, with the mater averaging the value for pi.
	This version uses low level sends and receivers to collect results
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000 // number of throws at dartboard
#define ROUNDS 100	// number of times "darks" is iterated
#define MASTER 0	// task ID of master task

int main(int argc, char *argv[])
{
	double homepi,	// value of pi calculated by current task
	pi,				// average of pi after "darks" is thrown
	avepi,			// average pi value for all iterations
	pirecv,			// pi received from worker
	pisum;			// sum os workers pi values
	int taskid,		// task ID - also used as seed number
	numtasks,		// number of tasks
	source,			// source of imcomming message
	mtype,			// message type
	rc,				// return code
	i,
	n;

	MPI_Status status;

	// Obtain number of tasks and task ID
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	printf("MPI task %d has started...\n", taskid);

	// Set seed for random number generator equal to task ID
	srandom(taskid);

	avepi = 0;
	for (i = 0; i < ROUNDS; i++) {
		// All tasks calculate pi using dartboard algorithm
		homepi = dboard(DARTS);

		// Workers send homepi to master
		// Message type will be set to the iteration count
		if (taskid != MASTER) {
			mtype = i;
			rc = MPI_Send(&homepi, 1, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
		}
		else
		{
		/*
			Master receives messages from all workers
			Message type will be set to the iteration count
			Message source will be set to the wildcard DONTCARE:
			A message can be received from any task, as long as the message types match
			The return code will be checked, and a message displayed
			if a problem occurred
		*/

			mtype = i;
			pisum = 0;
			for (n = 1; n < numtasks; n++) {
				rc = MPI_Recv(&pirecv, 1, MPI_DOUBLE, MPI_ANY_SOURCE, mtype, MPI_COMM_WORLD, &status);
				// Keep running total of pi
				pisum = pisum + pirecv;
			}
		}
	}
}