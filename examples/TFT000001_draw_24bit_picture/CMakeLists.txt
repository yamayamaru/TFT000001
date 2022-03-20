cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(TFT000001_draw_24bit_picture_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(
       TFT000001_draw_24bit_picture  TFT000001_draw_24bit_picture.cpp 
                                     TFT000001.cpp
)

pico_generate_pio_header(TFT000001_draw_24bit_picture
                       ${CMAKE_CURRENT_LIST_DIR}/pio_8bit_parallel.pio)

target_link_libraries(TFT000001_draw_24bit_picture
                       pico_stdlib
                       hardware_spi
                       hardware_pio
)

pico_enable_stdio_usb(TFT000001_draw_24bit_picture 0)
pico_enable_stdio_uart(TFT000001_draw_24bit_picture 1)

pico_add_extra_outputs(TFT000001_draw_24bit_picture)

