#pragma once

#include <cstdint>

namespace mpu {

/**
 * @brief MPU region size values
 */
enum class RegionSize : uint8_t {
    BYTES_32 = 4,      ///< 32 bytes
    BYTES_64 = 5,      ///< 64 bytes
    BYTES_128 = 6,     ///< 128 bytes
    BYTES_256 = 7,     ///< 256 bytes
    BYTES_512 = 8,     ///< 512 bytes
    KBYTES_1 = 9,      ///< 1KB
    KBYTES_2 = 10,     ///< 2KB
    KBYTES_4 = 11,     ///< 4KB
    KBYTES_8 = 12,     ///< 8KB
    KBYTES_16 = 13,    ///< 16KB
    KBYTES_32 = 14,    ///< 32KB
    KBYTES_64 = 15,    ///< 64KB
    KBYTES_128 = 16,   ///< 128KB
    KBYTES_256 = 17,   ///< 256KB
    KBYTES_512 = 18,   ///< 512KB
    MBYTES_1 = 19,     ///< 1MB
    MBYTES_2 = 20,     ///< 2MB
    MBYTES_4 = 21,     ///< 4MB
    MBYTES_8 = 22,     ///< 8MB
    MBYTES_16 = 23,    ///< 16MB
    MBYTES_32 = 24,    ///< 32MB
    MBYTES_64 = 25,    ///< 64MB
    MBYTES_128 = 26,   ///< 128MB
    MBYTES_256 = 27,   ///< 256MB
    MBYTES_512 = 28,   ///< 512MB
    GBYTES_1 = 29,     ///< 1GB
    GBYTES_2 = 30,     ///< 2GB
    GBYTES_4 = 31      ///< 4GB
};

/**
 * @brief MPU access permission values
 */
enum class AccessPermission : uint8_t {
    NO_ACCESS = 0,                     ///< No access for any privilege level
    PRIV_RW = 1,                      ///< Privileged access only, read-write
    PRIV_RW_UNPRIV_RO = 2,           ///< Privileged read-write, unprivileged read-only
    RW = 3,                           ///< Full read-write access at any privilege
    RESERVED = 4,                     ///< Reserved - do not use
    PRIV_RO = 5,                      ///< Privileged read-only
    PRIV_RO_UNPRIV_RO = 6,           ///< Read-only at any privilege level
};

/**
 * @brief MPU memory attributes
 */
enum class MemoryType : uint8_t {
    STRONGLY_ORDERED = 0,    ///< Strongly ordered memory
    DEVICE = 1,             ///< Device memory
    NORMAL = 2,             ///< Normal memory
    RESERVED = 3            ///< Reserved - do not use
};

/**
 * @brief MPU register structure
 */
struct MPU_Type {
    volatile uint32_t TYPE;      ///< Type Register
    volatile uint32_t CTRL;      ///< Control Register
    volatile uint32_t RNR;       ///< Region Number Register
    volatile uint32_t RBAR;      ///< Region Base Address Register
    volatile uint32_t RASR;      ///< Region Attribute and Size Register
};

// MPU register bit definitions
constexpr uint32_t MPU_CTRL_ENABLE = (1UL << 0);           ///< MPU enable bit
constexpr uint32_t MPU_CTRL_HFNMIENA = (1UL << 1);        ///< MPU enable during hard fault, NMI, and FAULTMASK handlers
constexpr uint32_t MPU_CTRL_PRIVDEFENA = (1UL << 2);      ///< Enable privileged default memory map

// Base address of MPU registers
constexpr uintptr_t MPU_BASE = 0xE000ED90UL;

// MPU instance
#define MPU     ((MPU_Type *)MPU_BASE)

/**
 * @brief Configure an MPU region
 * 
 * @param region Region number (0-7)
 * @param addr Base address for the region
 * @param size Size of the region
 * @param ap Access permissions
 * @param memType Memory type
 * @param xn Execute never flag
 * @param enable Enable the region
 */
inline void configure_region(uint32_t region, 
                           uint32_t addr, 
                           RegionSize size,
                           AccessPermission ap,
                           MemoryType memType,
                           bool xn = false,
                           bool enable = true) {
    // Set region number
    MPU->RNR = region & 0xFF;
    
    // Set region base address (must be aligned to size)
    MPU->RBAR = addr & ~((1UL << static_cast<uint32_t>(size)) - 1);
    
    // Configure region attributes
    MPU->RASR = ((static_cast<uint32_t>(size) << 1) & 0x3E)           | // Region size
                ((static_cast<uint32_t>(ap) << 24) & 0x07000000)       | // Access permissions
                ((static_cast<uint32_t>(memType) << 16) & 0x70000)     | // Memory type
                ((xn ? 1UL : 0UL) << 28)                               | // Execute never
                ((enable ? 1UL : 0UL) << 0);                             // Region enable
}

/**
 * @brief Enable the MPU
 * 
 * @param enableDefaultMap Enable privileged default memory map
 * @param enableNMI Enable MPU during hard fault, NMI, and FAULTMASK
 */
inline void enable(bool enableDefaultMap = true, bool enableNMI = true) {
    MPU->CTRL = MPU_CTRL_ENABLE |
                (enableDefaultMap ? MPU_CTRL_PRIVDEFENA : 0) |
                (enableNMI ? MPU_CTRL_HFNMIENA : 0);
    
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

/**
 * @brief Disable the MPU
 */
inline void disable() {
    __asm volatile ("dmb");
    
    MPU->CTRL = 0;
    
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

} // namespace mpu