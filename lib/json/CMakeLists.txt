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
add_library(sid-json-lib-shared SHARED ${SOURCES} ${HEADERS})
set_target_properties(sid-json-lib-shared PROPERTIES
    OUTPUT_NAME sid-json-lib
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

add_library(sid-json-lib-static STATIC ${SOURCES} ${HEADERS})
set_target_properties(sid-json-lib-static PROPERTIES
    OUTPUT_NAME sid-json-lib
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# For compatibility, create an alias target 'sid-json-lib' for the static library
add_library(sid-json-lib ALIAS sid-json-lib-static)

# Export both static and shared targets
install(TARGETS sid-json-lib-shared sid-json-lib-static
    EXPORT sid-jsonTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Add coverage flags for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(sid-json-lib-shared PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json-lib-shared PRIVATE -fprofile-instr-generate)
        target_compile_options(sid-json-lib-static PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(sid-json-lib-static PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(sid-json-lib-shared PRIVATE --coverage)
        target_link_options(sid-json-lib-shared PRIVATE --coverage)
        target_compile_options(sid-json-lib-static PRIVATE --coverage)
        target_link_options(sid-json-lib-static PRIVATE --coverage)
    endif()
endif()
