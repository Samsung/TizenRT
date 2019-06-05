examples/memory_fragmentation_test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  This is an example to intentionally fragment the heap memory
  by allocating and free-ing small and large memory segments in a mixed-up manner.
  
  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_MEMORY_FRAGMENTATION_TEST

  Depends on: DEBUG_CHECK_FRAGMENTATION
