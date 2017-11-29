###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
# Author: Jihun Ahn <jhun.ahn@samsung.com>
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

include ../define.mk

WD = $(abspath $(shell pwd))
BUILDDIR=build

define BUILD
	$(Q) $(MAKE) -C $(BUILDDIR)/$(1) -j$(CORES) $(BUILDOPT)
	$(Q) $(MAKE) -C $(BUILDDIR)/$(1) install
endef

CFLAGS += -O2 -pipe -W -Wall

ifeq ($(ARCH),64)
	CFLAGS += -mtune=generic
else
	CFLAGS += -m32 -march=i386
	CXXFLAGS += -m32
	LDFLAGS += -m32
endif

ifeq ($(ARCH),64)
CTARGET = $(patsubst x86_64,i686,$(CHOST))
else
CTARGET = $(patsubst i686,x86_64,$(CHOST))
endif

CBUILD := $(shell gcc -dumpmachine)

#
# nothing to be done 'all'
#
all:

download:
	$(Q) if [ -e $(pkg) ]; then rm -rf $(pkg); fi
	$(Q) git clone $(url) -b $(version) --depth=1 $(pkg)

.init:
	@echo "MKDIR:   $(_name:_init=)"
	$(Q) if [ -e $(BUILDDIR)/$(_name:_init=) ]; then \
		rm -rf $(BUILDDIR)/$(_name:_init=); \
	fi
	$(Q) mkdir -p $(BUILDDIR)/$(_name:_init=)
ifneq ($(wildcard $(WD)/../patches/$(_name:_init=)/*.patch),)
	@echo "PATCH:   $(notdir $(wildcard $(WD)/../patches/$(_name:_init=)/*.patch))"
	$(call CD,$(_name:_init=)) && patch -p1 < $(wildcard $(WD)/../patches/$(_name:_init=)/*.patch)
endif
ifneq ($(_initfile),)
	$(call CD,$(_name:_init=)) && ./$(_initfile) || exit 1;
endif

libusb_init:
	$(Q) $(MAKE) _name=$(@) _initfile=bootstrap.sh .init

libusb-compat-0.1_init:
	$(Q) $(MAKE) _name=$(@) _initfile=bootstrap.sh .init

libconfuse_init:
	$(Q) $(MAKE) _name=$(@) _initfile=autogen.sh .init

libftdi_init:
	$(Q) $(MAKE) _name=$(@) .init

hidapi_init:
	$(Q) $(MAKE) _name=$(@) _initfile=bootstrap .init

openocd_init:
	$(Q) $(MAKE) _name=$(@) _initfile=bootstrap .init
	$(eval LDFLAGS += -L$(PREFIXDIR)/bin -L$(PREFIXDIR)/lib)

#
# Make Targets
#

libusb: libusb_init
	$(call CD,$(BUILDDIR)/$(@)) && \
	$(WD)/$(@)/configure --enable-static --disable-shared \
		--prefix=$(PREFIXDIR) \
		--host=$(CHOST)  --target=$(CTARGET)  --build=$(CBUILD) \
		CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(call BUILD,$(@))

libusb-compat-0.1: libusb-compat-0.1_init
	$(call CD,$(BUILDDIR)/$(@)) && \
	$(WD)/$(@)/configure --enable-static  --disable-shared \
		--prefix=$(PREFIXDIR) \
		--includedir=$(PREFIXDIR)/include/libusb0 \
		--host=$(CHOST) --target=$(CTARGET) --build=$(CBUILD) \
		--disable-dependency-tracking \
		LIBUSB_1_0_CFLAGS=-I$(PREFIXDIR)/include/libusb-1.0 \
		LIBUSB_1_0_LIBS="-L$(PREFIXDIR)/lib -lusb-1.0" \
		PKG_CONFIG_LIBDIR="$(PREFIXDIR)/lib/pkgconfig" \
		CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(call BUILD,$(@))

libconfuse: libconfuse_init
	$(call CD,$(BUILDDIR)/$(@)) && \
	$(WD)/$(@)/configure --enable-static  --disable-shared \
		--disable-examples \
		--prefix=$(PREFIXDIR) --includedir=$(PREFIXDIR)/include/libconfuse \
		--host=$(CHOST) --target=$(CTARGET) --build=$(CBUILD) \
		CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(call BUILD,$(@))

libftdi: libftdi_init .libftdi_$(TOOL)
	$(call CD,$(BUILDDIR)/$(@)) && \
	cmake $(option) -DCMAKE_INSTALL_PREFIX=$(PREFIXDIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DPYTHON_BINDINGS=ON \
		-DBUILD_SHARED_LIBS=OFF \
		-DEXAMPLES=OFF -DBUILD_TESTS=OFF -DDOCUMENTATION=OFF \
		-DLIBUSB_INCLUDE_DIR=$(PREFIXDIR)/include/libusb-1.0 \
		-DLIBUSB_LIBRARIES=$(PREFIXDIR)/lib/libusb-1.0.a \
		-DCONFUSE_INCLUDE_DIR=$(PREFIXDIR)/include/libconfuse \
		-DCONFUSE_LIBRARY=$(PREFIXDIR)/lib/libconfuse.a \
		-DCMAKE_C_FLAGS="$(CFLAGS)" -DCMAKE_CXX_FLAGS="$(CXXFLAGS)" \
		$(WD)/$(@)
	$(call BUILD,$(@))

.libftdi_win:
	$(eval option += -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchain-$(CHOST).cmake)
	$(eval option += -DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS)")

.libftdi_linux:
	$(eval option := -DCMAKE_SYSTEM_NAME=Linux)

.libftdi_macos:
	$(eval option := -DCMAKE_SYSTEM_NAME=Darwin)
	$(eval option += -DCMAKE_EXE_LINKER_FLAGS="-lobjc -Wl,-framework,IOKit -Wl,-framework,CoreFoundation")

hidapi: hidapi_init
	$(call CD,$(BUILDDIR)/$(@)) && \
	$(WD)/$(@)/configure --enable-static --disable-shared \
		--disable-testgui \
		--prefix=$(PREFIXDIR) \
		--host=$(CHOST) --target=$(CTARGET) --build=$(CBUILD) \
		libusb_CFLAGS=-I$(PREFIXDIR)/include/libusb-1.0 \
		libusb_LIBS="-L$(PREFIXDIR)/lib -lusb-1.0" \
		CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(call BUILD,$(@))

openocd: openocd_init .openocd_$(TOOL)
	$(call CD,$(BUILDDIR)/$(@)) && \
	$(WD)/$(@)/configure $(option) \
		--enable-static --disable-shared \
		--prefix=$(PREFIXDIR) \
		--host=$(CHOST) --target=$(CTARGET) --build=$(CBUILD) \
		--enable-ftdi \
		--disable-stlink \
		--disable-ti-icdi \
		--disable-ulink \
		--disable-usb-blaster-2 \
		--disable-openjtag \
		--disable-jlink \
		--disable-vsllink \
		--disable-usbprog \
		--disable-rlink \
		--disable-osbdm \
		--disable-opendous \
		--disable-aice \
		--disable-armjtagew \
		--disable-cmsis-dap \
		--disable-usb-blaster \
		--disable-presto \
		--disable-openjtag \
		--disable-jtag_vpi \
		--disable-amtjtagaccel \
		--disable-zy1000-master \
		--disable-zy1000 \
		--disable-ep93xx \
		--disable-at91rm9200 \
		--disable-bcm2835gpio \
		--disable-buspirate \
		--disable-sysfsgpio \
		LIBFTDI_LIBS="-lftdi1 -lusb-1.0" \
		LIBFTDI_CFLAGS="-I$(PREFIXDIR)/include/libftdi1 -I$(PREFIXDIR)/include/libusb-1.0" \
		LIBUSB0_LIBS="-lusb" \
		LIBUSB0_CFLAGS="-I$(PREFIXDIR)/include/libusb0 -I$(PREFIXDIR)/include/libusb-1.0" \
		LIBUSB1_LIBS="-lusb-1.0" \
		LIBUSB1_CFLAGS="-I$(PREFIXDIR)/include/libusb-1.0" \
		HIDAPI_LIBS="$(HIDAPI_LIB)" \
		HIDAPI_CFLAGS="-I$(PREFIXDIR)/include/hidapi" \
		PKG_CONFIG_PATH="$(PKG_CONFIG_PATH):$(PREFIXDIR)/lib/pkgconfig" \
		LIBS="$(LIBS)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(call BUILD,$(@))

.openocd_win:
	$(eval option := --disable-werror --enable-parport-giveio)
	$(eval LDFLAGS += -L$(dir $(shell readlink -f `which x86_64-w64-mingw32-gcc`))/../lib -static -static-libgcc -static-libstdc++)
	$(eval LIBS := -lwinpthread)
	$(eval HIDAPI_LIB := -lhidapi)

.openocd_linux:
	$(eval LIBS := -ludev -lpthread)
	$(eval HIDAPI_LIB := -l:libhidapi-libusb.a)

.openocd_macos:
	$(eval option := --disable-werror)
	$(eval LIBS := -lSystem)
	$(eval LDFLAGS += -Bstatic)
	$(eval HIDAPI_LIB := -lhidapi)

clean:
	$(call DELFILE, $(filter-out Makefile, $(wildcard *)))
