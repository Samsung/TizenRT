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

/*********** Included Header Files **************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/binfmt/compression/compression.h>

#include <tinyara/zlib/zlib.h>
#include <tinyara/zlib/zconf.h>
#include <tinyara/lzma/LzmaLib.h>
#include "binary_header.h"

typedef int64_t off64_t;

/*********** Pre-processors *****************/

#define DIFF_SIGNATURE_SIZE		8	/* Size for Bsdiff Signature */
#define CRC_BLOCKSIZE			1024	/* Blocksize for CRC calculation of patch file */

#define COMP_TYPE_NONE			0	/* Same values as compression_formats enum */
#define COMP_TYPE_LZMA			1
#define COMP_TYPE_MAX			COMP_TYPE_LZMA

#define err_msg(...) { \
	printf("@ %s (%d): ", __FILE__, __LINE__); \
	printf(__VA_ARGS__); \
	printf("\n"); \
	exit(1); \
}

/************** Private Types ***************/

/* token for block_diff_header */
enum token {
	SKIP = 0,					/* No diff b/w old & new block, both are identical */
	MODIFY,						/* Diff b/w old & new block */
	ADD,						/* Block present only in new file */
	REMOVE,						/* Block no longer present in new file */
};

struct patch_header {
	unsigned int crc_patchfile;	/* crc checksum to verify integrity of patch file at patch side */
	char signature[DIFF_SIGNATURE_SIZE];	/* Bsdiff signature */
	int old_file_compression_format;	/* Compression Format for file to be patched on device side */
	unsigned int blocks_newfile;	/* No. of sections in new file */
	unsigned int blocks_oldfile;	/* No. of sections in old file */
	unsigned int blocksize_newfile;	/* Block size for new file (for uncompressed blocks) */
	unsigned int blocksize_oldfile;	/* Block size for old file (for uncompressed blocks) */
	unsigned int crc_newfile_size;	/* Size for checksum written at start of new file */
	unsigned int binary_header_size;	/* Binary Header size */
	unsigned int comp_header_size;	/* Compression Header size */
} __attribute__((packed));

struct block_diff_header {
	unsigned int token;			/* Token (Skip, Modify, Add, Remove) */
	unsigned int block_number;	/* Block number */
	off64_t size_ctrl_block;	/* Size of ctrl block */
	off64_t size_diff_block;	/* Size of diff block */
	off64_t size_extra_block;	/* Size of extra block */
	off64_t new_block_size_c;	/* Size of compressed new block */
	off64_t new_block_size_u;	/* Size of uncompressed new block */
	unsigned int new_block_crc;	/* CRC of new block (comp./uncomp.) to be verified on patch side */
} __attribute__((packed));

/* Struct for info regarding Old Uncompressed file */
struct old_uncomp_file {
	FILE *oufile;
	int fd;						/* File Descriptor */
	off64_t total_size;			/* Total size */
	off64_t size_for_blocks_calc;	/* Size without CRC and Bin. Header */
	unsigned int crc;			/* CRC read from file */
	binary_header_t binheader;	/* Binary Header */
} __attribute__((packed));

typedef struct old_uncomp_file old_uncomp_file;

/* Struct for info regarding New Uncompressed file */
struct new_uncomp_file {
	FILE *nufile;
	int fd;						/* File Descriptor */
	off64_t total_size;			/* Total size */
	off64_t size_for_blocks_calc;	/* Size without CRC and Bin. Header */
	unsigned int crc;			/* CRC read from file */
	binary_header_t binheader;	/* Binary Header */
} __attribute__((packed));

typedef struct new_uncomp_file new_uncomp_file;

/* Struct for info regarding New Compressed file */
struct new_comp_file {
	FILE *ncfile;				/* File Descriptor */
	int fd;
	off64_t total_size;			/* Total size */
	unsigned int crc;			/* CRC read from file */
	int compheader_size;		/* Size of Compression Header */
	binary_header_t binheader;	/* Binary Header */
	struct s_header *compheader;	/* Compression Header Struct */
} __attribute__((packed));

typedef struct new_comp_file new_comp_file;

/* Struct for info regarding Old Compressed file */
struct old_comp_file {
	FILE *ocfile;				/* File Descriptor */
	int fd;
	off64_t total_size;			/* Total size */
	unsigned int crc;			/* CRC read from file */
	int compheader_size;		/* Size of Compression Header */
	binary_header_t binheader;	/* Binary Header */
	struct s_header *compheader;	/* Compression Header Struct */
} __attribute__((packed));

typedef struct old_comp_file old_comp_file;

/* Struct for info regarding Patch file */
struct patch_file {
	FILE *pfile;				/* Patch file */
	gzFile pfgz;				/* GZIP file descriptor for patch file */
	unsigned int total_size;	/* Total size */
	unsigned int size_for_crc_calc;	/* Size without CRC */
	unsigned int crc;			/* CRC for file */
} __attribute__((packed));

typedef struct patch_file patch_file;

/*************** Public variables ******************/

static char patch_signature[] = "BLCKDIFF";	/* Signature to verify this is a Patch File */

/* Function declarations */

int blockwise_bin_update_main(int argc, char *argv[]);
