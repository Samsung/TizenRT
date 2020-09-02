/*
 * stm32_exflash.c
 *
 *  Created on: Sep 30, 2019
 *      Author: calebkang
 */
#include <tinyara/config.h>

#include <stdio.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/spi/ospi.h>

#include <arch/board/board.h>
#include <arch/board/stm32l4r9i_discovery_io.h>
#if defined(CONFIG_FLASH_PARTITION)
#include <sys/mount.h>
#include <tinyara/fs/mksmartfs.h>

#include "common.h"
#endif
#include "up_arch.h"
#include "stm32l4_ospi.h"


struct ospi_dev_s *g_ospi_ops;

#define EXFLASH_BUFF_LEN        (100)
static uint8_t exFlashRxData[EXFLASH_BUFF_LEN];
static uint8_t exFlashTxData[EXFLASH_BUFF_LEN]="1234567890**********1234567890**********1234567890**********1234567890**********1234567890**********";

static void initial_exflash_read_check(void)
{
  for(int i=0; i<0x4000; i=i+5)
  {
    stm32_exflash_read(&exFlashRxData[0], i, 5);
    printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
        exFlashRxData[0],
        exFlashRxData[1],
        exFlashRxData[2],
        exFlashRxData[3],
        exFlashRxData[4]);
  }
}

static void initial_exflash_write_check(void)
{
  for(int i=0; i<0x4000; i=i+5)
  {
    stm32_exflash_write(&exFlashTxData[0], i, 5);
  }
}

static void initial_exflash_all_erase_check(void)
{
  for(int i=0; i<0x800; i++)
  {
    stm32_exflash_erase_sector(i);
  }
}


static void initial_exflash_check(void)
{
  int i;

  stm32_exflash_read(exFlashRxData, 0x0, EXFLASH_BUFF_LEN);
  printf("First Read Flash===\n");
  for(i=0; i<EXFLASH_BUFF_LEN; i++)
  {
    printf("0x%02x\n", exFlashRxData[i]);
  }

  printf("Second Read Flash after erasing===\n");
  stm32_exflash_erase_sector(0);
  stm32_exflash_read(exFlashRxData, 0x0, EXFLASH_BUFF_LEN);
  for(i=0; i<EXFLASH_BUFF_LEN; i++)
  {
    printf("0x%02x\n", exFlashRxData[i]);
  }

  printf("Second Read Flash after writing===\n");
  stm32_exflash_write(exFlashTxData, 0x0, EXFLASH_BUFF_LEN);
  stm32_exflash_read(exFlashRxData, 0x0, EXFLASH_BUFF_LEN);

  for(i=0; i<EXFLASH_BUFF_LEN; i++)
  {
    printf("0x%02x\n", exFlashRxData[i]);

    if(exFlashRxData[i] != exFlashTxData[i])
      break;
  }

  if(i != EXFLASH_BUFF_LEN)
  {
    printf("initial_exflash_check fail\n");
  }
  else
  {
    printf("initial_exflash_check succeeded!!\n");
  }

  HAL_Delay(1000);
}

/****************************************************************************
 * Name: stm32_exflash_initialize
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_initialize(void)
{
  g_ospi_ops = stm32l4_ospi_initialize(0);

#if defined(CONFIG_FLASH_PARTITION)
    int ret;
    struct mtd_dev_s *mtd;

    mtd = (FAR struct mtd_dev_s *)mtd_initialize();
    ret = configure_partitions(mtd, &g_flash_part_data);
    if (ret != OK) {
        lldbg("ERROR: configure_mtd_partitions failed");
        return;
    }

    ret = mksmartfs("/dev/smart0p0", false);

    if (ret != OK) {
      printf("USERFS ERROR: mksmartfs failed\n");
    } else {
      printf("SUCCESS: mksmartfs\n");
      ret = mount("/dev/smart0p0",
          "/mnt",
          "smartfs", 0, NULL);
      if (ret != OK) {
        printf("USERFS ERROR: mounting failed\n");
      }
    }
#endif

}

void stm32_exflash_write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_WRITE;
  meminfo.addr = WriteAddr;
  meminfo.buffer = (uint8_t*)pData;
  meminfo.buflen = Size;

  int error = g_ospi_ops->ops->memory(g_ospi_ops, &meminfo);

  if(error != 0/*OSPI_NOR_OK*/)
  {
    printf("Flash write failed : %d!!\n", error);
  }
  else
  {
    printf("Flash write succeeded!!\n");
  }
}


/****************************************************************************
 * Name: stm32_exflash_read
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_READ;
  meminfo.addr = ReadAddr;
  meminfo.buffer = (uint8_t*)pData;
  meminfo.buflen = Size;

  int error = g_ospi_ops->ops->memory(g_ospi_ops, &meminfo);

  if(error != 0/*OSPI_NOR_OK*/)
  {
    printf("Flash read failed : %d!!\n", error);
  }
  else
  {
    printf("Flash read succeeded!!\n");
  }
}


/****************************************************************************
 * Name: stm32_exflash_erase_block
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_erase_block(uint32_t block)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_ERASEBLCK;
  meminfo.addr = block;

  int error = g_ospi_ops->ops->memory(g_ospi_ops, &meminfo);

  if(error != 0/*OSPI_NOR_OK*/)
  {
    printf("Flash erase block failed : %d!!\n", error);
  }
  else
  {
    printf("Flash erase block succeeded!!\n");
  }
}


/****************************************************************************
 * Name: stm32_exflash_erase_sector
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_erase_sector(uint32_t sector)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_ERASESECT;
  meminfo.addr = sector;

  int error = g_ospi_ops->ops->memory(g_ospi_ops, &meminfo);

  if(error != 0/*OSPI_NOR_OK*/)
  {
    printf("Flash erase sector failed : %d!!\n", error);
  }
  else
  {
    printf("Flash erase sector succeeded!!\n");
  }
}

/****************************************************************************
 * Name: stm32l4_ospi_enter_memorymapped
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_enter_memorymapped(void)
{
  stm32l4_ospi_enter_memorymapped(g_ospi_ops, NULL, 0);
}


/****************************************************************************
 * Name: stm32l4_ospi_enter_memorymapped
 *
 * Description:
 *   Put the OSPI device into memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *   meminfo - parameters like for a memory transfer used for reading
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void stm32_exflash_exit_memorymapped(void)
{
  stm32l4_ospi_exit_memorymapped(g_ospi_ops);
}










