/*
 * stm32l4_ospi.c
 *
 *  Created on: Sep 25, 2019
 *      Author: calebkang
 */

#include <tinyara/config.h>
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/semaphore.h>
#include <tinyara/spi/ospi.h>
#include <tinyara/pm/pm.h>

#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include "stm32l4.h"

#include <arch/board/board.h>
#include "stm32l4xx_hal_interface.h"

#include <stdio.h>
#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>


/**
  * @brief  MX25LM51245G Configuration
  */
#define MX25LM51245G_FLASH_SIZE                   (1024*1024*8)//(0x4000000) /* 512 MBits => 64 MBytes */
#define MX25LM51245G_BLOCK_SIZE                   (0x10000)   /* 1024 blocks of 64 KBytes */
#define MX25LM51245G_SECTOR_SIZE                  (0x1000)    /* 16384 sectors of 4 kBytes */
#define MX25LM51245G_PAGE_SIZE                    (0x100)     /* 262144 pages of 256 bytes */

#define MX25LM51245G_DUMMY_CYCLES_READ            8
#define MX25LM51245G_DUMMY_CYCLES_READ_OCTAL_66M  6
#define MX25LM51245G_DUMMY_CYCLES_READ_OCTAL_84M  8
#define MX25LM51245G_DUMMY_CYCLES_READ_OCTAL_104M 10
#define MX25LM51245G_DUMMY_CYCLES_READ_OCTAL_133M 14

#define MX25LM51245G_CR2_DC_66M                   MX25LM51245G_CR2_DC_6_CYCLES
#define MX25LM51245G_CR2_DC_84M                   MX25LM51245G_CR2_DC_8_CYCLES
#define MX25LM51245G_CR2_DC_104M                  MX25LM51245G_CR2_DC_10_CYCLES
#define MX25LM51245G_CR2_DC_133M                  MX25LM51245G_CR2_DC_14_CYCLES

#define MX25LM51245G_CHIP_ERASE_MAX_TIME          300000
#define MX25LM51245G_BLOCK_ERASE_MAX_TIME         2000
#define MX25LM51245G_SECTOR_ERASE_MAX_TIME        400
#define MX25LM51245G_WRITE_REG_MAX_TIME           40

/**
  * @brief  MX25LM51245G Commands
  */
/* Read Operations */
#define READ_CMD                                  0x03
#define READ_4_BYTE_ADDR_CMD                      0x13

#define FAST_READ_CMD                             0x0B
#define FAST_READ_4_BYTE_ADDR_CMD                 0x0C

#define OCTAL_IO_READ_CMD                         0xEC13
#define OCTAL_IO_DTR_READ_CMD                     0xEE11

/* Program Operations */
#define PAGE_PROG_CMD                             0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD                 0x12

#define OCTAL_PAGE_PROG_CMD                       0x12ED

/* Erase Operations */
#define SECTOR_ERASE_CMD                          0x20
#define SECTOR_ERASE_4_BYTE_ADDR_CMD              0x21

#define OCTAL_SECTOR_ERASE_CMD                    0x21DE

#define BLOCK_ERASE_CMD                           0xD8
#define BLOCK_ERASE_4_BYTE_ADDR_CMD               0xDC

#define OCTAL_BLOCK_ERASE_CMD                     0xDC23

#define CHIP_ERASE_CMD                            0x60
#define CHIP_ERASE_CMD_2                          0xC7

#define OCTAL_CHIP_ERASE_CMD                      0x609F
#define OCTAL_CHIP_ERASE_CMD_2                    0xC738

#define PROG_ERASE_RESUME_CMD                     0x30
#define PROG_ERASE_SUSPEND_CMD                    0xB0

#define OCTAL_PROG_ERASE_RESUME_CMD               0x30CF
#define OCTAL_PROG_ERASE_SUSPEND_CMD              0xB04F

/* Identification Operations */
#define READ_ID_CMD                               0x9F
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD         0x5A

#define OCTAL_READ_ID_CMD                         0x9F60
#define OCTAL_READ_SERIAL_FLASH_DISCO_PARAM_CMD   0x5AA5

/* Write Operations */
#define WRITE_ENABLE_CMD                          0x06
#define WRITE_DISABLE_CMD                         0x04

#define OCTAL_WRITE_ENABLE_CMD                    0x06F9
#define OCTAL_WRITE_DISABLE_CMD                   0x04FB

/* Register Operations */
#define READ_STATUS_REG_CMD                       0x05
#define READ_CFG_REG_CMD                          0x15
#define WRITE_STATUS_CFG_REG_CMD                  0x01

#define OCTAL_READ_STATUS_REG_CMD                 0x05FA
#define OCTAL_READ_CFG_REG_CMD                    0x15EA
#define OCTAL_WRITE_STATUS_CFG_REG_CMD            0x01FE

#define READ_CFG_REG_2_CMD                        0x71
#define WRITE_CFG_REG_2_CMD                       0x72

#define OCTAL_READ_CFG_REG_2_CMD                  0x718E
#define OCTAL_WRITE_CFG_REG_2_CMD                 0x728D

#define READ_FAST_BOOT_REG_CMD                    0x16
#define WRITE_FAST_BOOT_REG                       0x17
#define ERASE_FAST_BOOT_REG                       0x18

#define OCTAL_READ_FAST_BOOT_REG_CMD              0x16E9
#define OCTAL_WRITE_FAST_BOOT_REG                 0x17E8
#define OCTAL_ERASE_FAST_BOOT_REG                 0x18E7

#define READ_SEC_REG_CMD                          0x2B
#define WRITE_SEC_REG_CMD                         0x2F

#define OCTAL_READ_SECURITY_REG_CMD               0x2BD4
#define OCTAL_WRITE_SECURITY_REG_CMD              0x2FD0

#define READ_LOCK_REG_CMD                         0x2D
#define WRITE_LOCK_REG_CMD                        0x2C

#define OCTAL_READ_LOCK_REG_CMD                   0x2DD2
#define OCTAL_WRITE_LOCK_REG_CMD                  0x2CD3

#define READ_SPB_STATUS_CMD                       0xE2
#define PROG_SPB_BIT_CMD                          0xE3
#define ERASE_ALL_SPB_BIT_CMD                     0xE4

#define OCTAL_READ_SPB_STATUS_CMD                 0xE21D
#define OCTAL_PROG_SPB_BIT_CMD                    0xE31C
#define OCTAL_ERASE_ALL_SPB_BIT_CMD               0xE41B

#define READ_DPB_REG_CMD                          0xE0
#define WRITE_DPB_REG_CMD                         0xE1

#define OCTAL_READ_DPB_REG_CMD                    0xE01F
#define OCTAL_WRITE_DPB_REG_CMD                   0xE11E

/* Power Down Operations */
#define DEEP_POWER_DOWN_CMD                       0xB9
#define RELEASE_DEEP_POWER_DOWN_CMD               0xAB

#define OCTAL_ENTER_DEEP_POWER_DOWN_CMD           0xB946

/* Burst Operations */
#define SET_BURST_LENGTH_CMD                      0xC0

#define OCTAL_SET_BURST_LENGTH_CMD                0xC03F

/* One-Time Programmable Operations */
#define ENTER_SECURED_OTP_CMD                     0xB1
#define EXIT_SECURED_OTP_CMD                      0xC1

#define OCTAL_ENTER_SECURED_OTP_CMD               0xB14E
#define OCTAL_EXIT_SECURED_OTP_CMD                0xC13E

/* No Operation */
#define NO_OPERATION_CMD                          0x00

#define OCTAL_NO_OPERATION_CMD                    0x00FF

/* Reset Operations */
#define RESET_ENABLE_CMD                          0x66
#define RESET_MEMORY_CMD                          0x99

#define OCTAL_RESET_ENABLE_CMD                    0x6699
#define OCTAL_RESET_MEMORY_CMD                    0x9966

/* Protection Operations */
#define WRITE_PROTECT_SEL_CMD                     0x68

#define OCTAL_WRITE_PROTECT_SEL_CMD               0x6897

#define GANG_BLOCK_LOCK_CMD                       0x7E
#define GANG_BLOCK_UNLOCK_CMD                     0x98

#define OCTAL_GANG_BLOCK_LOCK_CMD                 0x7E81
#define OCTAL_GANG_BLOCK_UNLOCK_CMD               0x9867

/**
  * @brief  MX25LM51245G Registers
  */
/* Status Register */
#define MX25LM51245G_SR_WIP                       ((uint8_t)0x01)        /*!< Write in progress */
#define MX25LM51245G_SR_WEL                       ((uint8_t)0x02)        /*!< Write enable latch */
#define MX25LM51245G_SR_BP                        ((uint8_t)0x3C)        /*!< Block protect */

/* Configuration Register 1 */
#define MX25LM51245G_CR1_ODS                      ((uint8_t)0x07)        /*!< Output driver strength */
#define MX25LM51245G_CR1_TB                       ((uint8_t)0x08)        /*!< Top / bottom */

/* Configuration Register 2 */
/* Address : 0x00000000 */
#define MX25LM51245G_CR2_REG1_ADDR                ((uint32_t)0x00000000) /*!< CR2 register address 0x00000000 */
#define MX25LM51245G_CR2_SOPI                     ((uint8_t)0x01)        /*!< STR OPI Enable */
#define MX25LM51245G_CR2_DOPI                     ((uint8_t)0x02)        /*!< DTR OPI Enable */
/* Address : 0x00000200 */
#define MX25LM51245G_CR2_REG2_ADDR                ((uint32_t)0x00000200) /*!< CR2 register address 0x00000200 */
#define MX25LM51245G_CR2_DQSPRC                   ((uint8_t)0x01)        /*!< DTR DQS pre-cycle */
#define MX25LM51245G_CR2_DOS                      ((uint8_t)0x02)        /*!< DQS on STR mode */
/* Address : 0x00000300 */
#define MX25LM51245G_CR2_REG3_ADDR                ((uint32_t)0x00000300) /*!< CR2 register address 0x00000300 */
#define MX25LM51245G_CR2_DC                       ((uint8_t)0x07)        /*!< Dummy cycle */
#define MX25LM51245G_CR2_DC_20_CYCLES             ((uint8_t)0x00)        /*!< 20 Dummy cycles */
#define MX25LM51245G_CR2_DC_18_CYCLES             ((uint8_t)0x01)        /*!< 18 Dummy cycles */
#define MX25LM51245G_CR2_DC_16_CYCLES             ((uint8_t)0x02)        /*!< 16 Dummy cycles */
#define MX25LM51245G_CR2_DC_14_CYCLES             ((uint8_t)0x03)        /*!< 14 Dummy cycles */
#define MX25LM51245G_CR2_DC_12_CYCLES             ((uint8_t)0x04)        /*!< 12 Dummy cycles */
#define MX25LM51245G_CR2_DC_10_CYCLES             ((uint8_t)0x05)        /*!< 10 Dummy cycles */
#define MX25LM51245G_CR2_DC_8_CYCLES              ((uint8_t)0x06)        /*!< 8 Dummy cycles */
#define MX25LM51245G_CR2_DC_6_CYCLES              ((uint8_t)0x07)        /*!< 6 Dummy cycles */
/* Address : 0x00000500 */
#define MX25LM51245G_CR2_REG4_ADDR                ((uint32_t)0x00000500) /*!< CR2 register address 0x00000500 */
#define MX25LM51245G_CR2_PPTSEL                   ((uint8_t)0x01)        /*!< Preamble pattern selection */
/* Address : 0x40000000 */
#define MX25LM51245G_CR2_REG5_ADDR                ((uint32_t)0x40000000) /*!< CR2 register address 0x40000000 */
#define MX25LM51245G_CR2_DEFSOPI                  ((uint8_t)0x01)        /*!< Enable SOPI after power on reset */
#define MX25LM51245G_CR2_DEFDOPI                  ((uint8_t)0x02)        /*!< Enable DOPI after power on reset */

/* Security Register */
#define MX25LM51245G_SECR_SOI                     ((uint8_t)0x01)        /*!< Secured OTP indicator */
#define MX25LM51245G_SECR_LDSO                    ((uint8_t)0x02)        /*!< Lock-down secured OTP */
#define MX25LM51245G_SECR_PSB                     ((uint8_t)0x04)        /*!< Program suspend bit */
#define MX25LM51245G_SECR_ESB                     ((uint8_t)0x08)        /*!< Erase suspend bit */
#define MX25LM51245G_SECR_P_FAIL                  ((uint8_t)0x20)        /*!< Program fail flag */
#define MX25LM51245G_SECR_E_FAIL                  ((uint8_t)0x40)        /*!< Erase fail flag */
#define MX25LM51245G_SECR_WPSEL                   ((uint8_t)0x40)        /*!< Write protection selection */



/* Private constants --------------------------------------------------------*/
/** @defgroup STM32L4R9I_DISCOVERY_OSPI_NOR_Private_Constants Private Constants
  * @{
  */
#define OSPI_NOR_OCTAL_DISABLE      0x0
#define OSPI_NOR_OCTAL_ENABLE       0x1

#define DUMMY_CYCLES_READ_OCTAL     MX25LM51245G_DUMMY_CYCLES_READ_OCTAL_66M
#define DUMMY_CYCLES_CR_CFG         MX25LM51245G_CR2_DC_66M
/**
  * @}
  */


/* OSPI Error codes */
#define OSPI_NOR_OK            ((uint8_t)0x00)
#define OSPI_NOR_ERROR         ((uint8_t)0x01)
#define OSPI_NOR_BUSY          ((uint8_t)0x02)
#define OSPI_NOR_NOT_SUPPORTED ((uint8_t)0x04)
#define OSPI_NOR_SUSPENDED     ((uint8_t)0x08)

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32L4R9I_DISCOVERY_OSPI_NOR_Exported_Types Exported Types
  * @{
  */
/* OSPI Info */
typedef struct {
  uint32_t FlashSize;          /*!< Size of the flash */
  uint32_t EraseSectorSize;    /*!< Size of sectors for the erase operation */
  uint32_t EraseSectorsNumber; /*!< Number of sectors for the erase operation */
  uint32_t ProgPageSize;       /*!< Size of pages for the program operation */
  uint32_t ProgPagesNumber;    /*!< Number of pages for the program operation */
} OSPI_NOR_Info;


/* Uncomment this line to use the memory in SDR mode */
//#define BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
#define BSP_OSPI_MEMORY_MAPPED_ENABLE

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32L4R9I_DISCOVERY_OSPI_NOR_Private_Variables Private Variables
  * @{
  */
OSPI_HandleTypeDef OSPINORHandle;

/* OSPI methods */
static int      ospi_lock(struct ospi_dev_s *dev, bool lock);
static uint32_t ospi_setfrequency(struct ospi_dev_s *dev, uint32_t frequency);
static void     ospi_setmode(struct ospi_dev_s *dev, enum ospi_mode_e mode);
static void     ospi_setbits(struct ospi_dev_s *dev, int nbits);
static int      ospi_command(struct ospi_dev_s *dev, struct ospi_cmdinfo_s *cmdinfo);
static int      ospi_memory(struct ospi_dev_s *dev, struct ospi_meminfo_s *meminfo);
static FAR void *ospi_alloc(FAR struct ospi_dev_s *dev, size_t buflen);
static void     ospi_free(FAR struct ospi_dev_s *dev, FAR void *buffer);

static void    OSPI_NOR_MspInit            (void);
static void    OSPI_NOR_MspDeInit          (void);
static uint8_t OSPI_NOR_ResetMemory        (OSPI_HandleTypeDef *hospi);
static uint8_t OSPI_NOR_WriteEnable        (OSPI_HandleTypeDef *hospi);
static uint8_t OSPI_NOR_AutoPollingMemReady(OSPI_HandleTypeDef *hospi, uint32_t Timeout);
static uint8_t OSPI_NOR_OctalMode          (OSPI_HandleTypeDef *hospi, uint8_t Operation);

static int ospi0_interrupt(int irq, void *context, FAR void *arg);
static void Error_Handler(int no);

struct stm32l4_ospidev_s
{
  struct ospi_dev_s ospi;       /* Externally visible part of the OSPI interface */
  uint32_t base;                /* OSPI controller register base address */
  uint32_t frequency;           /* Requested clock frequency */
  uint32_t actual;              /* Actual clock frequency */
  uint8_t mode;                 /* Mode 0,3 */
  uint8_t nbits;                /* Width of word in bits (8 to 32) */
  uint8_t intf;                 /* OSPI controller number (0) */
  bool initialized;             /* TRUE: Controller has been initialized */
  sem_t exclsem;                /* Assures mutually exclusive access to OSPI */
  bool memmap;                  /* TRUE: Controller is in memory mapped mode */

#define STM32L4_OSPI_INTERRUPTS
#ifdef STM32L4_OSPI_INTERRUPTS
  xcpt_t handler;               /* Interrupt handler */
  uint8_t irq;                  /* Interrupt number */
  sem_t op_sem;                 /* Block until complete */
  struct ospi_xctnspec_s *xctn; /* context of transaction in progress */
#endif


#ifdef CONFIG_STM32L4_OSPI_DMA
  bool candma;                  /* DMA is supported */
  sem_t dmawait;                /* Used to wait for DMA completion */
  int result;                   /* DMA result */
  DMA_HANDLE dmach;             /* OSPI DMA handle */
  WDOG_ID dmadog;               /* Watchdog that handles DMA timeouts */
#endif

  /* Debug stuff */

#ifdef CONFIG_STM32L4_OSPI_DMADEBUG
  struct stm32l4_dmaregs_s dmaregs[DMA_NSAMPLES];
#endif

#ifdef CONFIG_STM32L4_OSPI_REGDEBUG
   bool     wrlast;            /* Last was a write */
   uint32_t addresslast;       /* Last address */
   uint32_t valuelast;         /* Last value */
   int      ntimes;            /* Number of times */
#endif
};


static const struct ospi_ops_s g_ospi0ops =
{
  .lock              = ospi_lock,
  .setfrequency      = ospi_setfrequency,
  .setmode           = ospi_setmode,
  .setbits           = ospi_setbits,
  .command           = ospi_command,
  .memory            = ospi_memory,
  .alloc             = ospi_alloc,
  .free              = ospi_free,
};


static struct stm32l4_ospidev_s g_ospi0dev =
{
  .ospi            =
  {
    .ops             = &g_ospi0ops,
  },
  .base              = STM32L4_OCTOSPI1_BASE,
#ifdef STM32L4_OSPI_INTERRUPTS
  .handler           = ospi0_interrupt,
  .irq               = STM32L4_IRQ_OCTOSPI1,
#endif
  .intf              = 0,
#ifdef CONFIG_STM32L4_OSPI_DMA
  .candma            = true,
#endif
};

static int ospi_lock(struct ospi_dev_s *dev, bool lock)
{
  return 0;
}

static uint32_t ospi_setfrequency(struct ospi_dev_s *dev, uint32_t frequency)
{
  return 0;
}

static void ospi_setmode(struct ospi_dev_s *dev, enum ospi_mode_e mode)
{

}

static void ospi_setbits(struct ospi_dev_s *dev, int nbits)
{

}

static int ospi_command(struct ospi_dev_s *dev, struct ospi_cmdinfo_s *cmdinfo)
{
  return 0;
}
#ifdef BSP_OSPI_MEMORY_MAPPED_ENABLE
#define MEMORY_MAPPED_OFF   (0x00)
#define MEMORY_MAPPED_ON    (0x01)
static int ospi_memory(struct ospi_dev_s *dev, struct ospi_meminfo_s *meminfo)
{
  static uint8_t mode = MEMORY_MAPPED_OFF;

  OSPI_RegularCmdTypeDef sCommand;

  /* octo spi write mode*/
  if (OSPIMEM_ISWRITE(meminfo->flags))
  {
    uint8_t* pData     = (uint8_t *)meminfo->buffer;
    uint32_t WriteAddr = meminfo->addr;
    uint32_t Size      = meminfo->buflen;
    uint8_t *mem_addr;
    uint32_t end_addr, current_size, current_addr;

    /* Calculation of the size between the write address and the end of the page */
    current_size = MX25LM51245G_PAGE_SIZE - (WriteAddr % MX25LM51245G_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the adress variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    if(mode == MEMORY_MAPPED_ON)
    {
      if(stm32l4_ospi_exit_memorymapped((struct ospi_dev_s *)&g_ospi0dev, NULL, 0) != OSPI_NOR_OK)
      {
        return OSPI_NOR_ERROR;
      }
      mode = MEMORY_MAPPED_OFF;
    }

  /* Initialize the program command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_PAGE_PROG_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_8_LINES;
    sCommand.DummyCycles           = 0;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

   #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
   #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_ENABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_ENABLE;
   #endif

    /* Perform the write page by page */
    do
    {
      sCommand.Address = current_addr;
      sCommand.NbData  = current_size;

      /* Enable write operations */
      if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
      {
        printf("Flash OSPI_NOR_WriteEnable error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Configure the command */
      if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        printf("Flash HAL_OSPI_Command error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Transmission of the data */
      if (HAL_OSPI_Transmit(&OSPINORHandle, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        printf("Flash HAL_OSPI_Transmit error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Configure automatic polling mode to wait for end of program */
      if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != OSPI_NOR_OK)
      {
        printf("Flash OSPI_NOR_AutoPollingMemReady error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Update the address and size variables for next page programming */
      current_addr += current_size;
      pData += current_size;
      current_size = ((current_addr + MX25LM51245G_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : MX25LM51245G_PAGE_SIZE;
    } while (current_addr < end_addr);


    return OSPI_NOR_OK;
  }
  /* octo spi read mode*/
  else if(OSPIMEM_ISREAD(meminfo->flags))
  {
    uint8_t* pData    = (uint8_t *)meminfo->buffer;
    uint32_t ReadAddr = meminfo->addr;
    uint32_t Size     = meminfo->buflen;
    uint8_t *mem_addr;

    if(mode == MEMORY_MAPPED_OFF)
    {
      if(stm32l4_ospi_enter_memorymapped((struct ospi_dev_s *)&g_ospi0dev, NULL, 0) != OSPI_NOR_OK)
      {
        return OSPI_NOR_ERROR;
      }
      mode = MEMORY_MAPPED_ON;
    }

    mem_addr = (uint8_t *)(OCTOSPI1_BASE + ReadAddr);
    for (int index = 0; index < Size; index++)
    {
      *pData = *mem_addr;

      mem_addr++;
      pData++;
    }

    return OSPI_NOR_OK;
  }
  /* octo spi block erase mode*/
  else if(OSPIMEM_ISERASEBLCK(meminfo->flags))
  {
    uint32_t BlockAddress = meminfo->addr;

    if(mode == MEMORY_MAPPED_ON)
    {
      if(stm32l4_ospi_exit_memorymapped((struct ospi_dev_s *)&g_ospi0dev, NULL, 0) != OSPI_NOR_OK)
      {
        return OSPI_NOR_ERROR;
      }
      mode = MEMORY_MAPPED_OFF;
    }

    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_BLOCK_ERASE_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address               = BlockAddress;
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_NONE;
    sCommand.DummyCycles           = 0;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  #endif

    /* Enable write operations */
    if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Send the command */
    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for end of erase */
    if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, MX25LM51245G_BLOCK_ERASE_MAX_TIME) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    return OSPI_NOR_OK;
  }
  /* octo spi sector erase mode*/
  else
  {
    uint32_t Sector = meminfo->addr;

    if (Sector >= (uint32_t)(MX25LM51245G_FLASH_SIZE/MX25LM51245G_SECTOR_SIZE))
    {
      return OSPI_NOR_ERROR;
    }

    if(mode == MEMORY_MAPPED_ON)
    {
      if(stm32l4_ospi_exit_memorymapped((struct ospi_dev_s *)&g_ospi0dev, NULL, 0) != OSPI_NOR_OK)
      {
        return OSPI_NOR_ERROR;
      }
      mode = MEMORY_MAPPED_OFF;
    }

    /* Initialize the erase command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_SECTOR_ERASE_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address               = (Sector * MX25LM51245G_SECTOR_SIZE);
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_NONE;
    sCommand.DummyCycles           = 0;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  #endif

    /* Enable write operations */
    if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
    {
      printf("Flash OSPI_NOR_WriteEnable erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    /* Send the command */
    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      printf("Flash HAL_OSPI_Command erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for end of erase */
    if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, MX25LM51245G_SECTOR_ERASE_MAX_TIME) != OSPI_NOR_OK)
    {
      printf("Flash OSPI_NOR_AutoPollingMemReady erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    return OSPI_NOR_OK;
  }
}
#else
static int ospi_memory(struct ospi_dev_s *dev, struct ospi_meminfo_s *meminfo)
{
  OSPI_RegularCmdTypeDef sCommand;

  /* octo spi write mode*/
  if (OSPIMEM_ISWRITE(meminfo->flags))
  {
    uint8_t* pData     = (uint8_t *)meminfo->buffer;
    uint32_t WriteAddr = meminfo->addr;
    uint32_t Size      = meminfo->buflen;
    uint32_t end_addr, current_size, current_addr;

    /* Calculation of the size between the write address and the end of the page */
    current_size = MX25LM51245G_PAGE_SIZE - (WriteAddr % MX25LM51245G_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the adress variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;

    /* Initialize the program command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_PAGE_PROG_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_8_LINES;
    sCommand.DummyCycles           = 0;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_ENABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_ENABLE;
  #endif

    /* Perform the write page by page */
    do
    {
      sCommand.Address = current_addr;
      sCommand.NbData  = current_size;

      /* Enable write operations */
      if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
      {
        printf("Flash OSPI_NOR_WriteEnable error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Configure the command */
      if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        printf("Flash HAL_OSPI_Command error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Transmission of the data */
      if (HAL_OSPI_Transmit(&OSPINORHandle, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
        printf("Flash HAL_OSPI_Transmit error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Configure automatic polling mode to wait for end of program */
      if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != OSPI_NOR_OK)
      {
        printf("Flash OSPI_NOR_AutoPollingMemReady error write status : 0x%x\n", OSPINORHandle.State);
        return OSPI_NOR_ERROR;
      }

      /* Update the address and size variables for next page programming */
      current_addr += current_size;
      pData += current_size;
      current_size = ((current_addr + MX25LM51245G_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : MX25LM51245G_PAGE_SIZE;
    } while (current_addr < end_addr);

    return OSPI_NOR_OK;
  }
  /* octo spi read mode*/
  else if(OSPIMEM_ISREAD(meminfo->flags))
  {
    uint8_t* pData    = (uint8_t *)meminfo->buffer;
    uint32_t ReadAddr = meminfo->addr;
    uint32_t Size     = meminfo->buflen;

    /* Initialize the read command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address               = ReadAddr;
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_8_LINES;
    sCommand.NbData                = Size;
    sCommand.DummyCycles           = DUMMY_CYCLES_READ_OCTAL;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.Instruction           = OCTAL_IO_READ_CMD;
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  #else
    sCommand.Instruction           = OCTAL_IO_DTR_READ_CMD;
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
    sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_ENABLE;
    sCommand.DQSMode               = HAL_OSPI_DQS_ENABLE;
  #endif

    /* Configure the command */
    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      printf("Flash HAL_OSPI_Command read error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    /* Reception of the data */
    if (HAL_OSPI_Receive(&OSPINORHandle, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      printf("Flash HAL_OSPI_Receive read error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }
#ifndef BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    *(volatile uint32_t *)(0xA0001400) |= 0x02;
    //volatile uint32_t dRead = *(volatile uint32_t *)(0xA0001450);
#endif
    return OSPI_NOR_OK;
  }
  /* octo spi block erase mode*/
  else if(OSPIMEM_ISERASEBLCK(meminfo->flags))
  {
    uint32_t BlockAddress = meminfo->addr;

    /* Initialize the erase command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_BLOCK_ERASE_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address               = BlockAddress;
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_NONE;
    sCommand.DummyCycles           = 0;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  #endif

    /* Enable write operations */
    if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Send the command */
    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for end of erase */
    if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, MX25LM51245G_BLOCK_ERASE_MAX_TIME) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    return OSPI_NOR_OK;
  }
  /* octo spi sector erase mode*/
  else if(OSPIMEM_ISERASESECT(meminfo->flags))
  {
    uint32_t Sector = meminfo->addr;

    if (Sector >= (uint32_t)(MX25LM51245G_FLASH_SIZE/MX25LM51245G_SECTOR_SIZE))
    {
      return OSPI_NOR_ERROR;
    }

    /* Initialize the erase command */
    sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
    sCommand.Instruction           = OCTAL_SECTOR_ERASE_CMD;
    sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address               = (Sector * MX25LM51245G_SECTOR_SIZE);
    sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode              = HAL_OSPI_DATA_NONE;
    sCommand.DummyCycles           = 0;
    sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  #else
    sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  #endif

    /* Enable write operations */
    if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
    {
      printf("Flash OSPI_NOR_WriteEnable erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    /* Send the command */
    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      printf("Flash HAL_OSPI_Command erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for end of erase */
    if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, MX25LM51245G_SECTOR_ERASE_MAX_TIME) != OSPI_NOR_OK)
    {
      printf("Flash OSPI_NOR_AutoPollingMemReady erase error status : 0x%x\n", OSPINORHandle.State);
      return OSPI_NOR_ERROR;
    }

    return OSPI_NOR_OK;
  }
}
#endif
static FAR void *ospi_alloc(FAR struct ospi_dev_s *dev, size_t buflen)
{
  return NULL;
}

static void ospi_free(FAR struct ospi_dev_s *dev, FAR void *buffer)
{

}

static int ospi0_interrupt(int irq, void *context, FAR void *arg)
{
  HAL_OSPI_IRQHandler(&OSPINORHandle);

  return OK;
}

void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef *hospi)
{
  Error_Handler(55);
}

void HAL_OSPI_AbortCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_FifoThresholdCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_CmdCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_RxHalfCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

void HAL_OSPI_TxHalfCpltCallback(OSPI_HandleTypeDef *hospi)
{

}

/* OSPI status flag polling mode functions */
void HAL_OSPI_StatusMatchCallback(OSPI_HandleTypeDef *hospi)
{

}

/* OSPI memory-mapped mode functions */
void HAL_OSPI_TimeOutCallback(OSPI_HandleTypeDef *hospi)
{

}

/**
  * @brief  Initializes the OSPI MSP.
  * @retval None
  */
static void OSPI_NOR_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the OctoSPI memory interface clock */
  __HAL_RCC_OSPIM_CLK_ENABLE();
  __HAL_RCC_OSPI1_CLK_ENABLE();

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();

  /* Enable GPIO clocks */
  /* IOSV bit MUST be set to access GPIO port G[2:15] */
  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->CR2, PWR_CR2_IOSV);

  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* OctoSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_12;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_OCTOSPIM_P2;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* OctoSPI DQS GPIO pins configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* OctoSPI CLK, D0, D1 and D2 GPIO pins configuration  */
  GPIO_InitStruct.Pin       = (GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* OctoSPI D3, D4 and D5 GPIO pins configuration  */
  GPIO_InitStruct.Pin       = (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* OctoSPI D6 and D7 GPIO pins configuration  */
  GPIO_InitStruct.Pin       = (GPIO_PIN_9 | GPIO_PIN_10);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
  * @brief  De-Initializes the OSPI MSP.
  * @retval None
  */
static void OSPI_NOR_MspDeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* OctoSPI CLK, CS, D0-D7, DQS GPIO pins de-configuration  */
  HAL_GPIO_DeInit(GPIOG, (GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_15));
  HAL_GPIO_DeInit(GPIOH, (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10));
  HAL_GPIO_DeInit(GPIOI, (GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11));

  /* Set GPIOG pin 12 in pull up mode (optimum default setting) */
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pin       = GPIO_PIN_12;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* Set GPIOG pin 15 in no pull, low state (optimum default setting) */
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Pin       = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI1_FORCE_RESET();
  __HAL_RCC_OSPI1_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI1_CLK_DISABLE();
}

/**
  * @brief  This function reset the OSPI memory.
  * @param  hospi: OSPI handle
  * @retval None
  */
static uint8_t OSPI_NOR_ResetMemory(OSPI_HandleTypeDef *hospi)
{
  OSPI_RegularCmdTypeDef sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  /* Initialize the reset enable command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction           = RESET_ENABLE_CMD;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode              = HAL_OSPI_DATA_NONE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }

  /* Send the reset memory command */
  sCommand.Instruction = RESET_MEMORY_CMD;
  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  sCommand.Instruction  = READ_STATUS_REG_CMD;
  sCommand.DataMode     = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData       = 1;
  sCommand.DataDtrMode  = HAL_OSPI_DATA_DTR_DISABLE;

  sConfig.Match         = 0;
  sConfig.Mask          = MX25LM51245G_SR_WIP;
  sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
  sConfig.Interval      = 0x10;
  sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }

  if (HAL_OSPI_AutoPolling(&OSPINORHandle, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }

  return OSPI_NOR_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hospi: OSPI handle
  * @retval None
  */
static uint8_t OSPI_NOR_WriteEnable(OSPI_HandleTypeDef *hospi)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;
  uint8_t reg[2];

  /* Enable write operations */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction           = OCTAL_WRITE_ENABLE_CMD;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode              = HAL_OSPI_DATA_NONE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
#else
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
#endif

  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  sConfig.Match           = MX25LM51245G_SR_WEL;
  sConfig.Mask            = MX25LM51245G_SR_WEL;

  sCommand.Instruction    = OCTAL_READ_STATUS_REG_CMD;
  sCommand.Address        = 0x0;
  sCommand.AddressMode    = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize    = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.DataMode       = HAL_OSPI_DATA_8_LINES;
  sCommand.NbData         = 2;

#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
  sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.DataDtrMode    = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode        = HAL_OSPI_DQS_DISABLE;
  sCommand.DummyCycles    = DUMMY_CYCLES_READ_OCTAL;
#else
  sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.DataDtrMode    = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode        = HAL_OSPI_DQS_ENABLE;
  sCommand.DummyCycles    = 4;
#endif

  do
  {
    HAL_Delay(1); /* sConfig.Interval(0x10) / Clock(55 MHz) = 0.29 ms */

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Receive(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }
  }while((reg[0] & sConfig.Mask ) != sConfig.Match);

  return OSPI_NOR_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hospi: OSPI handle
  * @param  Timeout: Timeout for auto-polling
  * @retval None
  */
static uint8_t OSPI_NOR_AutoPollingMemReady(OSPI_HandleTypeDef *hospi, uint32_t Timeout)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;
  uint8_t reg[2];

  /* Configure automatic polling mode to wait for memory ready */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction           = OCTAL_READ_STATUS_REG_CMD;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
  sCommand.Address               = 0;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode              = HAL_OSPI_DATA_8_LINES;
  sCommand.NbData                = 2;
  sCommand.DummyCycles           = DUMMY_CYCLES_READ_OCTAL;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
#else
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode               = HAL_OSPI_DQS_ENABLE;
#endif

  sConfig.Match           = 0;
  sConfig.Mask            = MX25LM51245G_SR_WIP;

  do
  {
    HAL_Delay(1); /* sConfig.Interval(0x10) / Clock(55 MHz) = 0.29 ms */

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Receive(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }
  }while((reg[0] & sConfig.Mask ) != sConfig.Match);

  return OSPI_NOR_OK;
}

/**
  * @brief  This function enables/disables the octal mode of the memory.
  * @param  hospi: OSPI handle
  * @param  Operation: Octal operation mode enable/disable
  * @retval None
  */
static uint8_t OSPI_NOR_OctalMode(OSPI_HandleTypeDef *hospi, uint8_t Operation)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;
  uint8_t reg[2];

  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
  sConfig.Interval      = 0x10;
  sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

  /* Activate/deactivate the Octal mode */
  if (Operation == OSPI_NOR_OCTAL_ENABLE)
  {
    /* Enable write operations */
    sCommand.Instruction     = WRITE_ENABLE_CMD;
    sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.AddressMode     = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode        = HAL_OSPI_DATA_NONE;
    sCommand.DummyCycles     = 0;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling */
    sConfig.Match = MX25LM51245G_SR_WEL;
    sConfig.Mask  = MX25LM51245G_SR_WEL;

    sCommand.Instruction = READ_STATUS_REG_CMD;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData      = 1;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_AutoPolling(&OSPINORHandle, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Write Configuration register 2 (with new dummy cycles) */
    sCommand.Instruction = WRITE_CFG_REG_2_CMD;
    sCommand.Address     = MX25LM51245G_CR2_REG3_ADDR;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
    reg[0] = DUMMY_CYCLES_CR_CFG;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Transmit(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Enable write operations */
    sCommand.Instruction = WRITE_ENABLE_CMD;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode    = HAL_OSPI_DATA_NONE;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling */
    sCommand.Instruction = READ_STATUS_REG_CMD;
    sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_AutoPolling(&OSPINORHandle, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Write Configuration register 2 (with Octal I/O SPI protocol) */
    sCommand.Instruction = WRITE_CFG_REG_2_CMD;
    sCommand.Address     = MX25LM51245G_CR2_REG1_ADDR;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    reg[0] = MX25LM51245G_CR2_SOPI;
#else
    reg[0] = MX25LM51245G_CR2_DOPI;
#endif

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Transmit(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

    if (OSPI_NOR_AutoPollingMemReady(&OSPINORHandle, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Check the configuration has been correctly done */
    sCommand.Instruction     = OCTAL_READ_CFG_REG_2_CMD;
    sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.AddressMode     = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.DataMode        = HAL_OSPI_DATA_8_LINES;
    sCommand.DummyCycles     = DUMMY_CYCLES_READ_OCTAL;
    sCommand.NbData          = 2;
    reg[0] = 0;

#if !defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
    sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
#endif

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Receive(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    if (reg[0] != MX25LM51245G_CR2_SOPI)
#else
    if (reg[0] != MX25LM51245G_CR2_DOPI)
#endif
    {
      return OSPI_NOR_ERROR;
    }
  }
  else
  {
    /* Enable write operations */
    if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Write Configuration register 2 (with Octal I/O SPI protocol) */
    sCommand.Instruction     = OCTAL_WRITE_CFG_REG_2_CMD;
    sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
    sCommand.Address         = MX25LM51245G_CR2_REG1_ADDR;
    sCommand.AddressMode     = HAL_OSPI_ADDRESS_8_LINES;
    sCommand.DataMode        = HAL_OSPI_DATA_8_LINES;
    sCommand.NbData          = 2;
    sCommand.DummyCycles     = 0;
    reg[0] = 0;
    reg[1] = 0;

#if !defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
#endif

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Transmit(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25LM51245G_WRITE_REG_MAX_TIME);

    sCommand.Instruction     = READ_STATUS_REG_CMD;
    sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.AddressMode     = HAL_OSPI_ADDRESS_NONE;
    sCommand.DataMode        = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData          = 1;
    sCommand.DummyCycles     = 0;

#if !defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
#endif

    sConfig.Match = 0;
    sConfig.Mask  = MX25LM51245G_SR_WIP;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_AutoPolling(&OSPINORHandle, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    /* Check the configuration has been correctly done */
    sCommand.Instruction = READ_CFG_REG_2_CMD;
    sCommand.Address     = MX25LM51245G_CR2_REG1_ADDR;
    sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;

    if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (HAL_OSPI_Receive(&OSPINORHandle, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return OSPI_NOR_ERROR;
    }

    if (reg[0] != 0)
    {
      return OSPI_NOR_ERROR;
    }
  }

  return OSPI_NOR_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(int no)
{
  printf("OSPI Error handlder : %d\n", no);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32l4_ospi_initialize
 *
 * Description:
 *   Initialize the selected OSPI port in master mode
 *
 * Input Parameters:
 *   intf - Interface number(must be zero)
 *
 * Returned Value:
 *   Valid OSPI device structure reference on success; a NULL on failure
 *
 ****************************************************************************/
struct ospi_dev_s *stm32l4_ospi_initialize(int intf)
{
  struct stm32l4_ospidev_s *priv;

  if (intf == 0)
  {
    priv = &g_ospi0dev;

    if (!priv->initialized)
    {
      priv->initialized = true;
      priv->memmap = false;
  #ifdef STM32L4_OSPI_INTERRUPTS
      irq_attach(priv->irq, priv->handler, NULL);
  #endif
    }

    OSPIM_CfgTypeDef OSPIM_Cfg_Struct;

    OSPINORHandle.Instance = OCTOSPI1;

    /* Call the DeInit function to reset the driver */
    if (HAL_OSPI_DeInit(&OSPINORHandle) != HAL_OK)
    {
      Error_Handler(OSPI_NOR_ERROR);
      return NULL;
    }

    /* System level initialization */
    OSPI_NOR_MspInit();

    /* OctoSPI initialization */
    OSPINORHandle.Init.FifoThreshold         = 4;
    OSPINORHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
    OSPINORHandle.Init.DeviceSize            = POSITION_VAL(MX25LM51245G_FLASH_SIZE); /* 512 MBits */
    OSPINORHandle.Init.ChipSelectHighTime    = 2;
    OSPINORHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
    OSPINORHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
    OSPINORHandle.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
    OSPINORHandle.Init.ClockPrescaler        = 2; /* OctoSPI clock = 120MHz / ClockPrescaler = 60MHz */
    OSPINORHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
    #if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
    OSPINORHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
    OSPINORHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
    #else
    OSPINORHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
    OSPINORHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
    #endif
    OSPINORHandle.Init.ChipSelectBoundary    = 0;

    if (HAL_OSPI_Init(&OSPINORHandle) != HAL_OK)
    {
      Error_Handler(OSPI_NOR_ERROR);
      return NULL;
    }

    /* OctoSPI IO Manager configuration */
    OSPIM_Cfg_Struct.ClkPort    = 2;
    OSPIM_Cfg_Struct.DQSPort    = 2;
    OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_2_HIGH;
    OSPIM_Cfg_Struct.IOLowPort  = HAL_OSPIM_IOPORT_2_LOW;
    OSPIM_Cfg_Struct.NCSPort    = 2;

    if (HAL_OSPIM_Config(&OSPINORHandle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      Error_Handler(OSPI_NOR_ERROR);
      return NULL;
    }

    /* OctoSPI memory reset */
    if (OSPI_NOR_ResetMemory(&OSPINORHandle) != OSPI_NOR_OK)
    {
      Error_Handler(OSPI_NOR_NOT_SUPPORTED);
      return NULL;
    }

    /* Insert delay */
    HAL_Delay(1);

    /* OctoSPI octal enable */
    if (OSPI_NOR_OctalMode(&OSPINORHandle, OSPI_NOR_OCTAL_ENABLE) != OSPI_NOR_OK)
    {
      Error_Handler(OSPI_NOR_ERROR);
      return NULL;
    }

    return &priv->ospi;
  }
  else
  {
    printf("ERROR: OSPI%d not supported\n", intf);
    return NULL;
  }

  return NULL;
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

int stm32l4_ospi_enter_memorymapped(struct ospi_dev_s *dev,
                                     const struct ospi_meminfo_s *meminfo,
                                     uint32_t lpto)
{
  OSPI_RegularCmdTypeDef   sCommand;
  OSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Enable write operations */
  if (OSPI_NOR_WriteEnable(&OSPINORHandle) != OSPI_NOR_OK)
  {
    Error_Handler(OSPI_NOR_ERROR+1);
  }

  /* Configure the command for the read instruction */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_16_BITS;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize           = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode              = HAL_OSPI_DATA_8_LINES;
  sCommand.DummyCycles           = DUMMY_CYCLES_READ_OCTAL;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

#if defined BSP_OSPI_NOR_DDR_MODE_DEACTIVATED
  sCommand.Instruction           = OCTAL_IO_READ_CMD;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
#else
  sCommand.Instruction           = OCTAL_IO_DTR_READ_CMD;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DQSMode               = HAL_OSPI_DQS_ENABLE;
#endif

  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler(OSPI_NOR_ERROR+2);
  }

  /* Configure the command for the write instruction */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.Instruction           = OCTAL_PAGE_PROG_CMD;
  sCommand.DummyCycles           = 0;

  if (HAL_OSPI_Command(&OSPINORHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler(OSPI_NOR_ERROR+3);
  }

   /* Configure the memory mapped mode */
  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_OSPI_MemoryMapped(&OSPINORHandle, &sMemMappedCfg) != HAL_OK)
  {
    Error_Handler(OSPI_NOR_ERROR+4);
  }

  return OSPI_NOR_OK;
}

/****************************************************************************
 * Name: stm32l4_ospi_exit_memorymapped
 *
 * Description:
 *   Take the OSPI device out of memory mapped mode
 *
 * Input Parameters:
 *   dev - OSPI device
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

int stm32l4_ospi_exit_memorymapped(struct ospi_dev_s *dev)
{
  if(HAL_OSPI_Abort(&OSPINORHandle) != HAL_OK)
  {
    return OSPI_NOR_ERROR;
  }
  return OSPI_NOR_OK;
}


/****************************************************************************
 * Name: up_flashinitialize
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
static int this_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
  struct ospi_meminfo_s meminfo;
  int ret;

  for(int i=0; i<nblocks; i++)
  {
    meminfo.flags = OSPIMEM_ERASESECT;
    meminfo.addr = startblock + i;
    ret = ospi_memory((struct ospi_dev_s *)&g_ospi0dev, &meminfo);
  }
  //printf("this_erase startblock:%d, nblocks:%d\n", startblock, nblocks);

  if(ret == OSPI_NOR_OK)
    return nblocks;

  return 0;
}

static ssize_t this_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buffer)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_READ;
  meminfo.addr = startblock * MX25LM51245G_SECTOR_SIZE;
  meminfo.buffer = (uint8_t*)buffer;
  meminfo.buflen = nblocks * MX25LM51245G_SECTOR_SIZE;

  int ret = ospi_memory((struct ospi_dev_s *)&g_ospi0dev, &meminfo);

  //printf("this_bread ret:%d, startblock:%d, nblocks:%d\n", ret, startblock, nblocks);

  if(ret == OSPI_NOR_OK)
    return  nblocks;

  return 0;
}
static ssize_t this_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buffer)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_WRITE;
  meminfo.addr = startblock * MX25LM51245G_SECTOR_SIZE;
  meminfo.buffer = (uint8_t*)buffer;
  meminfo.buflen = nblocks * MX25LM51245G_SECTOR_SIZE;

  int ret = ospi_memory((struct ospi_dev_s *)&g_ospi0dev, &meminfo);

  //printf("this_bwrite ret:%d, startblock:%d, nblocks:%d\n", ret, startblock, nblocks);

  if(ret == OSPI_NOR_OK)
    return  nblocks;

  return 0;
}
static ssize_t this_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
  struct ospi_meminfo_s meminfo;

  meminfo.flags = OSPIMEM_READ;
  meminfo.addr = offset;
  meminfo.buffer = (uint8_t*)buffer;
  meminfo.buflen = nbytes;

  int ret = ospi_memory((struct ospi_dev_s *)&g_ospi0dev, &meminfo);

  //printf("this_read ret:%d, offset:0x%08x, nbytes:%d\n", ret, offset, nbytes);

  if(ret == OSPI_NOR_OK)
    return nbytes;

  return 0;
}
static int this_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
  struct ospi_meminfo_s meminfo;

  switch (cmd)
  {
  case MTDIOC_GEOMETRY:
  {
    FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t)arg);
    if(geo)
    {
      geo->blocksize = MX25LM51245G_SECTOR_SIZE;
      geo->erasesize = MX25LM51245G_SECTOR_SIZE;
      geo->neraseblocks = MX25LM51245G_FLASH_SIZE / MX25LM51245G_SECTOR_SIZE;
    }
    //printf("MTDIOC_GEOMETRY\n");
  }
    break;
  case MTDIOC_BULKERASE:
    //printf("MTDIOC_BULKERASE\n");
    break;

  case MTDIOC_XIPBASE:
    //printf("MTDIOC_XIPBASE\n");
    break;

  default:
    break;
  }

  return 0;
}
/****************************************************************************
 * Name: up_flashinitialize
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
FAR struct mtd_dev_s *up_flashinitialize(void)
{
  FAR struct mtd_dev_s *priv;
  priv = (FAR struct mtd_dev_s *)kmm_zalloc(sizeof(struct mtd_dev_s));

  if(priv)
  {
    priv->erase = this_erase;
    priv->bread = this_bread;
    priv->bwrite = this_bwrite;
    priv->read = this_read;
    priv->ioctl = this_ioctl;
  }

  return priv;
}














