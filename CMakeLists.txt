cmake_minimum_required(VERSION 3.10)
project(maxclique VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
if(NOT CMAKE_BUILD_TYPE)
    # Options: Debug, Release, MinSizeRel, RelWithDebInfo
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type." FORCE)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

###############################################################################
# Options
###############################################################################

option(BUILD_BINDINGS_PYTHON "Build Python bindings" ON)
option(BUILD_BINDINGS_MATLAB "Build MATLAB bindings" ON)
option(BUILD_TESTS "Build testsuite" OFF)

###############################################################################
# Dependencies
###############################################################################

find_package(Eigen3 REQUIRED)
message(STATUS "Eigen Version: ${EIGEN3_VERSION_STRING} ${EIGEN3_VERSION}")

find_package(OpenMP)

include(FetchContent)
FetchContent_Declare(pmc
    GIT_REPOSITORY  https://github.com/jingnanshi/pmc
    GIT_TAG         master)
FetchContent_MakeAvailable(pmc)

if(BUILD_BINDINGS_PYTHON)
  set(PYBIND11_DIR "${CMAKE_CURRENT_BINARY_DIR}/pybind11-download" CACHE INTERNAL "pybind11 build dir" FORCE)
  configure_file(${CMAKE_MODULE_PATH}/pybind11.cmake.in ${PYBIND11_DIR}/CMakeLists.txt IMMEDIATE @ONLY)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${PYBIND11_DIR})
  execute_process(COMMAND "${CMAKE_COMMAND}" --build . WORKING_DIRECTORY ${PYBIND11_DIR})
  add_subdirectory(${PYBIND11_DIR}/src)
endif()

###############################################################################
# Targets
###############################################################################

add_library(maxclique SHARED)
target_sources(maxclique PRIVATE
  src/maxclique.cpp)
target_link_libraries(maxclique PUBLIC Eigen3::Eigen)
target_link_libraries(maxclique PRIVATE OpenMP::OpenMP_CXX pmc)
target_include_directories(maxclique PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_compile_options(maxclique PUBLIC -march=native -mavx -mfma)
set_target_properties(maxclique PROPERTIES
  VERSION ${PROJECT_VERSION})
target_compile_definitions(maxclique PUBLIC PROJECT_VERSION="${PROJECT_VERSION}")

###############################################################################
# Extras
###############################################################################

if(BUILD_BINDINGS_PYTHON)
    message(STATUS "Building Python bindings.")
    add_subdirectory(bindings/python)
endif()

# if(BUILD_BINDINGS_MATLAB)
#     message(STATUS "Attempting to build MATLAB bindings.")
#     add_subdirectory(bindings/matlab)
# endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

###############################################################################
# Install
###############################################################################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/maxclique)

install(TARGETS maxclique
    EXPORT maxclique-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT maxclique-targets
  FILE maxclique-targets.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/maxclique-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/maxclique-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/maxclique-config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/maxclique-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/maxclique-config-version.cmake
  DESTINATION ${INSTALL_CONFIGDIR})