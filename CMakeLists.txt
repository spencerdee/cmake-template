cmake_minimum_required(VERSION 3.5)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(ProjectName VERSION 1.0 LANGUAGES CXX)

set(PROJECT_SOURCES
    src/main.cpp
)

# add_library(LibraryName LIBRARY_SOURCES_HERE)

# Adding something we can run - Output name matches target name
add_executable(ProjectName ${PROJECT_SOURCES})

# target_link_libraries(ProjectName PRIVATE LibraryName)