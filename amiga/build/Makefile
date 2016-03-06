ROOT := ../..
ARCH := $(ROOT)/m68k
PLATFORM := $(ROOT)/amiga

CFLAGS = -march=68000 -fomit-frame-pointer -mpcrel -mshort -Os
CFLAGS+= -Wall -Wextra -pedantic -ansi
CPPFLAGS = -I$(ROOT)/include -I$(ARCH)/include -I$(PLATFORM)/include

TARGET=m68k-elf-

CC = $(TARGET)gcc
LD = $(TARGET)gcc
AS = vasmm68k_mot
IRA = ira
ASFLAGS = -quiet -phxass -m68000 -Felf
SIZE = $(TARGET)size
OBJDUMP = $(TARGET)objdump
OBJCOPY= $(TARGET)objcopy

VPATH=$(ROOT)
VPATH+=$(ARCH)
VPATH+=$(PLATFORM)
CSRCS := hex.c pkt.c cmd.c stub_arch.c a500_init.c uart.c
SSRCS := exception_68000.s standalone.s
OBJS := $(SSRCS:.s=.o)
OBJS := $(OBJS) $(CSRCS:.c=.o)

all: astub main main.asm main.bin

main: $(OBJS) a500.ld
	$(LD) -nostdlib $(OBJS) -T a500.ld -o $@ -Wl,-Map=$@.map
main.asm: main
	$(OBJDUMP) -d $< > $@
main.bin: main
	$(OBJCOPY) -O binary $< $@

astub: bload.s main.bin
	$(AS) -quiet -phxass -m68000 -Fhunkexe bload.s -o $@
astub.ira: astub
	$(IRA) -OLDSTYLE -a -M68000 $< $@ > /dev/null

.PHONY: size
size: $(OBJS) main
	$(SIZE) $(OBJS) -t --common
	$(SIZE) main

clean:
	rm -f $(OBJS) main *.asm *.map *.bin *.ira astub

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

