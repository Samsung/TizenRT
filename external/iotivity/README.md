# IoTivity

IoTivity is an open source software framework enabling seamless device-to-device and device-to-cloud connectivity to address the emerging needs of the Internet of Things.  
Official page: [IoTivity](https://www.iotivity.org/)

## How to build with IoTivity

### Prerequisites
SCons is a mandatory package to build IoTivity. Install it as below:  
```bash
sudo apt-get install scons
```
For more information refer to the [web site](http://www.scons.org/doc/production/HTML/scons-user.html#chap-build-install).

### How to enable IoTivity
There are two methods to enable IoTivity. One is using a pre-set configuration(defconfig), the other is using menuconfig.  
As a default configuration, you can use the IoTivity defconfig. Otherwise, you should enable this function with menuconfig.

#### Using pre-set configuration
Use the IoTivity configuration.  
```bash
~/TIZENRT_BASEDIR$ cd os/tools
~/TIZENRT_BASEDIR/os/tools$ ./configure.sh <board>/iotivity
```
#### Using menuconfig
Configuration can be modified through 'make menuconfig' from $TIZENRT_BASEDIR/os.  
```bash
~/TIZENRT_BASEDIR$ cd os
~/TIZENRT_BASEDIR/os$ make menuconfig
```
Select the 'enable / disable iotivity stack' in menuconfig.  
Also it is possible to enable security and cloud features for IoTivity.  
```bash
-+-External Libraries
      |
      +-enable / disable iotivity stack
```

