examples/secure_world
^^^^^^^^^^^^^^


  This example depicts how the access from non secure to secure world is restricted. 
  When non secure world tries to access(write/read) from the secure world region (0x7034C020 - 0x7039C000), 
  it causes a secure fault.
  
  SHOW_USAGE: It prints the valid options for read and write when invalid option gets pressed.
	      -r: run the read test
	      -w: run the write test

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_SECURE_WORLD
