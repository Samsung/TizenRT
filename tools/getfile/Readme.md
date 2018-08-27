# How to Use getfile tool
Here we explain how to enable build support for getfile, and how to use this function.

## Contents
> [Build Steps](#how-to-enable-getfile)
> [Use Steps](#how-to-use-getfile)

## How to enable getfile
Below configuration must be added your project configuration file to support getfile
CONFIG_SYSTEM_ENABLE_GETFILE=y

example:
project configuration file
$TIZENRT_BASEDIR/build/configs/artik053/tc/defconfig

## How to use getfile
With the getfile configured above, whenever use this tool to get file from the target board,
Use the following command:

<Target Side>

First, Open UART terminal
"minicom -D /dev/ttyUSB1"

Second, run "getfile" command in target
TASH>>getfile
After run this command, will displays the following message in minicom
```
	****************************************************
	Disconnect this serial terminal and Run getfile Tool
	****************************************************
```
Third, disconnect/close your serial terminal (may be minicom) by CTRL+A+Q

<PC Side>

First, enter getfile directory
cd $TIZENRT_BASEDIR/tools/getfile/

Second, run getfile.sh script
#./getfile.sh num "file path" "local store file path"

Note:
num       : how many files need to be get from target (example: 1, 2, 3....)
file path : files path in target
store path: file store path in PC side

After you see this message, you can get file by following the step below:

```
Target Handshake successful


Target recieve file path successful

Starting pull file via Uart from :/mnt/test.txt......
start_recv: file buf address = 0x020659d0, file size = 447
[>]
Successfully! Now can continue reconnect UART at terminal
```
