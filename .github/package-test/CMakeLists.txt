# Downstream project that checks a ColPack release package: it finds the
# installed CMake package and links a small program against both libraries.
cmake_minimum_required(VERSION 3.12)
project(ColPackPackageTest LANGUAGES CXX)

# COLPACK_EXPECTED_VERSION is set by the release workflow.
find_package(ColPack ${COLPACK_EXPECTED_VERSION} EXACT REQUIRED CONFIG)
message(STATUS "Found ColPack ${ColPack_VERSION} in ${ColPack_DIR}")

enable_testing()
foreach(lib static shared)
    add_executable(package_test_${lib} main.cpp)
    target_link_libraries(package_test_${lib} PRIVATE ColPack::ColPack_${lib})
    add_test(NAME package_test_${lib} COMMAND package_test_${lib})
endforeach()
