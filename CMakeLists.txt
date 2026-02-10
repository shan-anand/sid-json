cmake_minimum_required(VERSION 3.10)
project(sid-json VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-stdlib=libc++" HAS_LIBCXX)
  if(HAS_LIBCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
  endif()
endif()

# Show verbose compilation output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include directories
include_directories(include)
include_directories(include/sid)

# Add subdirectories
add_subdirectory(src/sid/json)
add_subdirectory(src/sid/json-client)

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
install(TARGETS sid-json-shared sid-json-static sid-json-client
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
    add_subdirectory(tests)
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