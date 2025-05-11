cmake_minimum_required(VERSION 3.5)

include(cmake/globber.cmake)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(ProjectName VERSION 1.0 LANGUAGES CXX)

# set(PATTERNS "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.h")
# file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(PATTERNS "*.cpp" "*.h")
globber("${PATTERNS}")

# add_library(LibraryName LIBRARY_SOURCES_HERE)

# Adding something we can run - Output name matches target name
add_executable(ProjectName ${SOURCE_FILES})

# find target directories for include paths
target_include_directories(ProjectName PUBLIC
    ${CMAKE_SOURCE_DIR}/src/common
)

# tell CMake where to find googletest
file(TO_CMAKE_PATH $ENV{HOME} HOME_CMAKE)

# append GTest to the CMAKE_PREFIX_PATH and include GTestConfig.cmake to find libraries
list(APPEND CMAKE_PREFIX_PATH "${HOME_CMAKE}/OneDrive/Desktop/Spencer/Projects/opt/googletest/lib/cmake")
include(GoogleTest)

# find packages
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(ProjectName Threads::Threads ws2_32)

# add tests
enable_testing()


add_executable(UDPTests test/UDPTests.cpp)
target_link_libraries(UDPTests PUBLIC GTest::gtest GTest::gtest_main)

# Discover and run tests
gtest_discover_tests(UDPTests)