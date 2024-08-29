cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mexican_hat05_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(
       mexican_hat05  mexican_hat05.cpp 
                                                       TFT000001.cpp
)

if(DEFINED PICO_PLATFORM)
  if(PICO_PLATFORM STREQUAL "rp2040")
    pico_generate_pio_header(mexican_hat05
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
  else()
    pico_generate_pio_header(mexican_hat05
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
  endif()
else()
    pico_generate_pio_header(mexican_hat05
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
endif()

target_link_libraries(mexican_hat05
                       pico_stdlib
                       hardware_spi
                       hardware_pio
)

pico_enable_stdio_usb(mexican_hat05 0)
pico_enable_stdio_uart(mexican_hat05 1)

pico_add_extra_outputs(mexican_hat05)

