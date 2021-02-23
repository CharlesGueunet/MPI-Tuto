#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])

{
  int mpi_err; // error code

  mpi_err = MPI_Init(&argc, &argv);

  if (mpi_err != 0) {
    std::cerr << "init error" << std::endl;
    exit(EXIT_FAILURE);
  }

  int nb_process;
  mpi_err = MPI_Comm_size(MPI_COMM_WORLD, &nb_process);

  int pid;
  mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  std::cout << "hello from process " << pid << std::endl;

  //  Get a group identifier for MPI_COMM_WORLD.
  MPI_Group world_group_id;
  MPI_Comm_group(MPI_COMM_WORLD, &world_group_id);

  // create group of even processes
  int nb_even = (nb_process + 1) / 2;
  int *even_rank = new int[nb_even];
  for (int i = 0; i < nb_even; i++) {
    even_rank[i] = 2 * i;
  }
  MPI_Group even_group_id;
  int even_id;
  int even_id_sum;
  MPI_Group_incl(world_group_id, nb_even, even_rank, &even_group_id);

  MPI_Comm even_comm_id;
  MPI_Comm_create(MPI_COMM_WORLD, even_group_id, &even_comm_id);

  // creat a group of odd processes
  int nb_odd = nb_process / 2;
  int *odd_rank = new int[nb_odd];
  for (int i = 0; i < nb_odd; i++) {
    odd_rank[i] = 2 * i + 1;
  }
  MPI_Group odd_group_id;
  int odd_id;
  int odd_id_sum;
  MPI_Group_incl(world_group_id, nb_odd, odd_rank, &odd_group_id);

  MPI_Comm odd_comm_id;
  MPI_Comm_create(MPI_COMM_WORLD, odd_group_id, &odd_comm_id);

  // retrieve the id of each process
  if (pid % 2 == 0) {
    mpi_err = MPI_Comm_rank(even_comm_id, &even_id);
    odd_id = -1;
  } else {
    mpi_err = MPI_Comm_rank(odd_comm_id, &odd_id);
    even_id = -1;
  }

  // reduce: sum the ids of all even processess
  if (even_id != -1) {
    MPI_Reduce(&pid, &even_id_sum, 1, MPI_INT, MPI_SUM, 0, even_comm_id);
  }
  if (even_id == 0) {
    std::cout << "Number of processes in even: " << nb_even << std::endl;
    std::cout << "Sum of ids in even: " << even_id_sum << std::endl;
  }

  // reduce: sum the ids of all odd processess
  if (odd_id != -1) {
    MPI_Reduce(&pid, &odd_id_sum, 1, MPI_INT, MPI_SUM, 0, odd_comm_id);
  }
  if (odd_id == 0) {
    std::cout << "Number of processes in odd: " << nb_odd << std::endl;
    std::cout << "Sum of ids in odd: " << odd_id_sum << std::endl;
  }

  mpi_err = MPI_Finalize();
  return 0;
}
