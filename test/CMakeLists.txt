include(CTest)
include(Catch)
add_executable(asl_tests
    ./main_test.cpp
    memory_test.cpp)

catch_discover_tests(asl_tests)
