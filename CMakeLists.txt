set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -Wformat=2")

file(GLOB_RECURSE HEADERS "cpplib/*.hpp")

add_library(cpplib INTERFACE)
target_include_directories(cpplib INTERFACE .)
target_sources(cpplib INTERFACE ${HEADERS})


file(GLOB_RECURSE TESTS "tests/*.?pp")
add_executable(unit_tests ${TESTS})
target_link_libraries(unit_tests cpplib)

add_subdirectory(gtest)
target_link_libraries(unit_tests gtest_main)
target_compile_options(unit_tests PRIVATE -Werror -std=c++1z)
