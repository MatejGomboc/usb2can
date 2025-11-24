cmake_minimum_required(VERSION 3.13)

project(usb2can
    LANGUAGES ASM CXX
    VERSION 0.1.0
    DESCRIPTION "USB to CAN converter dongle"
    HOMEPAGE_URL "https://github.com/MatejGomboc/usb2can"
)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

set(SUPPORTED_BUILD_TYPES
    Debug
    MinSize
    MaxSpeed
)

set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_CXX_FLAGS_MINSIZE "-Os")
set(CMAKE_CXX_FLAGS_MAXSPEED "-O3")

if(IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "${SUPPORTED_BUILD_TYPES}" CACHE STRING "" FORCE)
    set(CMAKE_DEFAULT_BUILD_TYPE "MinSize" CACHE STRING "" FORCE)

else()
    if(NOT CMAKE_BUILD_TYPE)
        message(FATAL_ERROR "CMake build type not set")
    endif()

    if(NOT CMAKE_BUILD_TYPE IN_LIST SUPPORTED_BUILD_TYPES)
        message(FATAL_ERROR "Unsupported CMake build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ARM_CORTEX_M_ARCH "M0PLUS")

add_subdirectory(ARMCortexM)

add_executable(${PROJECT_NAME}
    main.cpp
    vectors_table.cpp
    reset_isr.cpp
    fault_isrs.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:
        -ffunction-sections
        -fdata-sections
        -fstack-usage
        -Wmissing-include-dirs
    >

    $<$<COMPILE_LANGUAGE:CXX>:
        # Language features
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
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    -T${PROJECT_SOURCE_DIR}/linker_script.ld
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -nostdlib
    -Wl,--gc-sections
    -Wl,--check-sections
    -static
    -Wl,--print-memory-usage
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ARMCortexM
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
