# Introduction

This directory contains definitions and code for the *Motorola 68000* series of
microprocessors.

Some day the code in this directory should be extended to also include support
for other processors in the *Motorola 68000* family. That's why names such as
*m68k* and *m680x0* are used. :-)

`exception_68000.s` contains code which executes at CPU exception. It takes the
system into and out of the *GDB* stub main loop `stub_main`. CPU registers are
saved and restored.

`stub_arch.c` implements the architecture specific processing of the *gdb
Remote Serial Protocol* commands.

`include/m680x0.h` describes the *Motorola 68000 series of microprocessors*.
The idea is that this file shall be self-standing and independent of the world
of *GDB* stubs.

`include/stub_arch` contains definitions of how to store the `MC6800` registers
in memory. It exports the define `REGBYTES` which is used by the target
independent code (`cmd.c`).

