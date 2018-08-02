cmake_minimum_required(VERSION 3.2 FATAL_ERROR)


set(MODULE_NAME "main_app")
set(SRC_DIR "src")
set(INCL_DIR "include")
set(INCL_DIR_PRIVATE "include_private")
set(BIN_NAME "smart_location")

set(MODULE_SRC
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/utils.cpp
    )


add_executable(${BIN_NAME} ${MODULE_SRC})
target_include_directories(${BIN_NAME} PUBLIC ${INCL_DIR})
target_include_directories(${BIN_NAME} PRIVATE ${INCL_DIR_PRIVATE})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -std=c++17")


target_compile_features(${BIN_NAME}
    PUBLIC cxx_auto_type
    PUBLIC cxx_variadic_templates
    PUBLIC cxx_constexpr
    PUBLIC cxx_generic_lambdas
    PUBLIC cxx_nullptr
    PUBLIC cxx_noexcept
    PUBLIC cxx_static_assert
    PUBLIC cxx_variable_templates
    )

install(
    TARGETS ${BIN_NAME} DESTINATION "bin"
    ) 


install(
    DIRECTORY "${INCL_DIR}/${PROJECT_NAME}"
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
    )




