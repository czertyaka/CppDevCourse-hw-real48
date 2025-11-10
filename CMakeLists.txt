cmake_minimum_required(VERSION 3.30)
project(Real48 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_library(real48 real48.cpp)

option(WITH_TESTS OFF)
if (WITH_TESTS)
  enable_testing()
  add_executable(unit-tests unit-tests.cpp real48.cpp)
  target_compile_options(unit-tests PRIVATE -fsanitize=address,undefined)
  target_link_options(unit-tests PRIVATE -fsanitize=address,undefined)
  add_test(NAME unit-tests COMMAND unit-test)
endif ()
