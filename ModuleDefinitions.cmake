cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(SRC_DIR "src")
set(INCL_DIR "include")
set(INCL_DIR_PRIVATE "include_private")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -std=c++17")


set(MODULE_SRC
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/utils.cpp
    )


add_executable(smart_location_app ${MODULE_SRC})
target_include_directories(smart_location_app PUBLIC ${INCL_DIR})
target_include_directories(smart_location_app PRIVATE ${INCL_DIR_PRIVATE})




target_compile_features(smart_location_app
    PUBLIC cxx_auto_type
    PUBLIC cxx_variadic_templates
    PUBLIC cxx_constexpr
    PUBLIC cxx_generic_lambdas
    PUBLIC cxx_nullptr
    PUBLIC cxx_noexcept
    PUBLIC cxx_static_assert
    PUBLIC cxx_variable_templates
    )


add_library(saloclib "")
target_sources(
    saloclib
    PRIVATE
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/utils.cpp
    )
target_include_directories(saloclib PUBLIC ${INCL_DIR})
target_include_directories(saloclib PRIVATE ${INCL_DIR_PRIVATE})



target_compile_features(saloclib
    PUBLIC cxx_auto_type
    PUBLIC cxx_variadic_templates
    PUBLIC cxx_constexpr
    PUBLIC cxx_generic_lambdas
    PUBLIC cxx_nullptr
    PUBLIC cxx_noexcept
    PUBLIC cxx_static_assert
    PUBLIC cxx_variable_templates
    )


include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )
enable_testing()
add_subdirectory(tests)


install(TARGETS smart_location_app DESTINATION "bin")
install(TARGETS saloclib DESTINATION "lib")



install(
    DIRECTORY "${INCL_DIR}/${PROJECT_NAME}"
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
    )




