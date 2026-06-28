cmake_minimum_required(VERSION 3.16)

project(QBFSolver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
  GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

add_executable(
  qbf_solver
  ${SOURCES}
)

target_include_directories(
  qbf_solver
  PRIVATE include
)

target_link_libraries(
  qbf_solver
  PRIVATE
  gmp
)
