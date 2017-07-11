/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "scsc_mx_impl.h"
#include "miframman.h"
#include "mifmboxman.h"
#include "mxman.h"
#include "srvman.h"
#include "mxmgmt_transport.h"
#include "gdb_transport.h"
#include "mxconf.h"
#include "fwimage.h"
#include "fwhdr.h"
#include "mxlog.h"
#include "fw_panic_record.h"
#include "mxlog_transport.h"
#include "utils_misc.h"
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <tinyara/wqueue.h>
#include "tinyara_wrapper.h"
#include "utils_scsc.h"
#include "debug_scsc.h"

#ifdef CONFIG_ARCH_CHIP_S5JT200
#include "mif_reg.h"
#endif

#define STRING_BUFFER_MAX_LENGTH 128
#define NUMBER_OF_STRING_ARGS 1
#define MX_SRAM_SIZE (128 * 1024)
#define MX_FW_RUNTIME_LENGTH (1024 * 1024)
#define WAIT_FOR_FW_TO_START_DELAY_MS 1000
#define MBOX2_MAGIC_NUMBER 0xbcdeedcb
#define MBOX_INDEX_0 0
#define MBOX_INDEX_1 1
#define MBOX_INDEX_2 2
#define MBOX_INDEX_3 3
#define MBOX_INDEX_4 4
#define MBOX_INDEX_5 5
#define MBOX_INDEX_6 6
#define MBOX_INDEX_7 7

/* Allow unidentified firmware */
static bool allow_unidentified_firmware;

/* Assuming unidentified firmware */
static bool skip_header;

/* Allow skipping firmware CRC checks if CRC is not present */
static bool crc_check_allow_none = true;

/* Time period for checking the firmware CRCs */
static int crc_check_period_ms = 30000;

/* Timeout wait_for_mm_msg_start_ind (ms) - default 1000. 0 = infinite */
static ulong mm_completion_timeout_ms = 2000;

/* Allow skipping firmware mbox0 signature check */
static bool skip_mbox0_check;

/* Timeout send_mm_msg_stop_blocking (ms) - default 100 */
static uint mif_access_max_time_ms = 100;

/* 0 = Proceed as normal (default); Bit 0 = 1 - spin at start of CRT0; Other bits reserved = 0 */
static uint firmware_startup_flags;

/* Disable error handling */
static bool disable_error_handling;

/* Delay in ms before accessing the panic record */
static uint panic_record_delay = 1;

static bool disable_recovery_handling;

/**
 * Maxwell Agent Management Messages.
 *
 * The numbers here *must* match the firmware!
 */
enum {
	MM_START_IND = 0,
	MM_HALT_REQ = 1,
	MM_FORCE_PANIC = 2,
	MM_HOST_SUSPEND = 3,
	MM_HOST_RESUME = 4
} ma_msg;

/**
 * Format of the Maxwell agent messages
 * on the Maxwell management transport stream.
 */
struct ma_msg_packet {
	uint8_t ma_msg;				/* Message from ma_msg enum */
} __packed;

static void mxman_stop(struct mxman *mxman);
static void print_mailboxes(struct mxman *mxman);

static int wait_for_mm_msg_start_ind(struct mxman *mxman)
{
	int r;

	if (0 == mm_completion_timeout_ms) {
		/* Zero implies infinite wait */
		r = wait_for_completion_interruptible(&mxman->mm_msg_start_ind_completion);
		/* r = -ERESTARTSYS if interrupted, 0 if completed */
		return r;
	}

	r = wait_for_completion_timeout(&mxman->mm_msg_start_ind_completion, mm_completion_timeout_ms);
	if (r == 0) {
		SLSI_ERR_NODEV("timeout\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static int send_mm_msg_stop_blocking(struct mxman *mxman)
{
	struct ma_msg_packet message = {.ma_msg = MM_HALT_REQ };

	mxmgmt_transport_send(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, &message, sizeof(message));
	sys_msleep(mif_access_max_time_ms);

	return 0;
}

void mm_start_ind_complete(struct mxman *mxman)
{
	complete(&mxman->mm_msg_start_ind_completion);
}

/** Receive handler for messages from the FW along the maxwell management transport */
static void mxman_message_handler(const void *message, void *data)
{
	struct mxman *mxman = (struct mxman *)data;

	/* Forward the message to the applicable service to deal with */
	const struct ma_msg_packet *msg = message;

	switch (msg->ma_msg) {
	case MM_START_IND:
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
		SLSI_INFO_NODEV("Received MM_START_IND message from the firmware\n");
#endif
		work_queue(LPWORK, &mxman->mm_msg_start_ind_work, (worker_t)mm_start_ind_complete, mxman, 0);
		break;
	default:
		/* HERE: Unknown message, raise fault */
		SLSI_WARN_NODEV("Received unknown message from the firmware: msg->ma_msg=%d\n", msg->ma_msg);
		break;
	}
}

/*
 * This function calulates and checks two or three (depending on crc32_over_binary flag)
 * crc32 values in the firmware header. The function will check crc32 over the firmware binary
 * (i.e. everything in the file following the header) only if the crc32_over_binary is set to 'true'.
 * This includes initialised data regions so it can be used to check when loading but will not be
 * meaningful once execution starts.
 */
static int do_fw_crc32_checks(char *fw, u32 fw_image_size, struct fwhdr *fwhdr, bool crc32_over_binary)
{
	int r;

	SLSI_ERR_NODEV(" \n");
	if ((fwhdr->fw_crc32 == 0 || fwhdr->header_crc32 == 0 || fwhdr->const_crc32 == 0) && crc_check_allow_none == 0) {
		SLSI_ERR_NODEV("error: CRC is missing fw_crc32=%d header_crc32=%d crc_check_allow_none=%d\n", fwhdr->fw_crc32, fwhdr->header_crc32, crc_check_allow_none);
		return -ENOENT;
	}

	if (fwhdr->header_crc32 == 0 && crc_check_allow_none == 1) {
		SLSI_INFO_NODEV("Skipping CRC check header_crc32=%d crc_check_allow_none=%d\n", fwhdr->header_crc32, crc_check_allow_none);
	} else {
		/*
		 * CRC-32-IEEE of all preceding header fields (including other CRCs).
		 * Always the last word in the header.
		 */
		r = fwimage_check_fw_header_crc(fw, fwhdr->hdr_length, fwhdr->header_crc32);
		if (r) {
			SLSI_ERR_NODEV("fwimage_check_fw_header_crc() failed\n");
			return r;
		}
	}

	if (fwhdr->const_crc32 == 0 && crc_check_allow_none == 1) {
		SLSI_INFO_NODEV("Skipping CRC check const_crc32=%d crc_check_allow_none=%d\n", fwhdr->const_crc32, crc_check_allow_none);
	} else {
		/*
		 * CRC-32-IEEE over the constant sections grouped together at start of firmware binary.
		 * This CRC should remain valid during execution. It can be used by run-time checker on
		 * host to detect firmware corruption (not all memory masters are subject to MPUs).
		 */
		r = fwimage_check_fw_const_section_crc(fw, fwhdr->const_crc32, fwhdr->const_fw_length, fwhdr->hdr_length);
		if (r) {
			SLSI_ERR_NODEV("fwimage_check_fw_const_section_crc() failed\n");
			return r;
		}
	}

	if (crc32_over_binary) {
		if (fwhdr->fw_crc32 == 0 && crc_check_allow_none == 1) {
			SLSI_INFO_NODEV("Skipping CRC check fw_crc32=%d crc_check_allow_none=%d\n", fwhdr->fw_crc32, crc_check_allow_none);
		} else {
			/*
			 * CRC-32-IEEE over the firmware binary (i.e. everything
			 * in the file following this header).
			 * This includes initialised data regions so it can be used to
			 * check when loading but will not be meaningful once execution starts.
			 */
			r = fwimage_check_fw_crc(fw, fw_image_size, fwhdr->hdr_length, fwhdr->fw_crc32);
			if (r) {
				SLSI_ERR_NODEV("fwimage_check_fw_crc() failed\n");
				return r;
			}
		}
	}

	return 0;
}

static void fw_crc_work_func(FAR void *arg);
static void fw_crc_wq_start(struct mxman *mxman)
{
	work_queue(HPWORK, &mxman->fw_crc_work, fw_crc_work_func, mxman, (crc_check_period_ms));
}

static void fw_crc_work_func(FAR void *arg)
{
	int r;
	struct mxman *mxman = container_of(arg, struct mxman, fw_crc_work);

	r = do_fw_crc32_checks(mxman->fw, mxman->fw_image_size, &mxman->fwhdr, false);
	if (r) {
		SLSI_ERR_NODEV("do_fw_crc32_checks() failed r=%d\n", r);
		mxman_fail(mxman);
		return;
	}
	fw_crc_wq_start(mxman);
}

static void fw_crc_wq_init(struct mxman *mxman)
{
}

static void fw_crc_wq_stop(struct mxman *mxman)
{
	mxman->check_crc = false;

	work_cancel(HPWORK, &mxman->fw_crc_work);
}

static void fw_crc_wq_deinit(struct mxman *mxman)
{
	fw_crc_wq_stop(mxman);
	//destroy_workqueue(mxman->fw_crc_wq);
}

static int transports_init(struct mxman *mxman)
{
	struct mxconf *mxconf;
	int r;
	struct scsc_mx *mx = mxman->mx;

	/* Initialise mx management stack */
	r = mxmgmt_transport_init(scsc_mx_get_mxmgmt_transport(mx), mx);

	if (r) {
		SLSI_ERR_NODEV("mxmgmt_transport_init() failed\n");
		return r;
	}

	/* Initialise gdb transport for cortex-R4 */
	r = gdb_transport_init(scsc_mx_get_gdb_transport_r4(mx), mx, GDB_TRANSPORT_R4);
	if (r) {
		SLSI_ERR_NODEV("gdb_transport_init() failed\n");
		mxmgmt_transport_release(scsc_mx_get_mxmgmt_transport(mx));
		return r;
	}
#ifdef MX_LOG_PHASE_4
	/* Initialise mxlog transport */
	r = mxlog_transport_init(scsc_mx_get_mxlog_transport(mx), mx);
	if (r) {
		SLSI_ERR_NODEV("mxlog_transport_init() failed\n");
		gdb_transport_release(scsc_mx_get_gdb_transport_r4(mx));
		mxmgmt_transport_release(scsc_mx_get_mxmgmt_transport(mx));
		return r;
	}
#endif
	/*
	 * Allocate & Initialise Infrastructre Config Structure
	 * including the mx management stack config information.
	 */
	mxconf = miframman_alloc(scsc_mx_get_ramman(mx), sizeof(struct mxconf), 4);
	if (!mxconf) {
		SLSI_ERR_NODEV("miframman_alloc() failed\n");

		gdb_transport_release(scsc_mx_get_gdb_transport_r4(mx));
		mxmgmt_transport_release(scsc_mx_get_mxmgmt_transport(mx));
#ifdef MX_LOG_PHASE_4
		mxlog_transport_release(scsc_mx_get_mxlog_transport(mx));
#endif
		return -ENOMEM;
	}
	mxman->mxconf = mxconf;
	mxconf->magic = MXCONF_MAGIC;
	mxconf->version.major = MXCONF_VERSION_MAJOR;
	mxconf->version.minor = MXCONF_VERSION_MINOR;
	/* serialise mxmgmt transport */
	mxmgmt_transport_config_serialise(scsc_mx_get_mxmgmt_transport(mx), &mxconf->mx_trans_conf);
	/* serialise Cortex-R4 gdb transport */
	gdb_transport_config_serialise(scsc_mx_get_gdb_transport_r4(mx), &mxconf->mx_trans_conf_gdb_r4);
	/* serialise mxlog transport */
	mxlog_transport_config_serialise(scsc_mx_get_mxlog_transport(mx), &mxconf->mxlogconf);
	return 0;
}

static void transports_release(struct mxman *mxman)
{
#ifdef MX_LOG_PHASE_4
	mxlog_transport_release(scsc_mx_get_mxlog_transport(mxman->mx));
#endif
	mxmgmt_transport_release(scsc_mx_get_mxmgmt_transport(mxman->mx));
	gdb_transport_release(scsc_mx_get_gdb_transport_r4(mxman->mx));
	miframman_free(scsc_mx_get_ramman(mxman->mx), mxman->mxconf);
}

static void mbox_init(struct mxman *mxman, u32 firmware_entry_point)
{
	u32 *mbox0;
	u32 *mbox1;
	u32 *mbox2;
	u32 *mbox3;
	u32 *mbox4;
	scsc_mifram_ref mifram_ref;
	struct scsc_mx *mx = mxman->mx;
	struct scsc_mif_abs *mif = scsc_mx_get_mif_abs(mxman->mx);

	/* Place firmware entry address in MIF MBOX 0 so R4 ROM knows where to jump to! */
	mbox0 = mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif, MBOX_INDEX_0);
	mbox1 = mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif, MBOX_INDEX_1);

	/* Write (and flush) entry point to MailBox 0, config address to MBOX 1 */
	*mbox0 = firmware_entry_point;
	mif->get_mifram_ref(mif, mxman->mxconf, &mifram_ref);
	*mbox1 = mifram_ref;		/* must be R4-relative address here */
	/* CPU memory barrier */
	wmb();
	/*
	 * write the magic number "0xbcdeedcb" to MIF Mailbox #2 &
	 * copy the firmware_startup_flags to MIF Mailbox #3 before starting (reset = 0) the R4
	 */
	mbox2 = mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif, MBOX_INDEX_2);
	*mbox2 = MBOX2_MAGIC_NUMBER;
	mbox3 = mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif, MBOX_INDEX_3);
	*mbox3 = firmware_startup_flags;
	mbox4 = mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif, MBOX_INDEX_4);
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	SLSI_INFO_NODEV("mbox0: %08x, mbox1: %08x, mbox4 = %08x\n", *mbox0, *mbox1, *mbox4);
#endif
}

static int fwhdr_init(char *fw, struct fwhdr *fwhdr, bool *fwhdr_parsed_ok, bool *check_crc)
{
	/*
	 * Validate the fw image including checking the firmware header, majic #, version, checksum  so on
	 * then do CRC on the entire image
	 *
	 * Derive some values from header -
	 *
	 * PORT: assumes little endian
	 */
	if (skip_header) {
		*fwhdr_parsed_ok = false;    /* Allows the forced start address to be used */
	} else {
		*fwhdr_parsed_ok = fwhdr_parse(fw, fwhdr);
	}
	*check_crc = false;
	if (*fwhdr_parsed_ok) {
		SLSI_INFO_NODEV("FW HEADER version: hdr_major: %d hdr_minor: %d\n", fwhdr->hdr_major, fwhdr->hdr_minor);
		switch (fwhdr->hdr_major) {
		case 0:
			switch (fwhdr->hdr_minor) {
			case 2:
				*check_crc = true;
				break;
			default:
				SLSI_ERR_NODEV("Unsupported FW HEADER version: hdr_major: %d hdr_minor: %d\n", fwhdr->hdr_major, fwhdr->hdr_minor);
				return -ENOENT;
			}
			break;
		case 1:
			*check_crc = true;
			break;
		default:
			SLSI_ERR_NODEV("Unsupported FW HEADER version: hdr_major: %d hdr_minor: %d\n", fwhdr->hdr_major, fwhdr->hdr_minor);
			return -ENOENT;
		}
		switch (fwhdr->fwapi_major) {
		case 0:
			switch (fwhdr->fwapi_minor) {
			case 2:
				SLSI_INFO_NODEV("FWAPI version: fwapi_major: %d fwapi_minor: %d\n", fwhdr->fwapi_major, fwhdr->fwapi_minor);
				break;
			default:
				SLSI_ERR_NODEV("Unsupported FWAPI version: fwapi_major: %d fwapi_minor: %d\n", fwhdr->fwapi_major, fwhdr->fwapi_minor);
				return -ENOENT;
			}
			break;
		default:
			SLSI_ERR_NODEV("Unsupported FWAPI version: fwapi_major: %d fwapi_minor: %d\n", fwhdr->fwapi_major, fwhdr->fwapi_minor);
			return -ENOENT;
		}
	} else {
		/* This is unidetified pre-header firmware - assume it is built to run at 0xb8000000 == 0 for bootrom */
		if (allow_unidentified_firmware) {
			SLSI_INFO_NODEV("Unidentified firmware override\n");
			fwhdr->firmware_entry_point = 0;
			fwhdr->fw_runtime_length = MX_FW_RUNTIME_LENGTH;
		} else {
			SLSI_ERR_NODEV("Unidentified firmware is not allowed\n");
			return -ENOENT;
		}
	}
	return 0;
}

static int fw_init(struct mxman *mxman, bool *fwhdr_parsed_ok)
{
	int r;
	char *build_id;
	struct fwhdr *fwhdr = &mxman->fwhdr;
	char *fw = (void *)up_wlan_get_firmware();

	r = fwhdr_init(fw, fwhdr, fwhdr_parsed_ok, &mxman->check_crc);
	if (r) {
		SLSI_ERR_NODEV("fwhdr_init() failed\n");
		return r;
	}
	mxman->fw = fw;

	if (*fwhdr_parsed_ok) {
		build_id = fwhdr_get_build_id(fw, fwhdr);
		if (build_id) {
			(void)snprintf(mxman->fw_build_id, sizeof(mxman->fw_build_id), "%s", build_id);
			SLSI_INFO_NODEV("Firmware BUILD_ID: %s\n", mxman->fw_build_id);
		} else {
			SLSI_ERR_NODEV("Failed to get Firmware BUILD_ID\n");
		}
	}
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	pr_debug("firmware_entry_point=0x%x fw_runtime_length=%d\n", fwhdr->firmware_entry_point, fwhdr->fw_runtime_length);
#endif
	return 0;
}

static int mxman_start(struct mxman *mxman)
{
	void *start_sram;
	size_t size_sram = MX_SRAM_SIZE;
	struct scsc_mif_abs *mif;
	struct fwhdr *fwhdr = &mxman->fwhdr;
	bool fwhdr_parsed_ok = false;
	void *start_mifram_heap;
	u32 length_mifram_heap;
	int r;

	(void)snprintf(mxman->fw_build_id, sizeof(mxman->fw_build_id), "unknown");

	/* If the option is set to skip header, we must allow unidentified f/w */
	if (skip_header) {
		SLSI_INFO_NODEV("Ignoring firmware header block\n");
		allow_unidentified_firmware = true;
	}

	mif = scsc_mx_get_mif_abs(mxman->mx);
	start_sram = mif->map(mif, &size_sram);
	if (!start_sram) {
		SLSI_ERR_NODEV("Error allocating dram\n");
		return -ENOMEM;
	}

	//pr_debug("%s: Allocated %zu bytes start_dram %p\n", __func__, size_dram, start_dram);

	mxman->start_sram = start_sram;

	r = fw_init(mxman, &fwhdr_parsed_ok);
	if (r) {
		SLSI_ERR_NODEV("fw_init() failed\n");
		mif->unmap(mif, mxman->start_sram);
		return r;
	}

	start_mifram_heap = (void *)MIFRAMMAN_HEAP_START;
	length_mifram_heap = MIFRAMMAN_HEAP_SIZE;

#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	pr_debug("HEAP start: %p, HEAP size: %zu bytes\n", start_mifram_heap, length_mifram_heap);
#endif

	miframman_init(scsc_mx_get_ramman(mxman->mx), start_mifram_heap, length_mifram_heap);
	mifmboxman_init(scsc_mx_get_mboxman(mxman->mx));
	mifintrbit_init(scsc_mx_get_intrbit(mxman->mx), mif);

	/* Initialise transports */
	r = transports_init(mxman);
	if (r) {
		SLSI_ERR_NODEV("transports_init() failed\n");
		fw_crc_wq_stop(mxman);
		miframman_deinit(scsc_mx_get_ramman(mxman->mx));
		/* Release the MIF memory resources */
		mif->unmap(mif, mxman->start_sram);

		return r;
	}
	mbox_init(mxman, fwhdr->firmware_entry_point);
	init_completion(&mxman->mm_msg_start_ind_completion);
	mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, &mxman_message_handler, mxman);
	mxlog_init(scsc_mx_get_mxlog(mxman->mx), mxman->mx);

	mif->reset(mif, 0);
	if (fwhdr_parsed_ok) {
		r = wait_for_mm_msg_start_ind(mxman);
		if (r) {
			SLSI_ERR_NODEV("wait_for_MM_START_IND() failed: r=%d\n", r);
			print_mailboxes(mxman);
			if (skip_mbox0_check) {
				SLSI_ERR_NODEV("timeout ignored in skip_mbox0_check mode\n");
				return 0;
			}
			mxman_stop(mxman);
			return r;
		}
	} else {
		sleep(WAIT_FOR_FW_TO_START_DELAY_MS);
	}

	return 0;
}

/*
 * workqueue thread
 */
static void mxman_failure_work(FAR void *arg)
{
	struct mxman *mxman = (struct mxman *)arg;
	struct srvman *srvman;
	struct scsc_mif_abs *mif = scsc_mx_get_mif_abs(mxman->mx);
	struct scsc_mx *mx = mxman->mx;
	u16 scsc_panic_code;

	SLSI_INFO_NODEV("Complete mm_msg_start_ind_completion\n");
	pthread_mutex_lock(&mxman->mxman_mutex);

	complete(&mxman->mm_msg_start_ind_completion);
	srvman = scsc_mx_get_srvman(mxman->mx);

	if (mxman->mxman_state != MXMAN_STATE_STARTED) {
		pr_warn("%s: Not in started state: mxman->mxman_state=%d\n", __func__, mxman->mxman_state);
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return;
	}
	scsc_panic_code = mxman->scsc_panic_code;
	mxlog_release(scsc_mx_get_mxlog(mx));
	/* unregister channel handler */
	mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, NULL, NULL);
	mxmgmt_transport_set_error(scsc_mx_get_mxmgmt_transport(mx));
	srvman_set_error(srvman);
	fw_crc_wq_stop(mxman);

	mxman->mxman_state = mxman->mxman_next_state;

	if (mxman->mxman_state != MXMAN_STATE_FAILED && mxman->mxman_state != MXMAN_STATE_FREEZED) {
		WARN_ON(mxman->mxman_state != MXMAN_STATE_FAILED && mxman->mxman_state != MXMAN_STATE_FREEZED);
		SLSI_ERR_NODEV("Bad state=%d\n", mxman->mxman_state);
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return;
	}

	/* Signal panic to r4 processors */
	SLSI_INFO_NODEV("Setting MIFINTRBIT_RESERVED_PANIC_R4\n");
	mif->irq_bit_set(mif, MIFINTRBIT_RESERVED_PANIC_R4, SCSC_MIFINTR_TARGET_R4);

	srvman_freeze_services(srvman);
	if (mxman->mxman_state == MXMAN_STATE_FAILED) {
		u32 *r4_panic_record = (u32 *)((WIFI_IRAM_MEM_BASE << 12) + mxman->fwhdr.r4_panic_record_offset);
		bool r4_panic_record_ok = false;
		/* some configurable delay before accessing the panic record */
		sys_msleep(panic_record_delay);
		r4_panic_record_ok = fw_parse_r4_panic_record(r4_panic_record);
		/*
		 * Check if the panic was trigered by MX and
		 * set the subcode if so.
		 */
		if ((scsc_panic_code & 0x8000) == 0) {
			if (r4_panic_record_ok) {
				scsc_panic_code |= 0xFFF & r4_panic_record[2];
			}
		}
		/* Set unspecified technilogy for now */
		scsc_panic_code |= 0x03 << 13;
		SLSI_INFO_NODEV("scsc_panic_code=0x%x\n", scsc_panic_code);
	}
	if (!disable_recovery_handling) {
		srvman_clear_error(srvman);
	}
	pthread_mutex_unlock(&mxman->mxman_mutex);
	if (!disable_recovery_handling) {
		SLSI_INFO_NODEV("Calling srvman_unfreeze_services with scsc_panic_code=0x%x\n", scsc_panic_code);
		srvman_unfreeze_services(srvman, scsc_panic_code);
		if (scsc_mx_module_reset() < 0) {
			SLSI_INFO_NODEV("failed to call scsc_mx_module_reset\n");
		}
	}

	/**
	 * If recovery is disabled and an scsc_mx_service_open has been hold up,
	 * release it, rather than wait for the recovery_completion to timeout.
	 */
	if (disable_recovery_handling) {
		complete(&mxman->recovery_completion);
	}

}

static void failure_wq_stop(struct mxman *mxman)
{
	work_cancel(HPWORK, &mxman->failure_work);
}

static void failure_wq_deinit(struct mxman *mxman)
{
	failure_wq_stop(mxman);
}

static void failure_wq_start(struct mxman *mxman)
{
	if (disable_error_handling) {
		SLSI_INFO_NODEV("%s: error handling disabled\n", __func__);
	} else {
		work_queue(HPWORK, &mxman->failure_work, mxman_failure_work, mxman, 0);
	}
}

static void print_mailboxes(struct mxman *mxman)
{
	struct scsc_mif_abs *mif;
	struct mifmboxman *mboxman;
	int i;

	mif = scsc_mx_get_mif_abs(mxman->mx);
	mboxman = scsc_mx_get_mboxman(mxman->mx);

	SLSI_INFO_NODEV("Printing mailbox values:\n");
	for (i = 0; i < MIFMBOX_NUM; i++) {
		SLSI_INFO_NODEV("MBOX_%d: 0x%x\n", i, *mifmboxman_get_mbox_ptr(mboxman, mif, i));
	}
}

int mxman_open(struct mxman *mxman)
{
	int r;

	pthread_mutex_lock(&mxman->mxman_mutex);
	if (mxman->mxman_state == MXMAN_STATE_STARTED) {
		/* if in the STARTED state there MUST already be some users */
		if (WARN_ON(!mxman->users)) {
			SLSI_ERR_NODEV("%s: ERROR mxman->mxman_state=%d users=%d\n", __func__, mxman->mxman_state, mxman->users);
			pthread_mutex_unlock(&mxman->mxman_mutex);
			return -EINVAL;
		}
		mxman->users++;
		SLSI_INFO_NODEV("Already opened: users=%d\n", mxman->users);
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return 0;
	} else if (mxman->mxman_state == MXMAN_STATE_STOPPED) {
		r = mxman_start(mxman);
		if (r) {
			SLSI_ERR_NODEV("%s: maxwell_manager_start() failed r=%d users=%d\n", r, mxman->users);
			pthread_mutex_unlock(&mxman->mxman_mutex);
			return r;
		}
		mxman->users++;
		mxman->mxman_state = MXMAN_STATE_STARTED;
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return 0;
	}
	WARN_ON(mxman->mxman_state != MXMAN_STATE_STARTED && mxman->mxman_state != MXMAN_STATE_STOPPED);
	SLSI_ERR_NODEV("Bad state: mxman->mxman_state=%d\n", mxman->mxman_state);
	pthread_mutex_unlock(&mxman->mxman_mutex);
	return -EPERM;
}

static void mxman_stop(struct mxman *mxman)
{
	struct scsc_mif_abs *mif;

	SLSI_INFO_NODEV("\n");

	(void)snprintf(mxman->fw_build_id, sizeof(mxman->fw_build_id), "unknown");

	/* Shutdown the hardware */
	mif = scsc_mx_get_mif_abs(mxman->mx);
	mif->reset(mif, 1);
	transports_release(mxman);

	mxlog_release(scsc_mx_get_mxlog(mxman->mx));
	/* unregister channel handler */
	mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, NULL, NULL);
	fw_crc_wq_stop(mxman);

	/* Unitialise components (they may perform some checks - e.g. all memory freed) */
	mifintrbit_deinit(scsc_mx_get_intrbit(mxman->mx));
	miframman_deinit(scsc_mx_get_ramman(mxman->mx));
	mifmboxman_deinit(scsc_mx_get_mboxman(mxman->mx));

	/* Release the MIF memory resources */
	mif->unmap(mif, mxman->start_sram);
}

void mxman_close(struct mxman *mxman)
{
	int r;

	pthread_mutex_lock(&mxman->mxman_mutex);
	SLSI_INFO_NODEV("\n");

	if (mxman->mxman_state == MXMAN_STATE_STARTED) {
		if (WARN_ON(!mxman->users)) {
			SLSI_ERR_NODEV("%s: ERROR users=%d\n", __func__, mxman->users);
			pthread_mutex_unlock(&mxman->mxman_mutex);
			return;
		}
		mxman->users--;
		if (mxman->users) {
			SLSI_INFO_NODEV("Current number of users=%d\n", mxman->users);
			pthread_mutex_unlock(&mxman->mxman_mutex);
			return;
		}

		/*
		 * Ask the subsystem to stop (MM_STOP_REQ), and wait
		 * for response (MM_STOP_RSP).
		 */
		r = send_mm_msg_stop_blocking(mxman);
		if (r) {
			SLSI_ERR_NODEV("send_mm_msg_stop_blocking failed: r=%d\n", r);
		}

		mxman_stop(mxman);
		mxman->mxman_state = MXMAN_STATE_STOPPED;
		pthread_mutex_unlock(&mxman->mxman_mutex);
	} else if (mxman->mxman_state == MXMAN_STATE_FAILED) {
		if (WARN_ON(!mxman->users)) {
			SLSI_ERR_NODEV("ERROR users=%d\n", mxman->users);
		}

		mxman->users--;
		if (mxman->users) {
			SLSI_INFO_NODEV("Current number of users=%d\n", mxman->users);
			pthread_mutex_unlock(&mxman->mxman_mutex);
			return;
		}

		mxman_stop(mxman);
		mxman->mxman_state = MXMAN_STATE_STOPPED;
		pthread_mutex_unlock(&mxman->mxman_mutex);
		complete(&mxman->recovery_completion);
	} else {
		WARN_ON(mxman->mxman_state != MXMAN_STATE_STARTED);
		SLSI_ERR_NODEV("Bad state: mxman->mxman_state=%d\n", mxman->mxman_state);
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return;
	}
}

void mxman_fail(struct mxman *mxman)
{
	mxman->mxman_next_state = MXMAN_STATE_FAILED;
	failure_wq_start(mxman);
}

void mxman_freeze(struct mxman *mxman)
{
	mxman->mxman_next_state = MXMAN_STATE_FREEZED;
	failure_wq_start(mxman);
}

void mxman_init(struct mxman *mxman, struct scsc_mx *mx)
{
	mxman->mx = mx;
	mxman->suspended = 0;
	fw_crc_wq_init(mxman);
	pthread_mutex_init(&mxman->mxman_mutex, NULL);
	/* set the initial state */
	mxman->mxman_state = MXMAN_STATE_STOPPED;
	(void)snprintf(mxman->fw_build_id, sizeof(mxman->fw_build_id), "unknown");
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	disable_recovery_handling = 0;
#else
	disable_recovery_handling = 1;
#endif
}

void mxman_deinit(struct mxman *mxman)
{
	fw_crc_wq_deinit(mxman);
	failure_wq_deinit(mxman);
	pthread_mutex_destroy(&mxman->mxman_mutex);
}

int mxman_force_panic(struct mxman *mxman)
{
	struct ma_msg_packet message = {.ma_msg = MM_FORCE_PANIC };

	pthread_mutex_lock(&mxman->mxman_mutex);
	if (mxman->mxman_state == MXMAN_STATE_STARTED) {
		mxmgmt_transport_send(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, &message, sizeof(message));
		pthread_mutex_unlock(&mxman->mxman_mutex);
		return 0;
	}
	pthread_mutex_unlock(&mxman->mxman_mutex);
	return -EINVAL;
}

int mxman_suspend(struct mxman *mxman)
{
	struct ma_msg_packet message = {.ma_msg = MM_HOST_SUSPEND };

	SLSI_INFO_NODEV("\n");

	pthread_mutex_lock(&mxman->mxman_mutex);
	if (mxman->mxman_state == MXMAN_STATE_STARTED) {
		SLSI_INFO_NODEV("MM_HOST_SUSPEND\n");
		mxmgmt_transport_send(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, &message, sizeof(message));
		mxman->suspended = 1;
	}
	pthread_mutex_unlock(&mxman->mxman_mutex);
	return 0;
}

void mxman_resume(struct mxman *mxman)
{
	struct ma_msg_packet message = {.ma_msg = MM_HOST_RESUME };

	SLSI_INFO_NODEV("\n");

	pthread_mutex_lock(&mxman->mxman_mutex);
	if (mxman->mxman_state == MXMAN_STATE_STARTED) {
		SLSI_INFO_NODEV("MM_HOST_RESUME\n");
		mxmgmt_transport_send(scsc_mx_get_mxmgmt_transport(mxman->mx), MMTRANS_CHAN_ID_MAXWELL_MANAGEMENT, &message, sizeof(message));
		mxman->suspended = 0;
	}
	pthread_mutex_unlock(&mxman->mxman_mutex);
}
