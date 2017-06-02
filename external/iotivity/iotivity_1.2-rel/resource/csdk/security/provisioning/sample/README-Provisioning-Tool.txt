LAST UPDATED 3/28/2016

To execute Provisioning Tool sample:

1) Build IoTivity with security enabled:

    $ cd <iotivity-base>
	$ scons resource SECURED=1
	
2) Verify Provisioning Tool functionality using secure sample apps:

    Run Resource Server Device which needs to be 'provisioned' in the directory:
    $ ./sampleserver_justworks (Just Works)
	or
	$ ./sampleserver_randompin (Random Pin)
	
    Run Provisioning Tool Device:
    $ ./provisioningclient
    
    Provisioning Tool will provide prompts for discovery, ownership transfer, and provisioning.

    Enter 10 (Discover All Un/Owned Devices on Network)
    and you should see the list of discovered owned and unowned devices.
    
    Then enter 20 (Register/Own All Discovered Unowned Devices) to perform ownership transfer
    between the Server device and the Provisioning Tool device.

    If the random pin server is running, you must enter the PIN code that 
    appears on the server terminal to finish ownership transfer.

    Enter 12 (Discover Only Owned Devices on Network) to confirm that
    ownership transfer succeeded. If successful,
    you should find the Server device on the Owned device list.
    
 3) Verify Ownership Transfer using sample apps:

    If you would like to check whether ownership transfer successfully 
    created credentials, replace the server and client sample DAT files
    with oic_svr_db_client.dat and oic_svr_db_server.dat files from the 
    ownership transfer as follows:
    $ cp ./oic_svr_db_client.dat <iotivity-base>/out/<...>/release/resource/csdk/stack/samples/linux/secure/oic_svr_db.client.dat
    $ cp ./oic_svr_db_server_<...>.dat <iotivity-base>/out/<...>/release/resource/csdk/stack/samples/linux/secure/oic_svr_db.server.dat

    Then move to the sample app directory
    and execute the server and client apps:
    $ cd <iotivity-base>/out/release/resource/csdk/stack/samples/linux/secure/
    $ export LD_LIBRARY_PATH=<iotivity-base>/out/<...>/release/
    $ ./ocserverbasicops
    $ ./occlientbasicops -u 0 -t 3

    If successful, the client and server should successfully send and receive payloads.

    All security functionality operate using CBOR data (DAT files).
    JSON files are for reference only as they are human-readable.
    JSON files are not used by security-related functions.
    
    If you wish to test functionality with data file
    different from the provided default DAT file, modify the JSON files
    (oic_svr_db_server_justworks.json, oic_svr_db_server_randompin.json)
    and then use the JSON-to-CBOR conversion tool
    (<iotivity-base>/out/<...>/release/resource/csdk/security/tool/json2cbor)
    to create a new DAT file.
