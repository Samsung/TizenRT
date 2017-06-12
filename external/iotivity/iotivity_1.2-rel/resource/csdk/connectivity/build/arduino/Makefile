#
# override with `make BUILD=debug`
# override with `make PLATFORM=arduinomega` or `make PLATFORM=arduinodue`
# TRANSPORT can be ETHERNET, WIFI, BT, BLE. Include Corresponding Transport during compilation. For Eg: `make PLATFORM=arduinomega TRANSPORT=BLE
# default to release build
# default to build for arduinomega
# default to build for BLE Transport
BUILD	 := release
PLATFORM := arduinomega
TRANSPORT := BLE
OBJ_DIR := ./bin
APP_NAME := casample
ARDUINO_PORT := ttyACM0

include ./local.properties
include ./$(PLATFORM).properties

VPATH := $(SDIR_ARD_PLATFORM)

#Include __ARDUINO_BLE__ flag for BLE Transport. Currently, BLE Sample APP is segregated with this MACRO
ifeq ($(TRANSPORT),BLE)
CFLAGS		:= -Os -Wall -c -DTB_LOG -DOIC_ARDUINODUE -DINTERFACESAMPLE_ARDUINO -DARDUINO -DOIC_ARDUINODUE -D__ARDUINO__ -DWITH_ARDUINO -D__ARDUINO_BLE__ -DLE_ADAPTER
else ifeq ($(TRANSPORT),WIFI)
CFLAGS		:= -Os -Wall -c -DTB_LOG -DOIC_ARDUINODUE -DINTERFACESAMPLE_ARDUINO -DARDUINO -DOIC_ARDUINODUE -D__ARDUINO__ -DWITH_ARDUINO -DWIFI_ADAPTER
else
CFLAGS		:= -Os -Wall -c -DTB_LOG -DOIC_ARDUINODUE -DINTERFACESAMPLE_ARDUINO -DARDUINO -DOIC_ARDUINODUE -D__ARDUINO__ -DWITH_ARDUINO -DETHERNET_ADAPTER
endif

all: prep_dirs core.a $(APP_NAME).o $(APP_NAME).elf $(APP_NAME).hex

$(info *********PLATFORM_OBJS!!**********)
core.a: $(PLATFORM_OBJS)
	@cd $(OBJ_DIR) && $(AR) -x ../../../lib/libcoap-4.1.1/$(BUILD)/libcoap.a
	$(AR) rcs $@ $(CORE_COBJ) $(CORE_CPPOBJ) $(PLATFORM_OBJS) $(OBJ_DIR)/*.o
	$(RANLIB) $@

prep_dirs:
	-mkdir $(OBJ_DIR)

logger.c.o: logger.c
	$(CCPLUS) $(CFLAGS) $(CFLAGS_PLATFORM) $(INC_DIR_PLATFORM) $< -o $@

%.c.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_PLATFORM) $(INC_DIR_PLATFORM) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_PLATFORM) $(INC_DIR_PLATFORM) $< -o $@

%.o: %.cpp
	$(CCPLUS) $(CFLAGS) $(CFLAGS_PLATFORM) $(INC_DIR_PLATFORM) $< -o $@

%.cpp.o: %.cpp
	$(CCPLUS) $(CFLAGS) $(CFLAGS_PLATFORM) $(INC_DIR_PLATFORM) $< -o $@

# core.a and $(APP_NAME).cpp.o are combined to generate $(APP_NAME).elf.
# For Integration with Top Layer, we can link top layer with core.a
$(APP_NAME).elf: $(APP_NAME).cpp.o core.a
	$(CC) -w -Os -Wl,--gc-sections,--relax -mmcu=atmega2560 -o $@ $^ -L$(ARDUINO_DIR)/libraries -lm

$(APP_NAME).hex: $(APP_NAME).elf
	$(AVR_OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $(APP_NAME).eep
	$(AVR_OBJCOPY) -O ihex -R .eeprom $< $@

install: all
	$(AVR_PROGRAMMER) -C$(ARDUINO_DIR)/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega2560 -cstk500v2 -P/dev/$(ARDUINO_PORT) -b115200 -D -Uflash:w:$(APP_NAME).hex:i


.PHONY: clean
clean:
	@rm -f *.o *.d *.elf *.eep core.a *.hex *.bin *.map *-
	@rm -rf $(OBJ_DIR)




