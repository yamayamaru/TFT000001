cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(mandelbrot_set_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(
       mandelbrot_set  mandelbrot_set.cpp 
                                                       TFT000001.cpp
)

if(DEFINED PICO_PLATFORM)
  if(PICO_PLATFORM STREQUAL "rp2040")
    pico_generate_pio_header(mandelbrot_set
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
  else()
    pico_generate_pio_header(mandelbrot_set
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
  endif()
else()
    pico_generate_pio_header(mandelbrot_set
                                    ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)
endif()

target_link_libraries(mandelbrot_set
                       pico_stdlib
                       hardware_spi
                       hardware_pio
)

pico_enable_stdio_usb(mandelbrot_set 1)
pico_enable_stdio_uart(mandelbrot_set 0)

pico_add_extra_outputs(mandelbrot_set)

