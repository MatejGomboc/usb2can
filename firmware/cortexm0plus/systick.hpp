#pragma once

#include <cstdint>

namespace CortexM0Plus::SysTick {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000E010);

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

    union Calibration {
        struct Bits {
            /*!
                Indicates the calibration value when the SysTick counter runs on HCLK max/8 as external clock.
                When HCLK is programmed at the maximum frequency, the SysTick period is 1ms.
            */
            uint32_t calib_val: 24;
            uint32_t reserved: 6;
            uint32_t skew: 1; //!< always '1'
            uint32_t noref: 1; //!< always '0', indicates that a separate reference clock is provided (HCLK/8)
        } bits;

        uint32_t value = 0;

        Calibration() = default;

        Calibration(uint32_t new_value)
        {
            value = new_value;
        }
    };

    struct Registers
    {
        volatile uint32_t ctrl_status; //!< control and status register
        volatile uint32_t reload_val; //!< reload value at the restart of counting
        volatile uint32_t current_val; //!< current counter value
        volatile uint32_t calibration; //!< controls timer calibration
    };

    static inline Registers* registers()
    {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }
}
