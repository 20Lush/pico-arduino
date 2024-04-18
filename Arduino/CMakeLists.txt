cmake_minimum_required(VERSION 3.12)

# We add some additional DEFINE
add_definitions(-DPICO -DARDUINO)

set(ARD_API_PREFIX ${PROJECT_SOURCE_DIR}/Arduino/ArduinoCore-API/api)
set(ARD_PICO_PREFIX ${PROJECT_SOURCE_DIR}/Arduino/ArduinoCore-Pico)
set(ARD_SWSER_PREFIX ${PROJECT_SOURCE_DIR}/Arduino/SoftwareSerial)
set(ARD_TOOLS_PREFIX ${PROJECT_SOURCE_DIR}/Arduino/Tools)

# Define what header files are going to be exposed(only put interfaces!)
set(PUBLIC_INCLUDE_FILES
    ${ARD_API_PREFIX}/Udp.h
    ${ARD_API_PREFIX}/PluggableUSB.h
    ${ARD_API_PREFIX}/Print.h
    ${ARD_API_PREFIX}/ArduinoAPI.h
    ${ARD_API_PREFIX}/HardwareSerial.h
    ${ARD_API_PREFIX}/RingBufferN.h
    ${ARD_API_PREFIX}/Printable.h
    ${ARD_API_PREFIX}/Client.h
    ${ARD_API_PREFIX}/itoa.h
    ${ARD_API_PREFIX}/Compat.h
    ${ARD_API_PREFIX}/USBAPI.h
    ${ARD_API_PREFIX}/Server.h
    ${ARD_API_PREFIX}/WString.h
    ${ARD_API_PREFIX}/Common.h
    ${ARD_API_PREFIX}/IPAddress.h
    ${ARD_API_PREFIX}/HardwareI2C.h
    ${ARD_API_PREFIX}/Interrupts.h
    ${ARD_API_PREFIX}/Binary.h
    ${ARD_API_PREFIX}/Stream.h
    ${ARD_API_PREFIX}/WCharacter.h
    ${ARD_API_PREFIX}/HardwareSPI.h
    ${ARD_PICO_PREFIX}/PicoHardwareSPI.h
    ${ARD_PICO_PREFIX}/PicoQueue.h
    ${ARD_PICO_PREFIX}/PicoPinFunction.h
    ${ARD_PICO_PREFIX}/PicoPWM.h
    ${ARD_PICO_PREFIX}/pins_arduino.h
    ${ARD_PICO_PREFIX}/PicoStreamPrintf.h
    ${ARD_PICO_PREFIX}/PicoLogger.h
    ${ARD_PICO_PREFIX}/PicoSemaphore.h
    ${ARD_PICO_PREFIX}/network.h
    ${ARD_PICO_PREFIX}/PicoTimer.h
    ${ARD_PICO_PREFIX}/PicoPWMServo.h
    ${ARD_PICO_PREFIX}/PicoMulticoreFifo.h
    ${ARD_PICO_PREFIX}/PicoTone.h
    ${ARD_PICO_PREFIX}/PicoThread.h
    ${ARD_PICO_PREFIX}/PicoUSB.h
    ${ARD_PICO_PREFIX}/SoftwareSerial.h
    ${ARD_PICO_PREFIX}/PicoPIOManager.h
    ${ARD_PICO_PREFIX}/pgmspace.h
    ${ARD_PICO_PREFIX}/avr/pgmspace.h
    ${ARD_PICO_PREFIX}/avr/wdt.h
    ${ARD_PICO_PREFIX}/Wire.h
    ${ARD_PICO_PREFIX}/PicoHardwareSerial.h
    ${ARD_PICO_PREFIX}/board.h
    ${ARD_PICO_PREFIX}/Servo.h
    ${ARD_PICO_PREFIX}/PicoDMA.h
    ${ARD_PICO_PREFIX}/SPI.h
    ${ARD_PICO_PREFIX}/Arduino.h
    ${ARD_PICO_PREFIX}/PicoHardwareI2C.h
    ${ARD_SWSER_PREFIX}/PicoSoftwareSerial.h
    ${ARD_SWSER_PREFIX}/pio_uart_tx.h
    ${ARD_SWSER_PREFIX}/pio_uart_rx.h
    ${ARD_TOOLS_PREFIX}/SerialDAC.h
    ${ARD_TOOLS_PREFIX}/Buffers.h
    ${ARD_TOOLS_PREFIX}/Map.h
    ${ARD_TOOLS_PREFIX}/Vector.h
)

set(SOURCE_FILES
    ${ARD_API_PREFIX}/IPAddress.cpp
    ${ARD_API_PREFIX}/Print.cpp
    ${ARD_API_PREFIX}/PluggableUSB.cpp
    ${ARD_API_PREFIX}/Stream.cpp
    ${ARD_API_PREFIX}/Common.cpp
    ${ARD_API_PREFIX}/WString.cpp
    ${ARD_PICO_PREFIX}/Arduino.cpp
)

add_library(${PROJECT_NAME} 
    STATIC
        ${SOURCE_FILES}
)

# There might be some includes we want at build-time here
include_directories("${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${ARD_API_PREFIX}>
        $<BUILD_INTERFACE:${ARD_PICO_PREFIX}>
        $<BUILD_INTERFACE:${ARD_SWSER_PREFIX}>
        $<BUILD_INTERFACE:${ARD_TOOLS_PREFIX}>
)

# Put any compile features we'd want in here, like language standards
target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_20
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        $<BUILD_INTERFACE:pico_stdio_usb> 
        $<BUILD_INTERFACE:pico_stdlib>
        $<BUILD_INTERFACE:pico_runtime>
        $<BUILD_INTERFACE:pico_multicore>
        $<BUILD_INTERFACE:pico_util>
        $<BUILD_INTERFACE:pico_unique_id>
        $<BUILD_INTERFACE:hardware_pwm>
        $<BUILD_INTERFACE:hardware_adc>
        $<BUILD_INTERFACE:hardware_clocks>
        $<BUILD_INTERFACE:hardware_spi>
        $<BUILD_INTERFACE:hardware_i2c>
        $<BUILD_INTERFACE:hardware_pio>
        $<BUILD_INTERFACE:hardware_dma>
)

DefaultInstall(${PROJECT_NAME})

