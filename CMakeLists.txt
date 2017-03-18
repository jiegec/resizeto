cmake_minimum_required (VERSION 2.8.11)
project (resizeto)

find_library (DEVIL_LIBRARY IL)
add_executable (resizeto resizeto.cpp)
target_link_libraries(resizeto ${DEVIL_LIBRARY})
