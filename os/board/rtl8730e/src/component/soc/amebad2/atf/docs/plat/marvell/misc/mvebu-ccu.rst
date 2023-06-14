Marvell CCU address decoding bindings
=====================================

CCU configration driver (1st stage address translation) for Marvell Armada 8K and 8K+ SoCs.

The CCU node includes a description of the address decoding configuration.

Mandatory functions
-------------------

- marvell_get_ccu_memory_map
    Return the CCU windows configuration and the number of windows of the
    specific AP.

Mandatory structures
--------------------

- ccu_memory_map
    Array that includes the configuration of the windows. Every window/entry is
    a struct which has 3 parameters:

      - Base address of the window
      - Size of the window
      - Target-ID of the window

Example
-------

.. code:: c

	struct addr_map_win ccu_memory_map[] = {
		{0x00000000f2000000,     0x00000000e000000,      IO_0_TID}, /* IO window */
	};
