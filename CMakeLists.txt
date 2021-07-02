cmake_minimum_required(VERSION 3.17)
project(risc_V)

set(CMAKE_CXX_STANDARD 14)

add_executable(risc_V main.cpp CPUSimulator.h CPUSimulator.cpp order.h Process.cpp Process.h BranchPredictor.cpp BranchPredictor.h)