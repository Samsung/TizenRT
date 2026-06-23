examples/secure_storage
^^^^^^^^^^^^^^

  Secure Storage APIs is used to write data into flash with encryption.
  This is the Secure Storage example for AmebaD, AmebaLite, AmebaSmart and Armino.

  This shows how to call write and read from Secure Storage with SE APIs,
  and how to read data written to flash.

  Additionally, verify the Secure Storage area is protected and cannot be erased.

  This Example writes a user given byte of data (or default) to all bytes
  of a user given slot (or default) of the Secure Storage.

  Usage:
    SS API Commands (via seclink driver):
      sstorage ss_read  [-s SLOT]                    - Read via SS API
      sstorage ss_write [-s SLOT] [-d DATA]          - Write via SS API
      sstorage ss_erase [-s SLOT]                    - Erase via SS API
      sstorage ss_full  [-s SLOT] [-d DATA]          - Full test (read->write->read->erase)

    NS API Commands (via ioctl to driver):
      sstorage ns_read  [-s SLOT]                    - Read via NS API
      sstorage ns_write [-s SLOT] [-d DATA]          - Write via NS API
      sstorage ns_erase [-s SLOT]                    - Erase via NS API
      sstorage ns_full  [-s SLOT] [-d DATA]          - Full test (read->write->read->erase)

    Options:
      -s SLOT_INDEX : slot index (0-31), default 0
      -d DATA : data value to write (hex), default 0xAB

  Examples:
    # SS API tests
    sstorage ss_read -s 0
    sstorage ss_write -s 0 -d AB
    sstorage ss_erase -s 0
    sstorage ss_full -s 0 -d AB

    # NS API tests
    sstorage ns_read -s 0
    sstorage ns_write -s 0 -d AB
    sstorage ns_erase -s 0
    sstorage ns_full -s 0 -d AB

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_SSTORAGE
