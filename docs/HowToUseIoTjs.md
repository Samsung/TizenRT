# How to use IoT.js


IoT.js is an open source software platform
for Internet of Things with JavaScript.

More mode detailed information, please check:
http://www.iotjs.net

Or shipped [README](..//external/iotjs/README.md) in sources.


## Use reference configuration

    machine=artik053 # Adapt with your supported target  
    cd os && ./tools/configure.sh ${machine}/iotjs # Generate os/.config  
    make menuconfig # Verify WiFi credentials  
    make # Will build os image with IoT.js in and example  
  

## To enable IoT.js feature

Using menuconfig an example can be used to selected needed feature.

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

File can be loaded for different locations,
but it is convenient to use add a ROM partition as explained in
[HowToUseROMFS.md](HowToUseROMFS.md)

Then place your script(s) in tools/fs/contents, for instance:

    cat tools/fs/contents/example/index.js 
    console.log(JSON.stringify(process));


## How to configure Network

WiFi can be also enabled on boot if you want to join a network 
before running the script.

You need to run menuconfig again and edit credentials,
(typically for WPA2: Auth=5 and Enc=4)


## Resources:

* https://github.com/Samsung/iotjs/wiki/Build-for-ARTIK053-TizenRT
* https://archive.fosdem.org/2018/schedule/event/tizen_rt/
* https://www.slideshare.net/SamsungOSG/tizen-rt-a-lightweight-rtos-platform-for-lowend-iot-devices
* https://source.tizen.org/documentation/tizen-rt/tizen-rt-long-term-goals
