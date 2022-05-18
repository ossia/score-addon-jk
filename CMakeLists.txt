cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()

project(score_addon_jk LANGUAGES CXX)

add_library(
  score_addon_jk
  Jk/AnEffectModel.hpp
  Jk/AnEffectModel.cpp
  Jk/AnEffectUi.hpp
  Jk/AnEffect.hpp

  score_addon_jk.hpp
  score_addon_jk.cpp
)

setup_score_plugin(score_addon_jk)

target_link_libraries(score_addon_jk PUBLIC score_plugin_engine score_plugin_avnd)