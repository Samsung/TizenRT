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

typedef void* stream_t;

/* Compatibility layer for reading BLCKDIFF patch format */
typedef struct
{
	int (*read)(stream_t, void*, int);
} io_funcs_t;

#ifndef u_char
typedef unsigned char u_char;
#endif

enum state_blkheader {
	NEED_BLOCKHEADER = 0,
	HAVE_FULL_BLOCKHEADER,
};

enum state_blkdelta {
	NEED_BLOCKDELTA = 0,
	HAVE_FULL_BLOCKDELTA,
};


/****************** Private data *************************/

old_uncomp_file ouf;
new_uncomp_file nuf;
new_comp_file ncf;
old_comp_file ocf;
patch_file pf;

struct patch_header patchheader;		/* Header for output patch file */
struct block_diff_header blockheader;		/* Header info for diff b/w 2 blocks */

char download_buf[CONFIG_FOTA_DOWNLOAD_PACKET_SIZE];
char uncomp_blkdiff[3 * CONFIG_BLOCKSIZE_FOR_FOTA_DELTA];

int no_common_blocks;
u_char old_file_buffer[CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1];
u_char new_file_buffer[CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1];
io_funcs_t * io;
char *new_file_name;
int uncomp_ctrl_size, uncomp_diff_size;
unsigned int offset_to_first_block_of;	/* Offset to first block in old file */
unsigned int offset_to_first_block_nf;	/* Offset to first block in new file */


unsigned int total_size_packets = 0;

/**************** Function Definitions ********************/

static int BLCKDIFF_read(void *s, void *buf, int len)
{
	memcpy(buf, s, len);

	return len;
}

static io_funcs_t BLCKDIFF_funcs = {
	BLCKDIFF_read,
};

static off64_t offtin(u_char *buf)
{
	off64_t y;

	y=buf[7]&0x7F;
	y=y*256;y+=buf[6];
	y=y*256;y+=buf[5];
	y=y*256;y+=buf[4];
	y=y*256;y+=buf[3];
	y=y*256;y+=buf[2];
	y=y*256;y+=buf[1];
	y=y*256;y+=buf[0];

	if(buf[7]&0x80) y=-y;

	return y;
}

int inflate_in_memory(const void *src, int srcLen, void *dst, int dstLen) {
	z_stream strm  = {0};
	strm.total_in  = strm.avail_in  = srcLen;
	strm.total_out = strm.avail_out = dstLen;
	strm.next_in   = (Bytef *) src;
	strm.next_out  = (Bytef *) dst;

	strm.zalloc = Z_NULL;
	strm.zfree  = Z_NULL;
	strm.opaque = Z_NULL;

	int err = -1;
	int ret = -1;

	err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
	if (err == Z_OK) {
		err = inflate(&strm, Z_FINISH);
		if (err == Z_STREAM_END) {
			ret = strm.total_out;
		}
		else {
			inflateEnd(&strm);
			return err;
		}
	}
	else {
		inflateEnd(&strm);
		return err;
	}

	inflateEnd(&strm);
	return ret;
}

bool get_buffer(char **buffer, unsigned int *data_size)
{

	int nmemb;

	/* Downloading packet */
	nmemb = fread(download_buf, 1, CONFIG_FOTA_DOWNLOAD_PACKET_SIZE, pf.pfile);
	if (nmemb <= 0) {
		printf("fread failed in get_buffer\n");
		return false;
	}
	
	*buffer = download_buf;
	*data_size = nmemb;

	return true;
}

void patch_block_token_add_old_uncomp_to_new_uncomp(int i, char *ctrl_sec_ptr, char *diff_sec_ptr, char *extra_sec_ptr, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf)
{
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;
	FILE *epf;			/* Patch file streams */
	stream_t estream;

	block_crc = 0;

	/* Read unCompressed block from block diff data */
	if (io->read((void *) extra_sec_ptr, new_file_buffer, blockheader.new_block_size_u) != blockheader.new_block_size_u)
		err_msg("Failed read for block %d from patch file", i);

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *) new_file_buffer, blockheader.new_block_size_u);
	if (block_crc != blockheader.new_block_crc)
		err_msg("CRC check failed for the %d block", i);

	/* Seek to location where to write the block back in new file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET))
		err_msg("Failed to seek to offset for writing for %d block", i);

	/* Write block back to old file in-place */
	if (fwrite(new_file_buffer, blockheader.new_block_size_u, 1, nuf.nufile) != 1)
		err_msg("Failed write-back for %d block to new block", i);
}


void patch_block_token_skip_old_uncomp_to_new_uncomp(int i, char *ctrl_sec_ptr, char *diff_sec_ptr, char *extra_sec_ptr, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf)
{
	unsigned int block_start;	/* Used when taking backup of block */
	unsigned int block_size;	/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1)
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	else
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ?
					ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

	/* Read unCompressed block from old file */
	block_start = offset_to_first_block_of + (i*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

	/* Seek to position on old file from where to backup this block */
	if (fseek(ouf.oufile, block_start, SEEK_SET))
		err_msg("Failed to seek to position %d in old file", block_start);

	/* Copy block to buffer */
	if (fread(old_file_buffer, block_size, 1, ouf.oufile) != 1)
		err_msg("Failed to read block %d from old file", i);

	/* Verify new block size */
	if (block_size != blockheader.new_block_size_u)
		err_msg("Block size read from old file does not match size value from blockheader for %d block", i);

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *) old_file_buffer, block_size);
	if (block_crc != blockheader.new_block_crc)
		err_msg("CRC check failed for the %d block", i);

	/* Seek to location where to write the block back in new file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET))
		err_msg("Failed to seek to offset for writing for %d block", i);

	/* Write the block back to old file */
	if (fwrite(old_file_buffer, block_size, 1, nuf.nufile) != 1)
		err_msg("Failed to write patched %d block to oldfile", i);
}


void patch_block_token_modify_old_uncomp_to_new_uncomp(int i, char *ctrl_sec_ptr, char *diff_sec_ptr, char *extra_sec_ptr, unsigned int offset_to_first_block_of, unsigned int offset_to_first_block_nf)
{
	unsigned int block_start;	/* Used when taking backup of block */
	uint64_t block_size;		/* Used when taking backup of block */
	unsigned int block_crc;		/* holds CRC for block from new compressed file */
	int pfpos;

	/* Bspatch variables */
	FILE *cpf, *dpf, *epf;		/* Patch file streams */
	stream_t cstream, dstream, estream;
	int64_t oldsize,newsize;
	u_char buf[8];
	u_char *old, *new;
	off64_t oldpos,newpos;
	off64_t ctrl[3];
	off64_t lenread;
	off64_t j = 0;

	block_crc = 0;

	/* Calculate block size for current block in old file */
	if (i != no_common_blocks - 1)
		block_size = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	else
		block_size = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ?
				ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

	/* Read UnCompressed block from old file */
	block_start = offset_to_first_block_of + (i*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);

	/* Seek to position on old file from where to backup this block */
	if (fseek(ouf.oufile, block_start, SEEK_SET))
		err_msg("Failed to seek to position %d in old file", block_start);

	/* Copy block to buffer */
	if (fread(old_file_buffer, block_size, 1, ouf.oufile) != 1)
		err_msg("Failed to read block %d from old file", i);

	/* Patch old block using diff data */

	/* Bspatch */
	oldpos=0;newpos=0;
	newsize = blockheader.new_block_size_u;
	oldsize = block_size;
	new = new_file_buffer;
	old = old_file_buffer;
	while(newpos<newsize) {
		/* Read control data */
		for(j=0;j<=2;j++) {
			lenread = io->read((void *) ctrl_sec_ptr, buf, 8);
			if (lenread < 8)
				err_msg("Corrupt patch");

			/* Add offset to pointer */
			ctrl_sec_ptr += 8;
			ctrl[j]=offtin(buf);
		};

		/* Sanity-check */
		if(newpos+ctrl[0]>newsize)
			err_msg("Corrupt patch");

		/* Read diff string */
		lenread = io->read((void *) diff_sec_ptr, new + newpos, ctrl[0]);
		if (lenread < ctrl[0])
			err_msg("Corrupt patch");

		/* Add offset to pointer */
		diff_sec_ptr += ctrl[0];

		/* Add old data to diff string */
		for(j=0;j<ctrl[0];j++)
			if((oldpos+j>=0) && (oldpos+j<oldsize))
				new[newpos+j]+=old[oldpos+j];

		/* Adjust pointers */
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize)
			err_msg("Corrupt patch");

		/* Read extra string */
		lenread = io->read((void *) extra_sec_ptr, new + newpos, ctrl[1]);
		if (lenread < ctrl[1])
			err_msg("Corrupt patch");

		/* Add offset to pointer */
		extra_sec_ptr += ctrl[1];

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];
	};

	/* Verify Block CRC */
	block_crc = crc32_z(block_crc, (const Bytef *) new_file_buffer, newsize);
	if (block_crc != blockheader.new_block_crc)
		err_msg("CRC check failed for the %d block", i);

	/* Seek to location where to write the block back in old file */
	if (fseek(nuf.nufile, offset_to_first_block_nf + (i*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA), SEEK_SET))
		err_msg("Failed to seek to offset for writing for %d block", i);

	/* Write block back to old file in-place */
	if (fwrite(new_file_buffer, newsize, 1,nuf.nufile) != 1)
		err_msg("Failed write-back for %d block to new file", i);
}

void write_binheader (binary_header_t binheader)
{
	if (fwrite(&binheader, sizeof(binary_header_t), 1, nuf.nufile) != 1)
		err_msg("Failed write to binary header to new file\n");
}

void write_crc (unsigned int crc_newfile)
{
	if (fwrite(&crc_newfile, sizeof(nuf.crc), 1, nuf.nufile) != 1)
		err_msg("Failed write of CRC to new file\n");
}

void print_blockheader(void)
{
	printf("{%u, %u, %ld, %ld, %ld %ld, %ld, %u}\n", blockheader.token, blockheader.block_number, blockheader.size_ctrl_block,
		blockheader.size_diff_block, blockheader.size_extra_block, blockheader.new_block_size_c, blockheader.new_block_size_u,
		blockheader.new_block_crc);
}

/* Uncompress ZLIB compressed data to uncomp_blkdiff */
void uncompress_blockdelta(char *buf)
{
	int ret;
	struct block_diff_header blkheader;

	/* Assign blkheader */
	memcpy(&blkheader, buf, sizeof(struct block_diff_header));

	/* Copy Blockheader as such to uncomp_blkdiff */
	memcpy(uncomp_blkdiff, buf, sizeof(struct block_diff_header));

	/* Uncomp ctrl section of delta to uncomp_blkdiff */
	ret = inflate_in_memory((void *) &buf[sizeof(struct block_diff_header)], blkheader.size_ctrl_block, (void *) &uncomp_blkdiff[sizeof(struct block_diff_header)], CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	uncomp_ctrl_size = ret;

	/* Uncomp diff section of delta to uncomp_blkdiff */
	ret = inflate_in_memory((void *) &buf[sizeof(struct block_diff_header) + blkheader.size_ctrl_block], blkheader.size_diff_block, (void *) &uncomp_blkdiff[sizeof(struct block_diff_header) + uncomp_ctrl_size], CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	uncomp_diff_size = ret;

	/* Uncomp extra section of delta to uncomp_blkdiff */
	ret = inflate_in_memory((void *) &buf[sizeof(struct block_diff_header) + blkheader.size_ctrl_block + blkheader.size_diff_block], blkheader.size_extra_block, (void *) &uncomp_blkdiff[sizeof(struct block_diff_header) + uncomp_ctrl_size + uncomp_diff_size], CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
}

void patch(char *buf, int size)
{
	static int block_number_patching = -1;
	char *ctrl_sec_ptr;
	char *diff_sec_ptr;
	char *extra_sec_ptr;
	off64_t size_for_truncate_nf;

	//print_blockheader();
	//printf("Patching %d block with which has total delta size = %d\n", blockheader.block_number, size);
	if (sizeof(blockheader) + blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block - size != 0) {
		err_msg("------!!!!! Not matching !!!!!!--------\n");
	}

	total_size_packets += size;
	block_number_patching += 1;

	/* Uncompress ZLIB compressed data to uncomp_blkdiff */
	uncompress_blockdelta(buf);

	/* Get Blockheader */
	memcpy(&blockheader, buf, sizeof(blockheader));

	/* Assign pointers to diff, ctrl, extra sections */
	ctrl_sec_ptr = &uncomp_blkdiff[sizeof(struct block_diff_header)];
	diff_sec_ptr = &uncomp_blkdiff[sizeof(struct block_diff_header) + uncomp_ctrl_size];
	extra_sec_ptr = &uncomp_blkdiff[sizeof(struct block_diff_header) + uncomp_ctrl_size + uncomp_diff_size];

	/* Handle cases according to Token (Skip, Modify, Add, Remove) */
	if (blockheader.token == SKIP) {

		/* Patch Block with Token == SKIP */
		patch_block_token_skip_old_uncomp_to_new_uncomp(block_number_patching, ctrl_sec_ptr, diff_sec_ptr, extra_sec_ptr, offset_to_first_block_of, offset_to_first_block_nf);

	} else if (blockheader.token == MODIFY) {

		/* Patch Block with Token == MODIFY */
		patch_block_token_modify_old_uncomp_to_new_uncomp(block_number_patching, ctrl_sec_ptr, diff_sec_ptr, extra_sec_ptr, offset_to_first_block_of, offset_to_first_block_nf);

	} else if (blockheader.token == ADD) {

		/* Patch Block with Token == ADD */
		patch_block_token_add_old_uncomp_to_new_uncomp(block_number_patching, ctrl_sec_ptr, diff_sec_ptr, extra_sec_ptr, offset_to_first_block_of, offset_to_first_block_nf);

	} else if (blockheader.token == REMOVE) {

		/* Mark end of file. File pointer is already at appropriate location */
		size_for_truncate_nf = ftell(nuf.nufile);
		//if (truncate(new_file_name, size_for_truncate_nf))
		//	err_msg("Failed to truncate file");
	}
}

int blockwise_bin_update_main(int argc,char * argv[])
{
	int no_block_diffs;			/* Total number of block diffs in patch file */
	off64_t size_for_truncate_of;		/* Size to which old file is truncated */

	char *pbuf;
	char blkdiff_buffer[2*CONFIG_BLOCKSIZE_FOR_FOTA_DELTA];
	unsigned int data_size;
	unsigned int crc_get_buf;
	int loc_pbuf = 0, loc_blkdiff_buffer = 0;
	int packet_num = -1;
	bool download_flag;
	bool got_last_packet = false;
	int blkheader_state;
	int blkdelta_state;
	int block_delta_size;
	int blkheader_partial_size_copied;
	int blkdelta_partial_size_copied;

	if (argc != 3 && argc != 4) {
		err_msg("Patching Binary \nusage:\n\n%s old_file patchfile\n or \n%s old_file patchfile new_file", argv[0], argv[0]);
	}

	if (memcmp(argv[1], "/dev/mtdblock0", 14) == 0) {	/* Kernel */

		if (argc != 4)
			err_msg("Support present only for patching (kernel) to new file.\nUsage: %s old_file patch_file new_file", argv[0]);

		/* Open patch file */
		if ((pf.pfile = fopen(argv[2], "r")) == NULL)
			err_msg("fopen(%s)", argv[2]);

		/* Open old file */
		if ((ouf.oufile = fopen(argv[1], "r")) == NULL)
			err_msg("fopen(%s)", argv[1]);

		if (fseek(ouf.oufile, 0, SEEK_END))
			err_msg("fseek");

		ouf.total_size = ftell(ouf.oufile);
		ouf.size_for_blocks_calc = ouf.total_size;

		/* Seek to start of old uncompressed file */
		if (fseek(ouf.oufile, 0, SEEK_SET))
			err_msg("fseek");

		/* Open new file */
		if ((nuf.nufile = fopen(argv[3], "r+")) == NULL)
			err_msg("fopen(%s)", argv[3]);

		new_file_name = argv[3];

		offset_to_first_block_of = 0;
		offset_to_first_block_nf = 0;

		/*checking get_buffer */
		/*
		while (1) {
			download_flag = get_buffer(&pbuf, &data_size);
			if (download_flag == false) {
				err_msg("Failed to get packet\n");
			}
			
			crc_get_buf = crc32_z(0, (const Bytef *)pbuf, data_size);
			printf("Packet No. (%d), Data Size (%u), Packet CRC (%u)\n", num, data_size, crc_get_buf);
			if (data_size == CONFIG_FOTA_DOWNLOAD_PACKET_SIZE)
				num += 1;
			else
				break;
		}*/

		/* get packets and process them */

get_buf_kernel:

		/* Get packet is available for download */
		if (got_last_packet == false) {
			download_flag = get_buffer(&pbuf, &data_size);
		} else {
			goto done_patching_kernel;
		}

		/* Failed to get packet */
		if (download_flag == false) {
			printf("Failed to get packet\n");
			return 1;
		}

		printf("data_size [%d] = %d\n", packet_num + 1, data_size);

		/* Last packet if data_size is less than CONFIG_FOTA_DOWNLOAD_PACKET_SIZE */
		if (data_size < CONFIG_FOTA_DOWNLOAD_PACKET_SIZE)
			got_last_packet = true;

		/* Update variables */
		packet_num += 1;
		loc_pbuf = 0;

		/* Processing 1st packet */
		if (packet_num == 0) {

			printf("Packet Number - 0\n");

			/* Get Patchheader */
			memcpy(&patchheader, &pbuf[loc_pbuf], sizeof(patchheader));
			no_common_blocks = MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile);
			loc_pbuf += sizeof(patchheader);

			/* Verify Bsdiff patch_signature & assign (io_funcs_t *)io */
			if (memcmp(&patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE) == 0) {
				io = &BLCKDIFF_funcs;
			} else {
				err_msg("Bsdiff signature does not match. Incorrect patch file provided");
			}

			/* Patch blocks which are possible with this packet */
			while (loc_pbuf + sizeof(blockheader) < data_size) {
				/* Get Blockheader */
				memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
				blkheader_state = HAVE_FULL_BLOCKHEADER;
				loc_pbuf += sizeof(blockheader);
				loc_blkdiff_buffer += sizeof(blockheader);

				/* Get as much delta for block as possible from this packet */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				if (loc_pbuf + block_delta_size <= data_size) {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
					loc_blkdiff_buffer += block_delta_size;
					loc_pbuf += block_delta_size;
					blkdelta_state = HAVE_FULL_BLOCKDELTA;

				} else {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
					blkdelta_partial_size_copied = data_size - loc_pbuf;
					loc_blkdiff_buffer += data_size - loc_pbuf;
					loc_pbuf += data_size - loc_pbuf;
					blkdelta_state = NEED_BLOCKDELTA;

				}

				/* Patch block if entire delta available, else get new packet */
				if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
					patch(blkdiff_buffer, loc_blkdiff_buffer);
					loc_blkdiff_buffer = 0;
					blkheader_state = NEED_BLOCKHEADER;
					blkdelta_state = NEED_BLOCKDELTA;
				} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
					goto get_buf_kernel;
				}
				
			}

			/* Get as much Blockheader as possible from this packet */
			memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
			blkheader_state = NEED_BLOCKHEADER;
			loc_blkdiff_buffer += data_size - loc_pbuf;
			blkheader_partial_size_copied = data_size - loc_pbuf;
			goto get_buf_kernel;

		} else { /* Packets other than 1st packet */

			printf("Packet Number - %d\n", packet_num);

			/* If this new packet was needed to get full Blockheader in continuation from previous packet */
			if (blkheader_state == NEED_BLOCKHEADER) {

				/* Get remaining Blockheader from this packet */
				memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], sizeof(blockheader) - blkheader_partial_size_copied);
				blkheader_state = HAVE_FULL_BLOCKHEADER;
				loc_blkdiff_buffer += sizeof(blockheader) - blkheader_partial_size_copied;
				loc_pbuf += sizeof(blockheader) - blkheader_partial_size_copied;
				memcpy(&blockheader, blkdiff_buffer, sizeof(blockheader));

				/* Get Block delta for this block */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				if (loc_pbuf + block_delta_size <= data_size) {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
					loc_blkdiff_buffer += block_delta_size;
					loc_pbuf += block_delta_size;
					blkdelta_state = HAVE_FULL_BLOCKDELTA;

				} else {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
					blkdelta_partial_size_copied = data_size - loc_pbuf;
					loc_blkdiff_buffer += data_size - loc_pbuf;
					loc_pbuf += data_size - loc_pbuf;
					blkdelta_state = NEED_BLOCKDELTA;

				}

				/* Patch if entire delta for block is available */
				if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
					patch(blkdiff_buffer, loc_blkdiff_buffer);
					loc_blkdiff_buffer = 0;
					blkheader_state = NEED_BLOCKHEADER;
					blkdelta_state = NEED_BLOCKDELTA;
				} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
					goto get_buf_kernel;
				}

				/* Patch blocks which are possible with this packet */
				while (loc_pbuf + sizeof(blockheader) < data_size) {
					memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
					memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
					blkheader_state = HAVE_FULL_BLOCKHEADER;
					loc_pbuf += sizeof(blockheader);
					loc_blkdiff_buffer += sizeof(blockheader);

					block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
					if (loc_pbuf + block_delta_size <= data_size) {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
						loc_blkdiff_buffer += block_delta_size;
						loc_pbuf += block_delta_size;
						blkdelta_state = HAVE_FULL_BLOCKDELTA;

					} else {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
						blkdelta_partial_size_copied = data_size - loc_pbuf;
						loc_blkdiff_buffer += data_size - loc_pbuf;
						loc_pbuf += data_size - loc_pbuf;
						blkdelta_state = NEED_BLOCKDELTA;

					}

					if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
						patch(blkdiff_buffer, loc_blkdiff_buffer);
						loc_blkdiff_buffer = 0;
						blkheader_state = NEED_BLOCKHEADER;
						blkdelta_state = NEED_BLOCKDELTA;
					} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
						goto get_buf_kernel;
					}
				}

				/* Get as much Blockheader from this packet as possible */
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
				blkheader_state = NEED_BLOCKHEADER;
				loc_blkdiff_buffer += data_size - loc_pbuf;
				blkheader_partial_size_copied = data_size - loc_pbuf;
				goto get_buf_kernel;
				

			} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {

				/* Get remaining Block delta from this packet */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size - blkdelta_partial_size_copied);
				blkheader_state = HAVE_FULL_BLOCKDELTA;
				loc_blkdiff_buffer += block_delta_size - blkdelta_partial_size_copied;
				loc_pbuf += block_delta_size - blkdelta_partial_size_copied;

				patch(blkdiff_buffer, loc_blkdiff_buffer);
				loc_blkdiff_buffer = 0;
				blkheader_state = NEED_BLOCKHEADER;
				blkdelta_state = NEED_BLOCKDELTA;

				/* Patch blocks which are possible with this packet */
				while (loc_pbuf + sizeof(blockheader) < data_size) {
					memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
					memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
					blkheader_state = HAVE_FULL_BLOCKHEADER;
					loc_pbuf += sizeof(blockheader);
					loc_blkdiff_buffer += sizeof(blockheader);

					block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
					if (loc_pbuf + block_delta_size <= data_size) {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
						loc_blkdiff_buffer += block_delta_size;
						loc_pbuf += block_delta_size;
						blkdelta_state = HAVE_FULL_BLOCKDELTA;

					} else {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
						blkdelta_partial_size_copied = data_size - loc_pbuf;
						loc_blkdiff_buffer += data_size - loc_pbuf;
						loc_pbuf += data_size - loc_pbuf;
						blkdelta_state = NEED_BLOCKDELTA;

					}

					if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
						patch(blkdiff_buffer, loc_blkdiff_buffer);
						loc_blkdiff_buffer = 0;
						blkheader_state = NEED_BLOCKHEADER;
						blkdelta_state = NEED_BLOCKDELTA;
					} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
						goto get_buf_kernel;
					}
				}

				/* Get as much Blockheader from this packet as possible */
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
				blkheader_state = NEED_BLOCKHEADER;
				loc_blkdiff_buffer += data_size - loc_pbuf;
				blkheader_partial_size_copied = data_size - loc_pbuf;
				goto get_buf_kernel;

			}

		}

done_patching_kernel:

		printf("\nPatching with get_buffer implementation successfull for Kernel!!!");

		/* Close opened files */
		if (fclose(ouf.oufile))
			err_msg("Failed to close %s", argv[1]);

		if (fclose(pf.pfile))
			err_msg("Failed to close %s", argv[2]);

		if (fclose(nuf.nufile))
			err_msg("Failed to close %s", argv[3]);

		printf("\nTotal Size of Packets = %lu\n", total_size_packets + sizeof(patchheader));

	} else if (memcmp(argv[1], "/dev/mtdblock2", 14) == 0) {	/* Micom app */

		if (argc != 4)
			err_msg("Support present only for patching (uncompressed ELF) to new file.\nUsage: %s old_file patch_file new_file", argv[0]);

		/* Open patch file */
		if ((pf.pfile = fopen(argv[2], "r")) == NULL)
			err_msg("fopen(%s)", argv[2]);

		/* Open old file */
		if ((ouf.oufile = fopen(argv[1], "r")) == NULL)
			err_msg("fopen(%s)", argv[1]);

		if (fseek(ouf.oufile, 0, SEEK_END))
			err_msg("fseek");

		ouf.total_size = ftell(ouf.oufile);
		ouf.size_for_blocks_calc = ouf.total_size - sizeof(ouf.crc) - sizeof(ouf.binheader);

		/* Seek to start of old uncompressed file */
		if (fseek(ouf.oufile, 0, SEEK_SET))
			err_msg("fseek");

		/* Open new file */
		if ((nuf.nufile = fopen(argv[3], "r+")) == NULL)
			err_msg("fopen(%s)", argv[3]);

		new_file_name = argv[3];

		offset_to_first_block_of = sizeof(nuf.crc) + sizeof(nuf.binheader);
		offset_to_first_block_nf = sizeof(nuf.crc) + sizeof(nuf.binheader);

		/*checking get_buffer */
		/*
		while (1) {
			download_flag = get_buffer(&pbuf, &data_size);
			if (download_flag == false) {
				err_msg("Failed to get packet\n");
			}

			crc_get_buf = crc32_z(0, (const Bytef *)pbuf, data_size);
			printf("Packet No. (%d), Data Size (%u), Packet CRC (%u)\n", num, data_size, crc_get_buf);
			if (data_size == CONFIG_FOTA_DOWNLOAD_PACKET_SIZE)
				num += 1;
			else
				break;
		}*/

		/* get packets and process them */

get_buf_elf:

		/* Get packet is available for download */
		if (got_last_packet == false) {
			download_flag = get_buffer(&pbuf, &data_size);
		} else {
			goto done_patching_elf;
		}

		/* Failed to get packet */
		if (download_flag == false) {
			printf("Failed to get packet\n");
			return 1;
		}

		//printf("data_size [%d] = %d\n", packet_num + 1, data_size);

		/* Last packet if data_size is less than CONFIG_FOTA_DOWNLOAD_PACKET_SIZE */
		if (data_size < CONFIG_FOTA_DOWNLOAD_PACKET_SIZE)
			got_last_packet = true;

		/* Update variables */
		packet_num += 1;
		loc_pbuf = 0;

		/* Processing 1st packet */
		if (packet_num == 0) {

			printf("Packet Number - 0\n");

			/* Get Patchheader */
			memcpy(&patchheader, &pbuf[loc_pbuf], sizeof(patchheader));
			no_common_blocks = MIN(patchheader.blocks_newfile, patchheader.blocks_oldfile);
			loc_pbuf += sizeof(patchheader);

			/* Get CRC for new uncompressed binary */
			memcpy(&nuf.crc, &pbuf[loc_pbuf], sizeof(nuf.crc));
			loc_pbuf += sizeof(nuf.crc);

			/* Write CRC to new file */
			write_crc(nuf.crc);

			/* Get binary header for new uncompressed binary */
			memcpy(&nuf.binheader, &pbuf[loc_pbuf], sizeof(binary_header_t));
			loc_pbuf += sizeof(binary_header_t);

			/* Write binheader to new file */
			write_binheader(nuf.binheader);

			/* Verify Bsdiff patch_signature & assign (io_funcs_t *)io */
			if (memcmp(&patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE) == 0) {
				io = &BLCKDIFF_funcs;
			} else {
				err_msg("Bsdiff signature does not match. Incorrect patch file provided");
			}

			/* Patch blocks which are possible with this packet */
			while (loc_pbuf + sizeof(blockheader) < data_size) {
				/* Get Blockheader */
				memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
				blkheader_state = HAVE_FULL_BLOCKHEADER;
				loc_pbuf += sizeof(blockheader);
				loc_blkdiff_buffer += sizeof(blockheader);

				/* Get as much delta for block as possible from this packet */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				if (loc_pbuf + block_delta_size <= data_size) {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
					loc_blkdiff_buffer += block_delta_size;
					loc_pbuf += block_delta_size;
					blkdelta_state = HAVE_FULL_BLOCKDELTA;

				} else {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
					blkdelta_partial_size_copied = data_size - loc_pbuf;
					loc_blkdiff_buffer += data_size - loc_pbuf;
					loc_pbuf += data_size - loc_pbuf;
					blkdelta_state = NEED_BLOCKDELTA;

				}

				/* Patch block if entire delta available, else get new packet */
				if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
					patch(blkdiff_buffer, loc_blkdiff_buffer);
					loc_blkdiff_buffer = 0;
					blkheader_state = NEED_BLOCKHEADER;
					blkdelta_state = NEED_BLOCKDELTA;
				} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
					goto get_buf_elf;
				}

			}

			/* Get as much Blockheader as possible from this packet */
			memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
			blkheader_state = NEED_BLOCKHEADER;
			loc_blkdiff_buffer += data_size - loc_pbuf;
			blkheader_partial_size_copied = data_size - loc_pbuf;
			goto get_buf_elf;

		} else { /* Packets other than 1st packet */

			printf("Packet Number - %d\n", packet_num);

			/* If this new packet was needed to get full Blockheader in continuation from previous packet */
			if (blkheader_state == NEED_BLOCKHEADER) {

				/* Get remaining Blockheader from this packet */
				memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], sizeof(blockheader) - blkheader_partial_size_copied);
				blkheader_state = HAVE_FULL_BLOCKHEADER;
				loc_blkdiff_buffer += sizeof(blockheader) - blkheader_partial_size_copied;
				loc_pbuf += sizeof(blockheader) - blkheader_partial_size_copied;
				memcpy(&blockheader, blkdiff_buffer, sizeof(blockheader));

				/* Get Block delta for this block */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				if (loc_pbuf + block_delta_size <= data_size) {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
					loc_blkdiff_buffer += block_delta_size;
					loc_pbuf += block_delta_size;
					blkdelta_state = HAVE_FULL_BLOCKDELTA;

				} else {

					memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
					blkdelta_partial_size_copied = data_size - loc_pbuf;
					loc_blkdiff_buffer += data_size - loc_pbuf;
					loc_pbuf += data_size - loc_pbuf;
					blkdelta_state = NEED_BLOCKDELTA;

				}

				/* Patch if entire delta for block is available */
				if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
					patch(blkdiff_buffer, loc_blkdiff_buffer);
					loc_blkdiff_buffer = 0;
					blkheader_state = NEED_BLOCKHEADER;
					blkdelta_state = NEED_BLOCKDELTA;
				} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
					goto get_buf_elf;
				}

				/* Patch blocks which are possible with this packet */
				while (loc_pbuf + sizeof(blockheader) < data_size) {
					memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
					memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
					blkheader_state = HAVE_FULL_BLOCKHEADER;
					loc_pbuf += sizeof(blockheader);
					loc_blkdiff_buffer += sizeof(blockheader);

					block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
					if (loc_pbuf + block_delta_size <= data_size) {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
						loc_blkdiff_buffer += block_delta_size;
						loc_pbuf += block_delta_size;
						blkdelta_state = HAVE_FULL_BLOCKDELTA;

					} else {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
						blkdelta_partial_size_copied = data_size - loc_pbuf;
						loc_blkdiff_buffer += data_size - loc_pbuf;
						loc_pbuf += data_size - loc_pbuf;
						blkdelta_state = NEED_BLOCKDELTA;

					}

					if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
						patch(blkdiff_buffer, loc_blkdiff_buffer);
						loc_blkdiff_buffer = 0;
						blkheader_state = NEED_BLOCKHEADER;
						blkdelta_state = NEED_BLOCKDELTA;
					} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
						goto get_buf_elf;
					}
				}

				/* Get as much Blockheader from this packet as possible */
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
				blkheader_state = NEED_BLOCKHEADER;
				loc_blkdiff_buffer += data_size - loc_pbuf;
				blkheader_partial_size_copied = data_size - loc_pbuf;
				goto get_buf_elf;


			} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {

				/* Get remaining Block delta from this packet */
				block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
				memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size - blkdelta_partial_size_copied);
				blkheader_state = HAVE_FULL_BLOCKDELTA;
				loc_blkdiff_buffer += block_delta_size - blkdelta_partial_size_copied;
				loc_pbuf += block_delta_size - blkdelta_partial_size_copied;

				patch(blkdiff_buffer, loc_blkdiff_buffer);
				loc_blkdiff_buffer = 0;
				blkheader_state = NEED_BLOCKHEADER;
				blkdelta_state = NEED_BLOCKDELTA;

				/* Patch blocks which are possible with this packet */
				while (loc_pbuf + sizeof(blockheader) < data_size) {
					memcpy(&blockheader, &pbuf[loc_pbuf], sizeof(blockheader));
					memcpy(blkdiff_buffer, &pbuf[loc_pbuf], sizeof(blockheader));
					blkheader_state = HAVE_FULL_BLOCKHEADER;
					loc_pbuf += sizeof(blockheader);
					loc_blkdiff_buffer += sizeof(blockheader);

					block_delta_size = blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block;
					if (loc_pbuf + block_delta_size <= data_size) {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], block_delta_size);
						loc_blkdiff_buffer += block_delta_size;
						loc_pbuf += block_delta_size;
						blkdelta_state = HAVE_FULL_BLOCKDELTA;

					} else {

						memcpy(&blkdiff_buffer[loc_blkdiff_buffer], &pbuf[loc_pbuf], data_size - loc_pbuf);
						blkdelta_partial_size_copied = data_size - loc_pbuf;
						loc_blkdiff_buffer += data_size - loc_pbuf;
						loc_pbuf += data_size - loc_pbuf;
						blkdelta_state = NEED_BLOCKDELTA;

					}

					if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == HAVE_FULL_BLOCKDELTA) {
						patch(blkdiff_buffer, loc_blkdiff_buffer);
						loc_blkdiff_buffer = 0;
						blkheader_state = NEED_BLOCKHEADER;
						blkdelta_state = NEED_BLOCKDELTA;
					} else if (blkheader_state == HAVE_FULL_BLOCKHEADER && blkdelta_state == NEED_BLOCKDELTA) {
						goto get_buf_elf;
					}
				}

				/* Get as much Blockheader from this packet as possible */
				memcpy(blkdiff_buffer, &pbuf[loc_pbuf], data_size - loc_pbuf);
				blkheader_state = NEED_BLOCKHEADER;
				loc_blkdiff_buffer += data_size - loc_pbuf;
				blkheader_partial_size_copied = data_size - loc_pbuf;
				goto get_buf_elf;

			}

		}

done_patching_elf:

		printf("\nPatching with get_buffer implementation successfull for ELF!!!");

		/* Close opened files */
		if (fclose(ouf.oufile))
			err_msg("Failed to close %s", argv[1]);

		if (fclose(pf.pfile))
			err_msg("Failed to close %s", argv[2]);

		if (fclose(nuf.nufile))
			err_msg("Failed to close %s", argv[3]);

		printf("\nTotal Size of Packets = %lu\n", total_size_packets + sizeof(patchheader) + sizeof(nuf.crc) + sizeof(nuf.binheader));

	}

	return 0;
}
