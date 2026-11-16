###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
############################################################################
# board/stm32f4discovery/src/Makefile
#
#   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/Make.defs
ASRCS =
DEPPATH = --dep-path .
VPATH = .

CSRCS = stm32_boot.c stm32_bringup.c stm32_spi.c

ifeq ($(CONFIG_ARCH_LEDS),y)
CSRCS += stm32_autoleds.c
else
CSRCS += stm32_userleds.c
endif

ifeq ($(CONFIG_AUDIO_CS43L22),y)
CSRCS += stm32_cs43l22.c
endif

ifeq ($(CONFIG_ARCH_BUTTONS),y)
CSRCS +=  stm32_buttons.c
endif

ifeq ($(CONFIG_STM32_CAN_CHARDRIVER),y)
CSRCS += stm32_can.c
endif

ifeq ($(CONFIG_STM32_OTGFS),y)
CSRCS += stm32_usb.c
endif

ifeq ($(CONFIG_LCD_ST7567),y)
CSRCS += stm32_st7567.c
endif

ifeq ($(CONFIG_ENC28J60),y)
CSRCS += stm32_enc28j60.c
endif

ifeq ($(CONFIG_NET_W5500),y)
CSRCS += stm32_w5500.c
endif

ifeq ($(CONFIG_LPWAN_SX127X),y)
CSRCS += stm32_sx127x.c
endif

ifeq ($(CONFIG_LCD_MAX7219),y)
CSRCS += stm32_max7219.c
endif

ifeq ($(CONFIG_LCD_ST7032),y)
CSRCS += stm32_st7032.c
endif

ifeq ($(CONFIG_PCA9635PW),y)
CSRCS += stm32_pca9635.c
endif

ifeq ($(CONFIG_STM32_SDIO),y)
CSRCS += stm32_sdio.c
endif

ifeq ($(CONFIG_STM32_ETHMAC),y)
CSRCS += stm32_ethernet.c
endif

ifeq ($(CONFIG_LEDS_MAX7219),y)
CSRCS += stm32_max7219_leds.c
endif

ifeq ($(CONFIG_RGBLED),y)
CSRCS += stm32_rgbled.c
endif

ifeq ($(CONFIG_PWM),y)
CSRCS += stm32_pwm.c
endif

ifeq ($(CONFIG_CAPTURE),y)
CSRCS += stm32_capture.c
endif

ifeq ($(CONFIG_BOARDCTL),y)
CSRCS += stm32_appinit.c
ifeq ($(CONFIG_BOARDCTL_RESET),y)
CSRCS += stm32_reset.c
endif
endif

ifeq ($(CONFIG_ARCH_CUSTOM_PMINIT),y)
CSRCS += stm32_pm.c
endif

ifeq ($(CONFIG_PM_BUTTONS),y)
CSRCS += stm32_pmbuttons.c
endif

ifeq ($(CONFIG_ARCH_IDLE_CUSTOM),y)
CSRCS += stm32_idle.c
endif

ifeq ($(CONFIG_STM32_FSMC),y)
CSRCS += stm32_extmem.c

ifeq ($(CONFIG_LCD_SSD1289),y)
CSRCS += stm32_ssd1289.c
endif
endif

ifeq ($(CONFIG_LCD_APA102),y)
CSRCS += stm32_apa102.c
endif

ifeq ($(CONFIG_LCD_SSD1351),y)
CSRCS += stm32_ssd1351.c
endif

ifeq ($(CONFIG_LCD_UG2864AMBAG01),y)
CSRCS += stm32_ug2864ambag01.c
endif

ifeq ($(CONFIG_LCD_UG2864HSWEG01),y)
CSRCS += stm32_ug2864hsweg01.c
endif

ifeq ($(CONFIG_TIMER),y)
CSRCS += stm32_timer.c
endif

ifeq ($(CONFIG_STM32_HCIUART),y)
ifeq ($(CONFIG_BLUETOOTH_UART),y)
CSRCS += stm32_hciuart.c
endif
endif

ifeq ($(CONFIG_STM32_ROMFS),y)
CSRCS += stm32_romfs_initialize.c
endif

ifeq ($(CONFIG_BOARDCTL_UNIQUEID),y)
CSRCS += stm32_uid.c
endif

ifeq ($(CONFIG_USBMSC),y)
CSRCS += stm32_usbmsc.c
endif

ifneq ($(CONFIG_STM32_ETHMAC),y)
ifeq ($(CONFIG_NETDEVICES),y)
CSRCS += stm32_netinit.c
endif
endif

ifeq ($(CONFIG_MMCSD_SPI),y)
CSRCS += stm32_mmcsd.c
endif

ifeq ($(CONFIG_WL_GS2200M),y)
CSRCS += stm32_gs2200m.c
endif

ifeq ($(CONFIG_LCD_ST7789),y)
CSRCS += stm32_st7789.c
endif

ifeq ($(CONFIG_INPUT_DJOYSTICK),y)
  CSRCS += stm32_djoystick.c
endif

ifeq ($(CONFIG_USBDEV_COMPOSITE),y)
  CSRCS += stm32_composite.c
endif

ifeq ($(CONFIG_ADC_HX711),y)
CSRCS += stm32_hx711.c
endif


ARCH_FAMILY = $(patsubst "%",%,$(CONFIG_ARCH_FAMILY))
CFLAGS += -I$(TOPDIR)/kernel -I$(TOPDIR)/arch/arm/src/common -I$(TOPDIR)/arch/arm/src/chip -I$(TOPDIR)/arch/arm/src/$(ARCH_FAMILY)
AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libboard$(LIBEXT)

all: $(BIN)
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

context:
