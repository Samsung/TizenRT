# How To Port TizenRT Binary Header

## Contents
- [Binary Header Information](#binary-header-information)
- [How to add Binary Header](#how-to-add-binary-header)

## Binary Header Information

		 -----------------------------------------------------------------------------------------------
		|    CRC32   | Header size | Binary Version |  Binary size | Secure Header size | Secure Header |
		|  (4 bytes) |  (2 bytes)  |   (4 bytes)    |   (4 bytes)  |     (2 bytes)      |   (# bytes)   |
		 -----------------------------------------------------------------------------------------------

These binary header components are as follows:

### 1. CRC32
This is the CRC32 value for all header and actual TizenRT binary. It is checksum value for the entire binary from Header size to the end of binary. It is of type uint32_t and its size is 4 bytes.

### 2. Header size
This is the header total data size excluding CRC32. It is of type uint16_t and its size is 2 bytes.

### 3. Binary version
This is the version of the binary in "YYMMDD" format (CONFIG_BINARY_VERSION). It is of type uint32_t and its size is 4 bytes.

### 4. Binary size
This is the actual TizenRT binary size excluding header size of the binary. It is of type uint32_t and its size is 4 bytes.

### 5. Secure header size
This is the size of the secure header and it depends on the board. It is of type uint16_t and its size is 2 bytes.

### 6. Secure header
This is the secure header signature. It is of type char and its size may vary depending upon the securing method.  
Note: The size of the secure header and the entire binary may vary depending on the secure method in the future.

## Partitions

The binary header will be added at front of TizenRT binary and it will be located in kernel partition with TizenRT binary like below.

								   Binary 1			Binary 2
									|			    |
								 ----------------------------	 ---------------
								| Header|    TizenRT Binary  |	| H |		|
	 --------------------------------------------------------------------------------------------------------------------
	|  km0 boot  |  km4 boot  |  reserved  | secure storage |	    kernel		|	kernel		| .. |
	 --------------------------------------------------------------------------------------------------------------------
	|     16     |	    8     |     16     |       472      |	     1792		|	 1792		| .. |
	 --------------------------------------------------------------------------------------------------------------------

			Figure: Example of Flash partition information for  RTL8721CSM board


## How to add Binary Header

User should define MAKE_SAMSUNG_HEADER for binary header support in build/configs/`<board>`/`<name>`/Make.defs as below:

```
KERNEL_BINARY_PATH='xxxx.bin'
SECURE_HEADER_SIZE=xx

define MAKE_SAMSUNG_HEADER
  $(TOPDIR)/../os/tools/mksamsungheader.py ${KERNEL_BINARY_PATH} kernel ${SECURE_HEADER_SIZE}
endef
```
where,
KERNEL_BINARY_PATH is the path where kernel binary is generated and stored.
For example, in case of  RTL8721CSM it is generated here:
```
build/output/bin/km0_km4_image2.bin
```
Similarly in case of imxrt, the kernel binary is generated here:
```
build/output/bin/tinyara.bin
```
SECURE_HEADER_SIZE is the size of the secure header.

