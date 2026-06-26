add_executable(sid-json
    main.cpp
)

# Link with the sid-json library
target_link_libraries(sid-json PRIVATE sid-json-lib)

# Add coverage flags if defined
if(MY_COVERAGE_COMPILE_OPTIONS)
    target_compile_options(sid-json PRIVATE ${MY_COVERAGE_COMPILE_OPTIONS})
    target_link_options(sid-json PRIVATE ${MY_COVERAGE_LINK_OPTIONS})
endif()


