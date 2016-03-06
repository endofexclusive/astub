CFLAGS = -g
CFLAGS+= -Wall -Wextra -pedantic -std=c99
CPPFLAGS = -I../include -I. -Ivtest/include

ROOT := ..
ARCH := .
TARGET=

CC = $(TARGET)gcc
GDB= $(TARGET)gdb
SIZE = $(TARGET)size
OBJDUMP = $(TARGET)objdump

VPATH=$(ROOT)
VPATH+=$(ARCH)
VPATH+=$(ARCH)/vtest
SRCS := hex.c pkt.c cmd.c test.c fake_link.c vtest.c
OBJS := $(SRCS:.c=.o)

all: test_hex test_pkt

test_hex: test_hex.c hex.c vtest.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test_cmd: test_hex.c cmd.c vtest.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test_pkt: test_pkt.c fake_link.c pkt.c hex.c vtest.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -f test_hex test_cmd test_pkt

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

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

