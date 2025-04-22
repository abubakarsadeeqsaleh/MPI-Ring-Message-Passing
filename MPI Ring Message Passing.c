#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define MAX_MSG_LENGTH 100
int main(int argc, char *argv[]) {
    int rank, size;
    char message[MAX_MSG_LENGTH];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        fprintf(stderr, "This program requires at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    // Prepare the message to send
    sprintf(message, "Message from process %d", rank);
    if (rank == 0) {
        // Process 0 starts the ring by sending first
        MPI_Send(message, strlen(message)+1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent: %s\n", rank, message);
        // Then receive from the last process
        MPI_Recv(message, MAX_MSG_LENGTH, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received: %s\n", rank, message);
    } else {
        // Other processes receive first, then send
        MPI_Recv(message, MAX_MSG_LENGTH, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received: %s\n", rank, message); 
        // Prepare new message with current rank
        sprintf(message, "Message from process %d", rank);
        MPI_Send(message, strlen(message)+1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent: %s\n", rank, message);
    }
    MPI_Finalize();
    return 0;
}
