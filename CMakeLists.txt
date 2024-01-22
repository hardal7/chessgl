cmake_minimum_required(VERSION 3.5)
project(chessgl)
add_executable(chessgl ../src/main.cpp)
target_link_libraries(chessgl ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw.so ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglad.so)
