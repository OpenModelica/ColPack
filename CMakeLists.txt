cmake_minimum_required(VERSION 3.12)
project(ColPack VERSION 1.1.0 LANGUAGES CXX)

# Set the version number to the *next* release (not to the prevoius release).
set(COLPACK_VERSION 1.1.0)
set(COLPACK_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# CMake support was added by chrisdembia in 2017 to support building ColPack on
# Windows with Visual Studio and to supplement autotools. The autotools build
# has since been removed, CMake is the only supported build system.
# We install CMake config files (ColPackConfig.cmake) to support use of ColPack
# by downstream projects. The name of the lib directory is based on CMake's
# GNUInstallDirs module.
# The Basic examples are built and used as tests via CTest if
# COLPACK_ENABLE_TESTS is ON.
# The non-Basic examples are built if COLPACK_ENABLE_EXAMPLES is ON.
# The only example that we do not compile is SMB.
#
# On Windows the examples are linked to the static library rather than to the
# shared library.

# User options.
# -------------
option(
    COLPACK_ENABLE_TESTS
    "Build the ColPack executable and the Basic examples and run them as tests."
    ON
)
option(COLPACK_BUILD_SHARED "Build the shared library ColPack_shared." ON)
option(
    COLPACK_ENABLE_EXAMPLES
    "Build the examples in SampleDrivers/Matrix_Compression_and_Recovery."
    OFF
)
option(COLPACK_ENABLE_OPENMP "Enable OpenMP." ON)
set(CMAKE_CXX_STANDARD 11)

# Configuring variables, etc.
# ---------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    # MSVC and Xcode are mult-config generators that use
    # CMAKE_CONFIGURATION_TYPES; other generators (Makefiles) use
    # CMAKE_BUILD_TYPE.
    # By default, compile with optimizations and without debug symbols.
    set(CMAKE_BUILD_TYPE
        Release
        CACHE STRING
        "Choose the type of build, e.g., optimizaiton level, debug symbols."
        FORCE
    )
    # Show a list of options in the CMake-GUI or ccmake (curses interface).
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
    )
endif()

# ColPack_lib_to_use is either ColPack_shared or ColPack_static.
if(COLPACK_BUILD_SHARED)
    set(ColPack_lib_to_use ColPack_shared)
else()
    set(ColPack_lib_to_use ColPack_static)
endif()

if(WIN32)
    # Since we do not use __declspec(dllexport) etc, the shared library
    # does not export symbols and cannot be used in general. Use the the static
    # library for the examples.
    set(ColPack_lib_to_use ColPack_static)
    if(${CMAKE_VERSION} VERSION_LESS 3.4)
        message(
            STATUS
            "Cannot use ColPack's shared library on Windows if using CMake older than 3.4 (because no symbols are exported)."
        )
    else()
        # CMake can export symbols for us, allowing use of the shared library.
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
    # Some examples have file names of almost 100 characters. With the FULL
    # strategy their object paths (CMakeFiles/<target>.dir/.../<source>.obj)
    # exceed MAX_PATH and the build fails. SHORT uses hashed intermediate
    # paths; it needs CMake 4.2 and is ignored by older versions.
    set(CMAKE_INTERMEDIATE_DIR_STRATEGY
        SHORT
        CACHE STRING
        "Strategy for target intermediate directories (FULL or SHORT)."
    )
endif()

if(COLPACK_ENABLE_OPENMP)
    # MSVC's default OpenMP 2.0 lacks unsigned loop indices. Requires CMake 3.30.
    if(MSVC AND NOT DEFINED OpenMP_RUNTIME_MSVC)
        set(OpenMP_RUNTIME_MSVC llvm)
    endif()
    find_package(OpenMP REQUIRED COMPONENTS CXX)
endif()

# Define variables to use for organizing the installation.
include(GNUInstallDirs)

set(COLPACK_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ColPack)
set(COLPACK_INSTALL_HEADERSDIR ${CMAKE_INSTALL_INCLUDEDIR}/ColPack)

# Lists of the source files.
# -------------------------
file(
    GLOB HEADERS
    ${COLPACK_ROOT_DIR}/inc/ColPackHeaders.h
    ${COLPACK_ROOT_DIR}/inc/Definitions.h
    ${COLPACK_ROOT_DIR}/src/Utilities/*.h
    ${COLPACK_ROOT_DIR}/src/BipartiteGraphBicoloring/*.h
    ${COLPACK_ROOT_DIR}/src/BipartiteGraphPartialColoring/*.h
    ${COLPACK_ROOT_DIR}/src/GeneralGraphColoring/*.h
    ${COLPACK_ROOT_DIR}/src/Recovery/*.h
    #${COLPACK_ROOT_DIR}/src/PartialD2SMPGC/*.h
)
file(
    GLOB SOURCES
    ${COLPACK_ROOT_DIR}/src/Utilities/*.cpp
    ${COLPACK_ROOT_DIR}/src/BipartiteGraphBicoloring/*.cpp
    ${COLPACK_ROOT_DIR}/src/BipartiteGraphPartialColoring/*.cpp
    ${COLPACK_ROOT_DIR}/src/GeneralGraphColoring/*.cpp
    ${COLPACK_ROOT_DIR}/src/Recovery/*.cpp
    #${COLPACK_ROOT_DIR}/src/PartialD2SMPGC/*.cpp
)

# Use these include directories when building the ColPack library.
set(COLPACK_SOURCE_INCLUDE_DIRS
    "${COLPACK_ROOT_DIR}/inc"
    "${COLPACK_ROOT_DIR}/src/Utilities"
    "${COLPACK_ROOT_DIR}/src/BipartiteGraphPartialColoring"
    "${COLPACK_ROOT_DIR}/src/BipartiteGraphBicoloring"
    "${COLPACK_ROOT_DIR}/src/GeneralGraphColoring"
    "${COLPACK_ROOT_DIR}/src/Recovery"
)

# SMPGC is the OpenMP-parallel coloring and includes <omp.h> unconditionally.
# The rest of ColPack only uses OpenMP when _OPENMP is defined.
if(COLPACK_ENABLE_OPENMP)
    file(GLOB SMPGC_HEADERS ${COLPACK_ROOT_DIR}/src/SMPGC/*.h)
    file(GLOB SMPGC_SOURCES ${COLPACK_ROOT_DIR}/src/SMPGC/*.cpp)
    list(APPEND HEADERS ${SMPGC_HEADERS})
    list(APPEND SOURCES ${SMPGC_SOURCES})
    list(APPEND COLPACK_SOURCE_INCLUDE_DIRS "${COLPACK_ROOT_DIR}/src/SMPGC")
endif()

# Static library.
# ---------------
add_library(ColPack_static STATIC ${HEADERS} ${SOURCES})
if(NOT WIN32 OR NOT COLPACK_BUILD_SHARED)
    # On Windows, the static and shared libraries can't have the same
    # name (because both have .lib files that will conflict).
    set_target_properties(ColPack_static PROPERTIES OUTPUT_NAME ColPack)
endif()

target_include_directories(
    ColPack_static
    PRIVATE ${COLPACK_SOURCE_INCLUDE_DIRS}
)
# For clients of the ColPack target within this project or a project that adds
# ColPack with add_subdirectory().
# (Must use quotes because the variable is a list of paths.)
target_include_directories(
    ColPack_static
    INTERFACE "$<BUILD_INTERFACE:${COLPACK_SOURCE_INCLUDE_DIRS}>"
)
# For clients of an installation of ColPack.
target_include_directories(
    ColPack_static
    INTERFACE $<INSTALL_INTERFACE:${COLPACK_INSTALL_HEADERSDIR}>
)

# The SMPGC headers include <omp.h> unconditionally, so OpenMP is PUBLIC:
# OpenMP::OpenMP_CXX carries the compile flags, the include directories and
# the runtime library. Passing only OpenMP_CXX_FLAGS misses the latter two,
# which breaks on macOS where Apple clang needs libomp from Homebrew.
if(COLPACK_ENABLE_OPENMP)
    target_link_libraries(ColPack_static PUBLIC OpenMP::OpenMP_CXX)
endif()

# "EXPORT" helps with creating a ColPackConfig.cmake file to place in the
# installation, to help downstream projects use ColPack.
install(
    TARGETS ColPack_static
    EXPORT ColPackTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Shared library.
# ---------------
if(COLPACK_BUILD_SHARED)
    add_library(ColPack_shared SHARED ${HEADERS} ${SOURCES})
    set_target_properties(
        ColPack_shared
        PROPERTIES OUTPUT_NAME ColPack SOVERSION 0
    )

    # Use these include directories when building the ColPack library.
    target_include_directories(
        ColPack_shared
        PRIVATE ${COLPACK_SOURCE_INCLUDE_DIRS}
    )
    # For clients of the ColPack target within this project or a project that
    # adds ColPack with add_subdirectory().
    target_include_directories(
        ColPack_shared
        INTERFACE "$<BUILD_INTERFACE:${COLPACK_SOURCE_INCLUDE_DIRS}>"
    )
    # For clients of an installation of ColPack.
    target_include_directories(
        ColPack_shared
        INTERFACE $<INSTALL_INTERFACE:${COLPACK_INSTALL_HEADERSDIR}>
    )

    if(COLPACK_ENABLE_OPENMP)
        target_link_libraries(ColPack_shared PUBLIC OpenMP::OpenMP_CXX)
    endif()

    install(
        TARGETS ColPack_shared
        EXPORT ColPackTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(FILES ${HEADERS} DESTINATION ${COLPACK_INSTALL_HEADERSDIR})

if(COLPACK_ENABLE_TESTS OR COLPACK_ENABLE_EXAMPLES)
    # -------------------
    # The ColPack executable depends on getopt.h, which is POSIX
    # and not readily available when using Visual Studio.
    # -------------------
    # xin cheng: on Oct 2018. It seems getopt.h havn't been used. So the following check getopt
    #            could be removed. For safety, still pop up a message.
    find_path(HAS_GETOPT getopt.h)
    if(NOT HAS_GETOPT)
        message(STATUS "Cannot find getopt.h; ColPack may or may not build.")
    endif()

    # ColPack executable.
    add_executable(ColPack "${COLPACK_ROOT_DIR}/Examples/ColPackAll/Main.cpp")
    target_link_libraries(ColPack ${ColPack_lib_to_use})
    target_include_directories(ColPack PRIVATE ${COLPACK_SOURCE_INCLUDE_DIRS})

    # To facilitate running the ColPack executable from the build directory.
    file(
        COPY "${COLPACK_ROOT_DIR}/Graphs"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
    )
endif()

# Examples.
# ---------
# First, gather up all the example files.
set(EXAMPLES)
if(COLPACK_ENABLE_TESTS)
    enable_testing()
    file(
        GLOB SAMPLE_DRIVERS_BASIC
        RELATIVE "${COLPACK_ROOT_DIR}/Examples/SampleDrivers"
        ${COLPACK_ROOT_DIR}/Examples/SampleDrivers/Basic/*.cpp
    )
    list(APPEND EXAMPLES ${SAMPLE_DRIVERS_BASIC})
endif()
if(COLPACK_ENABLE_EXAMPLES)
    file(
        GLOB SAMPLE_DRIVERS_MATRIX_COMPRESSION_AND_RECOVERY
        RELATIVE "${COLPACK_ROOT_DIR}/Examples/SampleDrivers"
        ${COLPACK_ROOT_DIR}/Examples/SampleDrivers/Matrix_Compression_and_Recovery/ADIC/*.cpp
        ${COLPACK_ROOT_DIR}/Examples/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/*.cpp
        ${COLPACK_ROOT_DIR}/Examples/SampleDrivers/Matrix_Compression_and_Recovery/CSR_input/*.cpp
        # Leaving out SMB because that depends on ADOL-C.
    )
    list(APPEND EXAMPLES ${SAMPLE_DRIVERS_MATRIX_COMPRESSION_AND_RECOVERY})
endif()

# Now create targets for each example.
foreach(example_cpp ${EXAMPLES})
    # Get the file name without extension ("_WE"), and store it in `example`.
    get_filename_component(example ${example_cpp} NAME_WE)
    # Get a directory relative to the SampleDrivers directory.
    get_filename_component(
        example_rel_dir
        ${example_cpp}
        DIRECTORY
        BASE_DIR SampleDrivers
    )
    add_executable(
        ${example}
        ${COLPACK_ROOT_DIR}/Examples/SampleDrivers/${example_cpp}
    )
    target_link_libraries(${example} ${ColPack_lib_to_use})
    target_include_directories(
        ${example}
        PRIVATE ${COLPACK_SOURCE_INCLUDE_DIRS}
    )
    # Allow running the basic examples as tests.
    if(${example_rel_dir} STREQUAL Basic)
        # The examples read their input from ./Graphs, which is copied
        # into the build directory above.
        add_test(
            NAME ${example}
            COMMAND ${example}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()
endforeach()

# Install and support for downstream projects.
# --------------------------------------------
# Consider installing Graphs:
# install(DIRECTORY Graphs DESTINATION ${CMAKE_INSTALL_SHAREDIR})

# Create files that define the ColPack targets to import into other projects.
# https://cmake.org/cmake/help/v3.1/manual/cmake-packages.7.html
install(
    EXPORT ColPackTargets
    NAMESPACE ColPack::
    DESTINATION ${COLPACK_INSTALL_CMAKEDIR}
)

# CMake Config file., and version file, which CMake will look for when
# downstream projects use find_package(ColPack).
include(CMakePackageConfigHelpers)
# The ConfigVersion file determines if an installed copy of ColPack is
# compatible with a user's requested version for ColPack.
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ColPackConfigVersion.cmake"
    VERSION ${COLPACK_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    ColPackConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ColPackConfig.cmake"
    INSTALL_DESTINATION ${COLPACK_INSTALL_CMAKEDIR}
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ColPackConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ColPackConfigVersion.cmake"
    DESTINATION ${COLPACK_INSTALL_CMAKEDIR}
)
