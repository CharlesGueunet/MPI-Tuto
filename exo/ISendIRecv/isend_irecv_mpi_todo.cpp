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

  const int tag = 1; // mark our transaction

  //  Process 0 expects up to 200 real values, from any source.
  if (rank == 0) {
    MPI_Request request;
    // TODO call MPI_Irecv here 

    // [processing ... ]
    std::cout << "recieveing ... " << std::endl;

    MPI_Status status;
    // TODO call a MPI_Wait here

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

    // We can keep a blocking send here if we want, both can be mixed
    // mpi_err = MPI_Send(send, 100, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);

    MPI_Request request;
    // TODO: call the MPI_Isend here

    // [processing ... ]
    std::cout << "sending ... " << std::endl;

    MPI_Status status;
    // TODO Call a MPI_Wait here (optional)
  } else {
    //  idle
  }

  mpi_err = MPI_Finalize();
  return EXIT_SUCCESS;
}
