#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int num_elements = 100;

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int i;
  int *data = (int *)malloc(sizeof(int) * num_elements);
  assert(data != NULL);

  // Time MPI_Bcast
  double total_mpi_bcast_time = 0;

  MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  total_mpi_bcast_time += MPI_Wtime();

  // Print off timing information
  if (world_rank == 0) {
    printf("Data size = %d\n", num_elements * (int)sizeof(int));
    printf("MPI_Bcast time = %lf\n", total_mpi_bcast_time);
  }

  free(data);
  MPI_Finalize();
}
