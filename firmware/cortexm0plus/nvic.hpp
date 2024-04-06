#pragma once

#include "utils.hpp"
#include <cstdint>

namespace CortexM0Plus::Nvic {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000E100);

    struct Registers
    {
        volatile uint32_t iser; //!< enables interrupts, and shows which interrupts are enabled
        volatile uint32_t reserved0[31];
        volatile uint32_t icer; //!< disables interrupts, and shows which interrupts are enabled
        volatile uint32_t reserved1[31];
        volatile uint32_t ispr; //!< forces interrupts into pending state, and shows which interrupts are pending
        volatile uint32_t reserved2[31];
        volatile uint32_t icpr; //!< removes pending state from interrupts and shows which interrupts are pending
        volatile uint32_t reserved3[31];
        volatile uint32_t reserved4[64];
        volatile uint32_t ipr[8]; //!< sets priorities of interrupts
    };

    static inline Registers* registers()
    {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }

    static inline bool isIrqEnabled(uint8_t irq_number)
    {
        return Utils::isBitSet(registers()->iser, irq_number & 0x1F);
    }

    static inline void enableIrq(uint8_t irq_number)
    {
        Utils::setBit(registers()->iser, irq_number & 0x1F);
    }

    static inline void disableIrq(uint8_t irq_number)
    {
        Utils::setBit(registers()->icer, irq_number & 0x1F);
        asm volatile("DSB" : : : "memory");
        asm volatile("ISB" : : : "memory");
    }

    static inline bool isIrqPending(uint8_t irq_number)
    {
        return Utils::isBitSet(registers()->ispr, irq_number & 0x1F);
    }

    static inline void setPendingIrq(uint8_t irq_number)
    {
        Utils::setBit(registers()->ispr, irq_number & 0x1F);
    }

    static inline void clearPendingIrq(uint8_t irq_number)
    {
        Utils::setBit(registers()->icpr, irq_number & 0x1F);
    }

    static inline void setIrqPriority(uint8_t irq_number, uint8_t irq_priority)
    {
        irq_number &= 0x1F;
        uint8_t reg_idx = irq_number / 4;
        uint8_t bit_shift = (irq_number % 4) * 8;
        uint32_t new_value = registers()->ipr[reg_idx];
        new_value &= ~(0x000000FF << bit_shift);
        new_value |= static_cast<uint32_t>(irq_priority) << bit_shift;
        registers()->ipr[reg_idx] = new_value;
    }

    static inline uint8_t getIrqPriority(uint8_t irq_number)
    {
        irq_number &= 0x1F;
        uint8_t reg_idx = irq_number / 4;
        uint8_t bit_shift = (irq_number % 4) * 8;
        return static_cast<uint8_t>((registers()->ipr[reg_idx] >> bit_shift) & 0x000000FF);
    }
}
