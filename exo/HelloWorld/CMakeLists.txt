cmake_minimum_required(VERSION 3.12)
project(MPI_hello VERSION 0.1 LANGUAGES C)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "Build in sources is not supported by MPI_hello, please use a separate build folder")
endif()

find_package(MPI REQUIRED)

# NOTE: add this line only when you are using openmpi which has a different c++ bindings
# add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(hello mpi_hello_world.c)
target_link_libraries(hello PRIVATE MPI::MPI_C)
