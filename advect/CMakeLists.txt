cmake_minimum_required(VERSION 2.8)
project(advect)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

# for optimze
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories(include)
file(GLOB SOURCE_FILES src/*.C)
link_directories(/group/dpa/local/openvdb/lib)

add_executable(bunnyFile bunnyFile.C ${SOURCE_FILES})
target_link_libraries(bunnyFile Half tbb openvdb OpenImageIO)
target_include_directories(bunnyFile PUBLIC /group/dpa/local/openvdb/include)

add_executable(ears ears.C ${SOURCE_FILES})
target_link_libraries(ears Half tbb openvdb OpenImageIO)
target_include_directories(ears PUBLIC /group/dpa/local/openvdb/include)

add_executable(cumulo cumulo.C ${SOURCE_FILES})
target_link_libraries(cumulo Half tbb openvdb OpenImageIO)
target_include_directories(cumulo PUBLIC /group/dpa/local/openvdb/include)

# add_executable(test test.C)
# target_link_libraries(test Half tbb openvdb OpenImageIO)
# target_include_directories(test PUBLIC /group/dpa/local/openvdb/include)
