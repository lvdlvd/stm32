# Tool names
PREFIX=arm-none-eabi-
CC          := $(PREFIX)gcc
OBJCOPY     := $(PREFIX)objcopy
SIZE        := $(PREFIX)size

REVISION := $(shell git log -1 --format="%h" || echo "0")

ARCH_FLAGS	 = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LTO_FLAGS	 = -O3 -flto -fuse-linker-plugin -ffunction-sections -fdata-sections -fverbose-asm -ffat-lto-objects -fno-exceptions -fno-unwind-tables
WARN_FLAGS   = -Werror -Wfatal-errors -Wall -Wextra -Wunsafe-loop-optimizations -Wdouble-promotion -Wundef  -Wno-pedantic -Wno-enum-conversion 
DEBUG_FLAGS	 = -ggdb3 
CFLAGS 		 = -std=gnu17 $(ARCH_FLAGS) $(LTO_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS) -D__REVISION__='0x$(REVISION)'
LDFLAGS		 = -nostartfiles -lnosys -static $(ARCH_FLAGS) $(LTO_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS) -Wl,-gc-sections,-Map,main.map -Wl,--cref

LDFILE	= stm32g473.ld

# Vectors.c, stm32g4xx.h and devs.ld are generated from the SVD by genstruct, 
# but checked in. Delete them to have them regenerated.
vectors.c:
	(cd ../genstruct; go run genstruct.go -t vectors.c.tmpl -i devs.lst STM32G4xx.svd) > vectors.c 

stm32g4xx.h:
	(cd ../genstruct; go run genstruct.go -t header.h.tmpl -i devs.lst STM32G4xx.svd) > stm32g4xx.h

devs.ld:
	(cd ../genstruct; go run genstruct.go -t devs.ld.tmpl -i devs.lst STM32G4xx.svd) > devs.ld

.DEFAULT_GOAL := main.elf

OBJS = \
	vectors.o \
	boot.o \
	fault.o \
	gpio2.o \
	tprintf.o \


$(OBJS): Makefile 

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%elf: $(LDFILE)
%.elf: $(OBJS) %.o
	$(CC) -o $@ $^ $(LDFLAGS) -T$(LDFILE)
	$(SIZE) $@

flash: main.elf
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32g4x.cfg \
		-c init \
		-c halt \
		-c "flash probe 0" \
		-c "stm32l4x mass_erase 0" \
		-c "sleep 100" \
		-c "program $< verify" \
		-c reset \
		-c shutdown

clean:
	rm -f *~ *.o *.hex *.bin *.elf *.map a.out

format:
	clang-format-mp-14 -i *.c *.h
	clang-tidy-mp-14 -checks='-*,readability-braces-around-statements' -fix-errors *.c *.h


depend:
	makedepend -Y. -w150 *.c

# DO NOT DELETE

boot.o: arm_cm4.h stm32g4xx.h clock.h
fault.o: arm_cm4.h stm32g4xx.h
gpio2.o: gpio2.h stm32g4xx.h
main.o: arm_cm4.h stm32g4xx.h clock.h fifo.h gpio2.h nvic.h tprintf.h
tprintf.o: tprintf.h
vectors.o: arm_cm4.h stm32g4xx.h
