cmake_minimum_required(VERSION 3.1...3.14)

project(Tundra VERSION 0.1)

target_compile_features(Tundra PUBLIC cxx_std_17)
set(TARGET_Tundra "${PROJECT_NAME}_${CMAKE_GENERATOR_PLATFORM}")

add_executable(TARGET_Tundra)
