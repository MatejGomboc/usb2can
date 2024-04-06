#include <cstdint>

static constexpr uint32_t STACK_FREE_SPACE_IDICATOR = 0xDEADDEAD;

extern uint32_t __DATA_START__;
extern uint32_t __DATA_END__;
extern uint32_t __DATA_FLASH_START__;

extern uint32_t __BSS_START__;
extern uint32_t __BSS_END__;

extern void (* const __INIT_ARRAY_START__[])();
extern void (* const __INIT_ARRAY_END__[])();

extern uint32_t __GLOBAL_STACK_BOTTOM__;
extern uint32_t __GLOBAL_STACK_TOP__;

extern int main();

extern "C" void resetIsr()
{
    // copy the initialized global variables to RAM
    uint32_t* addr_flash = &__DATA_FLASH_START__;
    uint32_t* addr_ram = &__DATA_START__;
    for (int32_t offset = 0; offset < &__DATA_END__ - &__DATA_START__; offset++) {
        *(addr_ram++) = *(addr_flash++);
    }

    // fill uninitialized variables in RAM with zeros
    for (uint32_t* addr = &__BSS_START__; addr < &__BSS_END__; addr++) {
        *addr = 0;
    }

    // initialize static objects by calling their constructors
    for (int32_t i = 0; i < __INIT_ARRAY_END__ - __INIT_ARRAY_START__; i++) {
        __INIT_ARRAY_START__[i]();
    }

    // overwrite the entire stack with free space indicator
    for (uint32_t* addr = &__GLOBAL_STACK_BOTTOM__; addr < &__GLOBAL_STACK_TOP__; addr++) {
        *addr = STACK_FREE_SPACE_IDICATOR;
    }

    // branch to main()
    main();
}
