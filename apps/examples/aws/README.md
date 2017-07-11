# AWS Example
This example codes are from [AWS IoT device SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## How to Build
Configure the build from $TIZENRT_BASEDIR/os/tools directory
TIZENRT_BASEDIR was set at [[Getting the sources]](../../../README.md#getting-the-sources) tab of Quick Start.

This example was tested on sidk_s5jt200[[details]](../../../build/configs/sidk_s5jt200/README.md).

```bash
cd os/tools
./configure.sh sidk_s5jt200/sidk_tash_aws
```

Above copies the canned configuration-set into the $TIZENRT_BASEDIR/os directory.

Configuration can be modified through make menuconfig from $TIZENRT_BASEDIR/os.
```bash
cd ..
make menuconfig
```

Finally, initiate build by make from $TIZENRT_BASEDIR/os
```bash
make
```

Built binaries are in $TIZENRT_BASEDIR/build/output/bin.


## Modify User Specific Information
### Certification Key Data
User certification keys are needed to connect, because AWS uses secure network protocol(HTTPS/MQTT).

Key values for the three certificate files need to be copied to ***certData.c*** file in $TIZENRT_BASEDIR/external/aws/certs/ directory.
- copy contents of rootCA.crt file to *root_ca_pem[ ]* array
- copy contents of cert.pem file to *client_cert_pem[ ]* array
- copy contents of privkey.pem file to *client_private_key_pem[ ]* array

<certData.c>
```c
const unsigned char root_ca_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
"..."
"..."
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
"-----END CERTIFICATE-----\r\n";
const int rootCaLen = sizeof(root_ca_pem);

const unsigned char client_cert_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDWTCCAkGgAwIBAgIUSspy09t4c2O7tLrwenrkVvbbL8YwDQYJKoZIhvcNAQEL\r\n"
"..."
"..."
"mwMi4q9hN6MT9FGa0R0KwmD9y+o11yCbrNBbP0Gud/7m/y4r+09By8xr68Lz\r\n"
"-----END CERTIFICATE-----\r\n";
const int clientCertLen = sizeof(client_cert_pem);

const unsigned char client_private_key_pem[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIEFyBt7KWeu4tZ9h3NoTA1gTcIrOpTOt5GZ0YOj6epqAkCaC+XfkJHXX/9kKAwK\r\n"
"..."
"..."
"9bc8aGVeZ+mD3SYYJamB77XqH2kcCd5Axc2EckdL9ansf+S9v1FP\r\n"
"-----END RSA PRIVATE KEY-----\r\n";
const int clientPrivateKeyLen = sizeof(client_private_key_pem);
```

### Host and device information
AWS user specific HOST and DEVICE information is in ***aws_iot_config.h*** header file in each example code directory under $TIZENRT_BASEDIR/external/aws/samples/TizenRT/

The following values are need to be updated.
- AWS_IOT_MQTT_HOST
- AWS_IOT_MQTT_CLIENT_ID
- AWS_IOT_MY_THING_NAME

<aws_iot_config.h>
```c
#define AWS_IOT_MQTT_HOST              "" ///< Customer specific MQTT HOST. The same will be used for Thing Shadow
#define AWS_IOT_MQTT_PORT              8883 ///< default port for MQTT/S
#define AWS_IOT_MQTT_CLIENT_ID         "c-sdk-client-id" ///< MQTT client ID should be unique for every device
#define AWS_IOT_MY_THING_NAME          "AWS-IoT-C-SDK" ///< Thing Name of the Shadow this device is associated with
```

## How to program binary
Built binary can be written into flash, using OpenOCD.

Please see the [Environment Set-up](../../../build/configs/sidk_s5jt200/README.md#environment-set-up) to use OpenOCD.

After building Tizen RT, follow below steps at $TIZENRT_BASEDIR/os folder.

### Using download script

```bash
make download ALL
```
This makes complete set of binaries programmed.


## How to run example
To run AWS sample application, WiFi connection is established and current date value need to be set.

#### Step 1 : Start WiFi Station Mode
Start WiFi as station mode.
```bash
TASH>> wifi startsta
```
#### Step 2 : Scan WiFi AP
**'wifi scan'** command shows the list of WiFi APs.
This command shows the information of BSSID, SECURITY, CH, and SSID.

```bash
TASH>> wifi scan
```
#### Step 3 : Connect to AP
Use **'wifi join'** command to connect to AP.
SSID is required to connect AP and it was the one of the list on Step 2.
The key and security information also required when the Wi-Fi network to connect is not open mode.

```
usage : wifi join <ssid> [ <key> <security> ] 
        ssid : name of Wi-Fi AP (maximum 32 bytes) 
        key : passphrase (format depends on security mode)
        security : type of security, Choose between:
                   open, wep, wep_shared, wpa2_tkip, wpa2_aes,
                   wpa2_mixed, wpa_aes, wpa_tkip, wpa_mixed

        If <key> is omitted, then security mode "open" is expected.
        If <ssid> or <key> contains a space then encapsulate it with quotes ""
```

\<example\>
```bash
TASH>> wifi join NETGEAR48 testwifi371 wpa2_aes
```
#### Step 4 : Get IP address
```bash
TASH>> ifconfig wl1 dhcp
```
#### Step 5 : Set Date and time
Set current date and time on device. Network connection fails if there is big difference of date and time value between on device and current.
```bash
TASH>> date -s Jun 10 13:28:47 2017
```
#### Step 6 : Run AWS example
There are 5 examples :
1. shadow
2. shadow console echo
3. subscribe publish cpp
4. subscribe publish library
5. subscribe publish

**aws_sample** command runs one of the 5 examples.

If there is no argument, this command shows the list of examples.

```bash
TASH>> aws_sample
usage : aws_sample <example_no>
    ex) aws_sample 5 ==> run aws_subscribe_publish
    1. aws_shadow
    2. aws_shadow_console_echo
    3. aws_subscribe_publish_cpp
    4. aws_subscribe_publish_library
    5. aws_subscribe_publish

TASH>> aws_sample 5

AWS IoT SDK Version 2.1.1-

Connecting...
Subscribing...
```
