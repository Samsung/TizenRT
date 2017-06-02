##
# override with `make BUILD=debug`
# override with `make PLATFORM=arduinomega` or `make PLATFORM=arduinodue`
# default to release build
# default to build for linux
BUILD	 := debug
PLATFORM := tizen
OBJ_DIR := ./bin
ROOT_DIR := .
VERSION_MAJOR := 0
VERSION_MINOR := 1
VERSION_PATCH := 0
ADAPTER_MACRO := -DWIFI_ADAPTER

$(info *******************tizen Build !!******************)

CC=gcc
LIBCOAP=./lib/libcoap-4.1.1/
LIBCOAP_OBJ_DIR=./lib/libcoap-4.1.1/obj

# TODO : Currently Hardcoded WIFI_ADAPTER. Change it to generic
CFLAGS.debug = -DWITH_POSIX -D__TIZEN__ -Wall -std=c99 -DSLP_SDK_LOG -g -D_GNU_SOURCE -DTIZEN_DEBUG_ENABLE -DTB_LOG $(ADAPTER_MACRO)
CFLAGS.release = -DWITH_POSIX -D__TIZEN__ -Wall -std=c99 -DSLP_SDK_LOG -D_GNU_SOURCE -DTIZEN_DEBUG_ENABLE -DTB_LOG $(ADAPTER_MACRO)
COMPILEFLAG = `pkg-config --cflags --libs capi-network-wifi dlog ttrace capi-network-bluetooth glib-2.0`

# Include files will be copied into a single folder on gbsbuild.
# NO NEED to mention the path explicitly.

CA_INC              = $(ROOT_DIR)/
DTLS_INC           = $(ROOT_DIR)/dtls/

INC_DIRS        := -I$(CA_INC)
INC_DIRS        += -I$(DTLS_INC)

SRC = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SRC))

#Currently compiling libcoap objects. If it's provided by external party.
#Then don't compile, Instead directly extract objects and create liboicinterface.a
all: $(OBJECTS)
	$(MAKE) -C $(LIBCOAP) clean
	$(MAKE) -C $(LIBCOAP)
	-mkdir $(LIBCOAP_OBJ_DIR)
	@cd $(LIBCOAP_OBJ_DIR) && $(AR) -x ../libcoap.a
	ar rcs liboicinterface.a $(OBJECTS) $(LIBCOAP_OBJ_DIR)/*.o

%.o : %.c
	$(CC) -c $(CFLAGS.$(BUILD)) $(COMPILEFLAG) $(INC_DIRS) $< -o $@


pack:
#$(CC) $(CFLAGS) $(COMPILEFLAG) WiFiInterface.c bt_interface.c tc_interface.c sample_main.c -o sample /usr/lib/liboctbstack.a -lm -pthread
	$(info **************************************TEST***************************)
install:
#cp -rf sample ./usr/apps/com.samsung.oicca/bin/sample

clean:
	rm -rf *.o sample
	rm -f *.o *.d *.elf *.eep core.a *.hex *.bin *.map *-
	rm -rf $(OBJ_DIR)
	rm -rf $(LIBCOAP_OBJ_DIR)

