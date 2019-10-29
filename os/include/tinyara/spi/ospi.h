/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 * ospi.h
 *
 *  Created on: Sep 25, 2019
 *      Author: calebkang
 */

#ifndef __OS_INCLUDE_TINYARA_SPI_OSPI_H
#define __OS_INCLUDE_TINYARA_SPI_OSPI_H


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Access macros ************************************************************/

/****************************************************************************
 * Name: OSPI_LOCK
 *
 * Description:
 *   On OSPI busses where there are multiple devices, it will be necessary to
 *   lock OSPI to have exclusive access to the busses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the OSPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the OSPI is properly
 *   configured for the device.  If the OSPI buss is being shared, then it
 *   may have been left in an incompatible state.
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   lock - true: Lock ospi bus, false: unlock OSPI bus
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#define OSPI_LOCK(d, l) (d)->ops->lock(d, l)

/****************************************************************************
 * Name: OSPI_SETFREQUENCY
 *
 * Description:
 *   Set the OSPI frequency. Required.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   frequency - The OSPI frequency requested
 *
 * Returned Value:
 *   Returns the actual frequency selected
 *
 ****************************************************************************/

#define OSPI_SETFREQUENCY(d, f) ((d)->ops->setfrequency(d, f))

/****************************************************************************
 * Name: OSPI_SETMODE
 *
 * Description:
 *   Set the OSPI mode. Optional.  See enum ospi_mode_e for mode definitions.
 *
 * Input Parameters:
 *   dev -  Device-specific state data
 *   mode - The OSPI mode requested
 *
 * Returned Value:
 *   none
 *
 ****************************************************************************/

#define OSPI_SETMODE(d, m) (d)->ops->setmode(d, m)

/****************************************************************************
 * Name: OSPI_SETBITS
 *
 * Description:
 *   Set the number if bits per word.
 *
 * Input Parameters:
 *   dev -  Device-specific state data
 *   nbits - The number of bits requests.
 *           If value is greater > 0 then it implies MSB first
 *           If value is below < 0, then it implies LSB first with -nbits
 *
 * Returned Value:
 *   none
 *
 ****************************************************************************/

#define OSPI_SETBITS(d, b) (d)->ops->setbits(d, b)

/****************************************************************************
 * Name: OSPI_COMMAND
 *
 * Description:
 *   Perform one OSPI command transfer
 *
 * Input Parameters:
 *   dev     - Device-specific state data
 *   cmdinfo - Describes the command transfer to be performed.
 *
 * Returned Value:
 *   Zero (OK) on SUCCESS, a negated errno on value of failure
 *
 ****************************************************************************/

#define OSPI_COMMAND(d, c) (d)->ops->command(d, c)

/* OSPI Command Transfer Flags */

#define OSPICMD_ADDRESS       (1 << 0)  /* Bit 0: Enable address transfer */
#define OSPICMD_READDATA      (1 << 1)  /* Bit 1: Enable read data transfer */
#define OSPICMD_WRITEDATA     (1 << 2)  /* Bit 2: Enable write data transfer */

#define OSPICMD_ISADDRESS(f)  (((f) & OSPICMD_ADDRESS) != 0)
#define OSPICMD_ISDATA(f)     (((f) & (OSPICMD_READDATA | OSPICMD_WRITEDATA)) != 0)
#define OSPICMD_ISREAD(f)     (((f) & OSPICMD_READDATA) != 0)
#define OSPICMD_ISWRITE(f)    (((f) & OSPICMD_WRITEDATA) != 0)

/****************************************************************************
 * Name: OSPI_MEMORY
 *
 * Description:
 *   Perform one OSPI memory transfer
 *
 * Input Parameters:
 *   dev     - Device-specific state data
 *   meminfo - Describes the memory transfer to be performed.
 *
 * Returned Value:
 *   Zero (OK) on SUCCESS, a negated errno on value of failure
 *
 ****************************************************************************/

#define OSPI_MEMORY(d, m) (d)->ops->memory(d, m)

/* OSPI Memory Transfer Flags */

#define OSPIMEM_READ          (1 << 0)  /* Bit 0: 0=Memory read data transfer */
#define OSPIMEM_WRITE         (1 << 1)  /* Bit 1: 1=Memory write data transfer */
#define OSPIMEM_DUALIO        (1 << 2)  /* Bit 2: Use Dual I/O (READ only) */
#define OSPIMEM_QUADIO        (1 << 3)  /* Bit 3: Use Quad I/O (READ only) */
#define OSPIMEM_SCRAMBLE      (1 << 4)  /* Bit 4: Scramble data */
#define OSPIMEM_RANDOM        (1 << 5)  /* Bit 5: Use random key in scrambler */
#define OSPIMEM_ERASEBLCK     (1 << 6)
#define OSPIMEM_ERASESECT     (1 << 7)

#define OSPIMEM_ISREAD(f)     (((f) & OSPIMEM_READ) != 0)
#define OSPIMEM_ISWRITE(f)    (((f) & OSPIMEM_WRITE) != 0)
#define OSPIMEM_ISDUALIO(f)   (((f) & OSPIMEM_DUALIO) != 0)
#define OSPIMEM_ISQUADIO(f)   (((f) & OSPIMEM_QUADIO) != 0)
#define OSPIMEM_ISSCRAMBLE(f) (((f) & OSPIMEM_SCRAMBLE) != 0)
#define OSPIMEM_ISERASEBLCK(f) (((f) & OSPIMEM_ERASEBLCK) != 0)
#define OSPIMEM_ISERASESECT(f) (((f) & OSPIMEM_ERASESECT) != 0)

#define OSPIMEM_ISRANDOM(f) \
	(((f) & (OSPIMEM_SCRAMBLE|OSPIMEM_RANDOM)) == \
			(OSPIMEM_SCRAMBLE|OSPIMEM_RANDOM))

/****************************************************************************
 * Name: OSPI_ALLOC
 *
 * Description:
 *   Allocate a buffer suitable for DMA data transfer
 *
 * Input Parameters:
 *   dev    - Device-specific state data
 *   buflen - Buffer length to allocate in bytes
 *
 * Returned Value:
 *   Address of tha allocated memory on success; NULL is returned on any
 *   failure.
 *
 ****************************************************************************/

#define OSPI_ALLOC(d, b) (d)->ops->alloc(d, b)

/****************************************************************************
 * Name: OSPI_FREE
 *
 * Description:
 *   Free memory returned by OSPI_ALLOC
 *
 * Input Parameters:
 *   dev    - Device-specific state data
 *   buffer - Buffer previously allocated via OSPI_ALLOC
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

#define OSPI_FREE(d, b) (d)->ops->free(d, b)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Certain OSPI devices may required different clocking modes */

enum ospi_mode_e {
	OSPIDEV_MODE0 = 0,     /* CPOL=0 CHPHA=0 */
	OSPIDEV_MODE1,         /* CPOL=0 CHPHA=1 */
	OSPIDEV_MODE2,         /* CPOL=1 CHPHA=0 */
	OSPIDEV_MODE3          /* CPOL=1 CHPHA=1 */
};

/* This structure describes one command transfer */

struct ospi_cmdinfo_s {
	uint8_t   flags;       /* See OSPICMD_* definitions */
	uint8_t   addrlen;     /* Address length in bytes (if OSPICMD_ADDRESS) */
	uint16_t  cmd;         /* Command */
	uint16_t  buflen;      /* Data buffer length in bytes (if OSPICMD_DATA) */
	uint32_t  addr;        /* Address (if OSPICMD_ADDRESS) */
	FAR void *buffer;      /* Data buffer (if OSPICMD_DATA) */
};

/* This structure describes one memory transfer */

struct ospi_meminfo_s {
	uint8_t   flags;       /* See QSPMEM_* definitions */
	uint8_t   addrlen;     /* Address length in bytes */
	uint8_t   dummies;     /* Number of dummy read cycles (READ only) */
	uint16_t  buflen;      /* Data buffer length in bytes */
	uint16_t  cmd;         /* Memory access command */
	uint32_t  addr;        /* Memory Address */
	uint32_t  key;         /* Scrambler key */
	FAR void *buffer;      /* Data buffer */
};

/* The OSPI vtable */

struct ospi_dev_s;
struct ospi_ops_s {
	CODE int       (*lock)(FAR struct ospi_dev_s *dev, bool lock);
	CODE uint32_t  (*setfrequency)(FAR struct ospi_dev_s *dev, uint32_t frequency);
	CODE void      (*setmode)(FAR struct ospi_dev_s *dev, enum ospi_mode_e mode);
	CODE void      (*setbits)(FAR struct ospi_dev_s *dev, int nbits);
	CODE int       (*command)(FAR struct ospi_dev_s *dev, FAR struct ospi_cmdinfo_s *cmdinfo);
	CODE int       (*memory)(FAR struct ospi_dev_s *dev, FAR struct ospi_meminfo_s *meminfo);
	CODE FAR void *(*alloc)(FAR struct ospi_dev_s *dev, size_t buflen);
	CODE void      (*free)(FAR struct ospi_dev_s *dev, FAR void *buffer);
};

/* OSPI private data.  This structure only defines the initial fields of the
 * structure visible to the OSPI client.  The specific implementation may
 * add additional, device specific fields
 */

struct ospi_dev_s {
	FAR const struct ospi_ops_s *ops;
};


/****************************************************************************
 * Public Functions
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

FAR struct ospi_dev_s *up_ospiinitialize(int port);

FAR int ospi_uioregister(FAR int bus, FAR struct ospi_dev_s *dev);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __OS_INCLUDE_TINYARA_SPI_OSPI_H */
