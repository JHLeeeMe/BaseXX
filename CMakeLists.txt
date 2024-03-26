cmake_minimum_required(VERSION 3.15)
project("BaseXX")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add sub dir
add_subdirectory("test/")

message(STATUS "CMake Version:          ${CMAKE_VERSION}")
message(STATUS "Project:                ${PROJECT_NAME}")
message(STATUS "Generator:              ${CMAKE_GENERATOR}")
message(STATUS "Bulid for OS type:      ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build for OS version:   ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Build for CPU type:     ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Build type:             ${CMAKE_BUILD_TYPE}")
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "C flags:                ${CMAKE_C_FLAGS_${BUILD_TYPE}} ${CMAKE_C_FLAGS}")
message(STATUS "CXX flags:              ${CMAKE_CXX_FLAGS_${BUILD_TYPE}} ${CMAKE_CXX_FLAGS}")
message(STATUS "Build output path:      ${CMAKE_BINARY_DIR}")
message(STATUS "Install path:           ${CMAKE_INSTALL_PREFIX}")
