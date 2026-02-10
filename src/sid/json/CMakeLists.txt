# Source files
set(SOURCES
    utils.cpp
    format.cpp
    parser_stats.cpp
    time_calc.cpp
    value.cpp
    schema.cpp
)

# Header files
set(HEADERS
    ${CMAKE_SOURCE_DIR}/include/sid/json/format.h
    ${CMAKE_SOURCE_DIR}/include/sid/json/json.h
    ${CMAKE_SOURCE_DIR}/include/sid/json/parser_control.h
    ${CMAKE_SOURCE_DIR}/include/sid/json/parser_stats.h
    ${CMAKE_SOURCE_DIR}/include/sid/json/schema.h
    ${CMAKE_SOURCE_DIR}/include/sid/json/value.h
)

# Build both shared and static libraries
add_library(sid-json-shared SHARED ${SOURCES} ${HEADERS})
set_target_properties(sid-json-shared PROPERTIES
    OUTPUT_NAME sid-json
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

add_library(sid-json-static STATIC ${SOURCES} ${HEADERS})
set_target_properties(sid-json-static PROPERTIES
    OUTPUT_NAME sid-json
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# For compatibility, create an alias target 'sid-json' for the shared library (do not set properties or install this alias)
add_library(sid-json ALIAS sid-json-shared)

# Export both static and shared targets for install(EXPORT ...) in the top-level CMakeLists.txt
install(TARGETS sid-json-shared sid-json-static
    EXPORT sid-jsonTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Add coverage flags for Debug builds (only for real targets)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(sid-json-shared PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json-shared PRIVATE -fprofile-instr-generate)
        target_compile_options(sid-json-static PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json-static PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(sid-json-shared PRIVATE --coverage)
        target_link_options(sid-json-shared PRIVATE --coverage)
        target_compile_options(sid-json-static PRIVATE --coverage)
        target_link_options(sid-json-static PRIVATE --coverage)
    endif()
endif()
