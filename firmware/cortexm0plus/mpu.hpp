#pragma once

#include <cstdint>

namespace CortexM0Plus::Mpu {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000ED90);

    union CtrlStatus {
        //! timer clock source
        enum class ClkSource : bool {
            EXTERNAL = false, //!< external reference clock
            CPU = true //!< processor clock
        };

        struct Bits {
            uint32_t timer_enabled: 1; //!< enables counting
            uint32_t exception_enabled: 1; //!< enables SysTick exception
            uint32_t clk_source: 1; //!< selects the timer clock source
            uint32_t reserved0: 13;
            uint32_t reached_zero: 1; //!< '1' if counter reached 0 since last time this bit was read
            uint32_t reserved1: 15;
        } bits;

        uint32_t value = 0;

        CtrlStatus() = default;

        CtrlStatus(uint32_t new_value)
        {
            value = new_value;
        }
    };

    struct Registers
    {
        volatile uint32_t type; //!< indicates whether the MPU is present, and if so, how many regions it supports
        volatile uint32_t control; //!< MPU control register
        volatile uint32_t region_number; //!< selects which memory region is referenced by the region_base_address and region_attribute_and_size registers
        volatile uint32_t region_base_address; //!< defines the base address of the region selected by the region_number register
        volatile uint32_t region_attribute_and_size; //!< defines the region size and memory attributes of the region specified by the region_number register
    };

    static inline Registers* registers()
    {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }
}
