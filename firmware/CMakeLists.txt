cmake_minimum_required(VERSION 3.24.2)

project(motor_driver LANGUAGES ASM C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
        -fstack-usage
        -Wmissing-include-dirs
        -masm-syntax-unified
    >

    $<$<COMPILE_LANGUAGE:C>:
        -mcpu=cortex-m0plus
        -std=gnu18
        -ffunction-sections
        -fdata-sections
        -Wall
        -Wextra
        -pedantic
        -Wmissing-include-dirs
        -Wswitch-enum
        -Wconversion
        -fstack-usage
        -nostdlib
        -mfloat-abi=soft
        -mthumb
        -ffreestanding
        -masm-syntax-unified
    >

    $<$<COMPILE_LANGUAGE:CXX>:
        -mcpu=cortex-m0plus
        -std=gnu++20
        -ffunction-sections
        -fdata-sections
        -fno-exceptions
        -fno-rtti
        -fno-use-cxa-atexit
        -Wall
        -Wextra
        -pedantic
        -Wmissing-include-dirs
        -Wswitch-enum
        -Wconversion
        -fstack-usage
        -nostdlib
        -mfloat-abi=soft
        -mthumb
        -ffreestanding
        -fno-threadsafe-statics
        -masm-syntax-unified
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m0plus
    -T${PROJECT_SOURCE_DIR}/linker_script.ld
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -nostdlib
    -Wl,--cref
    -Wl,--gc-sections
    -static
    -Wl,--print-memory-usage
    -mfloat-abi=soft
    -mthumb
)

add_custom_target(${PROJECT_NAME}_size_report
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_SIZE}" -A ${PROJECT_NAME}.elf
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(${PROJECT_NAME}_bin
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_OBJCOPY}" --gap-fill 0 -O binary -S ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    BYPRODUCTS ${PROJECT_NAME}.bin
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(${PROJECT_NAME}_asm
    ALL
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND "${CMAKE_OBJDUMP}" -S -d -l -C -z ${PROJECT_NAME}.elf > ${PROJECT_NAME}.asm
    BYPRODUCTS ${PROJECT_NAME}.asm
    VERBATIM
    DEPENDS ${PROJECT_NAME}.elf
)

add_dependencies(${PROJECT_NAME}_size_report
    ${PROJECT_NAME}
)

add_dependencies(${PROJECT_NAME}_bin
    ${PROJECT_NAME}
)

add_dependencies(${PROJECT_NAME}_asm
    ${PROJECT_NAME}
)
