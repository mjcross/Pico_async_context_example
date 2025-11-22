# Generated Cmake Pico project file

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
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(async_context_example C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(async_context_example async_context_example.c )

pico_set_program_name(async_context_example "async_context_example")
pico_set_program_version(async_context_example "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(async_context_example 1)
pico_enable_stdio_usb(async_context_example 0)

# Add the standard include files to the build
target_include_directories(async_context_example PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add libraries for 
target_link_libraries(async_context_example
        pico_stdlib
        pico_async_context_threadsafe_background
        )

# Add libraries for Pico W and Pico 2_W
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(async_context_example 
        pico_cyw43_arch_none
        )
endif()

pico_add_extra_outputs(async_context_example)

