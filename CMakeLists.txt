#
# SCardGetAttribEval CMakeLists.txt
#

cmake_minimum_required(VERSION 3.30)
project(SCardGetAttribEval CXX C)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_executable(SCardGetAttribEval SCardGetAttribEval.cpp)
