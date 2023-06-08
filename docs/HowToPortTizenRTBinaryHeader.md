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
This is the version of the binary in "YYMMDD" format (CONFIG_BOARD_BUILD_DATE). It is of type uint32_t and its size is 4 bytes.

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

User should define MAKE_SAMSUNG_HEADER for binary header support in build/configs/`<board>`/Make.defs as below:

```
SECURE_HEADER_SIZE=xx

define MAKE_SAMSUNG_HEADER
  $(TOPDIR)/../os/tools/mksamsungheader.py $(1) kernel ${SECURE_HEADER_SIZE}
endef
```
This parameters are as follows:

1. `$(1)` is source binary extension. This is used to import the source binary path in `mksamsungheader.py`, so we should fill it out as it is.
2. `${SECURE_HEADER_SIZE}` is the size of the secure header. set a size as per board bootloader.


The `MAKE_SAMSUNG_HEADER` is called after `MAKE_BOARD_SPECIFIC_BIN`. The binary name can be changed in `MAKE_BOARD_SPECIFIC_BIN`. So we manage kernel binary name using `board_metadata.txt`. The `mksamsungheader.py` gets kernel binary name from board_metadata.txt

For example, rtl8721csm board's `board_metadata.txt` as below.
```
KERNEL="km0_km4_image2"
```
