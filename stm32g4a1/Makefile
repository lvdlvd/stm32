# Tool names
PREFIX=arm-none-eabi-
CC          := $(PREFIX)gcc
OBJCOPY     := $(PREFIX)objcopy
SIZE        := $(PREFIX)size

REVISION := $(shell git log -1 --format="%h" || echo "<NONE>")

ARCH_FLAGS	 = -mthumb -mcpu=cortex-m4
LTO_FLAGS	 = -O2 -flto -fuse-linker-plugin -ffunction-sections -fdata-sections -fverbose-asm -ffat-lto-objects
WARN_FLAGS   = -Werror -Wfatal-errors -Wall -Wextra -Wunsafe-loop-optimizations -Wdouble-promotion -Wundef  -Wno-pedantic -Wno-enum-conversion
DEBUG_FLAGS	 = -ggdb3 -DNDEBUG -D__REVISION__='"$(REVISION)"' 
CFLAGS 		 = -std=gnu99 $(ARCH_FLAGS) $(LTO_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS)
LDFLAGS		 = -nostartfiles -lnosys -static $(ARCH_FLAGS) $(LTO_FLAGS) $(WARN_FLAGS) $(DEBUG_FLAGS) -Wl,-gc-sections,-Map,main.map -Wl,--cref

.DEFAULT_GOAL := main.hex


OBJS = \
	vectors.o \
	boot.o \
	fault.o \
	gpio2.o \
	printf.o \
	usb.o \
	main.o \

$(OBJS): Makefile 

# Compile
%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.elf: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -Tstm32g4a1.ld

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex --set-start 0x08000000 $< $@

flash: main.hex
	st-flash --connect-under-reset --reset --format ihex write $<


flash2: main.elf
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32g4x.cfg \
		-f openocd.cfg



debug: main.elf
	st-util &
	$(PREFIX)gdb $^
	killall st-util
	
clean:
	rm -f *~ *.o *.hex *.bin *.elf *.map

depend:
	makedepend -Y. -w150 *.c
# DO NOT DELETE

boot.o: cortex_m4.h stm32g4xx.h clock.h
fault.o: cortex_m4.h stm32g4xx.h
gpio2.o: gpio2.h stm32g4xx.h
main.o: cortex_m4.h stm32g4xx.h clock.h gpio2.h nvic.h printf.h usart.h
printf.o: printf.h stb_sprintf.h
usb.o: usb.h cortex_m4.h stm32g4usb.h
vectors.o: cortex_m4.h stm32g4xx.h
