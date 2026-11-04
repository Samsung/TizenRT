# Control App for QuickTrack Conformance Test

## Build

```sh
make -j$(nproc)
```

Executable `app` is the control app.

## Usage

`./app -h` shows the help:

```
./app -h
Welcome to use QuickTrack Control App DUT version 1.2.0.70.
usage:
app [-h] [-p<port number>] [-i<wireless interface>|-i<band>:<interface>[,<band>:<interface>]] [-a<hostapd path>] [-s<wpa_supplicant path>]

usage:
  -a = specify hostapd path
  -d = debug received and sent message
  -i = specify the interface. E.g., -i wlan0. Or, <band>:<interface>.
       band can be 2 for 2.4GHz, 5 for 5GHz and 6 for 6GHz. E.g., -i 2:wlan0,2:wlan1,5:wlan32,5:wlan33
  -p = port number of the application
  -s = specify wpa_supplicant path
```

Use the following command to start conformance test.

```
./app -i 2:wlp4s0 -p 9004
```

where `2:wlp4s0` means 2.4G band, and ifname is `wlp4s0`. `-p` specifies port number.

## Compile wpa_supplicant

The following steps are the instructions to compile wpa_supplicant. If you use the embedded system, you may need to modify the .config based on the toolchain.
* Extract source code .tgz file
* Change the directory to wpa_supplicant
* Update wpa_supplicant configuration
Edit .config
Add CONFIG_CTRL_IFACE=y

* Use make to compile the wpa_supplicant
* Rename and install wpa_supplicant to specific path which is defined in vendor_specific.h of ControlAppC to run the wpa_supplicant command. Refer to section 7.2 for more information.

```
apt install libnl-3-dev libnl-genl-3-dev libdbus-1-dev libssl-dev libreadline-dev
```



```c
# vendor_specific.h

#define WPAS_EXEC_FILE_DEFAULT "/usr/local/bin/WFA-Hostapd-Supplicant/wpa_supplicant"
```

## Agent

For IoT devices, there are no ether, so a laptop may agent for IoT devcies.

Install python3-serial package:

```
apt install python3-serial
```

Run Agent:

```
python3 agent.py
```

You can specify UART name by `-d /dev/ttyUSB0`.

## application json

Import  [application json](./application_profile_29484_1657076647365_ID_115185.json) to WFA QuickTrack Tool.

## Reference

DUT Development @《QuickTrack Test Tool User Manual_v1.2.pdf》 



