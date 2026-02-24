add_executable(sid-json
    main.cpp
)

# Link with the sid-json library
target_link_libraries(sid-json sid-json-lib)

# Add coverage flags for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(sid-json PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(sid-json PRIVATE --coverage)
        target_link_options(sid-json PRIVATE --coverage)
    endif()
endif()
