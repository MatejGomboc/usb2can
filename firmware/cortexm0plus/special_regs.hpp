#pragma once

#include <cstdint>

namespace CortexM0Plus {
    //! the following values are saved into LR on exception entry
    enum class LrExecepReturn : uint32_t {
        HANDLER = 0xFFFFFFF1, //!< return to handler mode, uses MSP after return
        THREAD_MSP = 0xFFFFFFF9, //!< return to thread mode, uses MSP after return
        THREAD_PSP = 0xFFFFFFFD //!< return to thread mode, uses PSP after return
    };

    //! program status register
    union Psr {
        struct Bits {
            uint32_t current_exception: 6; //!< number of the currently executing exception
            uint32_t reserved0: 18;
            uint32_t thumb_mode: 1; //!< CPU running in Thumb mode
            uint32_t reserved1: 3;
            uint32_t v: 1; //!< overflow flag
            uint32_t c: 1; //!< carry or borrow flag
            uint32_t z: 1; //!< zero flag
            uint32_t n: 1; //!< negative or less than flag
        } bits;

        uint32_t value = 0;

        Psr() = default;

        Psr(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! priority mask register
    union Primask {
        struct Bits {
            uint32_t exceptions_disabled: 1; //!< all exceptions except NMI and hard fault are disabled
            uint32_t reserved: 31;
        } bits;

        uint32_t value = 0;

        Primask() = default;

        Primask(uint32_t new_value)
        {
            value = new_value;
        }
    };

    //! control register
    union Control {
        //! thread mode privilege level
        enum class ThreadModePrivilegeLevel : bool {
            PRIVILEGED = false, //!< privileged thread mode
            UNPRIVILEGED = true //!< unprivileged thread mode
        };

        //! currently used stack pointer
        enum class StackPointer : bool {
            MSP = false, //!< main stack pointer
            PSP = true //!< process stack pointer
        };

        struct Bits {
            uint32_t thread_mode_privilege_level: 1; //!< thread mode privilege level
            uint32_t active_stack_pointer: 1; //!< currently used stack pointer
            uint32_t reserved1: 30;
        } bits;

        uint32_t value = 0;

        Control() = default;

        Control(uint32_t new_value)
        {
            value = new_value;
        }
    };

    static inline uint32_t getLr()
    {
        uint32_t value;
        asm volatile("MOV %0, LR" : "=r" (value) : : );
        return value;
    }

    static inline Psr getApsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, APSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getIpsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, IPSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getEpsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, EPSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getIepsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, IEPSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getIapsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, IAPSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getEapsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, EAPSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline Psr getPsrReg()
    {
        Psr psr;
        asm volatile("MRS %0, PSR" : "=r" (psr.value) : : );
        return psr;
    }

    static inline uint32_t getMspReg()
    {
        uint32_t value;
        asm volatile("MRS %0, MSP" : "=r" (value) : : );
        return value;
    }

    static inline void setMspReg(uint32_t value)
    {
        asm volatile("MSR MSP, %0" : : "r" (value) : );
    }

    static inline uint32_t getPspReg()
    {
        uint32_t value;
        asm volatile("MRS %0, PSP" : "=r" (value) : : );
        return value;
    }

    static inline void setPspReg(uint32_t value)
    {
        asm volatile("MSR PSP, %0" : : "r" (value) : );
    }

    static inline Primask getPrimaskReg()
    {
        Primask primask;
        asm volatile("MRS %0, PRIMASK" : "=r" (primask.value) : : );
        return primask;
    }

    static inline void setPrimaskReg(Primask primask)
    {
        asm volatile("MSR PRIMASK, %0" : : "r" (primask.value) : "memory");
    }

    static inline Control getControlReg()
    {
        Control control;
        asm volatile("MRS %0, CONTROL" : "=r" (control.value) : : );
        return control;
    }

    static inline void setControlReg(Control control)
    {
        asm volatile("MSR CONTROL, %0" : : "r" (control.value) : "memory");
        asm volatile("ISB" : : : "memory");
    }
}
