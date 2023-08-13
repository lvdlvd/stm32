# stm32

WIP.  the idea is to take SVD's as published by STM, and use them to generate a development environment
that is aviation grade and has zero external dependencies.

the SVD'files are quite buggy, so i've had to patch them up to get usable output.

the core of this system is to generate devs.ld stm32XXXX.h and vectors.c from the SVD, the rest is a family
of rather generic drivers for all the myriad of devices built in to those chips.

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

