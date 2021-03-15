cmake_minimum_required(VERSION 3.12)
project(MPI_bcast VERSION 0.1 LANGUAGES C CXX)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "Build in sources is not supported by MPI_bcast, please use a separate build folder")
endif()

find_package(MPI REQUIRED)

add_executable(bcast bcast_mpi.c)
target_link_libraries(bcast PRIVATE MPI::MPI_C)

add_executable(bcast_todo bcast_mpi_todo.c)
target_link_libraries(bcast_todo PRIVATE MPI::MPI_C)
