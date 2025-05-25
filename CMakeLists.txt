cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

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
