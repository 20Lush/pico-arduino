#pragma once

#include "PicoStreamPrintf.h"
#include "pins_arduino.h"
#include "HardwareSerial.h"
#include "PicoUSB.h"
#include "PicoLogger.h"
#include "Stream.h"
#include "RingBufferN.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <tusb.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 512
#endif

namespace pico_arduino {

// PicoSerialUSB is not available with TinyUSB is used !
#if !defined(TINYUSB_HOST_LINKED) && !defined(TINYUSB_DEVICE_LINKED)

// Arduino Due provides SerialUSB - this is identical with Serial 
#define SerialUSB Serial

/**
 * @brief PicoUSBSerial is using the pico USB output. It is mapped to the Arduino Serial variable.
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class PicoSerialUSB : public HardwareSerial, public StreamPrintf {
    public:
        PicoSerialUSB() : StreamPrintf(this){
        }

        // opens the processing - calling the pico stdio_usb_init
        void begin() {
            stdio_usb_init ();
            is_open = true;
        };

        //needed because Stadnard Arduino requires this all but parameter has no impact on USB 
        virtual void begin(unsigned long baudrate) {
            begin();
        };

        // needed because Stadnard Arduino requires this all but both parameters have no impact on USB
        virtual void begin(unsigned long baudrate, uint16_t config){
            begin();
        }

        // deactivates the output to USB
        virtual void end() {
            is_open = false;
        }

        virtual int available(void){
            readBuffer();
            return buffer.available();
        }

        virtual int peek(void) {
            readBuffer();
            return buffer.peek();
        }

        virtual int read(void){
            // we might need to flush the current ouptut...
            flush();
            // try to refill the buffer
            readBuffer();
            return buffer.read_char();
        }

        virtual void flush(void) {
             stdio_flush();
        }

        /// provide implmentation of standard Arduino output of single character
        virtual size_t write(uint8_t c) {
            size_t len = putchar(c);
#ifndef PICO_ARDUINO_NO_FLUSH
            stdio_flush();
#endif
            return len;
        }

        /// provide implmentation of standard Arduino output of multiple characters
        virtual size_t write(const uint8_t *buffer, size_t size){
            int result;
            for (size_t j=0;j<size;j++){
                result += putchar(buffer[j]);
            }
            stdio_flush();
            return result;
        }

        // Pull in default implementations 
        using Print::write; // pull in write(str) and write(buf, size) from Print
        using Print::print; // pull in write(str) and write(buf, size) from Print
        using Print::println; // pull in write(str) and write(buf, size) from Print


        virtual operator bool(){
            return tud_cdc_connected() && is_open;
        };

    protected:
        bool is_open;
        RingBufferN<BUFFER_SIZE> buffer;

        void readBuffer() {
            // refill buffer only if it is empty
            if (buffer.available()==0){
                // fill buffer as long as we have space
                int wait_time = 1000;
                int c = getchar_timeout_us(wait_time);
                while(c!=PICO_ERROR_TIMEOUT && buffer.availableForStore()>0){
                    buffer.store_char(c);
                    c = getchar_timeout_us(wait_time);
                }
            }
        }

};

inline PicoSerialUSB Serial;

#endif

/**
 * @brief Serial Stream for a defined UART. By default we use the following pins: UART0 tx/rx = gp0/gp1; UART1 tx/rx = gp4/gp5; 
 * 
 */
class PicoSerialUART : public HardwareSerial, public StreamPrintf {
    public:
        PicoSerialUART() : StreamPrintf(this) {
        }

        PicoSerialUART(int uart_no) : StreamPrintf(this)  {
            this->uart_no = uart_no;
            this->uart = uart_no == 0 ? uart0 : uart1;
        }

        virtual void begin(unsigned long baudrate=PICO_DEFAULT_UART_BAUD_RATE) {
            begin(baudrate, -1, -1, SERIAL_8N1);
        }

        virtual void begin(unsigned long baudrate, uint16_t config) {
            begin(baudrate, -1, -1, config);
        }

        /**
         * @brief Initialization to output to UART
         * 
         * @param baudrate 
         * @param rxPin 
         * @param txPin 
         * @param config 
         * @param invert 
         * @param cts 
         * @param rts 
         */

        virtual void begin(unsigned long baudrate, int rxPin, int txPin, uint32_t config=SERIAL_8N1,  bool invert=false, bool cts=false, bool rts=false) {
            Logger.printf(PicoLogger::Info, "PicoHardwareSerial::begin %ld\n", baudrate);
            rx_pin = rxPin;
            tx_pin = txPin;
            uart_init(uart, baudrate);
            setupDefaultRxTxPins();
            uart_set_hw_flow(uart, cts, rts);
            set_config(config);
            uart_set_translate_crlf(uart, false);
            uart_set_fifo_enabled(uart, true);

            uint rate_effective = uart_set_baudrate(uart,baudrate);
            open = uart_is_enabled(uart);
            if (Logger.isLogging()) {
                Logger.printf(PicoLogger::Info, "baud_rate requested: %ld\n",baudrate);
                Logger.printf(PicoLogger::Info,"baud_rate effective: %ld\n",rate_effective);
                Logger.printf(PicoLogger::Info,"uart_is_enabled: %s\n", open ?  "true" :  "false");
            }
            
        }

        virtual void end(){
             Logger.printf(PicoLogger::Info, "PicoHardwareSerial::end %d\n",uart_no);
             uart_deinit(uart);
             open = false;
        }

        virtual int available(void){
            readBuffer();
            return buffer.available();
        }

        virtual int availableForWrite(void){
            return uart_is_writable(uart);
        }

        virtual int peek(void){
            readBuffer();
            return buffer.peek();
        }

        virtual int read(void){
            readBuffer();
            return buffer.read_char();
        }

        using Print::write; // pull in write(str) and write(buf, size) from Print
        using Print::print; // pull in write(str) and write(buf, size) from Print
        using Print::println; // pull in write(str) and write(buf, size) from Print

        virtual size_t write(uint8_t c) {
             bool ok = uart_is_writable (uart);
             if (ok){
                uart_putc(uart, c);
             }
             return ok ? 1 : 0;
        }

        virtual size_t write(const uint8_t *buffer, size_t size){
            uart_write_blocking(uart, buffer, size);
            return size;
        }

        uint32_t baudRate(){
            return baud_rate;
        }
        
        virtual void flush(void) {
        };

        virtual operator bool(){
            return open;
        };


    protected:
        RingBufferN<BUFFER_SIZE> buffer;
        uart_inst_t *uart;
        uint baud_rate;
        int tx_pin;
        int rx_pin;
        int uart_no;
        bool open = false;

        // filles the read buffer
        void readBuffer(bool refill=false) {
            // refill buffer only when requested or when it is empty
            if (refill || buffer.available()==0){
                while(buffer.availableForStore()>0 && uart_is_readable(uart) ) {
                    char c = uart_get_hw(uart)->dr;
                    buffer.store_char(c);
                }
            }
        }

        void set_config(uint32_t config){
            Logger.info("set_config");
            //data, parity, and stop bits
            switch(config){
            case SERIAL_5N1:
                uart_set_format(uart, 5, 1,UART_PARITY_NONE);
                break;
            case SERIAL_6N1:
                uart_set_format(uart, 6, 1,UART_PARITY_NONE);
                break;
            case SERIAL_7N1:
                uart_set_format(uart, 7, 1,UART_PARITY_NONE);
                break;
            case SERIAL_8N1:
                Logger.info("SERIAL_8N1 - UART_PARITY_NONE");
                uart_set_format(uart, 8, 1,UART_PARITY_NONE);
                break;
            case SERIAL_5N2:
                uart_set_format(uart, 5, 2,UART_PARITY_NONE);
                break;
            case SERIAL_6N2:
                uart_set_format(uart, 6, 2,UART_PARITY_NONE);
                break;
            case SERIAL_7N2:
                uart_set_format(uart, 7, 2,UART_PARITY_NONE);
                break;
            case SERIAL_8N2:
                uart_set_format(uart, 8, 2,UART_PARITY_NONE);
                break;
            case SERIAL_5E1: 
                uart_set_format(uart, 5, 1,UART_PARITY_EVEN);
                break;
            case SERIAL_6E1:
                uart_set_format(uart, 6, 1,UART_PARITY_EVEN);
                break;
            case SERIAL_7E1:
                uart_set_format(uart, 7, 1,UART_PARITY_EVEN);
                break;
            case SERIAL_8E1:
                uart_set_format(uart, 8, 1,UART_PARITY_EVEN);
                break;
            case SERIAL_5E2:
                uart_set_format(uart, 5, 2,UART_PARITY_EVEN);
                break;
            case SERIAL_6E2:
                uart_set_format(uart, 6, 2,UART_PARITY_EVEN);
                break;
            case SERIAL_7E2:
                uart_set_format(uart, 7, 2,UART_PARITY_EVEN);
                break;
            case SERIAL_8E2:
                uart_set_format(uart, 8, 2,UART_PARITY_EVEN);
                break;
            case SERIAL_5O1: 
                uart_set_format(uart, 5, 1,UART_PARITY_ODD);
                break;
            case SERIAL_6O1:
                uart_set_format(uart, 6, 1,UART_PARITY_ODD);
                break;
            case SERIAL_7O1:
                uart_set_format(uart, 7, 1,UART_PARITY_ODD);
                break;
            case SERIAL_8O1:
                uart_set_format(uart, 8, 1,UART_PARITY_ODD);
                break;
            case SERIAL_5O2:
                uart_set_format(uart, 5, 2,UART_PARITY_ODD);
                break;
            case SERIAL_6O2:
                uart_set_format(uart, 6, 2,UART_PARITY_ODD);
                break;
            case SERIAL_7O2:
                uart_set_format(uart, 7, 2,UART_PARITY_ODD);
                break;
            case SERIAL_8O2:
                uart_set_format(uart, 8, 2,UART_PARITY_ODD);
                break;
            };
        }

        void setupDefaultRxTxPins(){
            Logger.info("setupDefaultRxTxPins");
            // we use different pins for uart0 and uar1. We assign values only if it has not been defined in setup
            if (uart_no==0){
                if (rx_pin==-1) {
                    rx_pin = SERIAL1_RX;
                }
                if (tx_pin==-1){
                    tx_pin = SERIAL1_TX;
                }
            } else {
                if (rx_pin==-1){
                    rx_pin = SERIAL2_RX;
                }
                if (tx_pin==-1){
                    tx_pin = SERIAL2_TX;
                }
            }
            // display pin assignments
            if (Logger.isLogging()) {
                Logger.printf(PicoLogger::Info, "Using UART: %d \n", uart_no);
                Logger.printf(PicoLogger::Info,"txPin is %d\n", tx_pin);
                Logger.printf(PicoLogger::Info,"rxPin is %d\n", rx_pin);
            }
            if (tx_pin!=-1) {
                gpio_set_function(tx_pin, GPIO_FUNC_UART);
            }
            if (rx_pin!=-1){
                gpio_set_function(rx_pin, GPIO_FUNC_UART);
            }

        }


};

// Standard Arduino global variables
inline PicoSerialUART Serial1(0);
inline PicoSerialUART Serial2(1); 

}

