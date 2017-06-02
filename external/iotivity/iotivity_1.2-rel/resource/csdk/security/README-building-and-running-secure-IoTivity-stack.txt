LAST UPDATED 11/20/2016

To prepare the environment for building IoTivity:

1)  Install the tools and libraries for building IoTivity by following the
	instructions in the <iotivity-base>/Readme.scons.txt file.

2)	In addition, install the sqlite3 developer library:
	$ sudo apt-get install libsqlite3-dev

3)  Finally, download the mbedtls library:
	$ git clone https://github.com/ARMmbed/mbedtls.git extlibs/mbedtls/mbedtls

To build the IoTivity stack with the security features enabled (required for
	certification):

1) Build IoTivity with security enabled:
	$ cd <iotivity-base>
	$ scons SECURED=1

2) Verify functionality using secure sample apps:
	$ cd <iotivity-base>/out/<...>/release/resource/csdk/stack/samples/linux/secure
	$ export LD_LIBRARY_PATH=<iotivity-base>/out/<...>/release
	$ ./ocserverbasicops &
	$ ./occlientbasicops -t 1
        NOTE: Console messages below indicate success (may not be at bottom of
        response printout... search up a page or so on the console):
         "INFO: occlientbasicops: Uri -- /a/led
          INFO: occlientbasicops: SECUREPORT udp: 51516 <-- actual port may differ
          INFO: occlientbasicops: Secure -- YES"
    Press <ctrl-c> to quit occlientbasicops, then run:
	$ ./occlientbasicops -t 2
        NOTE: receipt of of 'Get Response', 'Put Response' and 'Post Response'
        from /a/led indicates success.
        Example Response:
         "33:43.222 INFO: occlientbasicops: Callback Context for PUT recvd successfully
		  33:43.222 INFO: occlientbasicops: StackResult: OC_STACK_UNAUTHORIZED_REQ
		  33:43.222 INFO: PayloadLog: NULL Payload
		  33:43.222 INFO: occlientbasicops: =============> Put Response
		  33:43.326 INFO: occlientbasicops: Callback Context for POST recvd successfully
		  33:43.326 INFO: occlientbasicops: StackResult: UNKNOWN
		  33:43.326 INFO: PayloadLog: Payload Type: Representation
		  33:43.326 INFO: PayloadLog: 	Resource #1
		  33:43.326 INFO: PayloadLog: 	Values:
		  33:43.326 INFO: PayloadLog: 		createduri(string):/a/led/0
		  33:43.326 INFO: occlientbasicops: =============> Post Response
		  33:43.426 INFO: occlientbasicops: Callback Context for POST recvd successfully
		  33:43.426 INFO: occlientbasicops: StackResult: UNKNOWN
		  33:43.426 INFO: PayloadLog: Payload Type: Representation
		  33:43.426 INFO: PayloadLog: 	Resource #1
		  33:43.426 INFO: PayloadLog: 	Values:
		  33:43.426 INFO: PayloadLog: 		createduri(string):/a/led/1
		  33:43.426 INFO: occlientbasicops: =============> Post Response
		  33:43.526 INFO: occlientbasicops: Callback Context for POST recvd successfully
		  33:43.526 INFO: occlientbasicops: StackResult: UNKNOWN
		  33:43.526 INFO: PayloadLog: Payload Type: Representation
		  33:43.526 INFO: PayloadLog: 	Resource #1
		  33:43.526 INFO: PayloadLog: 	Values:
		  33:43.526 INFO: PayloadLog: 		state(bool):true
		  33:43.526 INFO: PayloadLog: 		power(int):15
		  33:43.526 INFO: occlientbasicops: =============> Post Response"
