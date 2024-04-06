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

    __PROCESS_STACK_BOTTOM__ = __GLOBAL_STACK_BOTTOM__;
    __PROCESS_STACK_SIZE__ = DEFINED(__PROCESS_STACK_SIZE__) ? __PROCESS_STACK_SIZE__ : 0;
    __PROCESS_STACK_TOP__ = __PROCESS_STACK_BOTTOM__ + __PROCESS_STACK_SIZE__;

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
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        *(.gnu.linkonce.t.*)

        KEEP(*(.init));
        KEEP(*(.fini));

        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        . = ALIGN(4);
    } > FLASH

    .rodata : {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)

        . = ALIGN(4);
    } > FLASH

    .ARM.extab : {
        . = ALIGN(4);
        *(.ARM.extab*)
        *(.gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx : {
        . = ALIGN(4);
        __exidx_start = .;

        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)

        . = ALIGN(4);
        __exidx_end = .;
    } > FLASH

    .preinit_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__PREINIT_ARRAY_START__ = .);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array*));
        PROVIDE_HIDDEN(__PREINIT_ARRAY_END__ = .);
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } > FLASH

    .init_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__INIT_ARRAY_START__ = .);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)));
        KEEP(*(.init_array*));
        PROVIDE_HIDDEN(__INIT_ARRAY_END__ = .);
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } > FLASH

	.fini_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__FINI_ARRAY_START__ = .);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)));
        KEEP(*(.fini_array*));
        PROVIDE_HIDDEN(__FINI_ARRAY_END__ = .);
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } > FLASH

    .data : {
        . = ALIGN(4);
        __DATA_START__ = .;

        *(.data)
        *(.data*)
        *(.RamFunc)
        *(.RamFunc*)

        . = ALIGN(4);
        __DATA_END__ = .;
    } > RAM AT> FLASH

    .bss : {
        . = ALIGN(4);
        __BSS_START__ = .;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        __BSS_END__ = .;

        PROVIDE(end = .);
        PROVIDE(_end = .);
    } > RAM

    /DISCARD/ : {
        libc.a(*)
        libm.a(*)
        libgcc.a(*)
    }

    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
}
