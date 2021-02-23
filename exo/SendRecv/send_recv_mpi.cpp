#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <numeric>

int main(int argc, char *argv[]) {

  int mpi_err;

  float send[100];
  float recieve[200];

  mpi_err = MPI_Init(&argc, &argv);

  if (mpi_err != 0) {
    std::cerr << "init error" << std::endl;
    exit(EXIT_FAILURE);
  }

  int rank;
  mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //  Process 0 expects up to 200 real values, from any source.
  const int tag = 1;
  if (rank == 0) {
    MPI_Status status;
    mpi_err = MPI_Recv(recieve, 200, MPI_FLOAT, MPI_ANY_SOURCE, tag,
                       MPI_COMM_WORLD, &status);

    int count;
    mpi_err = MPI_Get_count(&status, MPI_FLOAT, &count);

    std::cout << "process 0 recieved " << count << " elements." << std::endl;
    for (int i = 0; i < 10; i++) {
      std::cout << "recieve[" << i << "] = " << recieve[i] << std::endl;
    }
    std::cout << "..." << std::endl;
  }
  //  Process 1 sends 100 real values to process 0.
  else if (rank == 1) {
    std::iota(send, send + 100, 0);
    const int dest = 0;
    mpi_err = MPI_Send(send, 100, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
  }
  else {
    //  idle
  }

  mpi_err = MPI_Finalize();
  return EXIT_SUCCESS;
}
