cmake_minimum_required(VERSION 3.12)

# PICO initialization
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# define project settins
set(PICO_SDK_ARDUINO_PATH $ENV{PICO_SDK_ARDUINO_PATH})
set(ARDUINO_SKETCH_NAME "hallo_world_ext" )
set(ARDUINO_SKETCH_SOURCE "hallo_world_ext.cpp" )

# Standard Sketch logic
project("${ARDUINO_SKETCH_NAME}")
include("${PICO_SDK_ARDUINO_PATH}/ArduinoSketch.cmake")

