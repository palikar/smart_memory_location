include(CTest)
include(Catch)
add_executable(asl_tests
    ./main_test.cpp
    memory_test.cpp)

target_link_libraries(asl_tests
    PUBLIC project_options project_warnings project_defines)

catch_discover_tests(asl_tests)
