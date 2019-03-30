set(CMAKE_CXX_FLAGS "-std=c++14")

# g++-8.3 has bug and will not compile constexpr function that throws.
# clang++-7, works properly in this regard.
set(CMAKE_CXX_COMPILER "clang++")

add_executable(units-test units-test.cpp)

# Have the units-test target depend on libunits.  Because libunits has an
# include directory specified along with the INTERFACE directive in the
# top-level 'CMakeLists.txt' file, specifying the dependency causes the
# compiler to be given the include directory on the build-command-linefor the
# target.
target_link_libraries(units-test libunits)
