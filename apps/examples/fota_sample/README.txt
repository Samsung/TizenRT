examples/fota_sample
^^^^^^^^^^^^^^^^^^^^

  This is a sample example to test the firmware update functionality working at
  TASH. FOTA has two modules, driver in kernel and hal in application. You should
  enable them before testing it. You can find that running partition is changed.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_FOTA_SAMPLE

  Depends on:
  * CONFIG_SYSTEM_FOTA_HAL
  * CONFIG_FOTA_DRIVER

