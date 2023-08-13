
unbricking a nucleo board: 

---------- hold HW reset button
lvd$ openocd -f interface/stlink.cfg -f  target/stm32g4x.cfg
Open On-Chip Debugger 0.11.0
Licensed under GNU GPL v2
For bug reports, read
	http://openocd.org/doc/doxygen/bugs.html
Info : auto-selecting first available session transport "hla_swd". To override use 'transport select <transport>'.
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : clock speed 2000 kHz
Info : STLINK V3J2M1 (API v3) VID:PID 0483:374E
Info : Target voltage: 3.302986
Info : stm32g4x.cpu: hardware has 6 breakpoints, 4 watchpoints
Info : starting gdb server for stm32g4x.cpu on 3333
Info : Listening on port 3333 for gdb connections

------here we go to the other shell see below

Info : accepting 'telnet' connection on tcp/4444
Info : Unable to match requested speed 2000 kHz, using 1000 kHz
Info : Unable to match requested speed 2000 kHz, using 1000 kHz
Error: timed out while waiting for target halted
TARGET: stm32g4x.cpu - Not halted
target halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x080015a0 msp: 0x10002800
Info : device idcode = 0x20036468 (STM32G43/G44xx - Rev 'unknown' : 0x2003)
Info : flash size = 128kbytes
Info : flash mode : single-bank
Info : dropped 'telnet' connection
^Cshutdown command invoked


other shell: 

 lvd$ nc localhost 4444
��������Open On-Chip Debugger
> reset halt
reset halt
Unable to match requested speed 2000 kHz, using 1000 kHz
Unable to match requested speed 2000 kHz, using 1000 kHz
timed out while waiting for target halted
TARGET: stm32g4x.cpu - Not halted

------ release HW reset button

target halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x080015a0 msp: 0x10002800
> stm32l4x mass_erase 0
stm32l4x mass_erase 0
device idcode = 0x20036468 (STM32G43/G44xx - Rev 'unknown' : 0x2003)
flash size = 128kbytes
flash mode : single-bank
stm32l4x mass erase complete