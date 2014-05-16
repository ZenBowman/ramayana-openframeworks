cmake_minimum_required(VERSION 2.8)

SET (CMAKE_C_COMPILER  "clang")
SET (CMAKE_CXX_COMPILER  "clang++")

project(Tests)
find_package(OpenCV REQUIRED)

set(OFX ../../../)

add_executable( 
  Tests
  ../src/test_collision.cpp
)

set(
  OFX_OPENCV
  ${OFX}/addons/ofxOpenCv/src/
)

set(
  OFX_MAIN
  ${OFX}/libs/openFrameworks
  ${OFX}/libs/openFrameworks/utils
  ${OFX}/libs/openFrameworks/sound
  ${OFX}/libs/openFrameworks/video
  ${OFX}/libs/openFrameworks/types
  ${OFX}/libs/openFrameworks/math
  ${OFX}/libs/openFrameworks/graphics
  ${OFX}/libs/openFrameworks/gl
  ${OFX}/libs/openFrameworks/3d
  ${OFX}/libs/openFrameworks/app
  ${OFX}/libs/openFrameworks/communication
  ${OFX}/libs/openFrameworks/events
  /usr/include/cairo
  /usr/include/gstreamer-0.10
  /usr/include/glib-2.0
  /usr/lib64/glib-2.0/include/
)

set(
  OFX_EXT_LIBS
  ${OFX}/libs/tess2/include
  ${OFX}/libs/poco/include
  ${OFX}/libs/assimp/include
  ${OFX}/libs/fmodex/include
  ${OFX}/libs/FreeImage/include
  ${OFX}/libs/freetype/include
  ${OFX}/libs/glew/include
  ${OFX}/libs/glfw/include
  ${OFX}/libs/kiss/include
  ${OFX}/libs/openssl/include
  ${OFX}/libs/rtAudio/include
)

set(
  OFX_POCO
  
)

include_directories(
  ${OFX_OPENCV}
  ${OFX_MAIN}
  ${OFX_EXT_LIBS}
)

target_link_libraries(
  Tests
  gtest
  gtest_main
  /home/psamtani/development/openFrameworks_0.8.1/libs/openFrameworksCompiled/lib/linux64/libopenFrameworks.a
)