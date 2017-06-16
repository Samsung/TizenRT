# IoTivity

IoTivity is an open source software framework enabling seamless device-to-device and device-to-cloud connectivity to address the emerging needs of the Internet of Things.

Official page: https://www.iotivity.org/

## How to build with IoTivity
Configure the build from $TIZENRT_BASEDIR/os/tools directory.
```bash
cd os/tools
./configure.sh artik053/iotivity
```

Configuration can be modified through make menuconfig from $TIZENRT_BASEDIR/os.
You can enable security and cloud features for IoTivity.
```bash
cd ..
make menuconfig
```

Finally, initiate build by make from $TIZENRT_BASEDIR/os
```bash
make
```
