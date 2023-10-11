examples/mem_protect
^^^^^^^^^^^^^^^^^^^^^

  Memory protection functionality test (read and write) example.
  
  This example app tests the memory protection functionality of the
  platform by performing several read or write operations to the 
  protected memory regions of either apps or kernel. If the memory 
  protection is properly setup, then any of these read/write operations 
  will not be allowed and will result in a corresponding memory protection
  fault and will be accordingly handled by the platform. 

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_MEM_PROTECT_TEST

  Depends on:
  * CONFIG_BUILD_PROTECTED

