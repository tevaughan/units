cmake_minimum_required(VERSION 3.13)
project(units)
add_subdirectory(test)
add_library(libunits INTERFACE) # "INTERFACE" here means no compiled sources.

# "INTERFACE" here seems to indicate that "-I${CMAKE_SOURCE_DIR}/include/" will
# be used for any target (such as test-executable) that depends on libunits.
target_include_directories(libunits INTERFACE include/)

# Copy tree of header files under ${CMAKE_SOURCE_DIR}/include into
# ${CMAKE_INSTALL_PREFIX}/include.
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h*")
