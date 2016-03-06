OUTPUT_ARCH(m68k)
ENTRY(_start)

SEARCH_DIR(.)
GROUP(-lc -lgcc)

/*
 * __stack
 * __bss_end
 * __bss_start
 * __data_end
 * __data_start
 * __text_end
 * __text_start
 */

/*
 * Describes A500 memory layout. First 1 KiB of address space is vector table.
 */
MEMORY
{
  vec   : ORIGIN = 0x00000000, LENGTH = 0x400
  chip  : ORIGIN = 0x00000400, LENGTH = 512K-0x400
  fast  : ORIGIN = 0x00200000, LENGTH = 8M
  slow  : ORIGIN = 0x00c00000, LENGTH = 1M
}

/*
 * head.s keeps the stack pointer if symbol __stack is zero.
 */
/* PROVIDE (__stack = 0); */
PROVIDE (__stack = 256K);

SECTIONS
{
  .text :
  {
    __text_start = .;
    head.o(.text .text.*)
    *(.text .text.*)

    . = ALIGN(0x4);
    *(.rodata .rodata.*)

    . = ALIGN(0x4);
    __text_end = .;
  } > chip

  .data :
  {
    . = ALIGN(0x4);
    __data_start = .;
    *(.data .data.*)

    . = ALIGN(0x4);
    __data_end = .;
  } > chip

  .bss :
  {
    . = ALIGN(0x4);
    __bss_start = . ;
    *(.bss .bss.*)
    *(COMMON)

    . = ALIGN (0x4);
    __bss_end = .;
  } > chip
}

