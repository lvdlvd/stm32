# stm32

WIP! not yet ready for production use.
with every new stm project i'm making changes to the output so it is not ready to have the output frozen.

the idea is to take SVD's as published by STM, and use them to generate a development environment
that is aviation grade and has zero external dependencies.

the SVD'files are quite buggy, so i've had to patch them up to get usable output.

the core of this system is to generate devs.ld stm32XXXX.h and vectors.c from the SVD, the rest is a family
of rather generic drivers for all the myriad of devices built in to those chips.  the generated code has
a very clean api that matches the programming reference manuals.  constants are enums, so they are known
to the debugger, and are named to allow for easy visual/regexp inspection on correct usage. eg

  SCB.SHCSR |= SCB_SHCSR_USGFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_MEMFAULTENA;

Devices are of type struct XXXX_Type, with the registers at the correct offset, and the addresses
known to the linker, so the compiler and linker generate very clean and optimal code.

TODO: 
- if there is only 1 device of a certain kind, omit the struct DeviceType* dev argument on the methods.
- for the most common base class device, omit the instance number from the type name.  currently we have USART1_Type, which gets
reused for all devices that are exact copies (registers and bits within registers).  the TIMx... are a bit confusing too. 
- maybe some way to cleanly re-use drivers between families, instead of dumb copying + modfiying. 

checklist for bringing up a platform

- Makefile
- cortex_mX.h
- .ld file
- SVD
- genstruct devs.ld, .h , vectors.c
- nvic
- boot Reset_Handler 
- gpio
- serial
- can 
- spi

