# How to use IoT.js


IoT.js is an open source software platform for Internet of Things with JavaScript.
For more details, please check: http://www.iotjs.net Or shipped [README](..//external/iotjs/README.md) in sources.

## Contents
- [Use reference configuration](#use-reference-configuration)
- [To enable IoT.js feature](#to-enable-iot.js-feature)
- [How to load scripts](#how-to-load-scripts)
- [How to configure Network](#how-to-configure-network)
- [Resources](#resources)

## Use reference configuration

    machine=artik053 # Adapt with your supported target  
    cd os && ./tools/configure.sh ${machine}/iotjs # Generate os/.config  
    make menuconfig # Verify WiFi credentials  
    make # Will build os image with IoT.js in and example  
  

## To enable IoT.js feature

To select the required feature, you can use the menuconfig as in the example below:

    make menuconfig  
    # Application Configuration  --->  
    #  Examples  --->  
    #   [*] IoT.js StartUp example  
    #     (/rom/example/index.js) Main javascript file  
    #     [ ]   Connect WiFi  
    #     (0)   Authentication type  
    #     (0)   Encryption type  
    # Application entry point (...) --->  
    #   (X) IoT.js StartUp example  


## How to load scripts

File can be loaded for different locations. For convenience, it is recommended to use the approach of adding a ROM partition as explained in
[HowToUseROMFS.md](HowToUseROMFS.md). Then place your script(s) in tools/fs/contents-romfs, such as, for instance:

    cat tools/fs/contents-romfs/example/index.js 
    console.log(JSON.stringify(process));


## How to configure Network

WiFi can be also enabled on boot if you want to join a network before running the script. You need to run "make menuconfig" again and edit credentials.

    make menuconfig  
    # Application Configuration  --->  
    #  Examples  --->  
    #   [*] IoT.js StartUp example  
    #     (/rom/example/index.js) Main javascript file  
    #     [*]   Connect WiFi
    #     ("APSSID") SSID of AP
    #     ("APPassword") Passphrase of AP    
    #     (4)   Authentication type  
    #     (4)   Encryption type  
    # Application entry point (...) --->  
    #   (X) IoT.js StartUp example  

Edit "SSID of AP" and "Passphrase of AP" with your WiFi SSID and password.
For values of "Authentication type" and "Encryption type", use <Help> menu to find values that match
your WiFi configurations. For example, 4 for Authentication type of "WPA and WPA2 PSK", and 4 for "Encryption type of "TKIP".


## Resources

* https://github.com/Samsung/iotjs/wiki/Build-for-ARTIK053-TizenRT
* https://archive.fosdem.org/2018/schedule/event/tizen_rt/
* https://www.slideshare.net/SamsungOSG/tizen-rt-a-lightweight-rtos-platform-for-lowend-iot-devices
* https://source.tizen.org/documentation/tizen-rt/tizen-rt-long-term-goals
