MEMORY
{
/*
- FLASHROM at 0x0800 0000  aliased at ITCMROM, the stm32F7xxxGyyy have 2MB, the stm32F7xxxIyyy only 1M.  This file maps only the first 512K
- SRAM1 mapped at address 0x2002 0000 and accessible by all AHB masters from AHB bus Matrix.
– SRAM2 mapped at address 0x2007 C000 and accessible by all AHB masters from AHB bus Matrix.
– DTCM-RAM mapped at address 0x2000 0000 and accessible by all AHB masters from AHB bus Matrix but through a specific AHB slave bus of the CPU.
– ITCM-RAM mapped at address 0x0000 0000 and accessible only by CPU.
*/
  ITCMRAM  (rwx) : ORIGIN = 0x00000000, LENGTH = 16K
  ITCMROM  (rx)  : ORIGIN = 0x00200000, LENGTH = 512K  /* alias for FLASHROM */  
  FLASHROM (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  DTCMRAM  (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
  SRAM1    (rw)  : ORIGIN = 0x20020000, LENGTH = 368K
  SRAM2    (rw)  : ORIGIN = 0x2007C000, LENGTH = 16K
  BKUPSRAM (rw)  : ORIGIN = 0x40024000, LENGTH = 4K 
}

INCLUDE devs.ld

GPIO_ALL = GPIOA;  /* needed for our gpio2.[ch] code */
UNIQUE_DEVICE_ID = 0x1FF0F420;  /* ref manual 45.1 */
FLASH_SIZE       = 0x1FF0F442;  /* ref manual 45.2 */

/* Highest address of the user mode stack */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);    /* end of DTCMRAM */


ENTRY(Reset_Handler)   /* not used by any tool, as far as i know */

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASHROM */
  .isr_vector :
  {
    . = ALIGN(4);
    PROVIDE (isr_vector_table_base = .);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >ITCMROM AT>FLASHROM

  /* The program code and other data goes into ITCMROM, aliased at FLASHROM */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)       /* exception handler stack unroll table */

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >ITCMROM AT> FLASHROM 

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ITCMROM AT> FLASHROM
  .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } >ITCMROM AT> FLASHROM

/* TODO(lvd) I don't think any of these are used for a plain C binary.

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >ITCMROM AT> FLASHROM
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >ITCMROM AT> FLASHROM
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ITCMROM AT> FLASHROM
  .pg_registry :
  {
    PROVIDE_HIDDEN (__pg_registry_start = .);
    KEEP (*(.pg_registry))
    KEEP (*(SORT(.pg_registry.*)))
    PROVIDE_HIDDEN (__pg_registry_end = .);
  } >ITCMROM AT> FLASHROM
  .pg_resetdata :
  {
    PROVIDE_HIDDEN (__pg_resetdata_start = .);
    KEEP (*(.pg_resetdata))
    PROVIDE_HIDDEN (__pg_resetdata_end = .);
  } >ITCMROM AT> FLASHROM

*/

  /* used by the startup to initialize data */
  _sidata = .;

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> FLASHROM

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(SORT_BY_ALIGNMENT(.bss*))
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCMRAM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

