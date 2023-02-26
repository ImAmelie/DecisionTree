cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(demo src/main.cpp src/raisin.cpp src/wdbc.cpp src/iris.cpp src/wine.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
