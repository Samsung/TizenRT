# OCF Light sample
- Smart Home Light sample.
  - icv: "ocf.1.0.0"
  - dmv: "ocf.res.1.3.0,ocf.sh.1.3.0"
  - device type: "oic.d.light"
  - otm: Justworks("oic.sec.doxm.jw")
- Resources
  - /light: connected to ARTIK-05X LED703
  - /dimm: virtual resource. not connected to physical LED

## Configure
```sh
cd tizenrt/os/tools
./configure.sh artik053/nettest
cd ..

make menuconfig
  External Functions --->
      IOTIVITY Config Parameters --->
          [*] enable / disable iotivity stack
          (1.3-rel) iotivity stack versiona
          (32768) Default pthread stack size for receive-handler thread
          [*] enable iotivity security
          [*] Set Iotivity to Release mode
  Application Configuration --->
      Examples --->
          [*] OCF My Light sample application

# Optional - Configure with localtime (timezone support)
  Library Routines --->
      [*] localtime API call support
      (255) Maximum number of times in timezone
      (50) Maximum number of TZ types
      (/mnt) zoneinfo directory path

# Optional - Configure with dimming resource
  Application Configuration --->
      Examples --->
          [*] OCF My Light sample application
          [*]   Add dimmer resource (oic.r.light.dimming)
```

## Build and flashing
- Flashing all (os image, user partition, ...)
```sh
# Build
cd tizenrt/os
make

# Flahsing
sudo make download ALL  # All
or
sudo make download os   # os image only
```

## Sample application features
### Factory reset
```sh
TASH> ocf_mylight factory
```
- Reset SVR-DB
- Remove configuration information (Wi-Fi, timezone, ...)

### Reset SVR-DB (Same as SW703 Button 3secs longpress)
```sh
TASH> ocf_mylight recovery
TASH> ls /mnt
/mnt:
 setup_info.dat       <--- wifi, time setting
 oic_svr_db.json
 oic_svr_db.dat       <--- iotivity virtual resource db
 introspection.json
 introspection.dat    <--- /introspecion/payload uri path response data
 KST                  <--- timezone file
 ...
```

### Wi-Fi auto-connection setup
```sh
TASH> ocf_mylight scan
...
Security-type: 0x00000040, SSID: IoT-1 (ssid_len=14)
Security-type: 0x00000018, SSID: dlink-1 (ssid_len=13)
...
TASH> ocf_mylight wifi {SSID} {SECURITY_TYPE} {PASSWORD}
```
- e.g. "ocf_mylight wifi IoT-1 0x40 12345678"
- Wi-Fi access point information is saved to /mnt/setup_info.txt

### Wi-Fi connection test
```sh
TASH> ocf_mylight net
...
IP address ...
Netmask    ...
Gateway    ...
```

### Timezone setting
```sh
TASH> ocf_mylght tz {KST/PST/MST}
```
- To use localtime, you can do this through the time zone file settings.
- KST, PST and MST zones are built-in to source code.
- You can add additional zoneinfo before the image build
  - copy {linux machine}/usr/share/zoneinfo/{something} to ocf_mylight/tz/zoneinfo/
  - rebuild the image

### Time setting (e.g. NTP unavailable environment)
```sh
TASH> ocf_mylight time {timestamp}
```
- You can get the timestamp from "date +%s" command on the linux shell
- At program start, the time is automatically restored using the previously
saved value. And if NTP succeeds, the time is updated back to the correct value.
- The timestamp is saved to /mnt/setup_info.dat

## Run
```sh
TASH> ocf_mylight
...(wifi connection)...
...(ntp time update)...
...(iotivity start)...
Entering ocserver main loop...
...(ready state)...
```

## Control
- SW702 Button
  - Click: Toggle /light status to ON and OFF
  - Long Press (2secs): Increase /dimm value
- SW703 Button
  - Click: Program normal exit and reboot the board
  - Long Press (3secs): Reset SVR-DB

