cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

# if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES aarch64)
#   return()
# endif()

if(NOT TARGET score_lib_base)
  include(ScoreExternalAddon)
endif()

if(NOT TARGET score_plugin_avnd)
  return()
endif()

project(score_addon_jk LANGUAGES CXX)

avnd_score_plugin_init(
  BASE_TARGET score_addon_jk
)

target_include_directories(score_addon_jk PRIVATE 3rdparty/jk/include)
target_link_libraries(score_addon_jk PUBLIC score_plugin_engine score_plugin_avnd)

avnd_score_plugin_add(
  BASE_TARGET score_addon_jk
  SOURCES
    Jk/Filter.hpp
    Jk/Filter.cpp
  TARGET jk
  MAIN_CLASS Filter
  NAMESPACE Jk
)

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_jk
  PLUGIN_VERSION 1
  PLUGIN_UUID "c8a00c5e-71ea-4eb7-bc21-4af7b7c6030f"
)

