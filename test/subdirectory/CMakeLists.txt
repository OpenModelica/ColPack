# Builds a program that uses ColPack as a subdirectory, the way OpenModelica
# does. Run from the ColPack root:
#   cmake -S test/subdirectory -B build-subdirectory
cmake_minimum_required(VERSION 3.12)
project(ColPackSubdirectoryTest LANGUAGES CXX)

enable_testing()

set(COLPACK_BUILD_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(../.. ColPack)

add_executable(subdirectory_test main.cpp)
target_link_libraries(subdirectory_test PRIVATE ColPack_static)
add_test(NAME subdirectory_test COMMAND subdirectory_test)
