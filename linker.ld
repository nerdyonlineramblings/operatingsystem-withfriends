ENTRY(main)

SECTIONS {
    . = 0x7e00;

    .text.start ALIGN(4K) : {
        *(.text.start)
        *(.text*)
    }

    .rodata ALIGN(4K) : { *(.rodata*) }
    .data ALIGN(4K) : { *(.data*) }
    .bss ALIGN(4K) : { *(.bss*) }
}
