#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){

    // Reading command line input
    int rankToPrint = 0;
    if(argc == 2){
        rankToPrint = atoi(argv[1]);
    }

    // Number of processes, rank of process
    int size, rank;
    // Grid dimensions
    int dims[2] = {3,3};
    // Grid periodicity
    int periods[2] = {0,0};
    // Grid coordinates of current process 
    int coords[2];
    // Ranks of neighbours
    int up,down,left,right;
    // Cartesian communicator
    MPI_Comm cart_comm;

    MPI_Init(&argc, &argv);

    //Creating cartesian communicator
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    // Finding size and rank
    MPI_Comm_size(cart_comm, &size);
    MPI_Comm_rank(cart_comm, &rank);

    // Finding grid coordinates of current process
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    // Finding neighbours
    MPI_Cart_shift(cart_comm, 0, 1, &up, &down);
    MPI_Cart_shift(cart_comm, 1, 1, &left, &right);


    // Printing info for one process
    if(rank == rankToPrint){
        printf(" 0 1 2\n 3 4 5\n 6 7 8\n\n");
        printf("Rank: %d\n", rank);
        printf("Coordinates: %d,%d\n\n", coords[0], coords[1]);
        printf("Neighbours: \n");
        printf("  %2d  \n%2d  %2d\n  %2d  \n", up, left, right, down);
        printf("\n");
    }

    // Initializing variables to hold received messages
    int fromUp = -1, fromDown = -1, fromLeft = -1, fromRight = -1;

    MPI_Request req[4];

    // Non-blocking receives from all four neighbours
    MPI_Irecv(&fromLeft, 1, MPI_INT, left, 0, cart_comm, req);
    MPI_Irecv(&fromDown, 1, MPI_INT, down, 0, cart_comm, req + 1);
    MPI_Irecv(&fromRight, 1, MPI_INT, right, 0, cart_comm, req + 2);
    MPI_Irecv(&fromUp, 1, MPI_INT, up, 0, cart_comm, req + 3);

    // Blocking sends to all four neighbours
    MPI_Send(&rank, 1, MPI_INT, up, 0, cart_comm);
    MPI_Send(&rank, 1, MPI_INT, down, 0, cart_comm);
    MPI_Send(&rank, 1, MPI_INT, left, 0, cart_comm);
    MPI_Send(&rank, 1, MPI_INT, right, 0, cart_comm);

    // Waiting for receives to complete
    MPI_Waitall(4, req, MPI_STATUSES_IGNORE);

    // Printing received messages for one process
    if(rank == rankToPrint){
        printf("Received messages\n");
        printf("  %2d  \n%2d  %2d\n  %2d  \n", fromUp, fromLeft, fromRight, fromDown);

    }

    MPI_Finalize();
}
