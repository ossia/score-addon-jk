cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()

project(score_addon_jk LANGUAGES CXX)

add_library(
  score_addon_jk
  Jk/Filter.hpp
  Jk/Filter.cpp

  score_addon_jk.hpp
  score_addon_jk.cpp
)

setup_score_plugin(score_addon_jk)

target_include_directories(score_addon_jk PRIVATE 3rdparty/jk/include)
target_link_libraries(score_addon_jk PUBLIC score_plugin_engine score_plugin_avnd)
