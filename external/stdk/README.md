# SmartThings Device SDK

The SmartThings Device SDK(for short STDK) is the core device library that allow device applications to securely connect to the SmartThings Cloud. 
It has been integrated on TizenRT as a submodule. 

## Prerequisites

- Mobile phone with SmartThings application installed.
- Wifi AP could connect internet.
- An Artik053 board or ESP32 Wrover kit board. (with necessary devices connected)
  * Artik053, input button use SW703 on board, RGB LED connect GPIO 53(R), 51(G), 52(B), and GND connect 3.3v
  * ESP32 Wrover, input button connect GPIO 5, RGB LED connect GPIO 18(R), 21(G), 19(B), and GND connect 3.3v

## How to build with STDK

### Update submodule
```shell
git submodule update --init "external/stdk/st-device-sdk-c"
git submodule update --init "external/libsodium/libsodium"
```

### Register device and update configure info
refering https://github.com/SmartThingsCommunity/st-device-sdk-c-ref/blob/master/doc/getting_started.md 

- Create device identity and update to \apps\examples\stdk_smart_lamp\device_info.json
- Registe device and update information to \apps\examples\stdk_smart_lamp\onboarding_config.json

### Select build config
There are build configs for STDK on different boards supported, choose one according to your testing board.
Reference example of the config is \apps\examples\stdk_smart_lamp
\artik053\STDK
\esp_wrover_kit\STDK

## Test

After build and install the test image on target board, you can easysetup and control the device like other SmartThings device.
- Reboot device.
- Smartthings APP: add device, scan, select STDKxx device.
- Press input button(SW703 for artik or GPIO 5 for esp32) when confirmation is request.
  If input button is unavailable, modify the "BUTTON" to "JUSTWORKS" in onboarding_config.json.
- Select proper WiFi AP on the APP.
- After onboarding is complete, you can control the device then.
