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

/**
 * mx140 Infrastructure Configuration Structure.
 *
 * Used to pass configuration data from AP to R4 infrastructure
 * on Maxwell Subsystem startup.
 *
 * Notes:
 *
 * - All multi-octet integers shall be stored LittleEndian.
 *
 * - All location fields ("*_loc") are 32 bit octet offsets w.r.t. the R4
 * address map. They can therefore refer to DRAM memory or Mailbox registers.
 *
 */
#ifndef MXCONF_H__
#define MXCONF_H__

/* Uses */

/* Definitions */

/**
 *  Config structure magic number.
 *
 *  The AP writes this value and the R4 checks it to trap endian mismatches.
 */
#define MXCONF_MAGIC 0x79828486

/**
 *  Config structure version
 *
 *  The AP writes these values and the R4 checks them to trap config structure
 *  mismatches.
 */
#define MXCONF_VERSION_MAJOR 0
#define MXCONF_VERSION_MINOR 1

/* Macros */
#define GCC_PACKED __attribute__((packed))

/* Types */

/**
 * Maxwell Circular Packet Buffer Configuration.
 */
GCC_PACKED struct mxcbufconf {
	scsc_mifram_ref buffer_loc;		 /**< Location of allocated buffer in DRAM */
	uint32_t num_packets;			 /**< Total number of packets that can be stored in the buffer */
	uint32_t packet_size;			 /**< Size of each individual packet within the buffer */
	scsc_mifram_ref read_index_loc;	 /**< Location of 32bit read index in DRAM or Mailbox */
	scsc_mifram_ref write_index_loc; /**< Location of 32bit write index */
};

/**
 * Maxwell Management Simplex Stream Configuration
 *
 * A circular buffer plus a pair of R/W signaling bits.
 */
GCC_PACKED struct mxstreamconf {
	/** Circular Packet Buffer configuration */
	struct mxcbufconf buf_conf;

	/** Allocated MIF Interrupt Read Bit Index */
	uint8_t read_bit_idx;

	/** Allocated MIF Interrupt Write Bit Index */
	uint8_t write_bit_idx;
};

/**
 * Maxwell Management Transport Configuration
 *
 * A pair of simplex streams.
 */
GCC_PACKED struct mxtransconf {
	struct mxstreamconf to_ap_stream_conf;
	struct mxstreamconf from_ap_stream_conf;
};

/**
 * Maxwell Infrastructure Configuration Version
 */
GCC_PACKED struct mxconfversion {
	uint16_t major;
	uint16_t minor;
};

/**
 * Mxlog Event Buffer Configuration.
 *
 * A circular buffer. Size must be a multiple of 2.
 */
GCC_PACKED struct mxlogconf {
	struct mxstreamconf stream_conf;
};

/**
 * Maxwell Infrastructure Configuration
 */
GCC_PACKED struct mxconf {
	/**
	 * Config Magic Number
	 *
	 * Always 1st field in config.
	 */
	uint32_t magic;

	/**
	 * Config Version.
	 *
	 * Always second field in config.
	 */
	struct mxconfversion version;

	/**
	 * MX Management Message Transport Configuration.
	 */
	struct mxtransconf mx_trans_conf;

	/**
	 * MX Management GDB Message Transport Configuration.
	 */
	/* Cortex-R4 channel */
	struct mxtransconf mx_trans_conf_gdb_r4;
	/* Cortex-M4 channel */
	struct mxtransconf mx_trans_conf_gdb_m4;

#ifdef MX_LOG_PHASE_4
	/**
	 * Mxlog Event Buffer Configuration.
	 */
	struct mxlogconf mxlogconf;
#endif
};

#endif							/* MXCONF_H__ */
