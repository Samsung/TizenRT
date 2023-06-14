Marvell IO WIN address decoding bindings
========================================

IO Window configration driver (2nd stage address translation) for Marvell Armada 8K and 8K+ SoCs.

The IO WIN includes a description of the address decoding configuration.

Transactions that are decoded by CCU windows as IO peripheral, have an additional
layer of decoding. This additional address decoding layer defines one of the
following targets:

- **0x0** = BootRom
- **0x1** = STM (Serial Trace Macro-cell, a programmer's port into trace stream)
- **0x2** = SPI direct access
- **0x3** = PCIe registers
- **0x4** = MCI Port
- **0x5** = PCIe port

Mandatory functions
-------------------

- marvell_get_io_win_memory_map
    Returns the IO windows configuration and the number of windows of the
    specific AP.

Mandatory structures
--------------------

- io_win_memory_map
    Array that include the configuration of the windows. Every window/entry is
    a struct which has 3 parameters:

	  - Base address of the window
	  - Size of the window
	  - Target-ID of the window

Example
-------

.. code:: c

	struct addr_map_win io_win_memory_map[] = {
		{0x00000000fe000000,	0x000000001f00000,	PCIE_PORT_TID}, /* PCIe window 31Mb for PCIe port*/
		{0x00000000ffe00000,	0x000000000100000,	PCIE_REGS_TID}, /* PCI-REG window 64Kb for PCIe-reg*/
		{0x00000000f6000000,	0x000000000100000,	MCIPHY_TID},	/* MCI window  1Mb for PHY-reg*/
	};
