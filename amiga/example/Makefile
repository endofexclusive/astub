PROJ = ex1
ROOT := ../..
PLATFORM := $(ROOT)/amiga
CFLAGS = -march=68000 -fomit-frame-pointer -g -Os -mpcrel -mshort
CFLAGS+= -Wall -Wextra -std=gnu99 -pedantic
CPPFLAGS = -I$(ROOT)/include

TARGET=m68k-elf-

CC = $(TARGET)gcc
LD = $(TARGET)gcc
AS = vasmm68k_mot
ASFLAGS = -quiet -phxass -m68000 -Felf
GDB = $(TARGET)gdb
SIZE = $(TARGET)size
OBJDUMP = $(TARGET)objdump
OBJCOPY = $(TARGET)objcopy
SIM = fs-uae
SIMFLAGS = a500.fs-uae
LDSCRIPT = $(PROJ).ld

CSRCS := $(PROJ).c
SSRCS := head.s
OBJS := $(SSRCS:.s=.o)
OBJS := $(OBJS) $(CSRCS:.c=.o)

all: $(PROJ) $(PROJ).asm size

$(PROJ): $(PROJ).o head.o $(LDSCRIPT)
	$(LD) -nostdlib $(PROJ).o head.o -T $(LDSCRIPT) -o $@ -Wl,-Map=$@.map
$(PROJ).asm: $(PROJ)
	$(OBJDUMP) -d $< > $@

.PHONY: size
size: $(OBJS) $(PROJ)
	$(SIZE) $(OBJS) -t --common
	$(SIZE) $(PROJ)

.PHONY: fs/astub
fs/astub:
	$(MAKE) -C $(PLATFORM)/build
	cp $(PLATFORM)/build/astub fs

.PHONY: sim
# ks is a Kickstart ROM file
# pty_sim is a pseudo terminal to connect with FS-UAE.
sim: ks pty_sim fs/astub
	$(SIM) $(SIMFLAGS)

.PHONY: gdbsim
gdbsim: pty_gdbsim $(PROJ) fs/astub
	$(GDB) $(PROJ) -ex "target remote pty_gdbsim" -b 230400 -x gdb.script

.PHONY: gdbhw
gdbhw: pty_gdbhw $(PROJ) fs/astub
	$(GDB) $(PROJ) -ex "target remote pty_gdbhw" -b 230400 -x gdb.script

clean:
	rm -f $(OBJS) $(PROJ) *.asm *.map fs/astub

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(CSRCS)))

