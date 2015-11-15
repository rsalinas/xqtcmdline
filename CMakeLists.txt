project(xqtcmdline)
cmake_minimum_required (VERSION 2.8.11)
find_package(Qt5Core)

add_library(lib${PROJECT_NAME} xqtparser.cpp xqtparser.h)
include_directories(${Qt5Core_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

add_executable(xqtcmdline-test main.cpp)
target_link_libraries(xqtcmdline-test LINK_PUBLIC lib${PROJECT_NAME})
target_link_libraries(xqtcmdline-test Qt5::Core)
