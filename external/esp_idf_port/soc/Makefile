include ${TOPDIR}/tools/Config.mk
include ${TOPDIR}/arch/xtensa/src/lx6/Toolchain.defs

CC = $(CROSSDEV)gcc
CXX = $(CROSSDEV)g++
CPP = $(CROSSDEV)gcc -E
LD = $(CROSSDEV)ld
STRIP = $(CROSSDEV)strip --strip-unneeded
AR = $(ARCROSSDEV)ar rcs
NM = $(ARCROSSDEV)nm
OBJCOPY = $(CROSSDEV)objcopy
OBJDUMP = $(CROSSDEV)objdump

PWD=$(shell pwd)

CFLAGS += -I${TOPDIR}/include
CFLAGS += -I${PWD}/../include 
CFLAGS += -I${PWD}/../heap/include
CFLAGS += -I${PWD}/../
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
CFLAGS += -fno-builtin -ffunction-sections -fdata-sections -pipe -mlongcalls

SOC_CSRCS += gpio_periph.c rtc_clk.c rtc_init.c rtc_sleep.c rtc_time.c 

SOC_COBJS = $(SOC_CSRCS:.c=$(OBJEXT))
BIN = libsoc$(LIBEXT)

all: $(BIN)
$(SOC_COBJS): %$(OBJEXT): %.c 
    $(call COMPILE, $<, $@)

$(BIN): $(SOC_COBJS)
	$(call ARCHIVE, $@, $(SOC_COBJS))

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(SOC_COBJS))
	$(call CLEAN)

