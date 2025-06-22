/*  The STM32G473 is a category 3 device with he following memory map */
MEMORY
{
  CODE     (rx)  : ORIGIN = 0x00000000, LENGTH = 80K  /* either flashrom or sram1 */
  FLASHROM (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  CCRAM    (rw)  : ORIGIN = 0x10000000, LENGTH = 32K /* Aliased at 0x2001 8000 */
  SRAM1    (rw)  : ORIGIN = 0x20000000, LENGTH = 80K 
  SRAM2    (rw)  : ORIGIN = 0x20014000, LENGTH = 16K /* can be kept in stop and stdby mode */
}

_estack = ORIGIN(CCRAM) + LENGTH(CCRAM); /* top of the stack */

INCLUDE devs.ld 

GPIO_ALL = GPIOA;                   /* required for gpio2.[ch] */
UNIQUE_DEVICE_ID = 0x1FFF7590;      /* ref manual 48.1 */
FDCAN_MSGBUF = FDCANs_Message_RAM;  /* required for fdcan.c */

TS_CAL1 = 0x1FFF75A8; /* ref man 21.4.31, datasheet 3.18.1 */
TS_CAL2 = 0x1FFF75CA;
VREFINT = 0x1FFF75AA; /* ref man 21.4.33, datasheet 3.18.2 */

INCLUDE sections.ld