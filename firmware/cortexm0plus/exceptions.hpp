#pragma once

#include <cstdint>

namespace CortexM0Plus {
    static constexpr uint8_t NUM_OF_IRQS = 32;

    enum class ExceptionNumber : uint8_t {
        THREAD_MODE = 0,
        NMI = 2,
        HARD_FAULT = 3,
        SVC_CALL = 11,
        PEND_SV = 14,
        SYS_TICK = 15,
        FIRST_IRQ = 16,
        LAST_IRQ = FIRST_IRQ + NUM_OF_IRQS - 1
    };

    static inline void enableExceptions()
    {
        asm volatile("cpsie i" : : : "memory");
    }

    static inline void disableExceptions()
    {
        asm volatile("cpsid i" : : : "memory");
    }
}
