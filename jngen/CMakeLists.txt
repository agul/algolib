cmake_minimum_required(VERSION 2.8.4)

add_library(jngen jngen.cpp)
target_compile_options(jngen PRIVATE -Werror -std=c++1z)
