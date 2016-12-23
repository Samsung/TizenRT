Flash Erase-all
===============

    Source: NuttX
    Author: Ken Pettit
    Date: 5 May 2013

This application erases the FLASH of an MTD flash block.  It is simply
a wrapper that calls the NuttX flash_eraseall interface.

Usage:
    flash_eraseall <flash_block_device>
