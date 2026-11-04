# Type is laptop or openwrt
TYPE = laptop
# Role is dut or platform
ROLE = dut
# Package Version
VERSION = "1.2.0.70"

OBJS = main.o eloop.o indigo_api.o indigo_packet.o utils.o wpa_ctrl.o
CFLAGS += -g -Wall -Werror -Wno-unused-function

ifeq ($(TYPE),laptop)
CC = gcc
CFLAGS += -D_LAPTOP_

else
# upstream OPENWRT
# 32 bit
#CC = /openwrt/QCA_Sniffer_11ax/qsdk/staging_dir/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/bin/arm-openwrt-linux-gcc
#LD = /openwrt/QCA_Sniffer_11ax/qsdk/staging_dir/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/bin/arm-openwrt-linux-ld
# 64 bit
CC = /openwrt/11305r3/qsdk/staging_dir/toolchain-aarch64_cortex-a53_gcc-5.2.0_musl-1.1.16/bin/aarch64-openwrt-linux-gcc
LD = /openwrt/11305r3/qsdk/staging_dir/toolchain-aarch64_cortex-a53_gcc-5.2.0_musl-1.1.16/bin/aarch64-openwrt-linux-ld
# _OPENWRT_: Use OPENWRT
CFLAGS += -D_OPENWRT_
endif

# Define the app is for DUT or platform
ifeq ($(ROLE),dut)
OBJS += indigo_api_callback_dut.o vendor_specific_dut.o
CFLAGS += -D_DUT_
else
OBJS += indigo_api_callback_tp.o vendor_specific_tp.o
CFLAGS += -DCONFIG_CTRL_IFACE_UDP
CFLAGS += -D_TEST_PLATFORM_
endif

# Define the package version
ifneq ($(VERSION),)
CFLAGS += -D_VERSION_='$(VERSION)'
endif

all: app

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

app: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf app *.o
