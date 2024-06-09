cmake_minimum_required(VERSION 3.20)

project(ttl)

# Set property of project
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(DEFAULT_BUILD_TYPE "Release")

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(DEFAULT_BUILD_TYPE "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -g")
else()
 set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Indicates include dir project use
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Get source files
file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cc)

# Get cpr package
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

# Set executable
add_executable(ttl ${SRC})

# Link library
target_link_libraries(ttl PRIVATE cpr::cpr)

# Install setting
set_target_properties(ttl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

if(DEFINED ENV{SHELL} AND $ENV{SHELL} MATCHES "bash")
    set(SHELL_RC "$ENV{HOME}/.bashrc")
    set(SHELL_PATH "export PATH=\$PATH:${CMAKE_INSTALL_PREFIX}/bin\n")
elseif (DEFINED ENV{SHELL} AND $ENV{SHELL} MATCHES "fish")
    set(SHELL_RC "$ENV{HOME}/.config/fish/config.fish")
    set(SHELL_PATH "set -x PATH \$PATH ${CMAKE_INSTALL_PREFIX}/bin\n")
else()
    message(FATAL_ERROR "Unsupported shell")
endif()

install(CODE "
    file(APPEND \"${SHELL_RC}\" \"${SHELL_PATH}\")
    message(\"-- Please run 'source ${SHELL_RC}' to update your PATH.\")
")

install(TARGETS ttl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# Custom commands
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_INSTALL_PREFIX}/bin/ttl
)