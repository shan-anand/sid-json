
# Enable testing
enable_testing()

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/sid)
include_directories(${CMAKE_SOURCE_DIR}/lib/json)

# Test sources
set(TEST_SOURCES
    test_value.cpp
    test_parser.cpp
    test_format.cpp
    test_schema.cpp
    test_utils.cpp
    test_time_calc.cpp
    test_main.cpp
)

# Create test executable
add_executable(sid-json-tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(sid-json-tests 
    PRIVATE
    sid-json-lib
    gtest
    gtest_main
)

# Add coverage flags if defined
if(MY_COVERAGE_COMPILE_OPTIONS)
    target_compile_options(sid-json-tests PRIVATE ${MY_COVERAGE_COMPILE_OPTIONS})
    target_link_options(sid-json-tests PRIVATE ${MY_COVERAGE_LINK_OPTIONS})
endif()

# Add test
add_test(NAME sid-json-unit-tests COMMAND sid-json-tests)