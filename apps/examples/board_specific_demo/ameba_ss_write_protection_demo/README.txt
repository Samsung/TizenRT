examples/ss_write_protection_test
^^^^^^^^^^^^^^

  Secure Storage APIs is used to write data into flash with encryption.
  This is the Secure Storage example for AmebaD, AmebaLite and AmebaSmart

  This shows how to call write and read from Secure Storage with SE APIs,
  and how to read data written to flash.

  Also, this shows the write protection function by writing directly to the flash.
  If the board has Secure Storage Encryption Key, write protection is enabled.

  This Example writes a user given byte of data (or default) to all bytes
  of a user given slot (or default) of the Secure Storage.

  This Example is only available in Flat build, and loadable is not supported.
  (In Loadable build, it is impossible to write a value to a specific address in the flash.)

  Usage: sstorage -s SLOT_INDEX -d DATA
        SLOT_INDEX : slot index to save the DATA, valid range 0 ~ 31
        DATA : value to be saved in byte

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_SSTORAGE
    (Application Configuration -> Examples -> Board Specific Demos -> Secure Storage Write Protection example)

  Configs to enable before use:
  * CONFIG_MTD_BYTE_WRITE
    (File Systems -> Memory Technology Device (MTD) Support -> Byte write)