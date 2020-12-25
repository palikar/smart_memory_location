cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(SRC_DIR "src")
set(INCL_DIR "include")
set(INCL_DIR_PRIVATE "include_private")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -std=c++17")


set(MODULE_SRC
    ${SRC_DIR}/salloc.cpp
    )


add_executable(smart_location_app ${MODULE_SRC})
target_include_directories(smart_location_app PUBLIC ${INCL_DIR})
target_include_directories(smart_location_app PRIVATE ${INCL_DIR_PRIVATE})

target_compile_options(smart_location_app PUBLIC -fno-exceptions)


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



add_executable(smart_containers_app src/scont.cpp)
target_include_directories(smart_containers_app PUBLIC ${INCL_DIR})
target_include_directories(smart_containers_app PRIVATE ${INCL_DIR_PRIVATE})

target_compile_options(smart_containers_app PUBLIC -fno-exceptions)


target_compile_features(smart_containers_app
    PUBLIC cxx_auto_type
    PUBLIC cxx_variadic_templates
    PUBLIC cxx_constexpr
    PUBLIC cxx_generic_lambdas
    PUBLIC cxx_nullptr
    PUBLIC cxx_noexcept
    PUBLIC cxx_static_assert
    PUBLIC cxx_variable_templates
    )
