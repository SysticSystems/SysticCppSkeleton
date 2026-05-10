cmake_minimum_required(VERSION 3.25)
project(SysticSkeleton C CXX)

# Enforce Handshake Requirements: C++23 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enforce Clang requirement per protocol
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "[!] Protocol Violation: Systic Skeleton requires Clang (Requested 21). Found: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Find dependencies provided by Conan (CMakeDeps generator)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)


# Common Includes
include_directories(include)

# ---------------------------------------------------------
# Clang-Tidy Configuration (Optional)
# ---------------------------------------------------------
find_program(CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-18 clang-tidy-17)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -checks=*,-fuchsia-*,-llvmlibc-* --warnings-as-errors=*)
    message(STATUS "Clang-Tidy found at ${CLANG_TIDY_EXE} - Static analysis enabled.")
endif()

# ---------------------------------------------------------
# Core Library Target (src / include)
# ---------------------------------------------------------
# We compile the core logic as a library so other components (tests, bench, apps) can consume it.
add_library(systic_core STATIC src/core.cpp)
target_include_directories(systic_core PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------
# Testing Target (tests/)
# ---------------------------------------------------------
enable_testing()
add_executable(systic_tests tests/main_test.cpp)
target_link_libraries(systic_tests PRIVATE systic_core GTest::gtest GTest::gtest_main)

# ---------------------------------------------------------
# Benchmark Target (bench/)
# ---------------------------------------------------------
add_executable(systic_bench bench/main_bench.cpp)
target_link_libraries(systic_bench PRIVATE systic_core benchmark::benchmark benchmark::benchmark_main)

# ---------------------------------------------------------
# Installation Instructions
# ---------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS systic_core
    EXPORT SysticTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/systic)
