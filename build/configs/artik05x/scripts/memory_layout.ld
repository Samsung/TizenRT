/* This file is auto-generated based on values from .config file */

MEMORY
{
  kflash (rx)  : ORIGIN = 0x040C8000, LENGTH = K
  uflash (rx)  : ORIGIN = 0x00000000, LENGTH = K
  ksram  (rwx)  : ORIGIN = 0x02023800, LENGTH = 242K
  usram  (rwx)  : ORIGIN = 0x02060000, LENGTH = 128K
}
