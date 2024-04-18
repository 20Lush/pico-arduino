cmake_minimum_required(VERSION 3.12)
# We grab our boilerplate stuff up here since we might need it multiple times in the repo
set(CMAKE_BOILERPLATE_PATH ${CMAKE_SOURCE_DIR}/.CMake-Boilerplate)
include(${CMAKE_BOILERPLATE_PATH}/INSTALLER.cmake)
include(${CMAKE_BOILERPLATE_PATH}/VARIABLES.cmake)
include(${CMAKE_BOILERPLATE_PATH}/TESTING.cmake)
include(${CMAKE_BOILERPLATE_PATH}/pico_sdk_import.cmake)

# Project version number. Good metadata to have, but ONLY INCREMENT WHEN THERE IS SOMETHING BEING RELEASED!
set(PROJECT_VERSION 1.0.0)

# Initialization 
set(PICO_SDK_ARDUINO_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(pico-arduino
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "pico-arduino linkable library"
    LANGUAGES C CXX
)

pico_sdk_init()

# This runtime directory will be used for stuff that has a runtime execution part
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/runtime")

# We declare where wwe wwant our cmake module stuff to live at build-time
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")

# There might be packages in our install path that we should use as hints later
set(PACKAGE_HINTS ${CMAKE_INSTALL_PREFIX})

add_subdirectory(Arduino)



