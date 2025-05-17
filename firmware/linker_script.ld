MEMORY {
    FLASH (rx)  : ORIGIN = 0x8000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 144K
}

OUTPUT_FORMAT("elf32-littlearm")

ENTRY(resetIsr)

SECTIONS {
    __DATA_FLASH_START__ = LOADADDR(.data);

    __GLOBAL_STACK_BOTTOM__ = ORIGIN(RAM) + SIZEOF(.data) + SIZEOF(.bss);
    __GLOBAL_STACK_SIZE__ = LENGTH(RAM) - SIZEOF(.data) - SIZEOF(.bss);
    __GLOBAL_STACK_TOP__ = ORIGIN(RAM) + LENGTH(RAM);

    ASSERT(__GLOBAL_STACK_SIZE__ > 0, "Error: No space remaining for stack!")

    __PROCESS_STACK_BOTTOM__ = __GLOBAL_STACK_BOTTOM__;
    __PROCESS_STACK_SIZE__ = DEFINED(__PROCESS_STACK_SIZE__) ? __PROCESS_STACK_SIZE__ : 0;
    __PROCESS_STACK_TOP__ = __PROCESS_STACK_BOTTOM__ + __PROCESS_STACK_SIZE__;

    ASSERT(__PROCESS_STACK_SIZE__ < __GLOBAL_STACK_SIZE__, "Error: Process stack size exceeds global stack size!")

    __MAIN_STACK_BOTTOM__ = __PROCESS_STACK_TOP__;
    __MAIN_STACK_SIZE__ =  __GLOBAL_STACK_SIZE__ - __PROCESS_STACK_SIZE__;
    __MAIN_STACK_TOP__ = __MAIN_STACK_BOTTOM__ + __MAIN_STACK_SIZE__;

    .vectors_table : {
        . = ALIGN(4);
        KEEP(*(.vectors_table));
        . = ALIGN(4);
    } > FLASH

    .text : {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.gnu.linkonce.t.*)
        . = ALIGN(4);
    } > FLASH

    .rodata : {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
    } > FLASH

    .init_array : {
        . = ALIGN(4);
        __INIT_ARRAY_START__ = .;
        KEEP(*(.init_array*));
        __INIT_ARRAY_END__ = .;
        . = ALIGN(4);
    } > FLASH

    .data : {
        . = ALIGN(4);
        __DATA_START__ = .;
        *(.data)
        *(.data*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(4);
        __DATA_END__ = .;
    } > RAM AT> FLASH

    .bss : {
        . = ALIGN(4);
        __BSS_START__ = .;
        *(.bss)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        . = ALIGN(4);
        __BSS_END__ = .;
    } > RAM

    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
}
