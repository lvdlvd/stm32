/*
  This file assumes FLASHROM, CODE, SRAM2 have been defined before inclusion.
*/ 
ENTRY(Reset_Handler)
EXTERN(vector_table)  /* force inclusion of vector.c::vector_table in the isr_vector segment */

SECTIONS
{

  .isr_vector :
  {

    . = ALIGN(4);
    PROVIDE (isr_vector_table_base = .);
    KEEP(*(.isr_vector)) /* Startup code */

    . = ALIGN(4);
  }  >FLASHROM

  .text :
  {
    . = ALIGN(4);

    *(.text)
    *(.text.*)
    *(.rodata)       
    *(.rodata.*)

    . = ALIGN(4);
    _etext = .;

  } >FLASHROM

  _sidata = .;

  .data :
  {
    . = ALIGN(4);
    _sdata = .;

    *(.data)
    *(SORT_BY_ALIGNMENT(.data.*))

    . = ALIGN(4);
    _edata = .;
  
  } >SRAM1 AT> FLASHROM

  _eidata = _sidata + (_edata - _sdata);

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;

    *(.bss)
    *(SORT_BY_ALIGNMENT(.bss.*))
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;

  } >SRAM1

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
    *(.ARM.exidx)   /* not removing this messes with the data segment initialisation */
    *(.ARM.exidx.*)   /* not removing this messes with the data segment initialisation */
  }

}
