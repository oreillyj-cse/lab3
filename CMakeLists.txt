cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  hello_test
  hello_test.cc
)

add_executable(
  map_test
  map_test.cc
  file_gen.h
  file_gen.cc
)

add_executable(
  pc
  pc.cc
)


add_executable(
  file_generator
  file_generator.cc
  file_gen.cc
  file_gen.h
)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

target_link_libraries(
  map_test
  GTest::gtest_main
)

target_link_libraries(
  pc
  pthread
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(map_test)
