# How to add(port) a new Secure Element

To support a new SE, you should make changes on */os/se* folder.  

## Contents

> [Folder structure](#folder-structure) \
> [How to include files](#how-to-modify-se-kconfig) \
> [How to implement SE chip](#how-to-implement-se-chip)

## Folder structure

```
	${TIZENRT_ROOT}/os/se/<se-chip-familyname> 
```

## How to modify SE Kconfig

To provide selection of a new Secure Element, **Kconfig** on *os/se* also need to be change like below:
```
config SE
	bool "Enable embedded SE"

if SE
choice
	prompt "SE selection"
	default SE_CHIP1

config SE_CHIP1
	bool "SE Chip 1"
	---help---
		SE chip1 support

config SE_CHIP2
	bool "SE Chip 2"
	---help---
		SE chip2 support
endchoice

if SE_CHIP1
source se/chip1/Kconfig
endif

if SE_CHIP2
source se/chip2/Kconfig
endif

endif
```

>Note: The Path of `source` should be same as folder name of `<se-chip-familyname>`.

## How to implement SE chip

To implement SE chip on TizenRT, The every library and files sholud be in *os/se/[se-chip-familyname]*.

> Note: The workspace is *os/se/[se-chip-familyname]*
1. Add files and library.
2. Create **Make.defs**
    ```
    ifeq ($(CONFIG_SE_CHIP1),y)
    SE_PATH = chip1
    SE_CSRCS += chip1_code_1.c chip1_code_2.c
    SE_LIB = libchip1.a
    SE_LIB_GEN = private_code1.c private_code2.c
    SE_LIB_OUT = libprivate.a
    endif
    ```
    At the first, check the chip config is enabled.\
    **SE_PATH** is the folder name of the chip files, and 
    **SE_CSRCS** includes source codes if needed. \
    **SE_LIB** defines SE library which can be set *only 1 file*.
    > Note1: the static library sholud be stored with gcc name such as *libchip1_arm.a* or *libchip1_xtensa.a* for cross compilation.
    
    > Note2: TizenRT will use **SE_LIB** following each compiler automatically.
    So you should set **SE_LIB** with common name.

    **SE_LIB_GEN** is only for private sources.
    If **SE_LIB_GEN** exists, the static library will be generated in **SE_LIB_OUT** following each compiler. (ex. libprivate_arm.a, libprivate_xtensa.a)

3. Create **Kconfig**
   ```
   config HW_RNG
	bool "HW RNG"
	default n
	---help---
		Supports true random generator by hardware
		Maximum random size is 256 bytes at one time.

    config HW_DH_PARAM
        bool "HW supports DH params"
        default n
        ---help---
            Generates and caculates diffie-hellman parameter with hardware.
            Supporting parameter size : 1024, 2048

    config HW_ECDH_PARAM
        bool "HW supports ECDH params"
        default n
        ---help---
            Generates and caculates eliptic curve params with hardware.
                    Supporting curves :
            . SECP 192, 224, 256, 384, 512
            . Brainpool 256

    config HW_RSA_VERIFICATION
        bool "HW rsa verification"
        default n
        ---help---
            Verifing a RSA signature based on hardware.
            Supporting key size : 1024, 2048

    config HW_ECDSA_VERIFICATION
        bool "HW ecdsa verification"
        default n
        ---help---
            Verifing a ECDSA signature based on hardware.
            Supporting curves :
            . SECP 192, 224, 256, 384, 512
            . Brainpool 256

    config HW_RSA_ENC
        bool "HW rsa encryption"
        default n
        ---help---
            Encrypts a data based on hardware.
            Supporting key size : 1024, 2048

    config SE_STORAGE
        bool "Secure Storage Support"
        default n
        ---help---
            SE supports Secure Stroage
   ```

   >HW_RNG \
   >HW_DH_PARAM \
   >HW_ECDH_PARAM \
   >HW_RSA_VERIFICATION\
   >HW_ECDSA_VERIFICATION\
   >HW_RSA_ENC \
   >SE_STORAGE

   Above 5 configs are the common config using in Security Module. \
   Extra configs can be added if necessary.

