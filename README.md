# Introduction

*astub* is a remote stub for *GNU: The GNU Project Debugger*, aimed for
resource constrainted systems. It allows source-level debugging of software
running on the target system, controlled by a host system running *GDB*.

For more information on *GDB*, *remote stubs* and the *gdb Remote Serial
Protocol*, see [Debugging with gdb: the gnu Source-Level
Debugger](https://www.gnu.org/software/gdb/).


# Purpose

The purpose of preparing this software was to create a cross-development and
debug environment for the *Commodore Amiga A500* computer.


# Target support

The following CPU architectures are currently supported:

- *Motorola 68000*


The following computer platforms are currently supported:

- *Commodore Amiga A500*. *A1000*, *A600* and *A2000* should also work but has
  not been tested.


# Implementation

Portability and reusability is a design goal for *astub*. The implementation
has been is split into the following parts:

- target independent code
- architecture specific code
- platform specific code


## Target independent code

The *target independent* code implements a selected subset of the *gdb Remote
Serial Protocol*. This includes a packetization layer (`pkt`), a command
interpretation layer (`cmd`) and support functions for working with data coded
as hexadecimal strings (`hex`).

- `include/pkt.h`, `pkt.c`
- `include/cmd.h`, `cmd.c`
- `include/hex.h`, `hex.c`
- `include/link.h` (only the API, not the implementation)


## Architecture specific code

*Architecture specific* code contains knowledge of the target CPU architecture
and how to perform the architecture specific processing of *gdb Remote Serial
Protocol* commands.

The *architecture specific* code shall make no assumptions on how the CPU is
used and connected in the target system. Such information is provided by the
platform specific code instead.

- `<architecture>/include/stub_arch.h`


## Platform specific code

*Platform specific* code is responsible for implementing initializations and
operations required for *astub* to operate on the target system. Platform
specific information includes for example:

- Which interrupts are connected to which peripheral
- Memory map
- Memory access restrictions
- Implementation of the `link.h` API

The *platform specific* code implements the communication channel (`link`)
between the *GDB* host system and the target system. It is typically a *UART*
driver.


# Coding convention

The [Linux kernel coding
style](https://www.kernel.org/doc/Documentation/CodingStyle) is a good starting
point.

## For *target independent* code

1. Use *C90* for the *target independent* code. This implies that `stdint.h`
   can not be used. For `gcc`, try `-ansi -Wall -Wextra -pedantic`.
2. Make no assumptions on how the C types are represented, except for what the
   C standard says.
3. Compiler extensions, pragmas, inline assembly, etc. shall not be used.

The rationale for all the above is to have the *target independent* code
compatible with a wide range of compilers for a wide range of exotic targets.

## For *architecture specific* code

Only point 3. above applies.

## For *platform specific* code

Only point 3. above applies.


# License

This software is licensed under [GPL
v3](http://www.gnu.org/licenses/gpl-3.0.en.html).

