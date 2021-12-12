cmake_minimum_required(VERSION 3.0.0)
project(vertaler VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(vertaler main.cpp)

add_library(vertaler_lib SHARED token.cpp scanner.cpp)

target_link_libraries(vertaler vertaler_lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
