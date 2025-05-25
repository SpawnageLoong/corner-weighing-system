cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(corner_weighing)

pico_sdk_init()

add_executable(corner_weighing
    main.cpp
    hx711.cpp
    ssd1306.cpp
)

target_include_directories(corner_weighing PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(corner_weighing
    pico_stdlib
    hardware_i2c
)

pico_add_extra_outputs(corner_weighing)
