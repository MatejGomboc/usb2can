#pragma once

#include <cstdint>

namespace CortexM0Plus::Mpu {
    static uint32_t* const BASE_ADDR = reinterpret_cast<uint32_t*>(0xE000ED90);

    // MPU Type Register bitfields
    union Type {
        struct Bits {
            uint32_t separate: 1;     //!< 0: unified MPU
            uint32_t reserved0: 7;    
            uint32_t dregion: 8;      //!< Number of supported MPU regions
            uint32_t iregion: 8;      //!< Number of supported MPU instruction regions
            uint32_t reserved1: 8;
        } bits;
        uint32_t value = 0;

        Type() = default;
        explicit Type(uint32_t new_value) { value = new_value; }
    };

    // MPU Control Register bitfields
    union Control {
        struct Bits {
            uint32_t enable: 1;           //!< Enables the MPU
            uint32_t hfnmiena: 1;         //!< Enables MPU during hard fault, NMI, and FAULTMASK handlers
            uint32_t privdefena: 1;       //!< Enables privileged software access to default memory map
            uint32_t reserved: 29;
        } bits;
        uint32_t value = 0;

        Control() = default;
        explicit Control(uint32_t new_value) { value = new_value; }
    };

    // MPU Region Base Address Register bitfields
    union RegionBaseAddress {
        struct Bits {
            uint32_t region: 4;       //!< MPU region field
            uint32_t valid: 1;        //!< MPU region number valid bit
            uint32_t addr: 27;        //!< Region base address field
        } bits;
        uint32_t value = 0;

        RegionBaseAddress() = default;
        explicit RegionBaseAddress(uint32_t new_value) { value = new_value; }
    };

    // MPU Region Attribute and Size Register bitfields
    union RegionAttributeSize {
        enum class AccessPermission : uint8_t {
            NO_ACCESS = 0x0,          //!< No access
            PRIV_RW = 0x1,           //!< Privileged access only, read-write
            PRIV_RW_UNPRIV_RO = 0x2, //!< Privileged read-write, unprivileged read-only
            FULL_ACCESS = 0x3,        //!< Full access
            RESERVED = 0x4,           //!< Reserved
            PRIV_RO = 0x5,           //!< Privileged read-only
            PRIV_RO_UNPRIV_RO = 0x6  //!< Read-only for both privileged and unprivileged
        };

        enum class MemoryType : uint8_t {
            STRONGLY_ORDERED = 0x0,   //!< Strongly ordered
            DEVICE = 0x1,             //!< Device
            NORMAL = 0x2,             //!< Normal memory
            RESERVED = 0x3            //!< Reserved
        };

        struct Bits {
            uint32_t enable: 1;           //!< Region enable bit
            uint32_t size: 5;             //!< Region size field (2^(size+1) bytes)
            uint32_t reserved0: 2;
            uint32_t srd: 8;              //!< Subregion disable bits
            uint32_t type: 2;             //!< Memory type
            uint32_t reserved1: 3;
            uint32_t access_perm: 3;      //!< Access permission field
            uint32_t xn: 1;               //!< Instruction access disable bit
            uint32_t reserved2: 7;
        } bits;
        uint32_t value = 0;

        RegionAttributeSize() = default;
        explicit RegionAttributeSize(uint32_t new_value) { value = new_value; }
    };

    struct Registers {
        volatile Type type;                           //!< MPU Type Register
        volatile Control control;                     //!< MPU Control Register
        volatile uint32_t region_number;             //!< MPU Region Number Register
        volatile RegionBaseAddress region_base_addr; //!< MPU Region Base Address Register
        volatile RegionAttributeSize region_attr;    //!< MPU Region Attribute and Size Register
    };

    static inline Registers* registers() {
        return reinterpret_cast<Registers*>(BASE_ADDR);
    }

    //! Configure an MPU region
    static inline void configure_region(
        uint8_t region,                              //!< Region number (0-7)
        uint32_t base_addr,                          //!< Base address of the region
        uint8_t size,                                //!< Size of region (2^(size+1) bytes)
        RegionAttributeSize::AccessPermission perm,  //!< Access permissions
        RegionAttributeSize::MemoryType mem_type,    //!< Memory type
        bool execute_never = false,                  //!< Whether to disable instruction fetches
        uint8_t subregion_disable = 0               //!< Subregion disable bits
    ) {
        // Set region number
        registers()->region_number = region;

        // Configure base address
        RegionBaseAddress addr;
        addr.bits.addr = base_addr >> 5;
        addr.bits.valid = 1;
        addr.bits.region = region;
        registers()->region_base_addr = addr;

        // Configure attributes
        RegionAttributeSize attr;
        attr.bits.enable = 1;
        attr.bits.size = size;
        attr.bits.srd = subregion_disable;
        attr.bits.type = static_cast<uint32_t>(mem_type);
        attr.bits.access_perm = static_cast<uint32_t>(perm);
        attr.bits.xn = execute_never ? 1 : 0;
        registers()->region_attr = attr;
    }

    //! Enable the MPU with default privileged access
    static inline void enable(bool enable_privdefena = true, bool enable_hfnmi = false) {
        Control ctrl;
        ctrl.bits.enable = 1;
        ctrl.bits.privdefena = enable_privdefena ? 1 : 0;
        ctrl.bits.hfnmiena = enable_hfnmi ? 1 : 0;
        registers()->control = ctrl;
    }

    //! Disable the MPU
    static inline void disable() {
        Control ctrl;
        ctrl.bits.enable = 0;
        registers()->control = ctrl;
    }
}