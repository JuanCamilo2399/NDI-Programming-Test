cmake_minimum_required(VERSION 3.10)
project(NDIExampleFind)

set(CMAKE_CXX_STANDARD 17)
set(NDI_LIB_PATH "C:/Program Files/NDI/NDI 5 Advanced SDK")
link_directories(${NDI_LIB_PATH}/Lib)
include_directories(${NDI_LIB_PATH}/Include)
add_executable(NDIExampleFind NDIlib_Find.cpp)

if (WIN32)
    target_link_libraries(NDIExampleFind Processing.NDI.Lib.Advanced.x64.lib)
    target_link_libraries(NDIExampleFind Processing.NDI.Lib.Advanced.x86.lib)
endif()