cmake_minimum_required(VERSION 3.28)
project(BlursedECS LANGUAGES CXX)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(BlursedECS ${SRC})
target_compile_features(BlursedECS PRIVATE cxx_std_20)
