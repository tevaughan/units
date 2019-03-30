cmake_minimum_required(VERSION 3.13)
project(units)
add_subdirectory(test)

# The INTERFACE keyword indicates here that there are no compiled source files
# for a particular library.
add_library(libunits INTERFACE)

# The INTERFACE keyword indicates here that an include directory will
# automatically be applied to any target that depends on a particular library.
target_include_directories(libunits INTERFACE include/)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h*")
