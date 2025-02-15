#pragma once

#include <cstdint>

namespace CortexM0Plus::Mpu {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000ED90);

    union Type {
        struct Bits {
            uint32_t separate_regions: 1; //!< support for unified/separate instruction and data regions
            uint32_t reserved0: 7;
            uint32_t num_of_data_regions: 8; //!< number of supported data regions
            uint32_t num_of_instr_regions: 8; //!< number of supported instruction regions
            uint32_t reserved1: 8;
        } bits;

        uint32_t value = 0;
    };

    union Control {
        struct Bits {
            uint32_t enabled: 1; //!< enables the MPU
            uint32_t enabled_in_hardfault_nmi: 1; //!< enables MPU during hard fault and NMI exception handlers
            uint32_t enabled_default_map: 1; //!< enables use of the default memory map as a background region for privileged software accesses
            uint32_t reserved: 29;
        } bits;

        uint32_t value = 0;
    };

    union RegionBaseAddress {
        struct Bits {
            uint32_t region_idx: 4; //!< index of the selected region
            uint32_t use_region_idx: 1; //!< use the value of region_idx field to select the region and also update value of region index register
            uint32_t region_base_addr: 27; //!< base address of the selected region
        } bits;

        uint32_t value = 0;
    };

    union RegionAttributes {
        enum class AccessPermission : uint8_t {
            NO_ACCESS = 0b000, //!< no access for any privilege level
            PRIV_RW = 0b001, //!< privileged access only, read-write
            PRIV_RW_UNPRIV_RO = 0b010, //!< privileged read-write, unprivileged read-only
            RW = 0b011, //!< full read-write access
            RESERVED = 0b100, //!< reserved
            PRIV_RO = 0b101, //!< privileged read-only
            RO1 = 0b110, //!< read-only for all privilege levels
            RO2 = 0b111 //!< read-only for all privilege levels
        };

        enum class MemoryType : uint8_t {
            PERIPHERAL = 0b011, //!< device peripherals
            FLASH = 0b100, //!< flash memory
            INTERN_SRAM = 0b101, //!< internal SRAM
            EXTERN_SRAM = 0b111 //!< external SRAM
        };

        struct Bits {
            uint32_t enabled: 1; //!< region enabled
            uint32_t size_exp: 5; //!< region size exponent (size = 2^(size_exp + 1) bytes)
            uint32_t reserved0: 2;
            uint32_t disabled_subregions: 8; //!< bitmap of disabled sub-regions
            uint32_t memory_type: 3; //!< memory type of the region
            uint32_t reserved1: 5;
            uint32_t access_permission: 3; //!< region access permission
            uint32_t reserved2: 1;
            uint32_t instrs_fetch_disabled: 1; //!< instruction fetches disabled
            uint32_t reserved3: 3;
        } bits;

        uint32_t value = 0;
    };

    struct Registers {
        volatile uint32_t type; //!< indicates whether the MPU is present, and if so, how many regions it supports
        volatile uint32_t control; //!< MPU control register
        volatile uint32_t region_idx; //!< region index register (selects a region to edit its attributes)
        volatile uint32_t region_base_address; //!< region base address register
        volatile uint32_t region_attributes; //!< region attributes register
    };

    static inline Registers* registers()
    {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }

    static inline void configureRegion(uint32_t idx, uint32_t base_addr, const RegionAttributes& attributes)
    {
        registers()->region_idx = idx;
        registers()->region_base_address = (base_addr & 0x7FFFFFF) << 5;
        registers()->region_attributes = attributes.value;
        asm volatile("DSB" : : : "memory");
        asm volatile("ISB" : : : "memory");
    }
}
