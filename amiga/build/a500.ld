OUTPUT_ARCH(m68k)
ENTRY(_start)

SEARCH_DIR(.)
GROUP(-lc -lgcc)

/*
 * The program is linked to address $400, but this is effectively ignored since
 * all code and data accesses are PC relative. The program can be loaded at any
 * address. Stack pointer is assumed to be set by the loader.
 *
 * (stack)
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

SECTIONS
{
  .text :
  {
    __text_start = .;
    standalone.o(.text .text.*)
    *(.text .text.*)

    . = ALIGN(0x4);
    *(.rodata .rodata.*)

    . = ALIGN(0x4);
    __text_end = .;
  } > chip

  .data :
  {
    __data_start = .;
    *(.data .data.*)
    __data_end = .;

    /*
     * NOTE: Put input object file .bss sections in the .data output section.
     */
    . = ALIGN(0x4);
    __bss_start = . ;
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN (0x8);
    __bss_end = .;
  } > chip
}

