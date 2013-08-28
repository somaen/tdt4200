#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int size, rank, in;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Initial value
	int out = 0;
	// The next target is the next rank
	int targetRank = rank + 1;
	// We receive from the rank right below us
	int sourceRank = rank - 1;

	// Special case rank 0, as it has to send first. (Otherwise we will deadlock)
	if (rank == 0) {
		sourceRank = size - 1; // Rank 0 receives from the last process (0-indexed)
		// Send initial value (0)
		MPI_Send(&out, 1, MPI_INT, targetRank, 1, MPI_COMM_WORLD);
		// Get the final result from the last node.
		MPI_Recv(&in, 1, MPI_INT, sourceRank, 1, MPI_COMM_WORLD, &status);
		printf("Rank %d received %d from rank %d\n", rank, in, sourceRank);
	} else {
		if (targetRank == size) { // The last node must wrap to target node 0
			targetRank = 0;
		}
		// Receive first, as we need the result from the lower rank to calculate what to send.
		MPI_Recv(&in, 1, MPI_INT, sourceRank, 1, MPI_COMM_WORLD, &status);
		out = in + rank;
		// Send it to the next-rank (or potentially rank 0, if we are the last node)
		MPI_Send(&out, 1, MPI_INT, targetRank, 1, MPI_COMM_WORLD);
		printf("Rank %d received %d from rank %d and sent %d to rank %d\n", rank, in, sourceRank, out, targetRank);
	}
	MPI_Finalize();
}
