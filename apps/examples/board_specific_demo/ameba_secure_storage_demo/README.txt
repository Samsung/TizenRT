examples/secure_storage
^^^^^^^^^^^^^^

  Secure Storage APIs is used to write data into flash with encryption.
  This is the Secure Storage example for AmebaD and AmebaLite.

  This shows how to call write and read from Secure Storage with SE APIs,
  and how to read data written to flash.

  This Example writes a user given byte of data (or default) to all bytes
  of a user given slot (or default) of the Secure Storage.

  Usage: sstorage -s SLOT_INDEX -d DATA
        SLOT_INDEX : slot index to save the DATA, valid range 0 ~ 31
        DATA : value to be saved in byte

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_SSTORAGE
