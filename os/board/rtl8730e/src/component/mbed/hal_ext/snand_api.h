/** mbed Microcontroller Library
  ******************************************************************************
  * @file    snand_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides mbed SNAND API
  ******************************************************************************
  * @attention
  *
  * Copyright(c) 2006 - 2022 Realtek Corporation. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef MBED_EXT_SNAND_API_EXT_H
#define MBED_EXT_SNAND_API_EXT_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup flash SPI NAND FLASH
 *  @ingroup    hal
 *  @brief      flash functions
 *  @{
 */

///@name Ameba Common
///@{

/* ======================================== */
/* For S-NAND Bad-Block Management (BBM)    */
/* ======================================== */
/**************************************************************************
 *                           C O N S T A N T S
 **************************************************************************/
#define BBM_FACTORY_BAD_TAG (0xFFFE)
#define BBM_USD_BAD_TAG (0xFFFC)
#define SNAND_BBM_UNKWNON_TAG (0xFFF8)

/**
 NAND flash pattern related definition
 */
#define DEF_SNAND_64MB_BLK_CNT (512)
#define DEF_SNAND_128MB_BLK_CNT (1024)
#define DEF_SNAND_256MB_BLK_CNT (2048)

#define DEF_SNAND_PAGE512_SIZE (512)
#define DEF_SNAND_PAGE2K_SIZE (2048)
#define DEF_SNAND_PAGE4K_SIZE (4096)

#define DEF_SNAND_SPARE16_SIZE (16)
#define DEF_SNAND_SPARE64_SIZE (64)
#define DEF_SNAND_SPARE128_SIZE (128)

#define DEF_SNAND_PAGE_PER_BLOCK (64)
#define DEF_TOTAL_BLK_CNT (DEF_SNAND_128MB_BLK_CNT)
#define DEF_SPARE_SIZE (DEF_SNAND_SPARE64_SIZE)
#define DEF_PAGE_SIZE (DEF_SNAND_PAGE2K_SIZE)

#define DEF_RESERVED_BLOCK_AREA_PERCENTAGE (5)

#define DEF_PAGE_BUF_SIZE (DEF_PAGE_SIZE+DEF_SPARE_SIZE)

#if 0
#define BBMT_BACKUP_COPY    (1)
#define BBMT_BACKUP_BLOCK_IDX   (0x8)
#else
#define BBMT_BACKUP_COPY    (2)
#define BBMT_BACKUP_BLOCK_IDX   (0x8) /* 0x08, and 0x09 */
#endif

#if 1
#define V2RT_BACKUP_COPY    (1)
#define V2RT_BACKUP_BLOCK_IDX   (0x0a)
#else
#define V2RT_BACKUP_COPY    (2)
#define V2RT_BACKUP_BLOCK_IDX   (0x0a) /* 0x0a, and 0x0b */
#endif

/**
 SPI NAND flash device support
 */
/* ---------------------------------------- */
/* Winbond */
#define WINBOND_MANUFACTURER_ID         (0xEF)
/* Winbond W25N512GV  */
#define WINBOND_DEVICE_ID_W25N512GV     (0xAA20)
#define WINBOND_RDID_W25N512GV          (0xEFAA20)
/* Winbond W25N01GV  */
#define WINBOND_DEVICE_ID_W25N01GV      (0xAA21)
#define WINBOND_RDID_W25N01GV           (0xEFAA21)
/* Winbond W25M02GV */
#define WINBOND_DEVICE_ID_W25M02GV      (0xAB21)
#define WINBOND_RDID_W25M02GV           (0xEFAB21)

/* ---------------------------------------- */
/* MXIC */
#define MXIC_MANUFACTURER_ID            (0xC2)
/* MXIC MX35LF1GE4AB */
#define MXIC_DEVICE_ID_MX35LF1GE4AB     (0x12)
#define MXIC_RDID_MX35LF1GE4AB          (0xC212)
/* MXIC MX35LF2GE4AB */
#define MXIC_DEVICE_ID_MX35LF2GE4AB     (0x22)
#define MXIC_RDID_MX35LF2GE4AB          (0xC222)

/* ---------------------------------------- */
/* GigaDevice (GD) */
#define GD_MANUFACTURER_ID              (0xC8)
/* GD5F1GQ4U */
#define GD_DEVICE_ID_GD5F1GQ4U          (0xD1)
#define GD_RDID_GD5F1GQ4U               (0xC8D1)
/* GD5F1GQ4A */
#define GD_DEVICE_ID_GD5F1GQ4A          (0xF1)
#define GD_RDID_GD5F1GQ4A               (0xC8F1)
/* GD5F1GQ4xFxxH */
#define GD_DEVICE_ID_GD5F1GQ4xFxxH      (0xB948)
#define GD_RDID_GD5F1GQ4xFxxH           (0xC8B948)
/* GD5F1GQ5xExxH */
#define GD_DEVICE_ID_GD5F1GQ5xExxH      (0x31)
#define GD_RDID_GD5F1GQ5xExxH           (0xC831)

/* ---------------------------------------- */
/* Kiaxio (Toshiba) */
#define TOSHIBA_MANUFACTURER_ID         (0x98)
#define KIAXIO_MANUFACTURER_ID          (0x98)

/* TC58CVG0S3HxAIx */
#define TOSHIBA_DEVICE_ID_TC58CVG0S3HxAIx   (0xC2)
#define TOSHIBA_RDID_TC58CVG0S3HxAIx        (0x98C2)
/* TC58CVG0S3HRAIJ */
#define TOSHIBA_DEVICE_ID_TC58CVG0S3HRAIJ   (0xE240)
#define TOSHIBA_RDID_TC58CVG0S3HRAIJ        (0x98E240)
/* TC58CVG1S3HRAIJ */
#define TOSHIBA_DEVICE_ID_TC58CVG1S3HRAIJ   (0xEB40)
#define TOSHIBA_RDID_TC58CVG1S3HRAIJ        (0x98EB40)

/* ---------------------------------------- */
/* Micron */
#define MICRON_MANUFACTURER_ID              (0x2C)

/* MT29F1G01AAADD */
#define MICRON_DEVICE_ID_MT29F1G01AAADD     (0x12)
#define MICRON_RDID_MT29F1G01AAADD          (0x2C12)
/* MT29F1G01ABAFDxx */
#define MICRON_DEVICE_ID_MT29F1G01ABAFD     (0x14)
#define MICRON_RDID_MT29F1G01ABAFD          (0x2C14)

/* ---------------------------------------- */
/* Biwin */
#define BIWIN_MANUFACTURER_ID               (0xBC)
/* BWJX08M1G */
#define BIWIN_DEVICE_ID_BWJX08M1G           (0xC1)
#define BIWIN_RDID_BWJX08M1G                (0xBCC1)

/* ---------------------------------------- */
/* XTX */
#define XTX_MANUFACTURER_ID                 (0x0B)
/* XT26G01C */
#define BIWIN_DEVICE_ID_XT26G01C            (0x11)
#define BIWIN_RDID_XT26G01C                 (0x0B11)

#ifdef __GNUC__
typedef long long      loff_t;
typedef uint8_t        u_char;
typedef uint16_t       u16;
#endif

struct erase_info {
	unsigned long start;
	unsigned long length;
};

struct mtd_info {
	uint8_t type;
	uint32_t flags;
	uint64_t size;   // Total size of the MTD

	/* "Major" erase size for the device. Naive users may take this
	 * to be the only erase size available, or may use the more detailed
	 * information below if they desire
	 */
	uint32_t erasesize;
	/* Minimal writable flash unit size. In case of NOR flash it is 1 (even
	 * though individual bits can be cleared), in case of NAND flash it is
	 * one NAND page (or half, or one-fourths of it), in case of ECC-ed NOR
	 * it is of ECC block size, etc. It is illegal to have writesize = 0.
	 * Any driver registering a struct mtd_info must ensure a writesize of
	 * 1 or larger.
	 */
	uint32_t writesize;

	/*
	 * Size of the write buffer used by the MTD. MTD devices having a write
	 * buffer can write multiple writesize chunks at a time. E.g. while
	 * writing 4 * writesize bytes to a device with 2 * writesize bytes
	 * buffer the MTD driver can (but doesn't have to) do 2 writesize
	 * operations, but not 4. Currently, all NANDs have writebufsize
	 * equivalent to writesize (NAND page size). Some NOR flashes do have
	 * writebufsize greater than writesize.
	 */
	uint32_t writebufsize;

	uint32_t oobsize;   // Amount of OOB data per block (e.g. 16)
	uint32_t oobavail;  // Available OOB bytes per block

	/*
	 * If erasesize is a power of 2 then the shift is stored in
	 * erasesize_shift otherwise erasesize_shift is zero. Ditto writesize.
	 */
	unsigned int erasesize_shift;
	unsigned int writesize_shift;
	/* Masks based on erasesize_shift and writesize_shift */
	unsigned int erasesize_mask;
	unsigned int writesize_mask;

	/*
	 * read ops return -EUCLEAN if max number of bitflips corrected on any
	 * one region comprising an ecc step equals or exceeds this value.
	 * Settable by driver, else defaults to ecc_strength.  User can override
	 * in sysfs.  N.B. The meaning of the -EUCLEAN return code has changed;
	 * see Documentation/ABI/testing/sysfs-class-mtd for more detail.
	 */
	unsigned int bitflip_threshold;

	// Kernel-only stuff starts here.
	const char *name;
	int index;

	/* max number of correctible bit errors per ecc step */
	unsigned int ecc_strength;

	/* Data for variable erase regions. If numeraseregions is zero,
	 * it means that the whole device has erasesize as given above.
	 */
	int numeraseregions;

	/*
	 * Do not call via these pointers, use corresponding mtd_*()
	 * wrappers instead.
	 */
	int (*_erase)(struct mtd_info *mtd, struct erase_info *instr);

	int (*_read)(struct mtd_info *mtd, loff_t from, size_t len,
				 size_t *retlen, u_char *buf);
	int (*_write)(struct mtd_info *mtd, loff_t to, size_t len,
				  size_t *retlen, const u_char *buf);

	void *priv;

//	struct module *owner;
//	struct device dev;
	int usecount;
};

typedef struct  __attribute__((__packed__))
{
	u16 BB_die;             //bad block chip
	u16 bad_block;          //bad block,from v2r bb
	u16 RB_die;             //remap block chip
	u16 remap_block;        //remap block, in RBA area
}
BB_t;


typedef struct  __attribute__((__packed__))
{
#if 0
	u32 block_r;
#else
	u16 block_r; /* For 128KB-per-block, reserve blk#0 to blk#0xFDFF can support 8,160 MB (= 7.96875 GB) */
#endif
}
BB_v2r;

typedef enum blockType_e {
	SNAND_BBM_GOOD_BLK = 0,
	SNAND_BBM_BAD_BLK = 1,
	SNAND_BBM_RSVD_BLK = 2,
	SNAND_BBM_UNKNOWN_BLK
} blockType_t;

typedef struct rtkNandInfo_s {
	uint8_t magicStr[4];    // magic string
	uint32_t totalsize; // Total size of the MTD
	/* "Major" erase size for the device. Naive users may take this
	 * to be the only erase size available, or may use the more detailed
	 * information below if they desire
	 */
	uint32_t erasesize;
	/* Minimal writable flash unit size. In case of NOR flash it is 1 (even
	 * though individual bits can be cleared), in case of NAND flash it is
	 * one NAND page (or half, or one-fourths of it), in case of ECC-ed NOR
	 * it is of ECC block size, etc. It is illegal to have writesize = 0.
	 * Any driver registering a struct mtd_info must ensure a writesize of
	 * 1 or larger.
	 */
	uint32_t writesize;
	/*
	 * Size of the write buffer used by the MTD. MTD devices having a write
	 * buffer can write multiple writesize chunks at a time. E.g. while
	 * writing 4 * writesize bytes to a device with 2 * writesize bytes
	 * buffer the MTD driver can (but doesn't have to) do 2 writesize
	 * operations, but not 4. Currently, all NANDs have writebufsize
	 * equivalent to writesize (NAND page size). Some NOR flashes do have
	 * writebufsize greater than writesize.
	 */
	uint32_t writebufsize;
	uint8_t *pBuf; /* = malloc(writebufsize) */

	/* (BBM) start block index to Reserved Block Area  */
	uint32_t RBA_PERCENT; /* (BBM) Percentage of total block for Reserved Block Area */
	uint32_t RBA_CNT; /* (BBM) Block cout of Reserved Block Area */
	uint32_t RBA_OFFSET; /* (BBM) start block index to Reserved Block Area */
	BB_t *pBBM;
	uint32_t nBBMBufSz;
	BB_v2r *pV2R;
	uint32_t nV2RBufSz;
} rtkNandInfo_t;

/************************************************rtknflash*******************************************/
/* nand chip param */
typedef struct nand_chip_param {
	unsigned int id;
#if 0 /* Deprecated in AmebaPro2 */
	unsigned char isLastPage;
#endif /* Deprecated in AmebaPro2 */
	unsigned char id_len;
	/* blocksize/pagesize/oobsize */
	unsigned int pagesize, oobsize;
	unsigned int num_chunk_per_block, num_block;
	unsigned char dienum;

	/* clk /pipe_lat*/
	unsigned int    maxclk;
	unsigned char   pipe_lat;
	/*io */
	bool            support_dio;
	bool            support_qio;
	bool            support_read_cache_2;
	bool            support_read_cache_4;
	bool            support_programme_cache_4;
	/* die on ecc */
	bool            enable_dieon_ecc;
#if 0 /* Deprecated in AmebaPro2 */
	bool            romcode_write_dieon_ecc;
	bool            romcode_read_dieon_ecc;
	int (*get_dieon_ecc_status)(void);
	void (*get_dieon_ecc_info)(void);
#endif /* Deprecated in AmebaPro2 */
} nand_chip_param_T, *nand_chip_param_Tp;


/**
 snand_bbm_init
 @brief Inital S-NAND BBM relate configuration before calling other snand_bbm APIs.
 @param[in] rtkNandInfo RTK NAND info, S-NAND Flash dependent configuration for SNAND BBM functions.
 @return SUCCESS
 @return FAIL
 */
uint32_t snand_bbm_init(rtkNandInfo_t *rtkNandInfo);

/**
 snand_bbm_dump
 @brief Dump V2R_T and BBM_T data structure.
 @param[in] rtkNandInfo RTK NAND info
 @param[in] opt option; 1 for V2R_T only; 2 for BBM_T only; 3 for both V2R_T and BBM_T.
 @return SUCCESS
 */
uint32_t snand_bbm_dump(rtkNandInfo_t *rtkNandInfo, uint32_t opt);

/**
 snand_bbm_create
 @brief Create V2R_T data structure for mapping VIRTUAL ADDRESS to PHYSICAL ADDRESS; and create BBM_T data structure for mapping BAD block to (pre-reserved) GOOD block.
 @param[in] rtkNandInfo RTK NAND info
 @return SUCCESS
 @return FAIL
*/
uint32_t snand_bbm_create(rtkNandInfo_t *rtkNandInfo);

/**
 snand_bbm_load
 @brief Load S-NAND BBM data struct (BBM_T and V2R_T) from flash to normal memory
 @param[in] rtkNandInfo RTK NAND info
 @return SUCCESS
 @return FAIL
*/
uint32_t snand_bbm_load(rtkNandInfo_t *rtkNandInfo);
/**
 snand_bbm_load
 @brief Load S-NAND BBM data struct (BBM_T and V2R_T) from flash memory device to normal memory
 @param[in] rtkNandInfo RTK NAND info
 @return SUCCESS
 @return FAIL
*/
uint32_t
snand_bbm_load(rtkNandInfo_t *rtkNandInfo);

/**
 snand_bbm_store
 @brief Store S-NAND BBM data struct (BBM_T and V2R_T) from normal memory to flash memory device.
 @param[in] rtkNandInfo RTK NAND info
 @return SUCCESS
 @return FAIL
*/
uint32_t snand_bbm_store(rtkNandInfo_t *rtkNandInfo);

/**
 snand_bbm_isBadBlock
 @brief Check specific flash block in BBM data structure is BAD or GOOD.
 @param[in] rtkNandInfo RTK NAND info
 @param[in] blkPageIdx S-NAND's blkPageIdx (page addr, not include column addr)
 @retval 0: SNAND_BBM_GOOD_BLK
 @retval 1: SNAND_BBM_BAD_BLK
 @retval 2: SNAND_BBM_RSVD_BLK
 @retval 3: SNAND_BBM_UNKNOWN_BLK
*/
uint32_t snand_bbm_isBadBlock(rtkNandInfo_t *rtkNandInfo, uint32_t blkPageIdx);

/**
 snand_bbm_markBad
 @brief Mark specific flash block in BBM data structure as BAD block.
 @param[in] rtkNandInfo RTK NAND info
 @param[in] blkPageIdx S-NAND's blkPageIdx (page addr, not include column addr)
 @retval 2: SNAND_BBM_BAD_BLK (already BAD before mark)
 @retval 3: SNAND_BBM_UNKNOWN_BLK
 @return SUCCESS
 @return FAIL
*/
uint32_t snand_bbm_markBad(rtkNandInfo_t *rtkNandInfo, uint32_t blkPageIdx);

/**
 snand_bbm_translate
 @brief Translate specific flash blkPageAddr from VIRTUAL ADDRESS to PHYSICAL ADDRESS according to V2R_T and BBM_T data structure.
 @param[in] rtkNandInfo RTK NAND info
 @param[in] blkPageIdx S-NAND's blkPageIdx (page addr, not include column addr)
 @retval 2: SNAND_BBM_BAD_BLK (already BAD before mark)
 @retval 3: SNAND_BBM_UNKNOWN_BLK
 @return the blkPageAddr after translate
*/
uint32_t snand_bbm_translate(rtkNandInfo_t *rtkNandInfo, uint32_t blkPageIdx);

/* ======================================== */
/* For access S-NAND flash device           */
/* ======================================== */
typedef struct snand_s snand_t;

/**
  * @brief  Init Flash Pinmux
  * @param  obj: address of the flash object
  * @param  snafcPinmuxSel_t: the pinmux selection of the flash
  * @retval   none
  */
void snand_pinmux_init(snand_t *obj, snafcPinmuxSel_t pin_sel);

/**
  * @brief  Init Flash
  * @param  obj: address of the flash object
  * @retval   none
  */
void snand_init(snand_t *obj);

/**
  * @brief  Get flash ID (command: 0x9F), works in SPI mode only.
  * @param  obj: Flash object define in application software.
  * @param  buf: Pointer to a byte array to save the readback ID.
  * @param  len: Specifies the length of the buf. It should be 3.
  * @retval -1: Fail, len: Succeed.
  */
int snand_read_id(snand_t *obj, uint8_t *buf, uint8_t len);

/**
  * @brief  Erase flash block, usually 1 block = 64K bytes
    Please refer to flash data sheet to confirm the actual block size.
    The actual address which being erased always aligned with block size.
  * @param  address: Specifies the starting address to be erased.
  * @retval SUCCESS, FAIL
  */
int snand_erase_block(snand_t *obj, uint32_t address);

/**
  * @brief  Read a stream of data from specified address vai user mode
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval SUCCESS, FAIL
  */
int  snand_page_read(snand_t *obj, uint32_t address, uint32_t Length, uint8_t *data);

/**
* @brief  Write a stream of data to specified address
* @param  obj: Specifies the parameter of flash object.
* @param  address: Specifies the address to be programmed.
* @param  Length: Specifies the length of the data to write.
* @param  data: Specified the pointer of the data to be written.
          If the address is in the flash, full address is required, i.e. SPI_SNAND_BASE + Offset
* @retval SUCCESS, FAIL

*/
int snand_page_write(snand_t *obj, uint32_t address, uint32_t Length, uint8_t *data);

/**
* @brief  Read Status register to check flash status
* @param  obj: Specifies the parameter of flash object.
* @param  offset: Specifies which register offset users like to access
* @retval status: the value of status register.
*/
int snand_get_feature(snand_t *obj, uint32_t offset);

/**
* @brief  Set Status register to enable desired operation
* @param  obj: Specifies the parameter of flash object.
* @param  offset: Specifies which register offset users like to set
* @param  data: Specifies which bit users like to set
   ex: if users want to set the third bit, data = 0x8.
* @retval SUCCESS, FAIL
*/
int snand_set_feature(snand_t *obj, uint32_t offset, uint32_t data);

/**
* @brief  Get flash density
* @param  obj: Specifies the parameter of flash object.
* @retval flash size (unit: Byte)
*/
uint32_t snand_get_size(snand_t *obj);

/*Set blockProtect for specified flash device */
void snand_set_lock_mode(uint32_t mode);

/*Lock whole flash chip*/
void snand_global_lock(void);

/*Unlock whole flash chip*/
void snand_global_unlock(void);

///@}

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif
