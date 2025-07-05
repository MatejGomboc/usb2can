cmake_minimum_required(VERSION 3.16...3.31.6)

# This project MUST use the ARM GCC toolchain - enforce it strictly
set(REQUIRED_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/firmware/ARMCortexM/arm_none_eabi_gcc.cmake")

# Check if the toolchain file exists
if(NOT EXISTS "${REQUIRED_TOOLCHAIN_FILE}")
    message(FATAL_ERROR 
        "Required ARM GCC toolchain file not found: ${REQUIRED_TOOLCHAIN_FILE}\n"
        "Make sure the ARMCortexM submodule is properly initialized.")
endif()

# Prevent toolchain override attempts
if(DEFINED CMAKE_TOOLCHAIN_FILE AND NOT "${CMAKE_TOOLCHAIN_FILE}" STREQUAL "${REQUIRED_TOOLCHAIN_FILE}")
    message(FATAL_ERROR 
        "This project is locked to use only the ARM GCC toolchain!\n"
        "Attempted toolchain: ${CMAKE_TOOLCHAIN_FILE}\n"
        "Required toolchain: ${REQUIRED_TOOLCHAIN_FILE}\n"
        "Remove any CMAKE_TOOLCHAIN_FILE specification and use only the provided presets.")
endif()

# Force the correct toolchain if not already set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${REQUIRED_TOOLCHAIN_FILE}" CACHE FILEPATH "Toolchain file" FORCE)
endif()

# Verify that the correct toolchain is being used after setting
if(NOT "${CMAKE_TOOLCHAIN_FILE}" STREQUAL "${REQUIRED_TOOLCHAIN_FILE}")
    message(FATAL_ERROR 
        "Toolchain enforcement failed!\n"
        "Current toolchain: ${CMAKE_TOOLCHAIN_FILE}\n"
        "Required toolchain: ${REQUIRED_TOOLCHAIN_FILE}")
endif()

# Additional compiler verification
macro(verify_arm_gcc_compiler)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER MATCHES "arm-none-eabi-gcc")
        # Correct ARM GCC compiler
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER MATCHES "arm-none-eabi-g\\+\\+")
        # Correct ARM G++ compiler
    else()
        if(DEFINED CMAKE_C_COMPILER)
            message(FATAL_ERROR 
                "Wrong compiler detected!\n"
                "This project requires arm-none-eabi-gcc toolchain.\n"
                "Current C compiler: ${CMAKE_C_COMPILER}\n"
                "Current C compiler ID: ${CMAKE_C_COMPILER_ID}\n"
                "Please use the provided CMake presets or ensure arm-none-eabi-gcc is in your PATH.")
        endif()
    endif()
endmacro()

project(usb2can_top
    LANGUAGES NONE
    VERSION 0.1.0
    DESCRIPTION "USB to CAN converter dongle - Top Level"
    HOMEPAGE_URL "https://github.com/MatejGomboc/usb2can"
)

# Enable the required languages after project() call
enable_language(C CXX ASM)

# Verify the compiler after languages are enabled
verify_arm_gcc_compiler()

# Add firmware subdirectory
add_subdirectory(firmware)

# Print confirmation message
message(STATUS "✓ ARM GCC toolchain enforcement: SUCCESS")
message(STATUS "✓ Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "✓ C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "✓ CXX Compiler: ${CMAKE_CXX_COMPILER}")
