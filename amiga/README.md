# Introduction

This directory contains files necessary for building *astub* for *Commodore
Amiga A500* (`build/`). It also contains an example application (`example/`)
which can be loaded and run with *stub*.

# Building the toolchain

This section describes how a toolchain compatible with *astub* for *Commodore
Amiga A500* can be built. Other toolchains may work aswell.

Tool versions used here:

- GNU binutils 2.26
- GCC 5.3.0
- Newlib 2.2.0
- GDB 7.10.1 (see the patch information below)
- vasm 1.7d, available [here](http://sun.hasenbraten.de/vasm/)

After the tools have been built, their respective binary directories shall be
added to your `PATH` environment variable.

## GNU binutils

    $ mkdir -p build-binutils
    $ cd build-binutils
    $ ../binutils-2.26/configure --target=m68k-elf --prefix=/opt/m68k-elf --disable-nls --enable-install-libbfd
    $ make -j 3 all
    $ make -k pdf
    $ make install-pdf install
    $ cd ..


## GCC and Newlib

### GCC pass 1
    $ mkdir -p build-gcc
    $ cd build-gcc
    $ ../gcc-5.3.0/configure --target=m68k-elf --with-arch=m68k --with-cpu=68000 --prefix=/opt/m68k-elf --disable-nls --without-headers --with-newlib --enable-languages=c --disable-libssp --disable-tls --disable-libgomp --disable-threads
    $ make -j 3 all
    $ make install
    $ cd ..


### Newlib

    $ mkdir -p build-newlib
    $ cd build-newlib
    $ ../newlib-2.2.0/configure --target=m68k-elf --with-arch=m68k --prefix=/opt/m68k-elf --disable-newlib-multithread --disable-newlib-supplied-syscalls --disable-newlib-io-c99-formats --enable-target-optspace --disable-shared --enable-static
    $ make -j 3 all
    $ make -k pdf
    $ make -k install-pdf
    $ make install
    $ cd ..


### GCC pass 2

    $ cd build-gcc
    $ ../gcc-5.3.0/configure --target=m68k-elf --with-arch=m68k --with-cpu=68000 --prefix=/opt/m68k-elf --disable-nls --with-newlib --enable-languages=c --disable-libssp --disable-tls --disable-libgomp --disable-threads
    $ make -j 3 all
    $ make -j 3 pdf
    $ make install-pdf install
    $ cd ..


## GDB

The file `gdb/m68k-tdep.c` must be patched so that the source code line

    set_gdbarch_decr_pc_after_break (gdbarch, 2);

is included. Simply remove the `flavour` condition with the preprocessor, like
in the following diff:

    +#if 0
       if (flavour == m68k_coldfire_flavour || flavour == m68k_fido_flavour)
    +#endif
        set_gdbarch_decr_pc_after_break (gdbarch, 2);
 
Then *GDB* can be built as normal.

    $ mkdir -p build-gdb
    $ cd build-gdb
    $ ../gdb-7.10.1/configure --target=m68k-elf --with-arch=m68k --with-cpu=68000 --prefix=/opt/m68k-elf --disable-nls --with-newlib --enable-languages=c --disable-libssp --disable-tls --disable-libgomp --disable-threads
    $ make -j 3 all
    $ make -j 3 pdf
    $ make install-pdf install
    $ cd ..


## vasm

    $ mkdir -p out/bin out/html
    $ tar xfz vasm.tar.gz
    $ cd vasm
    $ make CPU=m68k SYNTAX=mot
    $ yes "" | make doc/vasm.pdf
    $ make doc/vasm.html
    $ cp vasmm68k_mot vobjdump ../out/bin
    $ cp doc/*.html ../out/html
    $ cp doc/vasm.pdf ../out
    $ cd ..

