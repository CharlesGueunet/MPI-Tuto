cmake_minimum_required(VERSION 3.12)
project(MPI_comm VERSION 0.1 LANGUAGES C CXX)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "Build in sources is not supported by MPI_comm, please use a separate build folder")
endif()

find_package(MPI REQUIRED)

# NOTE: add this line only when you are using openmpi which has a different c++ bindings
# add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(comm communicator_mpi.cpp)
target_link_libraries(comm PRIVATE MPI::MPI_CXX)
