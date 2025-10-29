## Installation

```shell
$ apt install python3-pyserial python3-click python3-pyyaml 
$ python3 setup.py install --user
```

Or use `requirements.txt` with `pip`:

## Uart downloader Usage

```
Beken Uart Downloader.

optional arguments:
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-b', '--baudrate', default=2000000, type=int, help="Burn UART baudrate, defaults to 2000000")
@click.option('-r', '--reboot', is_flag=True,default = True, help="Reboot after download process")
@click.option('--link_type', type=click.Choice(['0', '1', '2']), default='0', help="link boot rom type")
@click.option('--reset_type', type=click.Choice(['0', '1', '2']), default='0', help="reset type,0=dtr/rts,1=command 'reboot',2=command '95 27'")
@click.option('-g','--getbus_times', default=100, type=int, help="get bus times, default is 100 times")
@click.option('--retrycnt', default=5, type=int, help="Retry times")
@click.option('--chipid', default=0, type=int, help="default 0 will read from chipset,0x3000 for generic BT project, 0x3436 is a special one")
@click.option('-c', '--chipall', is_flag=True,default = False, help="erase/read all flash")
@click.option('-d', '--debug', type=click.Choice(['0', '1', '2', '3', '999']), default='3',
        help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file")
@click.option('-i', '--infile', type=click.Path(),help="Input one for more existing binary files.")
@click.option('-s', '--startaddr', default='0x0', help="Burn flash start address, defaults to 0x0")
@click.option('--safe_json', type=click.Path(), help="Input safe json file path")
@click.option('--aes_key', type=str, help="Input aes key stream")
@click.option('--efuse', type=str,help="Write efuse as 1, address is efuse byte index, include: [address-bit], such as: --efuse 0x1-0x7,0x2-0x1")
@click.option('--otp', type=str,help="Write OTP,value must be hex format, include: [value@address-length], such as: --otp 73c7bf397f2ad6bf@0x4b100460-8,73c7bf397f2ad6bf@0x4b100460-8")
@click.option('--otp_pm', type=str,help="Set OTP permission,include: [pm@address-length], such as: --otp_pm ro@0x4b100460-32,ro@0x4b100460-32")
@click.option('--dev_name', type=str, help="Set device name,include : [data@startAddress-length],such as : --dev_name beken@0x2e000-5")
@click.option('-m','--mac', type=str, help="Set mac information,include : [data@startAddress-length],such as : --mac aabbccddeeff@0x1e000-6")
@click.option('--big_endian', is_flag = True,default = False, help="Write device ID(MAC),set big-endian")
```

## examples:

* baud rate can be modified for faster/slower, e.g. -b 115200 for more reliable with dodgy connections

# flash

run this, then re-power the unit, repeating until the flashing starts

`python bk_loader download -p 46 -i all-app.bin`
`python bk_loader download -p 39 -i all-app.bin --dev_name BEKEN@0x2e000-5 --mac aabbccddeeff@0x2e008-6`
`python bk_loader read -p 39 -f 0x1000-2000 --console`
`python bk_loader erase -p 39 -f 0x1000-2000`
`python bk_loader read -p 39 --read_efuse`
`python bk_loader read -p 39 --read_otp 0x4b100460-32`
`python bk_loader read -p 39 --read_uid`
`python bk_loader erase -p 39 -c`
`python bk_loader download -p 39 --otp_pm na@0x4b100460-32`

## Basic Option

### Write Binary Data to Flash: download
- Binary data can be written to the Beken chip via the option 'download': `bk_loader download -p 3 -i all-app.bin`
- The command line above means writing 'all-app.bin' to flash by serial port 'COM3'

### Read Data from Flash: read
- The option 'read' allows reading back the contents of flash: `bk_loader read -p 3 -f dump.bin@0-0x1000`
- The commandline above means reading flash by serial port 'COM3', start address is '0' and size is '0x1000', then save to 'dump.bin'


### Erase flash: erase
- The option 'erase' allows erasing flash: `bk_loader erase -p 3 -f 0-0x1000` 
- The commandline above means erasing flash by serial port 'COM3', start address is '0' and size is '0x1000'

### Tool Group: tools
- The option 'tools' can call some useful tools such as list serial ports: `bk_loader tools list_ports`, do chip linkcheck: `bk_loader tools linkcheck -p 3 -b 115200`, do chip reboot: `bk_loader tools reboot -p 3 -b 2000000`


## Basic Commands

### Serial Port

- The serial port is selected using the '-p' option, like '-p /dev/ttyUSB0'(Linux and macOS) or '-p COM1'(Windows).
- This option is required always.
- Windows and macOS may required drivers to be installed for a particular USB/serial adapter, before a serial port is available.

### Baud Rate

- Baud rate for download flash and dump flash may be set using '-b 921600', the option 'download' default baud rate is 2000000, the option 'erase' default baud rate is 2000000, and the option 'dump' default baud rate is 500000.
- The baud rate is limited to 115200 while 'bk_loader' establishes the initial connection with Beken chip, higher speeds are only used for data transfer.

### Reboot

- Using '-r' to reboot chip after downloading, erasing, reading work, if there is no this argument in commandline, do not reboot after downloading, erasing, reading

### Link Type

- There may be 3 options to establish connection with Beken chip, '--link_type 0/1/2', default value is '0'. '0' means link to bootrom, '1' means link to bootloader, and '2' means link to bootrom which is older version

### Reset Type

- There may be 3 options to reset Beken chip, and we need reset chip to get bus with our chip. '--reset_type 0/1/2', defualt value is '0'. '0' means reseting chip by setting serial port rts/dtr, and this operation execute hardware reset. '1' means app reset by hex command, and '2' means app reset by ascii command.

### Get Bus Times
- Use '--getbus_times' to set the times trying to handshake with chip, default config is 100 times

### Retry Count
- Use '--retrycnt' to set the times to retry if excute command failed, default retry count is 5

### Chip ID
- Some chip cannot read chip ID by command, we can use '--chipid' to set it instead of reading

### Debug Level
- Set log level by commandline '--debug 3', the level can be '[0, 1, 2, 3, 999]'

### Set erase/read all flash
- Use '-c/--chip_all' to set erase or read all flash

## Commands about download

### Binary file
- Use '-i all-app.bin' to set the binary file path, this command support setting multi binary file, use ',' to seprate them

### Flash start address
- Use '--startaddr' to set burn flash start address, default set to 0

### Safe json file
- Some security information may save in json file, if the json file path is provided in commandline like '--safe_json otp_efuse_config.json', then we wiil write the security information to chip according to the json file

### Set AES key
- Use '--aes_key' to set the AES key and will replace the AES key in safe json file

### Write efuse
- Use '--efuse' to provide efuse info, write efuse as 1, address is efuse byte index, or write efuse u8 value, include: [address-bit] or [1@address-bit], such as: '--efuse 0x1-0x10,1@0x2-0x1'

### Write OTP
- Use '--otp' to provide OTP info, value must be hex format, include: [value@address-length], such as: '--otp 73c7bf397f2ad6bf@0x4b100460-8,73c7bf397f2ad6bf@0x4b100460-8'

### Set OTP permission
- Use '--otp_pm' to provide OTP permission info, include: [pm@address-length], such as: '--otp_pm ro@0x4b100460-32,ro@0x4b100460-32'

### Set OTP content reverse
- Use '--otp_reverse' to set OTP data reverse per 4 bytes, such as '73c7bf39 7f2ad6bf 4e7403a7 b965dc5c e0645df0 39c2d69c 814ffb40 3183fb18' -> '3183fb18 814ffb40 39c2d69c e0645df0 b965dc5c 4e7403a7 7f2ad6bf 73c7bf39', BK7236 AES key need use this command

### Set OTP data big endian
- Use '--otp_big_endian' to set OTP data writting with big-endian, if the OTP data is big-endian, then use this command while writting OTP

### Write device name
- Use '--dev_name' to provide device name write to flash, include : [data@startAddress-length],such as : '--dev_name beken@0x2e000-5'

### Write device MAC
- Use '--mac' to provide device MAC write to flash, include : [data@startAddress-length],such as : --mac aabbccddeeff@0x1e000-6'

### Set data big-endian
- Use '--big_endian' to set MAC writting with big-endian


## Commands about read

### Set read out path
Use '--out_path' to set read file path

### Flash start address
- Use '--startaddr' to set read flash start address, default set to 0

### Freedom read flash
- Use '-f' to set flash start address and length to read, Hex data, such as [-f x-1.bin@0-10000] | [-f D:\all.bin@0x10000-0x1000], if the commands contains path, the read data will save to the file

### Set print on console
- If you want to print the data on console instead of saving in file, add '--console' in commandline

### Read UID
- Use '--read_uid' to set read chip UID flag

### Read efuse
- Use '--read_efuse' to set read efuse flag 

### Read OTP
- Use '--read_otp' to set read OTP information with reg address and length, such as [0x4b010000-4], it also support multi read, such as [0x4b010000-4,0x4b100460-32]

## Commands about erase

### Freedom erase flash
- Use '-f' to set flash start address and length to erase, Hex data, such as [-f 0-10000]



