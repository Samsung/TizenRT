/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <errno.h>
#include <debug.h>
#include <sys/types.h>
#include <tinyara/os_api_test_drv.h>


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Slot address definitions - moved from board-specific headers to driver side */
#if defined(CONFIG_ARCH_CHIP_BK7239N)
#define SLOT0_START_ADDR 0x2B000
#define SLOT1_START_ADDR 0x2E000
#define SLOT_AREA_OFFSET 0x2A000
#elif defined(CONFIG_AMEBAD_TRUSTZONE)
#define SLOT0_START_ADDR 0x1a000
#define SLOT1_START_ADDR 0x1d000
#define SLOT_AREA_OFFSET 0x23000
#elif defined(CONFIG_AMEBALITE_TRUSTZONE)
#define SLOT0_START_ADDR 0x24000
#define SLOT1_START_ADDR 0x27000
#define SLOT_AREA_OFFSET 0x23000
#elif defined(CONFIG_AMEBASMART_TRUSTZONE)
#define SLOT0_START_ADDR 0x2C000
#define SLOT1_START_ADDR 0x2F000
#define SLOT_AREA_OFFSET 0x2A000
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int driver_ns_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
static int driver_ns_flash_write(uint32_t address, uint32_t len, uint8_t *data, int en_display);
static void driver_flash_erase(uint32_t address);
static int driver_verify_flash_protect(void);
static int check_flash_data_empty(uint8_t *data, uint32_t length);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: check_flash_data_empty
 *
 * Description:
 *   Check if flash data is empty (all 0xFF)
 ****************************************************************************/
static int check_flash_data_empty(uint8_t *data, uint32_t length)
{
	for (uint32_t i = 0; i < length; ++i) {
		if (data[i] != 0xFF) {
			return 0; /* Not empty */
		}
	}
	return 1; /* Empty */
}

/****************************************************************************
 * Name: driver_ns_flash_read
 *
 * Description:
 *   Common driver-side flash read function for all boards
 ****************************************************************************/
static int driver_ns_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display)
{
#if defined(CONFIG_ARCH_CHIP_BK7239N)
	extern int bk_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
	return bk_ss_flash_read(address, len, data, en_display);
#elif defined(CONFIG_AMEBASMART_TRUSTZONE) || defined(CONFIG_AMEBAD_TRUSTZONE) || defined(CONFIG_AMEBALITE_TRUSTZONE)
	extern int rtl_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
	return rtl_ss_flash_read(address, len, data, en_display);
#else
	return -ENODEV;
#endif
}

/****************************************************************************
 * Name: driver_ns_flash_write
 *
 * Description:
 *   Common driver-side flash write function for all boards
 ****************************************************************************/
static int driver_ns_flash_write(uint32_t address, uint32_t len, uint8_t *data, int en_display)
{
#if defined(CONFIG_ARCH_CHIP_BK7239N)
	extern int bk_ss_flash_write(uint32_t address, uint32_t len, uint8_t *data, int en_display);
	return bk_ss_flash_write(address, len, data, en_display);
#elif defined(CONFIG_AMEBASMART_TRUSTZONE) || defined(CONFIG_AMEBAD_TRUSTZONE) || defined(CONFIG_AMEBALITE_TRUSTZONE)
	extern int rtl_ss_flash_write(uint32_t address, uint32_t len, uint8_t *data, int en_display);
	return rtl_ss_flash_write(address, len, data, en_display);
#else
	return -ENODEV;
#endif
}

/****************************************************************************
 * Name: driver_flash_erase
 *
 * Description:
 *   Common driver-side flash erase function for all boards
 ****************************************************************************/
static void driver_flash_erase(uint32_t address)
{
	extern void ns_flash_erase(uint32_t address);
	ns_flash_erase(address);
}

/****************************************************************************
 * Name: driver_verify_flash_protect
 *
 * Description:
 *   Common driver-side flash protect verification function for all boards
 ****************************************************************************/
static int driver_verify_flash_protect(void)
{
#if defined(CONFIG_ARCH_CHIP_BK7239N)
	extern int bk_verify_flash_protect(void);
	return bk_verify_flash_protect();
#elif defined(CONFIG_AMEBASMART_TRUSTZONE) || defined(CONFIG_AMEBAD_TRUSTZONE) || defined(CONFIG_AMEBALITE_TRUSTZONE)
	extern int rtl_verify_flash_protect(void);
	return rtl_verify_flash_protect();
#else
	return 0;
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_ns_flash_op
 *
 * Description:
 *   Unified handler for secure storage flash operations (read/write/erase).
 *   The operation type is determined by the op_type field in the argument.
 *   Uses common driver-side functions instead of board-specific macros.
 *
 * Input Parameters:
 *   cmd - ioctl command number
 *   arg - pointer to ns_flash_op_arg structure
 *
 * Returned Value:
 *   OK (0) on success, negative errno on failure
 *
 ****************************************************************************/

int test_ns_flash_op(int cmd, unsigned long arg)
{
	struct ns_flash_op_arg *op_arg = (struct ns_flash_op_arg *)arg;
	int ret = -EINVAL;

	if (!op_arg) {
		dbg("Invalid argument: NULL pointer\n");
		return -EINVAL;
	}

	dbg("NS Flash Op: type=%d, slot=%d, addr=0x%x, len=%d\n",
	     op_arg->op_type, op_arg->slot_index, op_arg->address, op_arg->length);

	/* Single function, different behavior based on op_type */
	switch (op_arg->op_type) {
	case NS_FLASH_OP_READ:
		dbg("[KERNEL] Raw flash read: addr=0x%x, len=%d\n",
		     op_arg->address, op_arg->length);
		ret = driver_ns_flash_read(op_arg->address, op_arg->length,
		                          op_arg->buffer, 1);
		if (ret != OK) {
			dbg("Flash read failed: ret=%d\n", ret);
		}
		break;

	case NS_FLASH_OP_WRITE:
		dbg("[KERNEL] Raw flash write: addr=0x%x, len=%d, data=0x%x\n",
		     op_arg->address, op_arg->length, op_arg->write_data);
		ret = driver_ns_flash_write(op_arg->address, op_arg->length,
		                           op_arg->buffer, 1);
		if (ret != OK) {
			dbg("Flash write failed: ret=%d\n", ret);
		}
		break;

	case NS_FLASH_OP_ERASE:
	{
		dbg("[KERNEL] Raw flash erase with protection verification: addr=0x%x\n", op_arg->address);
		
		int flash_protected = 0;
		int area_empty = 0;
		int empty = 0;
		
		/* Step 1: Read flash data before erase to check if data exists */
		ret = driver_ns_flash_read(op_arg->address, op_arg->length, op_arg->buffer, 0);
		if (ret != OK) {
			dbg("Flash read before erase failed: ret=%d\n", ret);
			area_empty = 1; /* Assume empty if read fails */
		} else {
			area_empty = check_flash_data_empty(op_arg->buffer, op_arg->length);
		}
		
		/* Step 2: Verify flash protection */
		flash_protected = driver_verify_flash_protect();
		dbg("Flash protected: %d, Area empty: %d\n", flash_protected, area_empty);
		
		/* Step 3: Attempt erase */
		driver_flash_erase(op_arg->address);
		ret = OK;
		
		/* Step 4: Read back to verify erase result */
		ret = driver_ns_flash_read(op_arg->address, op_arg->length, op_arg->buffer, 0);
		if (ret != OK) {
			dbg("Flash read after erase failed: ret=%d\n", ret);
			empty = 0; /* Assume not empty if read fails */
		} else {
			empty = check_flash_data_empty(op_arg->buffer, op_arg->length);
		}
		
		/* Step 5: Print appropriate log messages based on protection and erase result */
		if (flash_protected) {
			if (empty) {
				dbg("ERROR: Erase success, but flash block protection detected and original data corrupted.\n");
			} else {
				dbg("EXPECTED: Erase failed, flash block protection detected and original data retained.\n");
			}
		} else {
			if (empty) {
				dbg("EXPECTED: Erase success, no flash block protection detected.\n");
			} else {
				dbg("ERROR: Erase failed, no flash block protection detected.\n");
			}
		}
		break;
	}

	default:
		dbg("Unknown op_type: %d\n", op_arg->op_type);
		ret = -EINVAL;
		break;
	}

	return ret;
}


