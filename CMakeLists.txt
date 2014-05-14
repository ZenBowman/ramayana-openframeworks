cmake_minimum_required(VERSION 2.8)

SET (CMAKE_C_COMPILER  "clang")
SET (CMAKE_CXX_COMPILER  "clang++")

project(Tests)
find_package(OpenCV REQUIRED)

add_executable( 
  Tests
  ../src/test_collision.cpp
)

set(
  OFX_OPENCV
  ../../../addons/ofxOpenCv/src/
)

set(
  OFX_MAIN
  ../../../libs/openFrameworks
  ../../../libs/openFrameworks/utils
)

set(
  OFX_TESS2
  ../../../libs/tess2/include
)

set(
  OFX_POCO
  ../../../libs/poco/include
)

include_directories(
  ${OFX_OPENCV}
  ${OFX_MAIN}
  ${OFX_TESS2}
  ${OFX_POCO}
)

target_link_libraries(
  Tests
  gtest
  gtest_main
)