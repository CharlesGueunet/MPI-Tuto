cmake_minimum_required(VERSION 3.12)
project(MPI_send_recv VERSION 0.1 LANGUAGES C CXX)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "Build in sources is not supported by MPI_send_recv, please use a separate build folder")
endif()

find_package(MPI REQUIRED)

# NOTE: add this line only when you are using openmpi which has a different c++ bindings
# add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(send_recv send_recv_mpi.cpp)
target_link_libraries(send_recv PRIVATE MPI::MPI_CXX)
