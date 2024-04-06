#pragma once

#include <cstdint>

namespace CortexM0Plus::Scb {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000ED00);

    //! is a read-only register and contains the processor part number, version, and implementation information
    union Cpuid {
        struct Bits {
            uint32_t revision: 4; //!< the p value in the Rnpn product revision identifier, indicates patch release (0x1: patch 1)
            uint32_t part_no: 12; //!< part number of the processor (0xC60: Cortex-M0+)
            uint32_t constant: 4; //!< constant that defines the architecture of the processor (0xC: ARMv6-M architecture)
            uint32_t variant: 4; //!< variant number: the r value in the Rnpn product revision identifier (0x0: revision 0)
            uint32_t implementer: 4; //!< implementer code (0x41: ARM)
        } bits;

        uint32_t value = 0;

        Cpuid() = default;

        Cpuid(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! interrupt control and state register
    union Icsr {
        struct Bits {
            uint32_t active_exception: 9; //!< exception number of the currently active exception
            uint32_t reserved0: 3;
            uint32_t vect_pending: 9;
            uint32_t reserved1: 1;


            uint32_t isr_pending: 1;
            uint32_t isr_preempt: 1;
            uint32_t reserved2: 1;
            uint32_t pend_sys_tick_clr: 1; //!< removes the pending state from the SysTick exception
            uint32_t pend_sys_tick_set: 1; //!< changes SysTick exception state to pending
            uint32_t pend_sv_clr: 1; //!< removes the pending state from the PendSV exception
            uint32_t pend_sv_set: 1; //!< change PendSV exception state to pending
            uint32_t reserved3: 2;
            uint32_t nmi_pend_set: 1; //!< changes NMI exception state to pending
        } bits;

        uint32_t value = 0;

        Icsr() = default;

        Icsr(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! enables system reset
    union Aircr {
        static constexpr uint16_t VECT_KEY = 0x05FA; //!< magic number used for enabling writing to Aircr

        struct Bits {
            uint32_t reserved0: 1;
            uint32_t vect_clr_active: 1; //!< reserved, write '0' to this bit
            uint32_t sys_reset_req: 1; //!< requests a system reset
            uint32_t reserved1: 11;
            uint32_t endianess: 1; //!< reads as 0 - little endian
            uint32_t vect_key: 16; //!< on writes, VECT_KEY to this field, otherwise the write is ignored
        } bits;

        uint32_t value = 0;

        Aircr() = default;

        Aircr(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! controls features of entry to and exit from low power state
    union Scr {
        struct Bits {
            uint32_t reserved0: 1;
            uint32_t sleep_on_isr_exit: 1; //!< enter sleep or deep sleep on return from ISR
            uint32_t use_deep_sleep: 1; //! use deep sleep as low power mode
            uint32_t reserved1: 1;

            //! 0: only enabled exceptions or events can wakeup the processor, disabled exceptions are excluded
            //! 1: enabled events and all exceptions, including disabled exceptions, can wakeup the processor
            uint32_t send_event_on_except_pending: 1;

            uint32_t reserved2: 28;
        } bits;

        uint32_t value = 0;

        Scr() = default;

        Scr(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! is a read-only register and indicates some aspects of the behavior of the processor
    union Ccr {
        struct Bits {
            uint32_t reserved0: 3;
            uint32_t fault_on_unaligned_access: 1; //!< always '1', indicates that all unaligned accesses generate a HardFault
            uint32_t reserved1: 4;
            uint32_t stack_alignment: 1; //!< always '1', indicates 8-byte stack alignment on exception entry
            uint32_t reserved2: 22;
        } bits;

        uint32_t value = 0;

        Ccr() = default;

        Ccr(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! sets the priority level of the exception handlers that have configurable priority (SVCall)
    union Shpr2 {
        struct Bits {
            uint32_t reserved0: 24;
            uint32_t sv_call_except_priority: 8; //!< priority of SVCall exception
        } bits;

        uint32_t value = 0;

        Shpr2() = default;

        Shpr2(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! sets the priority level of the exception handlers that have configurable priority (PendSV, SysTick)
    union Shpr3 {
        struct Bits {
            uint32_t reserved0: 16;
            uint32_t pend_sv_except_priority: 8; //!< priority of PendSV exception
            uint32_t sys_tick_except_priority: 8; //!< priority of SysTick exception
        } bits;

        uint32_t value = 0;

        Shpr3() = default;

        Shpr3(uint32_t new_value)
        {
            value = new_value;
        }
    };

    struct Registers
    {
        volatile uint32_t cpuid; //!< contains the processor part number, version, and implementation information
        volatile uint32_t icsr; //!< interrupt control and state register
        volatile uint32_t reserved0;
        volatile uint32_t aircr; //!< enables system reset
        volatile uint32_t scr; //!< controls features of entry to and exit from low power state
        volatile uint32_t ccr; //!< is a read-only register and indicates some aspects of the behavior of the processor
        volatile uint32_t reserved1;
        volatile uint32_t shpr2; //!< sets the priority level of the exception handlers that have configurable priority (SVCall)
        volatile uint32_t shpr3; //!< sets the priority level of the exception handlers that have configurable priority (PendSV, SysTick)
    };

    static inline Registers* registers()
    {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }

    __attribute__((noreturn)) static inline void systemReset()
    {
        asm volatile("DSB" : : : "memory");

        Aircr aircr { registers()->aircr };

        aircr.bits.sys_reset_req = true;
        aircr.bits.vect_key = Aircr::VECT_KEY;

        registers()->aircr = aircr.value;

        asm volatile("DSB" : : : "memory");

        while(true);
    }
}
