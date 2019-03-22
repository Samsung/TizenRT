include ${TOPDIR}/.config
include ${TOPDIR}/tools/Config.mk
include ${TOPDIR}/arch/xtensa/src/lx6/Toolchain.defs

SPIFLASH_DIR=$(shell pwd)

CC = $(CROSSDEV)gcc
CXX = $(CROSSDEV)g++
CPP = $(CROSSDEV)gcc -E
LD = $(CROSSDEV)ld
STRIP = $(CROSSDEV)strip --strip-unneeded
AR = $(ARCROSSDEV)ar rcs
NM = $(ARCROSSDEV)nm
OBJCOPY = $(CROSSDEV)objcopy
OBJDUMP = $(CROSSDEV)objdump

CFLAGS += -I${TOPDIR}/include
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
CFLAGS += -I${SPIFLASH_DIR}/../include 
CFLAGS += -I${SPIFLASH_DIR}/../
CFLAGS += -I${SPIFLASH_DIR}/include 
CFLAGS += -fno-builtin -ffunction-sections -fdata-sections -pipe -mlongcalls

SPI_CSRCS += cache_utils.c flash_mmap.c flash_ops.c partition.c spi_flash_rom_patch.c esp_ota_eps.c esp32_flash.c
SPI_COBJS = $(SPI_CSRCS:.c=$(OBJEXT))
SPI_BIN = libspiflash$(LIBEXT)

all: $(SPI_BIN)
$(SPI_COBJS): %$(OBJEXT): %.c 
    $(call COMPILE, $<, $@)

$(SPI_BIN): $(SPI_COBJS)
	$(call ARCHIVE, $@, $(SPI_COBJS))

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(SPI_COBJS))
	$(call CLEAN)

