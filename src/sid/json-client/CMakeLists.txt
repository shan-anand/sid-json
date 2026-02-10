add_executable(sid-json-client
    main.cpp
)

# Link with the sid-json library (shared by default)
target_link_libraries(sid-json-client sid-json)

# Add coverage flags for Debug builds to client
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(sid-json-client PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json-client PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(sid-json-client PRIVATE --coverage)
        target_link_options(sid-json-client PRIVATE --coverage)
    endif()
endif()
