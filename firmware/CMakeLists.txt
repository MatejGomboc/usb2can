cmake_minimum_required(VERSION 3.16)

project(usb2can LANGUAGES ASM CXX)

if(NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Build type not set")
endif()

set(SUPPORTED_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)

if(NOT CMAKE_BUILD_TYPE IN_LIST SUPPORTED_BUILD_TYPES)
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    main.cpp
    vectors_table.cpp
    reset_isr.cpp
    fault_isrs.cpp
    utils.hpp
    cortexm0plus/exceptions.hpp
    cortexm0plus/special_regs.hpp
    cortexm0plus/nvic.hpp
    cortexm0plus/scb.hpp
    cortexm0plus/systick.hpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:
        -mcpu=cortex-m0plus
        -mthumb
        -mfloat-abi=soft
        -ffunction-sections
        -fdata-sections
        -fstack-usage
        -Wmissing-include-dirs
        -masm-syntax-unified
    >

    $<$<COMPILE_LANGUAGE:CXX>:
        # Architecture
        -mcpu=cortex-m0plus
        -mthumb
        -mfloat-abi=soft

        # Language features
        -std=gnu++17
        -ffreestanding
        -nostdlib
        -fno-exceptions
        -fno-rtti
        -fno-use-cxa-atexit
        -fno-threadsafe-statics
        -fno-common
        -fno-unwind-tables

        # Optimization
        -ffunction-sections
        -fdata-sections
        -fstack-usage

        # Warnings
        -Wall
        -Wextra
        -pedantic
        -Wmissing-include-dirs
        -Wswitch-enum
        -Wconversion
        -Wdouble-promotion

        # Assembly
        -masm-syntax-unified
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m0plus
    -T${PROJECT_SOURCE_DIR}/linker_script.ld
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -nostdlib
    -Wl,--gc-sections
    -static
    -Wl,--print-memory-usage
    -mfloat-abi=soft
    -mthumb
)

# Print size report
add_custom_target(${PROJECT_NAME}_size_report
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_SIZE}" -A ${PROJECT_NAME}.elf
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

# Generate BIN file
add_custom_target(${PROJECT_NAME}_bin
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_OBJCOPY}" --gap-fill 0 -O binary -S ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    BYPRODUCTS ${PROJECT_NAME}.bin
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

# Generate dissassebly file
add_custom_target(${PROJECT_NAME}_asm
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_OBJDUMP}" -S -d -l -C -z ${PROJECT_NAME}.elf > ${PROJECT_NAME}.asm
    BYPRODUCTS ${PROJECT_NAME}.asm
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

add_dependencies(
    ${PROJECT_NAME}_size_report
    ${PROJECT_NAME}_bin
    ${PROJECT_NAME}_asm
    ${PROJECT_NAME}
)
