cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(score_addon_jk LANGUAGES CXX)

# Use the Avendish the host (ossia score) provides; otherwise fetch it.
find_package(Avendish QUIET)
if(NOT Avendish_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Avendish
    GIT_REPOSITORY "https://github.com/celtera/avendish"
    GIT_TAG 64e1b98dbc227b7df079561b4aa9e66330e47263)
  FetchContent_Populate(Avendish)
  list(APPEND CMAKE_PREFIX_PATH "${avendish_SOURCE_DIR}")
  find_package(Avendish REQUIRED)
endif()

# Standalone Max/MSP externals link the static CRT (/MT) on Windows, as the
# official max-sdk-base requires: set it before the libraries are created so
# every target of the external shares one runtime (mixing /MT and /MD trips MSVC
# with LNK2038). Inside ossia score (in-tree or through its SDK) the add-on is
# linked into score, which uses the DLL runtime (/MD), so it keeps the host's.
# (CMP0091 is NEW via cmake_minimum_required.)
if(MSVC AND AVND_ENABLE_MAX AND NOT COMMAND avnd_score_plugin_add)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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
  # Absolute MAIN_FILE: the generated dump/back-end sources live in the build tree
  # and #include it, so a relative path wouldn't resolve from there.
  MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Jk/Filter.hpp"
  SOURCES
    Jk/Filter.hpp
    Jk/Filter.cpp)

if(COMMAND score_register_license)
  score_register_license(jk URL "https://github.com/jcelerier/jk" FILES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/jk/LICENSE")
endif()

avnd_addon_finalize(NAME score_addon_jk UUID c8a00c5e-71ea-4eb7-bc21-4af7b7c6030f VERSION 1)
