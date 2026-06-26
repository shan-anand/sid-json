cmake_minimum_required(VERSION 3.10)
project(sid-json VERSION 1.0.0)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# Option to enable code coverage
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default optimization, but allow override for coverage
if(NOT ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Define coverage flags
if(ENABLE_COVERAGE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      set(MY_COVERAGE_COMPILE_OPTIONS -g -O0 -fprofile-arcs -ftest-coverage)
      set(MY_COVERAGE_LINK_OPTIONS -fprofile-arcs -ftest-coverage)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(MY_COVERAGE_COMPILE_OPTIONS -g -O0 -fprofile-instr-generate -fcoverage-mapping)
      set(MY_COVERAGE_LINK_OPTIONS -fprofile-instr-generate)
    endif()
    message(STATUS "Coverage instrumentation enabled for sid-json targets.")
  endif()
endif()


# Use libc++ when building with Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-stdlib=libc++" HAS_LIBCXX)
  if(HAS_LIBCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
    message(STATUS "Linking against libc++")
  else()
    message(STATUS "Linking against libstdc++ (default)")
  endif()
else()
  message(STATUS "Linking against libstdc++ (default)")
endif()


# Show verbose compilation output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include directories
include_directories(include)
include_directories(include/sid)

# Add subdirectories
add_subdirectory(lib/json)
add_subdirectory(tools/json)

# Export targets for CMake package
install(EXPORT sid-jsonTargets
  FILE sid-jsonTargets.cmake
  NAMESPACE sidjson::
  DESTINATION lib/cmake/sid-json
)

# Generate and install the ConfigVersion and Config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/sid-json-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sid-json-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/sid-json-config.cmake"
  INSTALL_DESTINATION lib/cmake/sid-json
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/sid-json-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/sid-json-config-version.cmake"
  DESTINATION lib/cmake/sid-json
)

# Install targets
install(TARGETS sid-json-lib-shared sid-json-lib-static sid-json
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/sid
    DESTINATION include
)


# Show install messages
set(CMAKE_INSTALL_MESSAGE ALWAYS)

# Show a message to build using Ninja if Ninja is the generator
if(CMAKE_GENERATOR STREQUAL "Ninja")
  message(STATUS "================================")
  message(STATUS "To build the project, run: ninja")
  message(STATUS "================================")
endif()

# Add tests subdirectory
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(unittests/json)
endif()

# Uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()