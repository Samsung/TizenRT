examples/iotjs_startup
^^^^^^^^^^^^^^^^^^^^^^

  This is an helper example to start
  a javascript program using IoT.js (once system booted).

  Also connection to WiFi AP can be established
  before launching the javascript app.

  Usage:
  
  * Configure romfs (CONFIG_*_FLASH_PART_* and *_AUTOMOUNT_*)
  * Build main javascript in image, for instance:
    cat tools/fs/contents/example/index.js
    console.log(JSON.stringify(process));
  * Set Application entry point to "StartUp example"

  Configs (see the details on Kconfig):
