cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(score_addon_jk LANGUAGES CXX)

# Use the Avendish the host (ossia score) provides; otherwise fetch it.
find_package(Avendish QUIET)
if(NOT Avendish_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Avendish
    GIT_REPOSITORY "https://github.com/celtera/avendish"
    GIT_TAG 6dacc12)
  FetchContent_Populate(Avendish)
  list(APPEND CMAKE_PREFIX_PATH "${avendish_SOURCE_DIR}")
  find_package(Avendish REQUIRED)
endif()

# Object dependency: linked so it reaches the score base target and every standalone
# back-end (each back-end compiles the object header directly).
add_library(jk_deps INTERFACE)
target_include_directories(jk_deps INTERFACE 3rdparty/jk/include)

avnd_addon_init(NAME score_addon_jk)

avnd_addon_object(
  BASE score_addon_jk
  C_NAME jk
  CLASS Filter
  NAMESPACE Jk
  DEPENDENCIES jk_deps
  SOURCES
    Jk/Filter.hpp
    Jk/Filter.cpp)

avnd_addon_finalize(NAME score_addon_jk UUID c8a00c5e-71ea-4eb7-bc21-4af7b7c6030f VERSION 1)
