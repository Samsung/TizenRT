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

/*-
 * Copyright 2003-2005 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if 0
__FBSDID("$FreeBSD: src/usr.bin/bsdiff/bspatch/bspatch.c,v 1.1 2005/08/06 01:59:06 cperciva Exp $");
#endif

/**************** Included Header Files  *******************/

#include <apps/system/diff.h>

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

/****************** Private Types ************************/

/* Struct for buffers */
struct buffers {
	u_char **buf_oldblock_c_backup;	/* Buffer for backup of compressed blocks from old comp. file */
	u_char *buf_newblock_c;		/* Buffer to hold new patched compressed block */
	u_char *buf_newblock_u;		/* Buffer to hold new patched uncompressed block */
	u_char *buf_oldblock_u;		/* Buffer to hold old uncompressed block */
	u_char **buf_oldblock_u_backup;	/* Buffer for backup of uncompressed blocks from old uncomp. file */
	bool *block_patch_status;	/* Status for which blocks in old file have been patched */
} __attribute__((packed));

typedef struct buffers buffers;

typedef gzFile stream_t;

/* Compatibility layer for reading BLCKDIFF patch format */
typedef struct {
	stream_t(*open)(FILE *);
	void (*close)(stream_t);
	int (*read)(stream_t, void *, int);
} io_funcs_t;

#ifndef u_char
typedef unsigned char u_char;
#endif

/****************** Private data *************************/

old_uncomp_file ouf;
new_uncomp_file nuf;
new_comp_file ncf;
old_comp_file ocf;
patch_file pf;

buffers bufs;

struct patch_header patchheader;	/* Header for output patch file */
struct block_diff_header blockheader;	/* Header info for diff b/w 2 blocks */

/**************** Function Definitions ********************/

static stream_t BLCKDIFF_open(FILE *f)
{
	gzFile gzf;

	if ((gzf = gzdopen(dup(fileno(f)), "r")) == NULL) {
		err_msg("gzopen, errno = %d", errno);
	}

	return gzf;
}

static void BLCKDIFF_close(stream_t s)
{
	int rc;

	rc = gzclose(s);
	if (rc != Z_OK) {
		err_msg("gzclose, gzerr = %d", rc);
	}
}

static int BLCKDIFF_read(stream_t s, void *buf, int len)
{

	return gzfread(buf, 1, len, s);
}

static io_funcs_t BLCKDIFF_funcs = {
	BLCKDIFF_open,
	BLCKDIFF_close,
	BLCKDIFF_read
};

static off64_t offtin(u_char *buf)
{
	off64_t y;

	y = buf[7] & 0x7F;
	y = y * 256;
	y += buf[6];
	y = y * 256;
	y += buf[5];
	y = y * 256;
	y += buf[4];
	y = y * 256;
	y += buf[3];
	y = y * 256;
	y += buf[2];
	y = y * 256;
	y += buf[1];
	y = y * 256;
	y += buf[0];

	if (buf[7] & 0x80) {
		y = -y;
	}

	return y;
}

/* Find out which blocks in old uncompressed file are overwritten during this fwrite.
   Take backup for those blocks. Then, fwrite to file.
*/
uint64_t fwrite_inplace_uncompressed_file(const void *ptr, uint64_t size, uint64_t nmemb, FILE *stream)
{
	unsigned int first_block_overwritten;	/* First block overwritten in oldfile during this fwrite; First block index = 1 */
	unsigned int last_block_overwritten;	/* Last block overwritten in oldfile during this fwrite */
	unsigned int start_offset, end_offset;	/* Start and end offset for fwrite to oldfile in-place */
	unsigned int offset_to_first_block_of;	/* Offset to first block in old uncompressed file */
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	uint64_t nmemb_ret;			/* Return value */
	int i;

	start_offset = ftell(stream);
	end_offset = start_offset + (size * nmemb) - 1;

	/* Basic sanity check */
	if (start_offset < 0 || end_offset < start_offset) {
		err_msg("Offset and Size for this fwrite to oldfile should be positive");
	}

	/* Calculate offset to first block for old file */
	offset_to_first_block_of = sizeof(ouf.crc) + sizeof(ouf.binheader);

	/* If no blocks overwritten, skip backup */
	if (end_offset < offset_to_first_block_of || start_offset >= ouf.total_size) {
		goto gt_fwrite_ouf;
	}

	/* Find out first block that will be overwritten */
	for (i = 1; i <= patchheader.blocks_oldfile; i++) {
		if (start_offset < offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA)) {
			break;
		}
	}

	first_block_overwritten = i - 1;

	/* Find out last block that will be overwritten */
	for (i = 1; i <= patchheader.blocks_oldfile; i++) {
		if (end_offset < offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA)) {
			break;
		}
	}

	last_block_overwritten = (i == patchheader.blocks_oldfile + 1 ? patchheader.blocks_oldfile - 1 : i - 1);

	/* Take backup for these blocks, if needed */
	for (i = first_block_overwritten; i <= last_block_overwritten; i++) {

		/* If backup of this block is not already taken & this block has not been patched */
		if (bufs.buf_oldblock_u_backup[i] == NULL && bufs.block_patch_status[i] == false) {
			block_start = offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
			if (i != patchheader.blocks_oldfile - 1) {
				block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
			} else {
				block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
			}

			/* Malloc for buf_oldblock_c[i] for backup for this block */
			if ((bufs.buf_oldblock_u_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 1))) == NULL) {
				err_msg("Malloc failed for Buffer needed for backup of block %d", i);
			}

			/* Seek to position on old file from where to backup this block */
			if (fseek(stream, block_start, SEEK_SET)) {
				err_msg("Failed to seek to position %d in old file", block_start);
			}

			/* Copy block to buffer */
			if (fread(bufs.buf_oldblock_u_backup[i], block_size, 1, stream) != 1) {
				err_msg("Failed to read block %d from old file", i);
			}
		}
	}

gt_fwrite_ouf:

	/* Seek back to position in old file from where to fwrite */
	if (fseek(stream, start_offset, SEEK_SET)) {
		err_msg("Failed to seek to position %d in old file", start_offset);
	}

	/* Fwrite to old file */
	nmemb_ret = fwrite(ptr, size, nmemb, stream);

	return nmemb_ret;

}

uint64_t fwrite_to_new_uncompressed_file(const void *ptr, uint64_t size, uint64_t nmemb, FILE *stream)
{
	uint64_t nmemb_ret;			/* Return value */

	/* Fwrite to old file */
	nmemb_ret = fwrite(ptr, size, nmemb, stream);

	return nmemb_ret;
}

/* Use info of Compression header for old compressed file to find out which blocks in
   old compressed file are overwritten during this fwrite. Take backup for those blocks.
   Then, fwrite to file.
*/
uint64_t fwrite_inplace_compressed_file(const void *ptr, uint64_t size, uint64_t nmemb, FILE *stream)
{
	unsigned int first_block_overwritten;	/* First block overwritten in oldfile during this fwrite; First block index = 1 */
	unsigned int last_block_overwritten;	/* Last block overwritten in oldfile during this fwrite */
	unsigned int start_offset, end_offset;	/* Start and end offset for fwrite to oldfile in-place */
	unsigned int offset_to_first_block_of;	/* Offset to first block in old compressed file */
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	uint64_t nmemb_ret;			/* Return value */
	int i;

	start_offset = ftell(stream);
	end_offset = start_offset + (size * nmemb) - 1;

	/* Basic sanity check */
	if (start_offset < 0 || end_offset < start_offset) {
		err_msg("Offset and Size for this fwrite to oldfile should be positive");
	}

	/* Calculate offset to first block for old file */
	offset_to_first_block_of = sizeof(ocf.crc) + sizeof(ocf.binheader) + ocf.compheader->size_header;

	/* If no blocks overwritten, skip backup */
	if (end_offset < offset_to_first_block_of || start_offset >= offset_to_first_block_of + ocf.compheader->secoff[ocf.compheader->sections - 1]) {
		goto gt_fwrite_ocf;
	}

	/* Find out first block that will be overwritten */
	for (i = 1; i < ocf.compheader->sections; i++) {
		if (start_offset < offset_to_first_block_of + ocf.compheader->secoff[i]) {
			break;
		}
	}

	first_block_overwritten = i - 1;

	/* Find out last block that will be overwritten */
	for (i = 1; i < ocf.compheader->sections; i++) {
		if (end_offset < offset_to_first_block_of + ocf.compheader->secoff[i]) {
			break;
		}
	}

	last_block_overwritten = (i == ocf.compheader->sections ? ocf.compheader->sections - 2 : i - 1);

	/* Take backup for these blocks, if needed */
	for (i = first_block_overwritten; i <= last_block_overwritten; i++) {

		/* If backup of this block is not already taken & this block has not been patched */
		if (bufs.buf_oldblock_c_backup[i] == NULL && bufs.block_patch_status[i] == false) {
			block_start = offset_to_first_block_of + ocf.compheader->secoff[i];
			block_size = ocf.compheader->secoff[i + 1] - ocf.compheader->secoff[i];

			/* Malloc for buf_oldblock_c[i] for backup for this block */
			if ((bufs.buf_oldblock_c_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 1))) == NULL) {
				err_msg("Malloc failed for Buffer needed for backup of block %d", i);
			}

			/* Seek to position on old file from where to backup this block */
			if (fseek(stream, block_start, SEEK_SET)) {
				err_msg("Failed to seek to position %d in old file", block_start);
			}

			/* Copy block to buffer */
			if (fread(bufs.buf_oldblock_c_backup[i], block_size, 1, stream) != 1) {
				err_msg("Failed to read block %d from old file", i);
			}
		}
	}

gt_fwrite_ocf:

	/* Seek back to position in old file from where to fwrite */
	if (fseek(stream, start_offset, SEEK_SET)) {
		err_msg("Failed to seek to position %d in old file", start_offset);
	}

	/* Fwrite to old file */
	nmemb_ret = fwrite(ptr, size, nmemb, stream);

	return nmemb_ret;

}

uint64_t fwrite_to_new_compressed_file(const void *ptr, uint64_t size, uint64_t nmemb, FILE *stream)
{
	uint64_t nmemb_ret;			/* Return value */

	/* Fwrite to old file */
	nmemb_ret = fwrite(ptr, size, nmemb, stream);

	return nmemb_ret;

}

/* Verify CRC for Patch File */
void verify_patch_file_crc(char *pf_file)
{
	unsigned int pf_blocks;		/* Holds number of blocks in patch file, depending on CRC_BLOCKSIZE */
	unsigned int pf_crc;		/* Calculated CRC for patch file */
	char *pf_crc_buffer;		/* Buffer for reading data for CRC calculation of patch file */

	/* Read CRC from patch file */
	if (fread(&pf.crc, sizeof(pf.crc), 1, pf.pfile) != 1) {
		err_msg("Failed to read CRC from patch file");
	}

	/* Seek to the end of patch file to get patchfile size */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("fseek");
	}

	pf.total_size = ftell(pf.pfile);

	/* Seek to location of data from where to start CRC calculation of patch file */
	if (fseek(pf.pfile, sizeof(pf.crc), SEEK_SET)) {
		err_msg("fseek");
	}

	/* Calculate no. of blocks CRC_BLOCKSIZE divides patch file into */
	pf.size_for_crc_calc = pf.total_size - sizeof(pf.crc);
	pf_blocks = pf.size_for_crc_calc / CRC_BLOCKSIZE;
	if (pf.size_for_crc_calc % CRC_BLOCKSIZE) {
		pf_blocks++;
	}

	/* Malloc for buffer for reading data */
	if ((pf_crc_buffer = malloc((CRC_BLOCKSIZE + 1) * sizeof(char))) == NULL) {
		err_msg("malloc failed for pf_crc_buffer");
	}

	/* Compute crc block-by-block, using crc for previous block as base for current block CRC */
	pf_crc = 0;
	for (int cnt = 0; cnt < pf_blocks; cnt++) {
		if (cnt != pf_blocks - 1) {
			if (fread(pf_crc_buffer, CRC_BLOCKSIZE, 1, pf.pfile) != 1) {
				err_msg("Read failed for block %d for %s", cnt, pf_file);
			}

			pf_crc = crc32_z(pf_crc, (const Bytef *)pf_crc_buffer, CRC_BLOCKSIZE);
		} else {
			if (fread(pf_crc_buffer, (pf.size_for_crc_calc % CRC_BLOCKSIZE != 0 ? pf.size_for_crc_calc % CRC_BLOCKSIZE : CRC_BLOCKSIZE), 1, pf.pfile) != 1) {
				err_msg("Read failed for block %d for %s", cnt, pf_file);
			}

			pf_crc = crc32_z(pf_crc, (const Bytef *)pf_crc_buffer, (pf.size_for_crc_calc % CRC_BLOCKSIZE != 0 ? pf.size_for_crc_calc % CRC_BLOCKSIZE : CRC_BLOCKSIZE));
		}
	}

	/* CRC comparison for patch file */
	if (pf_crc != pf.crc) {
		err_msg("Corrupt patch file. CRC check failed for %s", pf_file);
	}
}

/* Read patch header, Verify Signature and Compression Format values, Assign io structure */
void parse_patch_header(char *pf_file, io_funcs_t **io)
{
	/* Seek to start of patch file */
	if (fseek(pf.pfile, 0, SEEK_SET)) {
		err_msg("fseek");
	}

	/* Read Patch Header */
	if (fread(&patchheader, sizeof(patchheader), 1, pf.pfile) != 1) {
		err_msg("Failed to read patch header from %s", pf_file);
	}

	/* Verify Bsdiff signature & assign (io_funcs_t *)io */
	if (memcmp(&patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE) == 0) {
		*io = &BLCKDIFF_funcs;
	} else {
		err_msg("Bsdiff signature does not match. %s is not a Bsdiff generated patch file", pf_file);
	}

	/* Check if "patchheader.old_file_compression_format" value is in valid range */
	if (patchheader.old_file_compression_format < COMPRESSION_TYPE_NONE || patchheader.old_file_compression_format > COMPRESSION_TYPE_MAX) {
		err_msg("Patchheader's old_file_compression_format value (%d) is not in valid range", patchheader.old_file_compression_format);
	}
}

/* Read CRC, Binheader, Compresssion Header for New Compressed File from patch file */
void parse_metadata_for_new_comp(char *pf_file)
{
	/* Read CRC */
	if (fread(&ncf.crc, patchheader.crc_newfile_size, 1, pf.pfile) != 1) {
		err_msg("Failed to read CRC for new file from %s", pf_file);
	}

	/* Read Binary Header */
	if (fread(&ncf.binheader, patchheader.binary_header_size, 1, pf.pfile) != 1) {
		err_msg("Failed to read Binary Header for new file from %s", pf_file);
	}

	/* Malloc for compression header */
	if ((ncf.compheader = malloc(patchheader.comp_header_size)) == NULL) {
		err_msg("malloc failed for compheader for new file");
	}

	/* Read Compression Header */
	if (fread(ncf.compheader, patchheader.comp_header_size, 1, pf.pfile) != 1) {
		err_msg("Read failed for compheader for new file from %s", pf_file);
	}
}

/* Read CRC, Binheader for New UnCompressed File from patch file */
void parse_metadata_for_new_uncomp(char *pf_file)
{
	/* Read CRC for New UnCompressed file */
	if (fread(&nuf.crc, patchheader.crc_newfile_size, 1, pf.pfile) != 1) {
		err_msg("Failed to read CRC for new file from %s", pf_file);
	}

	/* Read Binary Header for New UnCompressed file */
	if (fread(&nuf.binheader, patchheader.binary_header_size, 1, pf.pfile) != 1) {
		err_msg("Failed to read Binary Header for new file from %s", pf_file);
	}
}

/* Open and Parse metadata from old compressed file */
void parse_old_comp_file(char *oc_file)
{
	/* Opening old compressed file */
	if ((ocf.ocfile = fopen(oc_file, "r+")) == NULL) {
		err_msg("fopen(%s)", oc_file);
	}

	/* Read checksum and binary header */
	if (fread(&ocf.crc, sizeof(ocf.crc), 1, ocf.ocfile) != 1) {
		err_msg("Failed to read CRC from %s", oc_file);
	}

	if (fread(&ocf.binheader, sizeof(ocf.binheader), 1, ocf.ocfile) != 1) {
		err_msg("Failed to read binary header from %s", oc_file);
	}

	/* Read size of Compression header */
	if (fread(&ocf.compheader_size, sizeof(ocf.compheader_size), 1, ocf.ocfile) != 1) {
		err_msg("Failed to get compression header size from %s", oc_file);
	}

	/* Seek to location of start of Compression header */
	if (fseek(ocf.ocfile, -sizeof(ocf.compheader_size), SEEK_CUR)) {
		err_msg("Failed to seek to start of Compression header in %s", oc_file);
	}

	/* Malloc for compression header for old file */
	if ((ocf.compheader = malloc(ocf.compheader_size)) == NULL) {
		err_msg("Failed malloc for Compression header for %s", oc_file);
	}

	/* Read compression header from old file */
	if (fread(ocf.compheader, ocf.compheader_size, 1, ocf.ocfile) != 1) {
		err_msg("Failed to read Compression header from %s", oc_file);
	}
}

/* Open and Parse metadata from old uncompressed file */
void parse_old_uncomp_file(char *ou_file)
{
	/* Opening old uncompressed file */
	if ((ouf.oufile = fopen(ou_file, "r+")) == NULL) {
		err_msg("fopen(%s)", ou_file);
	}

	/* Seek to end of old uncompressed file */
	if (fseek(ouf.oufile, 0, SEEK_END)) {
		err_msg("fseek");
	}

	/* Total size of old uncompressed file */
	ouf.total_size = ftell(ouf.oufile);
	ouf.size_for_blocks_calc = ouf.total_size - sizeof(ouf.crc) - sizeof(ouf.binheader);

	/* Seek to start of old uncompressed file */
	if (fseek(ouf.oufile, 0, SEEK_SET)) {
		err_msg("fseek");
	}

	/* Read checksum and binary header from old uncompressed file */
	if (fread(&ouf.crc, sizeof(ouf.crc), 1, ouf.oufile) != 1) {
		err_msg("Failed to read CRC from %s", ou_file);
	}

	if (fread(&ouf.binheader, sizeof(ouf.binheader), 1, ouf.oufile) != 1) {
		err_msg("Failed to read binary header from %s", ou_file);
	}
}

void open_new_uncompressed_file(char *new_u_file)
{
	/* Opening old uncompressed file */
	if ((nuf.nufile = fopen(new_u_file, "r+")) == NULL) {
		err_msg("fopen(%s)", new_u_file);
	}
}

void open_new_compressed_file(char *new_c_file)
{
	/* Opening old uncompressed file */
	if ((ncf.ncfile = fopen(new_c_file, "r+")) == NULL) {
		err_msg("fopen(%s)", new_c_file);
	}
}

/* Verify conditions for patching (compression blocksize and compression format) */
void verify_patching_conditions(void)
{
	/* Check if blocksize is same for old compressed file and new compressed file */
	if (ncf.compheader->blocksize != ocf.compheader->blocksize)
		err_msg("Blocksize chosen for compression in new compressed file is not \
			same as that of old compressed file. This case is not handled in our patching algorithm");

	/* Check if Compression format is same for old compressed file and new compressed file */
	if (ncf.compheader->compression_format != ocf.compheader->compression_format)
		err_msg("Compression format chosen for new compressed file is not same as that of old \
					compressed file. This case is not handled in our patching algorithm");
}

/* Malloc for buffers for patching old compressed file */
void malloc_buffers_for_old_comp(void)
{
	/* malloc for buffer which holds compressed blocks for oldfile in RAM */
	if ((bufs.buf_oldblock_c_backup = (u_char **) malloc((patchheader.blocks_oldfile) * sizeof(u_char *))) == NULL) {
		err_msg("Failed malloc for buffer which holds old compressed blocks");
	}

	memset(bufs.buf_oldblock_c_backup, 0, (patchheader.blocks_oldfile) * sizeof(u_char *));

	/* malloc for buffer which holds block for oldfile after uncompression */
	if ((bufs.buf_oldblock_u = malloc((ocf.compheader->blocksize + 5) * sizeof(u_char))) == NULL) {
		err_msg("Failed malloc for buffer which holds old uncompressed blocks");
	}

	/* malloc for buffer which holds patched block in  uncompressed form */
	if ((bufs.buf_newblock_u = malloc(sizeof(u_char) * (ncf.compheader->blocksize + 5))) == NULL) {
		err_msg("Failed malloc for buffer which holds new uncompressed blocks");
	}

	/* malloc for buffer which holds patched block in compressed form */
	if ((bufs.buf_newblock_c = malloc(sizeof(u_char) * (ncf.compheader->blocksize + 5))) == NULL) {
		err_msg("Failed malloc for buffer which holds new compressed blocks");
	}

	/* malloc for buffer which holds status for which blocks in old file have been patched */
	if ((bufs.block_patch_status = malloc(sizeof(bool) * (patchheader.blocks_oldfile))) == NULL) {
		err_msg("Failed malloc for buffer which holds status for blocks patched/unpatched");
	}

	/* Initializing patched status for all blocks to false */
	for (int k = 0; k < MIN(patchheader.blocks_oldfile, patchheader.blocks_newfile); k++) {
		bufs.block_patch_status[k] = false;
	}
}

/* Malloc for buffers for patching old uncompressed file */
void malloc_buffers_for_old_uncomp(void)
{
	/* malloc for buffer which holds compressed blocks for oldfile in RAM */
	if ((bufs.buf_oldblock_u_backup = (u_char **) malloc((patchheader.blocks_oldfile) * sizeof(u_char *))) == NULL) {
		err_msg("Failed malloc for buffer which holds old uncompressed blocks");
	}

	memset(bufs.buf_oldblock_u_backup, 0, (patchheader.blocks_oldfile) * sizeof(u_char *));

	/* malloc for buffer which holds patched block in uncompressed form */
	if ((bufs.buf_newblock_u = malloc(sizeof(u_char) * (CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 5))) == NULL) {
		err_msg("Failed malloc for buffer which holds new uncompressed blocks");
	}

	/* malloc for buffer which holds status for which blocks in old file have been patched */
	if ((bufs.block_patch_status = malloc(sizeof(bool) * (patchheader.blocks_oldfile))) == NULL) {
		err_msg("Failed malloc for buffer which holds status for blocks patched/unpatched");
	}

	/* Initializing patched status for all blocks to false */
	for (int k = 0; k < MIN(patchheader.blocks_oldfile, patchheader.blocks_newfile); k++) {
		bufs.block_patch_status[k] = false;
	}
}

/* Writing new CRC, Binary Header and Comp. header to old comp. file */
void write_new_metadata_to_old_comp(void)
{
	/* Seek to start of old file */
	if (fseek(ocf.ocfile, 0, SEEK_SET)) {
		err_msg("Failed to seek to start of old file");
	}

	/* Write CRC for new file to old file in-place */
	if (fwrite_inplace_compressed_file(&ncf.crc, patchheader.crc_newfile_size, 1, ocf.ocfile) != 1) {
		err_msg("Failed to write CRC for new file to old file in-place");
	}

	/* Write Binary Header for new file to old file in-place */
	if (fwrite_inplace_compressed_file(&ncf.binheader, patchheader.binary_header_size, 1, ocf.ocfile) != 1) {
		err_msg("Failed to write binary header for new file to old file in-place");
	}

	/* Write Compression Header for new file to old file in-place */
	if (fwrite_inplace_compressed_file(ncf.compheader, patchheader.comp_header_size, 1, ocf.ocfile) != 1) {
		err_msg("Failed to write compression header for new file to old file in-place");
	}
}

void write_new_metadata_to_new_comp(void)
{
	/* Seek to start of old file */
	if (fseek(ncf.ncfile, 0, SEEK_SET)) {
		err_msg("Failed to seek to start of new file");
	}

	/* Write CRC for new file to old file in-place */
	if (fwrite_to_new_compressed_file(&ncf.crc, patchheader.crc_newfile_size, 1, ncf.ncfile) != 1) {
		err_msg("Failed to write CRC for new file to old file in-place");
	}

	/* Write Binary Header for new file to old file in-place */
	if (fwrite_to_new_compressed_file(&ncf.binheader, patchheader.binary_header_size, 1, ncf.ncfile) != 1) {
		err_msg("Failed to write binary header for new file to old file in-place");
	}

	/* Write Compression Header for new file to old file in-place */
	if (fwrite_to_new_compressed_file(ncf.compheader, patchheader.comp_header_size, 1, ncf.ncfile) != 1) {
		err_msg("Failed to write compression header for new file to old file in-place");
	}
}

/* Writing new CRC, Binary Header and Comp. header to old uncomp. file */
void write_new_metadata_to_old_uncomp(void)
{
	/* Seek to start of old file */
	if (fseek(ouf.oufile, 0, SEEK_SET)) {
		err_msg("Failed to seek to start of old file");
	}

	/* Write CRC for new file to old file in-place */
	if (fwrite_inplace_uncompressed_file(&nuf.crc, patchheader.crc_newfile_size, 1, ouf.oufile) != 1) {
		err_msg("Failed to write CRC for new file to old file in-place");
	}

	/* Write Binary Header for new file to old file in-place */
	if (fwrite_inplace_uncompressed_file(&nuf.binheader, patchheader.binary_header_size, 1, ouf.oufile) != 1) {
		err_msg("Failed to write binary header for new file to old file in-place");
	}
}

void write_new_metadata_to_new_uncomp(void)
{
	/* Seek to start of new file */
	if (fseek(nuf.nufile, 0, SEEK_SET)) {
		err_msg("Failed to seek to start of new file");
	}

	/* Write CRC for new file to new file */
	if (fwrite_to_new_uncompressed_file(&nuf.crc, patchheader.crc_newfile_size, 1, nuf.nufile) != 1) {
		err_msg("Failed to write CRC for new file to old file in-place");
	}

	/* Write Binary Header for new file to new file */
	if (fwrite_to_new_uncompressed_file(&nuf.binheader, patchheader.binary_header_size, 1, nuf.nufile) != 1) {
		err_msg("Failed to write binary header for new file to new file");
	}
}

/* Patch Block with Token == SKIP for Old Compressed File */
void patch_block_token_skip_old_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf)
{
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */

	block_crc = 0;

	/* Calculate block size for current block in old file */
	block_size = ocf.compheader->secoff[i + 1] - ocf.compheader->secoff[i];

	/* Read Compressed block from old file, if no backup */
	if (bufs.buf_oldblock_c_backup[i] == NULL) {
		block_start = offset_to_first_block_of + ocf.compheader->secoff[i];

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_c_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ocf.ocfile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_c_backup[i], block_size, 1, ocf.ocfile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Verify new block size */
	if (block_size != blockheader.new_block_size_c) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *)bufs.buf_oldblock_c_backup[i], block_size);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ocf.ocfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write the block back to old file */
	if (fwrite_inplace_compressed_file(bufs.buf_oldblock_c_backup[i], block_size, 1, ocf.ocfile) != 1) {
		err_msg("Failed to write patched %d block to oldfile", i);
	}

	/* Free old compressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_c_backup[i] != NULL) {
		free(bufs.buf_oldblock_c_backup[i]);
		bufs.buf_oldblock_c_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

void patch_block_token_skip_old_comp_to_new_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf)
{
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */

	block_crc = 0;

	/* Calculate block size for current block in old file */
	block_size = ocf.compheader->secoff[i + 1] - ocf.compheader->secoff[i];

	/* Read Compressed block from old file */
	if (bufs.buf_oldblock_c_backup[i] == NULL) {
		block_start = offset_to_first_block_of + ocf.compheader->secoff[i];

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_c_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ocf.ocfile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_c_backup[i], block_size, 1, ocf.ocfile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Verify new block size */
	if (block_size != blockheader.new_block_size_c) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *)bufs.buf_oldblock_c_backup[i], block_size);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in new file */
	if (fseek(ncf.ncfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write the block back to old file */
	if (fwrite_to_new_compressed_file(bufs.buf_oldblock_c_backup[i], block_size, 1, ncf.ncfile) != 1) {
		err_msg("Failed to write patched %d block to oldfile", i);
	}

	/* Free old compressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_c_backup[i] != NULL) {
		free(bufs.buf_oldblock_c_backup[i]);
		bufs.buf_oldblock_c_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

/* Patch Block with Token == SKIP for Old UnCompressed File */
void patch_block_token_skip_old_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, int no_common_blocks)
{
	unsigned int block_start;	/* Used when taking backup of block */
	unsigned int block_size;	/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1) {
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	}

	/* Read unCompressed block from old file, if no backup */
	if (bufs.buf_oldblock_u_backup[i] == NULL) {
		block_start = offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_u_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ouf.oufile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_u_backup[i], block_size, 1, ouf.oufile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Verify new block size */
	if (block_size != blockheader.new_block_size_u) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *)bufs.buf_oldblock_u_backup[i], block_size);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ouf.oufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write the block back to old file */
	if (fwrite_inplace_uncompressed_file(bufs.buf_oldblock_u_backup[i], block_size, 1, ouf.oufile) != 1) {
		err_msg("Failed to write patched %d block to oldfile", i);
	}

	/* Free old uncompressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_u_backup[i] != NULL) {
		free(bufs.buf_oldblock_u_backup[i]);
		bufs.buf_oldblock_u_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

void patch_block_token_skip_old_uncomp_to_new_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, int no_common_blocks)
{
	unsigned int block_start;	/* Used when taking backup of block */
	unsigned int block_size;	/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1) {
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	}

	/* Read unCompressed block from old file */
	if (bufs.buf_oldblock_u_backup[i] == NULL) {
		block_start = offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_u_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ouf.oufile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_u_backup[i], block_size, 1, ouf.oufile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Verify new block size */
	if (block_size != blockheader.new_block_size_u) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *)bufs.buf_oldblock_u_backup[i], block_size);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in new file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write the block back to old file */
	if (fwrite_to_new_uncompressed_file(bufs.buf_oldblock_u_backup[i], block_size, 1, nuf.nufile) != 1) {
		err_msg("Failed to write patched %d block to oldfile", i);
	}

	/* Free old uncompressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_u_backup[i] != NULL) {
		free(bufs.buf_oldblock_u_backup[i]);
		bufs.buf_oldblock_u_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

/* Patch Block with Token == MODIFY for old compressed file */
void patch_block_token_modify_old_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_start;	/* Used when taking backup of block */
	unsigned int block_size;	/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	unsigned int writesize, readsize;	/* Used for LZMA decompression */
	unsigned int propsSize = LZMA_PROPS_SIZE;
	int pfpos;
	int ret;

	/* Bspatch variables */
	FILE *cpf, *dpf, *epf;		/* Patch file streams */
	stream_t cstream, dstream, estream;
	int64_t oldsize, newsize;
	u_char buf[8];
	u_char *old, *new;
	off64_t oldpos, newpos;
	off64_t ctrl[3];
	off64_t lenread;
	off64_t j = 0;

	block_crc = 0;

	/* Calculate block size for current block in old file */
	block_size = ocf.compheader->secoff[i + 1] - ocf.compheader->secoff[i];

	/* Read Compressed block from old file, if needed (no backup already) */
	if (bufs.buf_oldblock_c_backup[i] == NULL) {
		block_start = offset_to_first_block_of + ocf.compheader->secoff[i];

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_c_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ocf.ocfile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_c_backup[i], block_size, 1, ocf.ocfile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Uncompress compressed block read from old file */
	writesize = ocf.compheader->blocksize;
	readsize = block_size - LZMA_PROPS_SIZE;
	ret = LzmaUncompress(bufs.buf_oldblock_u, &writesize, &bufs.buf_oldblock_c_backup[i][LZMA_PROPS_SIZE], &readsize, bufs.buf_oldblock_c_backup[i], LZMA_PROPS_SIZE);
	if (ret == SZ_ERROR_FAIL) {
		err_msg("LZMA Uncompress failed for block %d, ret = %d", i, ret);
	}

	/* Patch old block using diff data */

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((cpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open cpf stream");
	}
	if (fseek(cpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for CPF");
	}
	cstream = io->open(cpf);

	if (fseek(pf.pfile, blockheader.size_ctrl_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of diff section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((dpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open dpf stream");
	}
	if (fseek(dpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for DPF");
	}
	dstream = io->open(dpf);

	if (fseek(pf.pfile, blockheader.size_diff_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of extra section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Bspatch */
	oldpos = 0;
	newpos = 0;
	newsize = blockheader.new_block_size_u;
	oldsize = writesize;
	new = bufs.buf_newblock_u;
	old = bufs.buf_oldblock_u;
	while (newpos < newsize) {
		/* Read control data */
		for (j = 0; j <= 2; j++) {
			lenread = io->read(cstream, buf, 8);
			if (lenread < 8) {
				err_msg("Corrupt patch");
			}
			ctrl[j] = offtin(buf);
		};

		/* Sanity-check */
		if (newpos + ctrl[0] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read diff string */
		lenread = io->read(dstream, new + newpos, ctrl[0]);
		if (lenread < ctrl[0]) {
			err_msg("Corrupt patch");
		}

		/* Add old data to diff string */
		for (j = 0; j < ctrl[0]; j++)
			if ((oldpos + j >= 0) && (oldpos + j < oldsize)) {
				new[newpos + j] += old[oldpos + j];
			}

		/* Adjust pointers */
		newpos += ctrl[0];
		oldpos += ctrl[0];

		/* Sanity-check */
		if (newpos + ctrl[1] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read extra string */
		lenread = io->read(estream, new + newpos, ctrl[1]);
		if (lenread < ctrl[1]) {
			err_msg("Corrupt patch");
		}

		/* Adjust pointers */
		newpos += ctrl[1];
		oldpos += ctrl[2];
	};

	/* Close opened streams */
	io->close(cstream);
	io->close(dstream);
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}
	if (fclose(dpf)) {
		err_msg("Failed to close dpf");
	}
	if (fclose(cpf)) {
		err_msg("Failed to close cpf");
	}

	/* Compress patched block */
	writesize = ncf.compheader->blocksize;
	readsize = blockheader.new_block_size_u;
	ret = LzmaCompress(&bufs.buf_newblock_c[LZMA_PROPS_SIZE], &writesize, bufs.buf_newblock_u, readsize, bufs.buf_newblock_c, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
	if (ret != SZ_OK) {
		err_msg("LZMA Compress failed for block %d", i);
	}

	/* Verify new compressed block size */
	if (writesize != blockheader.new_block_size_c) {
		err_msg("Compressed block size(%lu) does not match size(%ld) info from blockheader for %d block", writesize, blockheader.new_block_size_c, i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_c, writesize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ocf.ocfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_inplace_compressed_file(bufs.buf_newblock_c, writesize, 1, ocf.ocfile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}

	/* Free old compressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_c_backup[i] != NULL) {
		free(bufs.buf_oldblock_c_backup[i]);
		bufs.buf_oldblock_c_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

void patch_block_token_modify_old_comp_to_new_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_start;	/* Used when taking backup of block */
	unsigned int block_size;	/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	unsigned int writesize, readsize;	/* Used for LZMA decompression */
	unsigned int propsSize = LZMA_PROPS_SIZE;
	int pfpos;
	int ret;

	/* Bspatch variables */
	FILE *cpf, *dpf, *epf;		/* Patch file streams */
	stream_t cstream, dstream, estream;
	int64_t oldsize, newsize;
	u_char buf[8];
	u_char *old, *new;
	off64_t oldpos, newpos;
	off64_t ctrl[3];
	off64_t lenread;
	off64_t j = 0;

	block_crc = 0;

	/* Calculate block size for current block in old file */
	block_size = ocf.compheader->secoff[i + 1] - ocf.compheader->secoff[i];

	/* Read Compressed block from old file */
	if (bufs.buf_oldblock_c_backup[i] == NULL) {
		block_start = offset_to_first_block_of + ocf.compheader->secoff[i];

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_c_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ocf.ocfile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_c_backup[i], block_size, 1, ocf.ocfile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Uncompress compressed block read from old file */
	writesize = ocf.compheader->blocksize;
	readsize = block_size - LZMA_PROPS_SIZE;
	ret = LzmaUncompress(bufs.buf_oldblock_u, &writesize, &bufs.buf_oldblock_c_backup[i][LZMA_PROPS_SIZE], &readsize, bufs.buf_oldblock_c_backup[i], LZMA_PROPS_SIZE);
	if (ret == SZ_ERROR_FAIL) {
		err_msg("LZMA Uncompress failed for block %d, ret = %d", i, ret);
	}

	/* Patch old block using diff data */

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((cpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open cpf stream");
	}
	if (fseek(cpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for CPF");
	}
	cstream = io->open(cpf);

	if (fseek(pf.pfile, blockheader.size_ctrl_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of diff section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((dpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open dpf stream");
	}
	if (fseek(dpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for DPF");
	}
	dstream = io->open(dpf);

	if (fseek(pf.pfile, blockheader.size_diff_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of extra section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Bspatch */
	oldpos = 0;
	newpos = 0;
	newsize = blockheader.new_block_size_u;
	oldsize = writesize;
	new = bufs.buf_newblock_u;
	old = bufs.buf_oldblock_u;
	while (newpos < newsize) {
		/* Read control data */
		for (j = 0; j <= 2; j++) {
			lenread = io->read(cstream, buf, 8);
			if (lenread < 8) {
				err_msg("Corrupt patch");
			}
			ctrl[j] = offtin(buf);
		};

		/* Sanity-check */
		if (newpos + ctrl[0] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read diff string */
		lenread = io->read(dstream, new + newpos, ctrl[0]);
		if (lenread < ctrl[0]) {
			err_msg("Corrupt patch");
		}

		/* Add old data to diff string */
		for (j = 0; j < ctrl[0]; j++)
			if ((oldpos + j >= 0) && (oldpos + j < oldsize)) {
				new[newpos + j] += old[oldpos + j];
			}

		/* Adjust pointers */
		newpos += ctrl[0];
		oldpos += ctrl[0];

		/* Sanity-check */
		if (newpos + ctrl[1] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read extra string */
		lenread = io->read(estream, new + newpos, ctrl[1]);
		if (lenread < ctrl[1]) {
			err_msg("Corrupt patch");
		}

		/* Adjust pointers */
		newpos += ctrl[1];
		oldpos += ctrl[2];
	};

	/* Close opened streams */
	io->close(cstream);
	io->close(dstream);
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}
	if (fclose(dpf)) {
		err_msg("Failed to close dpf");
	}
	if (fclose(cpf)) {
		err_msg("Failed to close cpf");
	}

	/* Compress patched block */
	writesize = ncf.compheader->blocksize;
	readsize = blockheader.new_block_size_u;
	ret = LzmaCompress(&bufs.buf_newblock_c[LZMA_PROPS_SIZE], &writesize, bufs.buf_newblock_u, readsize, bufs.buf_newblock_c, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
	if (ret != SZ_OK) {
		err_msg("LZMA Compress failed for block %d", i);
	}

	/* Verify new compressed block size */
	if (writesize != blockheader.new_block_size_c) {
		err_msg("Compressed block size(%lu) does not match size(%ld) info from blockheader for %d block", writesize, blockheader.new_block_size_c, i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_c, writesize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in new file */
	if (fseek(ncf.ncfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_to_new_compressed_file(bufs.buf_newblock_c, writesize, 1, ncf.ncfile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}

	/* Free old compressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_c_backup[i] != NULL) {
		free(bufs.buf_oldblock_c_backup[i]);
		bufs.buf_oldblock_c_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

/* Patch Block with Token == MODIFY for Old Uncompressed File */
void patch_block_token_modify_old_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io, int no_common_blocks)
{
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;

	/* Bspatch variables */
	FILE *cpf, *dpf, *epf;		/* Patch file streams */
	stream_t cstream, dstream, estream;
	int64_t oldsize, newsize;
	u_char buf[8];
	u_char *old, *new;
	off64_t oldpos, newpos;
	off64_t ctrl[3];
	off64_t lenread;
	off64_t j = 0;

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1) {
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	}

	/* Read UnCompressed block from old file, if needed (no backup already) */
	if (bufs.buf_oldblock_u_backup[i] == NULL) {
		block_start = offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_u_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ouf.oufile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_u_backup[i], block_size, 1, ouf.oufile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Patch old block using diff data */

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((cpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open cpf stream");
	}
	if (fseek(cpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for CPF");
	}
	cstream = io->open(cpf);

	if (fseek(pf.pfile, blockheader.size_ctrl_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of diff section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((dpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open dpf stream");
	}
	if (fseek(dpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for DPF");
	}
	dstream = io->open(dpf);

	if (fseek(pf.pfile, blockheader.size_diff_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of extra section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Bspatch */
	oldpos = 0;
	newpos = 0;
	newsize = blockheader.new_block_size_u;
	oldsize = block_size;
	new = bufs.buf_newblock_u;
	old = bufs.buf_oldblock_u_backup[i];
	while (newpos < newsize) {
		/* Read control data */
		for (j = 0; j <= 2; j++) {
			lenread = io->read(cstream, buf, 8);
			if (lenread < 8) {
				err_msg("Corrupt patch");
			}
			ctrl[j] = offtin(buf);
		};

		/* Sanity-check */
		if (newpos + ctrl[0] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read diff string */
		lenread = io->read(dstream, new + newpos, ctrl[0]);
		if (lenread < ctrl[0]) {
			err_msg("Corrupt patch");
		}

		/* Add old data to diff string */
		for (j = 0; j < ctrl[0]; j++)
			if ((oldpos + j >= 0) && (oldpos + j < oldsize)) {
				new[newpos + j] += old[oldpos + j];
			}

		/* Adjust pointers */
		newpos += ctrl[0];
		oldpos += ctrl[0];

		/* Sanity-check */
		if (newpos + ctrl[1] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read extra string */
		lenread = io->read(estream, new + newpos, ctrl[1]);
		if (lenread < ctrl[1]) {
			err_msg("Corrupt patch");
		}

		/* Adjust pointers */
		newpos += ctrl[1];
		oldpos += ctrl[2];
	};

	/* Close opened streams */
	io->close(cstream);
	io->close(dstream);
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}
	if (fclose(dpf)) {
		err_msg("Failed to close dpf");
	}
	if (fclose(cpf)) {
		err_msg("Failed to close cpf");
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_u, newsize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ouf.oufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_inplace_uncompressed_file(bufs.buf_newblock_u, newsize, 1, ouf.oufile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}

	/* Free old uncompressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_u_backup[i] != NULL) {
		free(bufs.buf_oldblock_u_backup[i]);
		bufs.buf_oldblock_u_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

void patch_block_token_modify_old_uncomp_to_new_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io, int no_common_blocks)
{
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;

	/* Bspatch variables */
	FILE *cpf, *dpf, *epf;		/* Patch file streams */
	stream_t cstream, dstream, estream;
	int64_t oldsize, newsize;
	u_char buf[8];
	u_char *old, *new;
	off64_t oldpos, newpos;
	off64_t ctrl[3];
	off64_t lenread;
	off64_t j = 0;

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1) {
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	}

	/* Read UnCompressed block from old file */
	if (bufs.buf_oldblock_u_backup[i] == NULL) {
		block_start = offset_to_first_block_of + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

		/* Malloc for buf_oldblock_c[i] for backup for this block */
		if ((bufs.buf_oldblock_u_backup[i] = (u_char *)malloc(sizeof(u_char) * (block_size + 5))) == NULL) {
			err_msg("Malloc failed for Buffer needed for backup of block %d", i);
		}

		/* Seek to position on old file from where to backup this block */
		if (fseek(ouf.oufile, block_start, SEEK_SET)) {
			err_msg("Failed to seek to position %d in old file", block_start);
		}

		/* Copy block to buffer */
		if (fread(bufs.buf_oldblock_u_backup[i], block_size, 1, ouf.oufile) != 1) {
			err_msg("Failed to read block %d from old file", i);
		}
	}

	/* Patch old block using diff data */

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((cpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open cpf stream");
	}
	if (fseek(cpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for CPF");
	}
	cstream = io->open(cpf);

	if (fseek(pf.pfile, blockheader.size_ctrl_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of diff section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((dpf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open dpf stream");
	}
	if (fseek(dpf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for DPF");
	}
	dstream = io->open(dpf);

	if (fseek(pf.pfile, blockheader.size_diff_block, SEEK_CUR)) {
		err_msg("File Seek failed to start of extra section for block %d", i);
	}

	pfpos = ftell(pf.pfile);
	/* Create streams for control, diff, extra sections */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Bspatch */
	oldpos = 0;
	newpos = 0;
	newsize = blockheader.new_block_size_u;
	oldsize = block_size;
	new = bufs.buf_newblock_u;
	old = bufs.buf_oldblock_u_backup[i];
	while (newpos < newsize) {
		/* Read control data */
		for (j = 0; j <= 2; j++) {
			lenread = io->read(cstream, buf, 8);
			if (lenread < 8) {
				err_msg("Corrupt patch");
			}
			ctrl[j] = offtin(buf);
		};

		/* Sanity-check */
		if (newpos + ctrl[0] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read diff string */
		lenread = io->read(dstream, new + newpos, ctrl[0]);
		if (lenread < ctrl[0]) {
			err_msg("Corrupt patch");
		}

		/* Add old data to diff string */
		for (j = 0; j < ctrl[0]; j++)
			if ((oldpos + j >= 0) && (oldpos + j < oldsize)) {
				new[newpos + j] += old[oldpos + j];
			}

		/* Adjust pointers */
		newpos += ctrl[0];
		oldpos += ctrl[0];

		/* Sanity-check */
		if (newpos + ctrl[1] > newsize) {
			err_msg("Corrupt patch");
		}

		/* Read extra string */
		lenread = io->read(estream, new + newpos, ctrl[1]);
		if (lenread < ctrl[1]) {
			err_msg("Corrupt patch");
		}

		/* Adjust pointers */
		newpos += ctrl[1];
		oldpos += ctrl[2];
	};

	/* Close opened streams */
	io->close(cstream);
	io->close(dstream);
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}
	if (fclose(dpf)) {
		err_msg("Failed to close dpf");
	}
	if (fclose(cpf)) {
		err_msg("Failed to close cpf");
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_u, newsize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_to_new_uncompressed_file(bufs.buf_newblock_u, newsize, 1, nuf.nufile) != 1) {
		err_msg("Failed write-back for %d block to new file", i);
	}

	/* Free old uncompressed block read from old file. No longer needed in RAM */
	if (bufs.buf_oldblock_u_backup[i] != NULL) {
		free(bufs.buf_oldblock_u_backup[i]);
		bufs.buf_oldblock_u_backup[i] = NULL;
	}

	/* Mark status that this block in old file has been patched */
	bufs.block_patch_status[i] = true;
}

/* Patch Block with Token == ADD for old compressed file */
void patch_block_token_add_old_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	unsigned int writesize, readsize;	/* Used for LZMA decompression */
	unsigned int propsSize = LZMA_PROPS_SIZE;
	int pfpos;
	int ret;
	FILE *epf;					/* Patch file streams */
	stream_t estream;

	block_crc = 0;

	pfpos = ftell(pf.pfile);
	/* Create streams for extra section */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	/* Seek to end of extra block in patch file */
	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Read Compressed block from block diff data */
	if (io->read(estream, bufs.buf_newblock_u, blockheader.new_block_size_u) != blockheader.new_block_size_u) {
		err_msg("Failed read for block %d from patch file", i);
	}

	/* Close opened streams */
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}

	/* Compress block */
	writesize = ncf.compheader->blocksize;
	readsize = blockheader.new_block_size_u;
	ret = LzmaCompress(&bufs.buf_newblock_c[LZMA_PROPS_SIZE], &writesize, bufs.buf_newblock_u, readsize, bufs.buf_newblock_c, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
	if (ret != SZ_OK) {
		err_msg("LZMA Compress failed for block %d", i);
	}

	/* Verify new compressed block size */
	if (writesize != blockheader.new_block_size_c) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_c, writesize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ocf.ocfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_inplace_compressed_file(bufs.buf_newblock_c, writesize, 1, ocf.ocfile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}
}

void patch_block_token_add_old_comp_to_new_comp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	unsigned int writesize, readsize;	/* Used for LZMA decompression */
	unsigned int propsSize = LZMA_PROPS_SIZE;
	int pfpos;
	int ret;
	FILE *epf;					/* Patch file streams */
	stream_t estream;

	block_crc = 0;

	pfpos = ftell(pf.pfile);
	/* Create streams for extra section */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	/* Seek to end of extra block in patch file */
	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Read Compressed block from block diff data */
	if (io->read(estream, bufs.buf_newblock_u, blockheader.new_block_size_u) != blockheader.new_block_size_u) {
		err_msg("Failed read for block %d from patch file", i);
	}

	/* Close opened streams */
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}

	/* Compress block */
	writesize = ncf.compheader->blocksize;
	readsize = blockheader.new_block_size_u;
	ret = LzmaCompress(&bufs.buf_newblock_c[LZMA_PROPS_SIZE], &writesize, bufs.buf_newblock_u, readsize, bufs.buf_newblock_c, &propsSize, 0, 1 << 13, -1, -1, -1, -1, 1);
	if (ret != SZ_OK) {
		err_msg("LZMA Compress failed for block %d", i);
	}

	/* Verify new compressed block size */
	if (writesize != blockheader.new_block_size_c) {
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_c, writesize);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in new file */
	if (fseek(ncf.ncfile, offset_to_first_block_nf + ncf.compheader->secoff[i], SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_to_new_compressed_file(bufs.buf_newblock_c, writesize, 1, ncf.ncfile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}
}

/* Patch Block with Token == ADD for old uncompressed file */
void patch_block_token_add_old_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;
	FILE *epf;					/* Patch file streams */
	stream_t estream;

	block_crc = 0;

	pfpos = ftell(pf.pfile);
	/* Create streams for extra section */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	/* Seek to end of extra block in patch file */
	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Read unCompressed block from block diff data */
	if (io->read(estream, bufs.buf_newblock_u, blockheader.new_block_size_u) != blockheader.new_block_size_u) {
		err_msg("Failed read for block %d from patch file", i);
	}

	/* Close opened streams */
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_u, blockheader.new_block_size_u);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in old file */
	if (fseek(ouf.oufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_inplace_uncompressed_file(bufs.buf_newblock_u, blockheader.new_block_size_u, 1, ouf.oufile) != 1) {
		err_msg("Failed write-back for %d block to old file in-place", i);
	}
}

void patch_block_token_add_old_uncomp_to_new_uncomp(int i, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf, char *p_file, io_funcs_t *io)
{
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;
	FILE *epf;					/* Patch file streams */
	stream_t estream;

	block_crc = 0;

	pfpos = ftell(pf.pfile);
	/* Create streams for extra section */
	if ((epf = fopen(p_file, "r")) == NULL) {
		err_msg("Failed to open epf stream");
	}
	if (fseek(epf, pfpos, SEEK_SET)) {
		err_msg("Fseek failed for EPF");
	}
	estream = io->open(epf);

	/* Seek to end of extra block in patch file */
	if (fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR)) {
		err_msg("File Seek failed to end of extra section for block %d", i);
	}

	/* Read unCompressed block from block diff data */
	if (io->read(estream, bufs.buf_newblock_u, blockheader.new_block_size_u) != blockheader.new_block_size_u) {
		err_msg("Failed read for block %d from patch file", i);
	}

	/* Close opened streams */
	io->close(estream);

	if (fclose(epf)) {
		err_msg("Failed to close epf");
	}

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, bufs.buf_newblock_u, blockheader.new_block_size_u);
	if (block_crc != blockheader.new_block_crc) {
		err_msg("CRC check failed for the %d block", i);
	}

	/* Seek to location where to write the block back in new file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET)) {
		err_msg("Failed to seek to offset for writing for %d block", i);
	}

	/* Write block back to old file in-place */
	if (fwrite_to_new_uncompressed_file(bufs.buf_newblock_u, blockheader.new_block_size_u, 1, nuf.nufile) != 1) {
		err_msg("Failed write-back for %d block to new block", i);
	}
}

int blockwise_bin_update_main(int argc, char *argv[])
{
	io_funcs_t *io;
	int no_block_diffs;			/* Total number of block diffs in patch file */
	int no_common_blocks;		/* Number of blocks common in old/new file */
	unsigned int offset_to_first_block_of;	/* Offset to first block in old file */
	unsigned int offset_to_first_block_nf;	/* Offset to first block in new file */
	off64_t size_for_truncate_of;	/* Size to which old file is truncated */

	if (argc != 3 && argc != 4) {
		err_msg("Patching Binary \nusage:\n\n%s old_file patchfile\n or \n%s old_file patchfile new_file", argv[0], argv[0]);
	}

	if (memcmp(argv[1], "/dev/mtdblock2", 14) == 0) {	/* Micom app */

		if (argc == 3) {		/* Patching old file in-place */

			/* Patch File */

			/* Opening patch file */
			if ((pf.pfile = fopen(argv[2], "r")) == NULL) {
				err_msg("fopen(%s)", argv[2]);
			}

			/* Verify CRC for patch file */
			verify_patch_file_crc(argv[2]);

			/* Read Patch Header and Verify signature and Compression Format values, Assign io structure */
			parse_patch_header(argv[2], &io);

			/* Branch according to "patchheader.old_file_compression_format" value */
			if (patchheader.old_file_compression_format == COMPRESSION_TYPE_LZMA) {

				/* Parse CRC, Bin Header, Comp. header from patch file */
				parse_metadata_for_new_comp(argv[2]);

				/* Open and Parse metadata from old compressed file */
				parse_old_comp_file(argv[1]);

				/* Verify conditions for patching (compression blocksize and compression format) */
				verify_patching_conditions();

				/* Malloc for buffers */
				malloc_buffers_for_old_comp();

				/* Writing new CRC, Binary Header and Comp. header to old file */
				write_new_metadata_to_old_comp();

				/* Patching Block-by-Block */

				/* Calculate total number of block diffs in patch file */
				no_block_diffs = MAX(patchheader.blocks_newfile, patchheader.blocks_oldfile);

				/* Assigning offset for 1st block in old, new file */
				offset_to_first_block_of = sizeof(ocf.crc) + sizeof(ocf.binheader) + ocf.compheader->size_header;
				offset_to_first_block_nf = sizeof(ncf.crc) + sizeof(ncf.binheader) + ncf.compheader->size_header;

				/* Patch block-by-block */
				for (int i = 0; i < no_block_diffs; i++) {

					/* Read Block Header */
					if (fread(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
						err_msg("Failed to read blockheader for block index = %d from %s", i, argv[2]);
					}

					/* Handle cases according to Token (Skip, Modify, Add, Remove) */
					if (blockheader.token == SKIP) {

						/* Patch Block with Token == SKIP */
						patch_block_token_skip_old_comp(i, offset_to_first_block_of, offset_to_first_block_nf);

					} else if (blockheader.token == MODIFY) {

						/* Patch Block with Token == MODIFY */
						patch_block_token_modify_old_comp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == ADD) {

						/* Patch Block with Token == ADD */
						patch_block_token_add_old_comp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == REMOVE) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ocf.ocfile);
						//if (truncate(argv[1], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");

						/* No need to process remaining block diffs from patch file
						 * They too will have token == REMOVE;
						 */
						break;

					}

					/* If this is last block diff info in patch file, truncate file here */
					if (i == no_block_diffs - 1) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ocf.ocfile);
						//if (truncate(argv[1], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");
					}

				}

				/* Close Files and Free Memory */

				if (fclose(ocf.ocfile)) {
					err_msg("Failed to close %s", argv[1]);
				}

				if (fclose(pf.pfile)) {
					err_msg("Failed to close epf");
				}

				if (bufs.buf_newblock_c != NULL) {
					free(bufs.buf_newblock_c);
				}
				if (bufs.buf_newblock_u != NULL) {
					free(bufs.buf_newblock_u);
				}
				if (bufs.buf_oldblock_u != NULL) {
					free(bufs.buf_oldblock_u);
				}
				for (int k = 0; k < MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile); k++) {
					if (bufs.buf_oldblock_c_backup[k] != NULL) {
						free(bufs.buf_oldblock_c_backup[k]);
					}
				}

				if (bufs.block_patch_status != NULL) {
					free(bufs.block_patch_status);
				}

				free(ncf.compheader);
				free(ocf.compheader);

			} else if (patchheader.old_file_compression_format == COMPRESSION_TYPE_NONE) {

				/* Parse CRC, Bin Header from patch file */
				parse_metadata_for_new_uncomp(argv[2]);

				/* Open and Parse metadata from old uncompressed file */
				parse_old_uncomp_file(argv[1]);

				/* Malloc for buffers */
				malloc_buffers_for_old_uncomp();

				/*  Writing new CRC and Binary Header to old file  */
				write_new_metadata_to_old_uncomp();

				/* Patching Block-by-Block */

				/* Calculate total number of block diffs and number of common blocks in patch file */
				no_block_diffs = MAX(patchheader.blocks_newfile, patchheader.blocks_oldfile);
				no_common_blocks = MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile);

				/* Assigning offset for 1st block in old file */
				offset_to_first_block_of = sizeof(ouf.crc) + sizeof(ouf.binheader);
				offset_to_first_block_nf = sizeof(nuf.crc) + sizeof(nuf.binheader);

				/* Patch block-by-block */
				for (int i = 0; i < no_block_diffs; i++) {

					/* Read Block Header */
					if (fread(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
						err_msg("Failed to read blockheader for block index = %d from %s", i, argv[2]);
					}

					/* Handle cases according to Token (Skip, Modify, Add, Remove) */
					if (blockheader.token == SKIP) {

						patch_block_token_skip_old_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, no_common_blocks);

					} else if (blockheader.token == MODIFY) {

						patch_block_token_modify_old_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io, no_common_blocks);

					} else if (blockheader.token == ADD) {

						patch_block_token_add_old_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == REMOVE) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ouf.oufile);
						//if (truncate(argv[1], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");

						/* No need to process remaining block diffs from patch file
						 * They too will have token == REMOVE;
						 */
						break;

					}

					/* If this is last block diff info in patch file, truncate file here */
					if (i == no_block_diffs - 1) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ouf.oufile);
						//if (truncate(argv[1], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");
					}

				}

				/* Close Files and Free Memory */

				/* Close new compressed file (created from patching in-place) */
				if (fclose(ouf.oufile)) {
					err_msg("Failed to close %s", argv[1]);
				}

				if (fclose(pf.pfile)) {
					err_msg("Failed to close epf");
				}

				if (bufs.buf_newblock_u != NULL) {
					free(bufs.buf_newblock_u);
				}

				for (int k = 0; k < MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile); k++) {
					if (bufs.buf_oldblock_u_backup[k] != NULL) {
						free(bufs.buf_oldblock_u_backup[k]);
					}
				}

				if (bufs.block_patch_status != NULL) {
					free(bufs.block_patch_status);
				}
			}
		} else if (argc == 4) {	/* Patching to a new file */

			/* Patch File */

			/* Opening patch file */
			if ((pf.pfile = fopen(argv[2], "r")) == NULL) {
				err_msg("fopen(%s)", argv[2]);
			}

			/* Verify CRC for patch file */
			verify_patch_file_crc(argv[2]);

			/* Read Patch Header and Verify signature and Compression Format values, Assign io structure */
			parse_patch_header(argv[2], &io);

			/* Branch according to "patchheader.old_file_compression_format" value */
			if (patchheader.old_file_compression_format == COMPRESSION_TYPE_LZMA) {

				/* Parse CRC, Bin Header, Comp. header from patch file */
				parse_metadata_for_new_comp(argv[2]);

				/* Open and Parse metadata from old compressed file */
				parse_old_comp_file(argv[1]);

				/* Open new compressed file */
				open_new_compressed_file(argv[3]);

				/* Verify conditions for patching (compression blocksize and compression format) */
				verify_patching_conditions();

				/* Malloc for buffers */
				malloc_buffers_for_old_comp();

				/* Writing new CRC, Binary Header and Comp. header to old file */
				//write_new_metadata_to_old_comp();

				/* Writing new CRC, Binary Header and Comp. header to new comp. file */
				write_new_metadata_to_new_comp();

				/* Patching Block-by-Block */

				/* Calculate total number of block diffs in patch file */
				no_block_diffs = MAX(patchheader.blocks_newfile, patchheader.blocks_oldfile);

				/* Assigning offset for 1st block in old, new file */
				offset_to_first_block_of = sizeof(ocf.crc) + sizeof(ocf.binheader) + ocf.compheader->size_header;
				offset_to_first_block_nf = sizeof(ncf.crc) + sizeof(ncf.binheader) + ncf.compheader->size_header;

				/* Patch block-by-block */
				for (int i = 0; i < no_block_diffs; i++) {

					/* Read Block Header */
					if (fread(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
						err_msg("Failed to read blockheader for block index = %d from %s", i, argv[2]);
					}

					/* Handle cases according to Token (Skip, Modify, Add, Remove) */
					if (blockheader.token == SKIP) {

						/* Patch Block with Token == SKIP */
						patch_block_token_skip_old_comp_to_new_comp(i, offset_to_first_block_of, offset_to_first_block_nf);

					} else if (blockheader.token == MODIFY) {

						/* Patch Block with Token == MODIFY */
						patch_block_token_modify_old_comp_to_new_comp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == ADD) {

						/* Patch Block with Token == ADD */
						patch_block_token_add_old_comp_to_new_comp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == REMOVE) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ncf.ncfile);
						//if (truncate(argv[3], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");

						/* No need to process remaining block diffs from patch file
						 * They too will have token == REMOVE;
						 */
						break;

					}

					/* If this is last block diff info in patch file, truncate file here */
					if (i == no_block_diffs - 1) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(ncf.ncfile);
						//if (truncate(argv[3], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");
					}

				}

				printf("Patching Successful\n");

				/* Close Files and Free Memory */

				if (fclose(ocf.ocfile)) {
					err_msg("Failed to close %s", argv[1]);
				}

				if (fclose(ncf.ncfile)) {
					err_msg("Failed to close %s", argv[3]);
				}

				if (fclose(pf.pfile)) {
					err_msg("Failed to close epf");
				}

				if (bufs.buf_newblock_c != NULL) {
					free(bufs.buf_newblock_c);
				}
				if (bufs.buf_newblock_u != NULL) {
					free(bufs.buf_newblock_u);
				}
				if (bufs.buf_oldblock_u != NULL) {
					free(bufs.buf_oldblock_u);
				}
				for (int k = 0; k < MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile); k++) {
					if (bufs.buf_oldblock_c_backup[k] != NULL) {
						free(bufs.buf_oldblock_c_backup[k]);
					}
				}

				if (bufs.block_patch_status != NULL) {
					free(bufs.block_patch_status);
				}

				free(ncf.compheader);
				free(ocf.compheader);

			} else if (patchheader.old_file_compression_format == COMPRESSION_TYPE_NONE) {

				/* Parse CRC, Bin Header from patch file */
				parse_metadata_for_new_uncomp(argv[2]);

				/* Open and Parse metadata from old uncompressed file */
				parse_old_uncomp_file(argv[1]);

				/* Open new uncompressed file */
				open_new_uncompressed_file(argv[3]);

				/* Malloc for buffers */
				malloc_buffers_for_old_uncomp();

				/*  Writing new CRC and Binary Header to old file  */
				//write_new_metadata_to_old_uncomp();

				/* Write new CRC and binary header to new uncompressed file */
				write_new_metadata_to_new_uncomp();

				/* Patching Block-by-Block */

				/* Calculate total number of block diffs and number of common blocks in patch file */
				no_block_diffs = MAX(patchheader.blocks_newfile, patchheader.blocks_oldfile);
				no_common_blocks = MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile);

				/* Assigning offset for 1st block in old file */
				offset_to_first_block_of = sizeof(ouf.crc) + sizeof(ouf.binheader);
				offset_to_first_block_nf = sizeof(nuf.crc) + sizeof(nuf.binheader);

				/* Patch block-by-block */
				for (int i = 0; i < no_block_diffs; i++) {

					/* Read Block Header */
					if (fread(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
						err_msg("Failed to read blockheader for block index = %d from %s", i, argv[2]);
					}

					/* Handle cases according to Token (Skip, Modify, Add, Remove) */
					if (blockheader.token == SKIP) {

						patch_block_token_skip_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, no_common_blocks);

					} else if (blockheader.token == MODIFY) {

						patch_block_token_modify_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io, no_common_blocks);

					} else if (blockheader.token == ADD) {

						patch_block_token_add_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

					} else if (blockheader.token == REMOVE) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(nuf.nufile);
						//if (truncate(argv[3], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");

						/* No need to process remaining block diffs from patch file
						 * They too will have token == REMOVE;
						 */
						break;

					}

					/* If this is last block diff info in patch file, truncate file here */
					if (i == no_block_diffs - 1) {

						/* Mark end of file. File pointer is already at appropriate location */
						size_for_truncate_of = ftell(nuf.nufile);
						//if (truncate(argv[3], size_for_truncate_of) != 0)
						//  err_msg("Failed to truncate file");
					}

				}

				printf("Patching Successful\n");

				/* Close Files and Free Memory */

				/* Close new compressed file (created from patching in-place) */
				if (fclose(ouf.oufile)) {
					err_msg("Failed to close %s", argv[1]);
				}

				if (fclose(nuf.nufile)) {
					err_msg("Failed to close %s", argv[3]);
				}

				if (fclose(pf.pfile)) {
					err_msg("Failed to close epf");
				}

				if (bufs.buf_newblock_u != NULL) {
					free(bufs.buf_newblock_u);
				}

				for (int k = 0; k < MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile); k++) {
					if (bufs.buf_oldblock_u_backup[k] != NULL) {
						free(bufs.buf_oldblock_u_backup[k]);
					}
				}

				if (bufs.block_patch_status != NULL) {
					free(bufs.block_patch_status);
				}

			}
		}
	} else if (memcmp(argv[1], "/dev/mtdblock0", 14) == 0) {	/* Kernel */

		if (argc != 4) {
			err_msg("Support present only for patching (kernel) to new file.\nUsage: %s old_file patch_file new_file", argv[0]);
		}

		/* Patch File */

		/* Opening patch file */
		if ((pf.pfile = fopen(argv[2], "r")) == NULL) {
			err_msg("fopen(%s)", argv[2]);
		}

		/* Verify CRC for patch file */
		verify_patch_file_crc(argv[2]);

		/* Read Patch Header and Verify signature and Compression Format values, Assign io structure */
		parse_patch_header(argv[2], &io);

		/* Branch according to "patchheader.old_file_compression_format" value */
		if (patchheader.old_file_compression_format == COMPRESSION_TYPE_LZMA) {
			err_msg("Support only for uncompressed kernel");

		} else if (patchheader.old_file_compression_format == COMPRESSION_TYPE_NONE) {

			/* Opening old uncompressed file */
			if ((ouf.oufile = fopen(argv[1], "r+")) == NULL) {
				err_msg("fopen(%s)", argv[1]);
			}

			/* Seek to end of old uncompressed file */
			if (fseek(ouf.oufile, 0, SEEK_END)) {
				err_msg("fseek");
			}

			/* Total size of old uncompressed file */
			ouf.total_size = ftell(ouf.oufile);
			ouf.size_for_blocks_calc = ouf.total_size;

			/* Seek to start of old uncompressed file */
			if (fseek(ouf.oufile, 0, SEEK_SET)) {
				err_msg("fseek");
			}

			/* Open new uncompressed file */
			open_new_uncompressed_file(argv[3]);

			/* Malloc for buffers */
			malloc_buffers_for_old_uncomp();

			/* Patching Block-by-Block */

			/* Calculate total number of block diffs and number of common blocks in patch file */
			no_block_diffs = MAX(patchheader.blocks_newfile, patchheader.blocks_oldfile);
			no_common_blocks = MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile);

			/* Assigning offset for 1st block in old file */
			offset_to_first_block_of = 0;
			offset_to_first_block_nf = 0;

			/* Patch block-by-block */
			for (int i = 0; i < no_block_diffs; i++) {

				/* Read Block Header */
				if (fread(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
					err_msg("Failed to read blockheader for block index = %d from %s", i, argv[2]);
				}

				/* Handle cases according to Token (Skip, Modify, Add, Remove) */
				if (blockheader.token == SKIP) {

					patch_block_token_skip_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, no_common_blocks);

				} else if (blockheader.token == MODIFY) {

					patch_block_token_modify_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io, no_common_blocks);

				} else if (blockheader.token == ADD) {

					patch_block_token_add_old_uncomp_to_new_uncomp(i, offset_to_first_block_of, offset_to_first_block_nf, argv[2], io);

				} else if (blockheader.token == REMOVE) {

					/* Mark end of file. File pointer is already at appropriate location */
					size_for_truncate_of = ftell(nuf.nufile);
					//if (truncate(argv[3], size_for_truncate_of) != 0)
					//  err_msg("Failed to truncate file");

					/* No need to process remaining block diffs from patch file
					 * They too will have token == REMOVE;
					 */
					break;

				}

				/* If this is last block diff info in patch file, truncate file here */
				if (i == no_block_diffs - 1) {

					/* Mark end of file. File pointer is already at appropriate location */
					size_for_truncate_of = ftell(nuf.nufile);
					//if (truncate(argv[3], size_for_truncate_of) != 0)
					//  err_msg("Failed to truncate file");
				}

			}

			printf("Patching Successful\n");

			/* Close Files and Free Memory */

			/* Close new compressed file (created from patching in-place) */
			if (fclose(ouf.oufile)) {
				err_msg("Failed to close %s", argv[1]);
			}

			if (fclose(nuf.nufile)) {
				err_msg("Failed to close %s", argv[3]);
			}

			if (fclose(pf.pfile)) {
				err_msg("Failed to close epf");
			}

			if (bufs.buf_newblock_u != NULL) {
				free(bufs.buf_newblock_u);
			}

			for (int k = 0; k < MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile); k++) {
				if (bufs.buf_oldblock_u_backup[k] != NULL) {
					free(bufs.buf_oldblock_u_backup[k]);
				}
			}

			if (bufs.block_patch_status != NULL) {
				free(bufs.block_patch_status);
			}

		}
	}

	return 0;
}
