cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(asl LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)
include(cmake/git_revision.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_INSTALL_PREFIX "/usr/local/")

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)
add_library(project_defines INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()


option(ENABLE_ASAN "Enable address sanitizer" FALSE)
if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
endif()


target_compile_options(project_warnings
    INTERFACE
    -Wall
    -Wextra
    -Wshadow
    )

option(ALL_WARNINGS "Enable all waring that the compiler supports" FALSE)
if(${ALL_WARNINGS})
    target_compile_options(project_warnings
        INTERFACE
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2)
endif()

option(LIBCPP "Build with libc++" FALSE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND ${ALL_WARNINGS})
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.3)
        target_compile_options(project_warnings INTERFACE
            -Wduplicated-branches)
    endif()
    target_compile_options(project_warnings INTERFACE
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
        -Wuseless-cast)
    target_link_libraries(project_options INTERFACE stdc++fs)
else()
    if(LIBCPP)
        target_compile_options(project_options INTERFACE -stdlib=libc++)
    else()
        target_link_libraries(project_options INTERFACE stdc++fs)
    endif()
endif()

target_link_libraries(project_options INTERFACE -fno-exceptions)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)


target_compile_definitions(project_defines INTERFACE AY_VERSION_MAJOR=1)
target_compile_definitions(project_defines INTERFACE AY_VERSION_MINOR=0)
target_compile_definitions(project_defines INTERFACE AY_VERSION_PATCH=0)

add_subdirectory(lib)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_include_directories(project_options INTERFACE include)

add_executable(asl_alloc ./src/salloc.cpp)
target_link_libraries(asl_alloc
    PUBLIC project_options project_warnings project_defines)

add_executable(asl_containers ./src/scont.cpp)
target_link_libraries(asl_containers
    PUBLIC project_options project_warnings project_defines spooky_hash)


add_executable(asl_talloc ./src/talloc.cpp)
target_link_libraries(asl_talloc
    PUBLIC project_options project_warnings project_defines spooky_hash)


add_executable(asl_splat ./src/splat.cpp)
target_link_libraries(asl_splat
    PUBLIC project_options project_warnings project_defines spooky_hash)


option(ENABLE_TESTING "Enablel tsting" FALSE)
if(${ENABLE_TESTING})
    enable_testing()
    add_subdirectory(test)
endif()
