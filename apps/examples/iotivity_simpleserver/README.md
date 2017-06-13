# IoTivity Sample apps - simple server

This is an example for an IoTivity simple server test.

## How to build
Configure the build from $TIZENRT_BASEDIR/os/tools directory.
```bash
./configure.sh artik053/iotivity
```

Select the 'Enable build IoTivity Samples' example in menuconfig.

    -+-Application Configuration
          |
          +-Examples
               |
               +-Enable build IoTivity Samples


## How to test
### Server (Tizen RT)
Flash Tizen RT binary to the board.

Run the TASH
```bash
1. Connect to Wi-Fi
	TASH	>> wifi startsta
		>> wifi join <SSID> <PW> <Security Type>
2. Get an IP address
	TASH	>> ifconfig wl1 dhcp
3. Execute a simple server
	TASH	>> simpleServer
```


### Client (Linux)
Build IoTivity using scons.
Reference page: https://www.iotivity.org/documentation/linux/getting-started

Execute a simple client
```bash
/IOTIVITY_BASEDIR/out/linux/x86_64/release/resource/csdk/stack/samples/linux/SimpleClientServer
	$ ./occlient -t <number>
```





