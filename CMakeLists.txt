cmake_minimum_required(VERSION 3.23)
project(eit)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp vehicle.cpp)

set(HEADERS utility.h vehicle.h)

add_executable(eit ${SOURCES} ${HEADERS})
