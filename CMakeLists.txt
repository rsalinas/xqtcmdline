project(xqtcmdline)
cmake_minimum_required (VERSION 2.8.11)
find_package(Qt5Core 5.0.0 REQUIRED)

set(CMAKE_AUTOMOC ON)
add_library(lib${PROJECT_NAME} xqtparser.cpp xqtparser.h)
qt5_use_modules(lib${PROJECT_NAME} Core)
include_directories(${Qt5Core_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

add_executable(xqtcmdline-test main.cpp)
qt5_use_modules(xqtcmdline-test Core)
target_link_libraries(xqtcmdline-test Qt5::Core lib${PROJECT_NAME})
