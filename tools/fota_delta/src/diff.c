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
 * Copyright 2003 - 2005 Colin Percival
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
__FBSDID("$FreeBSD: src/usr.bin/bsdiff/bsdiff/bsdiff.c, v 1.1 2005/08/06 01:59:05 cperciva Exp $");
#endif

/**************** Included Header files *****************/

#include <apps/system/diff.h>
#include <sys/param.h>
#include <tinyara/config.h>

/*****************  Private Variables *******************/

struct patch_header patchheader;	/* Header for output patch file */
struct block_diff_header blockheader;	/* Header info for diff b/w 2 blocks */

old_uncomp_file ouf;
new_uncomp_file nuf;
new_comp_file ncf;
patch_file pf;

/****************  Function Definitions *****************/

static void split(off64_t *I, off64_t *V, off64_t start, off64_t len, off64_t h)
{
	off64_t i, j, k, x, tmp, jj, kk;

	if (len < 16) {
		for (k = start; k < start + len; k += j) {
			j = 1;
			x = V[I[k] + h];
			for (i = 1; k + i < start + len; i++) {
				if (V[I[k + i] + h] < x) {
					x = V[I[k + i] + h];
					j = 0;
				};
				if (V[I[k + i] + h] == x) {
					tmp = I[k + j];
					I[k + j] = I[k + i];
					I[k + i] = tmp;
					j++;
				};
			};
			for (i = 0; i < j; i++) {
				V[I[k + i]] = k + j - 1;
			}
			if (j == 1) {
				I[k] = -1;
			}
		};
		return;
	};

	x = V[I[start + len / 2] + h];
	jj = 0;
	kk = 0;
	for (i = start; i < start + len; i++) {
		if (V[I[i] + h] < x) {
			jj++;
		}
		if (V[I[i] + h] == x) {
			kk++;
		}
	};
	jj += start;
	kk += jj;

	i = start;
	j = 0;
	k = 0;
	while (i < jj) {
		if (V[I[i] + h] < x) {
			i++;
		} else if (V[I[i] + h] == x) {
			tmp = I[i];
			I[i] = I[jj + j];
			I[jj + j] = tmp;
			j++;
		} else {
			tmp = I[i];
			I[i] = I[kk + k];
			I[kk + k] = tmp;
			k++;
		};
	};

	while (jj + j < kk) {
		if (V[I[jj + j] + h] == x) {
			j++;
		} else {
			tmp = I[jj + j];
			I[jj + j] = I[kk + k];
			I[kk + k] = tmp;
			k++;
		};
	};

	if (jj > start) {
		split(I, V, start, jj - start, h);
	}

	for (i = 0; i < kk - jj; i++) {
		V[I[jj + i]] = kk - 1;
	}
	if (jj == kk - 1) {
		I[jj] = -1;
	}

	if (start + len > kk) {
		split(I, V, kk, start + len - kk, h);
	}
}

/* qsufsort(I, V, old, oldsize)
 *
 * Computes the suffix sort of the string at 'old' and stores the resulting
 * indices in 'I', using 'V' as a temporary array for the computation. */
static void qsufsort(off64_t *I, off64_t *V, u_char *old, off64_t oldsize)
{
	off64_t buckets[256];
	off64_t i, h, len;

	/* count number of each byte  */
	for (i = 0; i < 256; i++) {
		buckets[i] = 0;
	}
	for (i = 0; i < oldsize; i++) {
		buckets[old[i]]++;
	}
	/* make buckets cumulative */
	for (i = 1; i < 256; i++) {
		buckets[i] += buckets[i - 1];
	}
	/* shift right by one */
	for (i = 255; i > 0; i--) {
		buckets[i] = buckets[i - 1];
	}
	buckets[0] = 0;
	/* at this point, buckets[c] is the number of bytes in the old file with
	 * value less than c. */

	/* set up the sort order of the suffixes based solely on the first
	 * character */
	for (i = 0; i < oldsize; i++) {
		I[++buckets[old[i]]] = i;
	}
	I[0] = oldsize;
	/* ? */
	for (i = 0; i < oldsize; i++) {
		V[i] = buckets[old[i]];
	}
	V[oldsize] = 0;
	/* forward any entries in the ordering which have the same initial
	 * character */
	for (i = 1; i < 256; i++) {
		if (buckets[i] == buckets[i - 1] + 1) {
			I[buckets[i]] = -1;
		}
	}
	I[0] = -1;

	for (h = 1; I[0] != -(oldsize + 1); h += h) {
		len = 0;
		for (i = 0; i < oldsize + 1;) {
			if (I[i] < 0) {
				len -= I[i];
				i -= I[i];
			} else {
				if (len) {
					I[i - len] = -len;
				}
				len = V[I[i]] + 1 - i;
				split(I, V, i, len, h);
				i += len;
				len = 0;
			}
		}
		if (len) {
			I[i - len] = -len;
		}
	};

	for (i = 0; i < oldsize + 1; i++) {
		I[V[i]] = i;
	}
}

/* matchlen(old, oldsize, new, newsize)
 *
 * Returns the length of the longest common prefix between 'old' and 'new'. */
static off64_t matchlen(u_char *old, off64_t oldsize, u_char *new, off64_t newsize)
{
	off64_t i;

	for (i = 0; (i < oldsize) && (i < newsize); i++) {
		if (old[i] != new[i]) {
			break;
		}
	}

	return i;
}

/* search(I, old, oldsize, new, newsize, st, en, pos)
 *
 * Searches for the longest prefix of 'new' that occurs in 'old', stores its
 * offset in '*pos', and returns its length. 'I' should be the suffix sort of
 * 'old', and 'st' and 'en' are the lowest and highest indices in the suffix
 * sort to consider. If you're searching all suffixes, 'st = 0' and 'en =
 * oldsize - 1'. */
static off64_t search(off64_t *I, u_char *old, off64_t oldsize, u_char *new, off64_t newsize, off64_t st, off64_t en, off64_t *pos)
{
	off64_t x, y;

	if (en - st < 2) {
		x = matchlen(old + I[st], oldsize - I[st], new, newsize);
		y = matchlen(old + I[en], oldsize - I[en], new, newsize);

		if (x > y) {
			*pos = I[st];
			return x;
		} else {
			*pos = I[en];
			return y;
		}
	}

	x = st + (en - st) / 2;
	if (memcmp(old + I[x], new, MIN(oldsize - I[x], newsize)) < 0) {
		return search(I, old, oldsize, new, newsize, x, en, pos);
	} else {
		return search(I, old, oldsize, new, newsize, st, x, pos);
	};
}

/* offtout(x, buf)
 *
 * Writes the off64_t 'x' portably to the array 'buf'. */
static void offtout(off64_t x, u_char *buf)
{
	off64_t y;

	if (x < 0) {
		y = -x;
	} else {
		y = x;
	}

	buf[0] = y % 256;
	y -= buf[0];
	y = y / 256;
	buf[1] = y % 256;
	y -= buf[1];
	y = y / 256;
	buf[2] = y % 256;
	y -= buf[2];
	y = y / 256;
	buf[3] = y % 256;
	y -= buf[3];
	y = y / 256;
	buf[4] = y % 256;
	y -= buf[4];
	y = y / 256;
	buf[5] = y % 256;
	y -= buf[5];
	y = y / 256;
	buf[6] = y % 256;
	y -= buf[6];
	y = y / 256;
	buf[7] = y % 256;

	if (x < 0) {
		buf[7] |= 0x80;
	}
}

/* Core Algorithm of Bsdiff; Writing block diff data to patch file side-by-side */
void bsdiff(u_char *old, u_char *new, char *db, char *eb, off64_t *I, off64_t *V, int bytes_to_read_ou, int bytes_to_read_nu)
{
	off64_t oldsize, newsize;	/* Variables used by Bsdiff; Correspond to size of old/new blocks */
	off64_t scan;				/* position of current match in old file */
	off64_t pos = 0;			/* position of current match in new file */
	off64_t len;				/* length of current match */
	off64_t lastscan;			/* position of previous match in old file */
	off64_t lastpos;			/* position of previous match in new file */
	off64_t lastoffset;			/* lastpos - lastscan */
	off64_t oldscore, scsc;		/* temp variables in match search */
	off64_t s, Sf, lenf, Sb, lenb;	/* temp vars in match extension */
	off64_t overlap, Ss, lens;
	off64_t i;
	off64_t dblen, eblen, ctrllen;	/* length of diff, extra, control sections */
	u_char buf[8];

	int rc;
	off64_t pf_offset_old, pf_offset_new;	/* Used for finding size of GZIP compressed write into patch file */

	/* Assign this offset as start of control section of diff data */
	pf_offset_old = ftell(pf.pfile);

	/* Open file for GZIP compressed write (for control section of diff data) */
	if ((pf.pfgz = gzdopen(dup(fileno(pf.pfile)), "w")) == NULL) {
		err_msg("gzopen, errno = %d", errno);
	}

	/* Update size of data in old/new buffers */
	newsize = bytes_to_read_nu;
	oldsize = bytes_to_read_ou;

	/* Do a suffix sort on block from old file */
	qsufsort(I, V, old, oldsize);

	/* Compute the differences, writing ctrl as we go */
	dblen = 0;
	eblen = 0;
	ctrllen = 0;

	scan = 0;
	len = 0;
	lastscan = 0;
	lastpos = 0;
	lastoffset = 0;
	/* This while loop contains Bsdiff internal logic for creating ctrl, diff and extra sections */
	while (scan < newsize) {
		oldscore = 0;

		for (scsc = scan += len; scan < newsize; scan++) {
			len = search(I, old, oldsize, new + scan, newsize - scan, 0, oldsize, &pos);

			for (; scsc < scan + len; scsc++)
				if ((scsc + lastoffset < oldsize) && (old[scsc + lastoffset] == new[scsc])) {
					oldscore++;
				}

			if (((len == oldscore) && (len != 0)) || (len > oldscore + 8)) {
				break;
			}

			if ((scan + lastoffset < oldsize) && (old[scan + lastoffset] == new[scan])) {
				oldscore--;
			}
		};

		if ((len != oldscore) || (scan == newsize)) {
			s = 0;
			Sf = 0;
			lenf = 0;
			for (i = 0; (lastscan + i < scan) && (lastpos + i < oldsize);) {
				if (old[lastpos + i] == new[lastscan + i]) {
					s++;
				}
				i++;
				if (s * 2 - i > Sf * 2 - lenf) {
					Sf = s;
					lenf = i;
				};
			};

			lenb = 0;
			if (scan < newsize) {
				s = 0;
				Sb = 0;
				for (i = 1; (scan >= lastscan + i) && (pos >= i); i++) {
					if (old[pos - i] == new[scan - i]) {
						s++;
					}
					if (s * 2 - i > Sb * 2 - lenb) {
						Sb = s;
						lenb = i;
					};
				};
			};

			if (lastscan + lenf > scan - lenb) {
				overlap = (lastscan + lenf) - (scan - lenb);
				s = 0;
				Ss = 0;
				lens = 0;
				for (i = 0; i < overlap; i++) {
					if (new[lastscan + lenf - overlap + i] == old[lastpos + lenf - overlap + i]) {
						s++;
					}
					if (new[scan - lenb + i] == old[pos - lenb + i]) {
						s--;
					}
					if (s > Ss) {
						Ss = s;
						lens = i + 1;
					};
				};

				lenf += lens - overlap;
				lenb -= lens;
			};

			for (i = 0; i < lenf; i++) {
				db[dblen + i] = new[lastscan + i] - old[lastpos + i];
			}
			for (i = 0; i < (scan - lenb) - (lastscan + lenf); i++) {
				eb[eblen + i] = new[lastscan + lenf + i];
			}

			dblen += lenf;
			eblen += (scan - lenb) - (lastscan + lenf);

			/*
			 * Write the following triple of integers to the control section:
			 *  - length of the diff
			 *  - length of the extra section
			 *  - offset between the end of the diff and the start of the next
			 *      diff, in the old file
			 */

			offtout(lenf, buf);
			if (gzfwrite(buf, 8, 1, pf.pfgz) != 1) {
				err_msg("gzfwrite, errno = %d", errno);
			}

			offtout((scan - lenb) - (lastscan + lenf), buf);
			if (gzfwrite(buf, 8, 1, pf.pfgz) != 1) {
				err_msg("gzfwrite, errno = %d", errno);
			}

			offtout((pos - lenb) - (lastpos + lenf), buf);
			if (gzfwrite(buf, 8, 1, pf.pfgz) != 1) {
				err_msg("gzfwrite, errno = %d", errno);
			}

			lastscan = scan - lenb;
			lastpos = pos - lenb;
			lastoffset = pos - scan;
		};
	};

	/* Close GZIP compressed write of control section */
	rc = gzclose(pf.pfgz);
	if (rc != Z_OK) {
		err_msg("gzclose, gzerr = %d", rc);
	}

	/* Seek to end of patch file; This marks end of control section */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("Failed to seek to end of patch file");
	}

	/* Calculate size of GZIP Compressed Control section */
	pf_offset_new = ftell(pf.pfile);
	ctrllen = pf_offset_new - pf_offset_old;

	/* Assign size of ctrl data */
	blockheader.size_ctrl_block = ctrllen;

	/* Mark offset of beginning of Diff section */
	pf_offset_old = pf_offset_new;

	/* Open file for GZIP Compressed Write (of Diff section of diff data) */
	if ((pf.pfgz = gzdopen(dup(fileno(pf.pfile)), "w")) == NULL) {
		err_msg("gzopen, errno = %d", errno);
	}

	/* Write diff data to patch file */
	if (dblen && gzfwrite(db, dblen, 1, pf.pfgz) != 1) {
		err_msg("gzfwrite, errno = %d", errno);
	}

	/* Close GZIP File for write */
	rc = gzclose(pf.pfgz);
	if (rc != Z_OK) {
		err_msg("gzclose, gzerr = %d", rc);
	}

	/* Seek to end of patch file; This marks end of Diff section */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("Failed to seek to end of patch file");
	}

	/* Calculate size of GZIP Compressed Diff section */
	pf_offset_new = ftell(pf.pfile);
	dblen = pf_offset_new - pf_offset_old;

	/* Assign size of diff data */
	blockheader.size_diff_block = dblen;

	/* Mark offset of beginning of Extra section */
	pf_offset_old = pf_offset_new;

	/* Open file for GZIP Compressed Write (of Extra section of diff data) */
	if ((pf.pfgz = gzdopen(dup(fileno(pf.pfile)), "w")) == NULL) {
		err_msg("gzopen, errno = %d", errno);
	}

	/* Write extra data to patch file */
	if (eblen && gzfwrite(eb, eblen, 1, pf.pfgz) != 1) {
		err_msg("gzfwrite, errno = %d", errno);
	}

	/* Close GZIP File for write */
	rc = gzclose(pf.pfgz);
	if (rc != Z_OK) {
		err_msg("gzclose, gzerr = %d", rc);
	}

	/* Seek to end of patch file; This marks end of Extra section */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("Failed to seek to end of patch file");
	}

	/* Calculate size of GZIP Compressed Extra section */
	pf_offset_new = ftell(pf.pfile);
	eblen = pf_offset_new - pf_offset_old;

	/* Assign size of Compressed Extra data */
	blockheader.size_extra_block = eblen;
}

/* Calculate, Update and write CRC for patchfile */
void patch_file_crc_calculation(char *pf_crc_buffer, char *p_file)
{
	int pf_blocks;				/* Number of blocks in patch file with CRC_BLOCKSIZE */
	unsigned int pf_crc;		/* patch file crc */

	/* Seek to the end of patch file to get patchfile size */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("fseek");
	}

	pf.total_size = ftell(pf.pfile);

	/* Seek to location of data from where to start CRC calculation of patch file */
	if (fseek(pf.pfile, sizeof(patchheader.crc_patchfile), SEEK_SET)) {
		err_msg("fseek");
	}

	/* Calculate no. of blocks CRC_BLOCKSIZE divides patch file into */
	pf.size_for_crc_calc = pf.total_size - sizeof(patchheader.crc_patchfile);
	pf_blocks = pf.size_for_crc_calc / CRC_BLOCKSIZE;
	if (pf.size_for_crc_calc % CRC_BLOCKSIZE) {
		pf_blocks++;
	}

	/* Compute crc block-by-block, using crc for previous block as base for current block CRC */
	pf_crc = 0;
	for (int cnt = 0; cnt < pf_blocks; cnt++) {
		if (cnt != pf_blocks - 1) {
			if (fread(pf_crc_buffer, CRC_BLOCKSIZE, 1, pf.pfile) != 1) {
				err_msg("Read failed for block %d for %s", cnt, p_file);
			}

			pf_crc = crc32_z(pf_crc, (const Bytef *)pf_crc_buffer, CRC_BLOCKSIZE);
		} else {
			if (fread(pf_crc_buffer, (pf.size_for_crc_calc % CRC_BLOCKSIZE != 0 ? pf.size_for_crc_calc % CRC_BLOCKSIZE : CRC_BLOCKSIZE), 1, pf.pfile) != 1) {
				err_msg("Read failed for block %d for %s", cnt, p_file);
			}

			pf_crc = crc32_z(pf_crc, (const Bytef *)pf_crc_buffer, (pf.size_for_crc_calc % CRC_BLOCKSIZE != 0 ? pf.size_for_crc_calc % CRC_BLOCKSIZE : CRC_BLOCKSIZE));
		}
	}

	patchheader.crc_patchfile = pf_crc;

	/* Seek to start of patch file and write CRC value */
	if (fseek(pf.pfile, 0, SEEK_SET)) {
		err_msg("fseek");
	}

	/* Write Patch header to patch file */
	if (fwrite(&patchheader.crc_patchfile, sizeof(patchheader.crc_patchfile), 1, pf.pfile) != 1) {
		err_msg("fwrite(%s)", p_file);
	}
}

/* Write extra block from new comp. file to patch file */
void write_extra_block(u_char *new)
{
	off64_t pf_offset_old, pf_offset_new;	/* Used for finding size of GZIP compressed write into patch file */
	int rc;

	/* Assign this offset as start of diff data */
	pf_offset_old = ftell(pf.pfile);

	/* Open file for GZIP Compressed Write (of Diff section of diff data) */
	if ((pf.pfgz = gzdopen(dup(fileno(pf.pfile)), "w")) == NULL) {
		err_msg("gzopen, errno = %d", errno);
	}

	/* Write block (our diff data) as such to patch file */
	if (blockheader.new_block_size_u != 0 && gzfwrite(new, blockheader.new_block_size_u, 1, pf.pfgz) != 1) {
		err_msg("gzfwrite, errno = %d", errno);
	}

	/* Close GZIP File for write */
	rc = gzclose(pf.pfgz);
	if (rc != Z_OK) {
		err_msg("gzclose, gzerr = %d", rc);
	}

	/* Seek to end of patch file; This marks end of diff section */
	if (fseek(pf.pfile, 0, SEEK_END)) {
		err_msg("Failed to seek to end of patch file");
	}

	/* Calculate size of GZIP Compressed Diff section */
	pf_offset_new = ftell(pf.pfile);
	blockheader.size_extra_block = pf_offset_new - pf_offset_old;
}

/* Write blockheader to patch file for extra blocks */
void write_blockheader_extra_blocks(char *p_file)
{
	int rpos;

	/* Seek to starting of block info in patch file */
	rpos = fseek(pf.pfile, -(blockheader.size_extra_block + sizeof(blockheader)), SEEK_CUR);
	if (rpos != 0) {
		err_msg("fseek : failed seek in file %s", p_file);
	}

	/* Write blockheader to patch file */
	if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
		err_msg("fwrite : failed write of blockheader to %s", p_file);
	}

	/* Seek to end of block info */
	rpos = fseek(pf.pfile, blockheader.size_extra_block, SEEK_CUR);
	if (rpos != 0) {
		err_msg("fseek : failed seek in file %s", p_file);
	}
}

/* Write blockheader to patch file for common blocks */
void write_blockheader_common_blocks(char *p_file)
{
	int rpos;

	/* Write blockheader info into patch file */
	if (blockheader.token == SKIP) {
		/* Assign sizes for diff, ctrl, extra sections in blockheader */
		blockheader.size_ctrl_block = 0;
		blockheader.size_diff_block = 0;
		blockheader.size_extra_block = 0;

		/* Already at required location in patch file; Write block header */
		if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
			err_msg("fwrite : failed write of blockheader to %s", p_file);
		}

	} else if (blockheader.token == MODIFY) {
		/* Seek to starting of block info in patch file */
		rpos = fseek(pf.pfile, -(blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block + sizeof(blockheader)), SEEK_CUR);
		if (rpos != 0) {
			err_msg("fseek : failed seek in file %s", p_file);
		}

		/* Write blockheader at this location */
		if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
			err_msg("fwrite : failed for blockheader to %s", p_file);
		}

		/* Seek to end of block info */
		rpos = fseek(pf.pfile, blockheader.size_ctrl_block + blockheader.size_diff_block + blockheader.size_extra_block, SEEK_CUR);
		if (rpos != 0) {
			err_msg("fseek : failed seek in file %s", p_file);
		}
	}
}

/* Read extra blocks from new uncompressed file to new buffers */
void read_extra_blocks_from_files(int bytes_to_read_nu, u_char *new, int block_index, char *nu_file)
{
	int nbytes;

	nbytes = read(nuf.fd, new, bytes_to_read_nu);
	if (nbytes != bytes_to_read_nu) {
		err_msg("Failed read of block %d from %s", block_index, nu_file);
	}
}

/* Read common blocks from old/new uncompressed files to old, new buffers */
void read_common_blocks_from_files(int bytes_to_read_ou, int bytes_to_read_nu, u_char *old, u_char *new, int block_index, char *ou_file, char *nu_file)
{
	int nbytes;

	nbytes = read(ouf.fd, old, bytes_to_read_ou);
	if (nbytes != bytes_to_read_ou) {
		err_msg("Failed read of block %d from %s", block_index, ou_file);
	}

	nbytes = read(nuf.fd, new, bytes_to_read_nu);
	if (nbytes != bytes_to_read_nu) {
		err_msg("Failed read of block %d from %s", block_index, nu_file);
	}
}

/* Open and Parse Old Uncompressed File. Read CRC, Binary Header */
void parse_old_uncomp_file(char *ou_file)
{
	int nbytes;

	/* Opening old uncompressed file and getting filesize */
	if (((ouf.fd = open(ou_file, O_RDONLY, 0)) < 0) || ((ouf.total_size = lseek(ouf.fd, 0, SEEK_END)) == -1) || (lseek(ouf.fd, 0, SEEK_SET) != 0)) {
		err_msg("%s", ou_file);
	}

	/* Reading crc */
	nbytes = read(ouf.fd, &ouf.crc, sizeof(ouf.crc));
	if (nbytes != sizeof(ouf.crc)) {
		err_msg("CRC read failed for %s", ou_file);
	}

	/* Reading Binary Header */
	nbytes = read(ouf.fd, &ouf.binheader, sizeof(binary_header_t));
	if (nbytes != sizeof(binary_header_t)) {
		err_msg("Binary Header read failed for %s", ou_file);
	}

	/* Assign size of file without CRC, Bin Header */
	ouf.size_for_blocks_calc = ouf.total_size - sizeof(binary_header_t) - sizeof(ouf.crc);
}

/* Open and Parse New Uncompressed File. Read CRC, Binary Header */
void parse_new_uncomp_file(char *nu_file)
{
	int nbytes;

	/* Opening file and getting filesize */
	if (((nuf.fd = open(nu_file, O_RDONLY, 0)) < 0) || ((nuf.total_size = lseek(nuf.fd, 0, SEEK_END)) == -1) || (lseek(nuf.fd, 0, SEEK_SET) != 0)) {
		err_msg("%s", nu_file);
	}

	/* Reading crc */
	nbytes = read(nuf.fd, &nuf.crc, sizeof(nuf.crc));
	if (nbytes != sizeof(nuf.crc)) {
		err_msg("CRC read failed for %s", nu_file);
	}

	/* Reading Binary Header */
	nbytes = read(nuf.fd, &nuf.binheader, sizeof(binary_header_t));
	if (nbytes != sizeof(binary_header_t)) {
		err_msg("Binary Header read failed for %s", nu_file);
	}

	/* Assign size of file without CRC, Bin Header */
	nuf.size_for_blocks_calc = nuf.total_size - sizeof(binary_header_t) - sizeof(nuf.crc);
}

#if CONFIG_COMPRESSED_BINARY == 1	/* Create Patch for Compressed Binary */

/* Calculate crc for block from new comp. file.
 * CRC Value is assigned in blockheader.
 */
void calculate_block_crc_comp(char *block_crc_buffer, char *nc_file, int block_index)
{
	int nbytes;
	unsigned int block_crc;

	nbytes = read(ncf.fd, block_crc_buffer, blockheader.new_block_size_c);
	if (nbytes != blockheader.new_block_size_c) {
		err_msg("Failed read of block %d from %s", block_index, nc_file);
	}

	block_crc = 0;
	block_crc = crc32_z(block_crc, (const Bytef *)block_crc_buffer, blockheader.new_block_size_c);
	blockheader.new_block_crc = block_crc;
}

/* Calculate bytes to read for extra blocks from new uncompressed file for block diff */
void calculate_bytes_to_read_extra_blocks(int block_index, int no_common_blocks, int no_extra_blocks, int *bytes_to_read_nu)
{
	/* Get bytes to read for newfile for current block */
	if (block_index != no_common_blocks + no_extra_blocks - 1) {
		*bytes_to_read_nu = ncf.compheader->blocksize;
	} else {
		*bytes_to_read_nu = (nuf.size_for_blocks_calc % ncf.compheader->blocksize != 0 ? nuf.size_for_blocks_calc % ncf.compheader->blocksize : ncf.compheader->blocksize);
	}
}

/* Calculate bytes to read for common blocks from old/new uncompressed files for block diff */
void calculate_bytes_to_read_common_blocks(int block_index, int no_common_blocks, int no_blocks_nu, int no_blocks_ou, int *bytes_to_read_ou, int *bytes_to_read_nu)
{
	/*
	 * Calculate bytes to read from files for current block diff
	 *
	 * Cases :
	 * - This is not the last common block.
	 * - This is last common block:
	 *      1.) Both files have same number of blocks
	 *      2.) New file has less blocks than old file
	 *      3.) New file has more blocks than old file
	 */
	if (block_index != no_common_blocks - 1) {
		*bytes_to_read_ou = ncf.compheader->blocksize;
		*bytes_to_read_nu = ncf.compheader->blocksize;
	} else {
		if (no_blocks_nu == no_blocks_ou) {
			*bytes_to_read_nu = (nuf.size_for_blocks_calc % ncf.compheader->blocksize != 0 ? nuf.size_for_blocks_calc % ncf.compheader->blocksize : ncf.compheader->blocksize);
			*bytes_to_read_ou = (ouf.size_for_blocks_calc % ncf.compheader->blocksize != 0 ? ouf.size_for_blocks_calc % ncf.compheader->blocksize : ncf.compheader->blocksize);
		} else if (no_blocks_nu < no_blocks_ou) {
			*bytes_to_read_ou = ncf.compheader->blocksize;
			*bytes_to_read_nu = (nuf.size_for_blocks_calc % ncf.compheader->blocksize != 0 ? nuf.size_for_blocks_calc % ncf.compheader->blocksize : ncf.compheader->blocksize);
		} else {
			*bytes_to_read_nu = ncf.compheader->blocksize;
			*bytes_to_read_ou = (ouf.size_for_blocks_calc % ncf.compheader->blocksize != 0 ? ouf.size_for_blocks_calc % ncf.compheader->blocksize : ncf.compheader->blocksize);
		}
	}
}

/* Malloc for buffers */
void malloc_for_buffers(char **db, char **eb, off64_t **I, off64_t **V, u_char **old, u_char **new, char **block_crc_buffer, char **pf_crc_buffer)
{
	/* Buffers for diff/extra sections; max size needed is blocksize */
	if (((*db = malloc(ncf.compheader->blocksize + 1)) == NULL) || ((*eb = malloc(ncf.compheader->blocksize + 1)) == NULL)) {
		err_msg("Malloc of db/eb failed");
	}

	/* Malloc for I/V buffers, used for suffix sort */
	if (((*I = malloc((ncf.compheader->blocksize + 1) * sizeof(off64_t))) == NULL) || ((*V = malloc((ncf.compheader->blocksize + 1) * sizeof(off64_t))) == NULL)) {
		err_msg("malloc for I/V buffers failed");
	}

	/* Malloc for old/new buffers into which blocks will be read */
	if (((*old = malloc((ncf.compheader->blocksize + 1) * sizeof(u_char))) == NULL) || ((*new = malloc((ncf.compheader->blocksize + 1) * sizeof(u_char))) == NULL)) {
		err_msg("malloc for old/new buffers failed");
	}

	/* Buffer for CRC calculation of blocks */
	if ((*block_crc_buffer = malloc((ncf.compheader->blocksize + 1) * sizeof(char))) == NULL) {
		err_msg("malloc for crc_buffer failed");
	}

	/* Buffer for CRC calculation of patch file */
	if ((*pf_crc_buffer = malloc((CRC_BLOCKSIZE + 1) * sizeof(char))) == NULL) {
		err_msg("malloc failed for pf_crc_buffer");
	}
}

/* Open Patch File for write. Write CRC, Binary Header, Comp. header */
void initialize_patch_file(char *p_file)
{
	int rpos;

	/* Create the patch file */
	if ((pf.pfile = fopen(p_file, "w+")) == NULL) {
		err_msg("%s", p_file);
	}

	/* Seek to position where we will write crc for new compressed file */
	rpos = fseek(pf.pfile, sizeof(patchheader), SEEK_SET);
	if (rpos != 0) {
		err_msg("Seek failed to end of patch header for %s", p_file);
	}

	/* Copying crc for new compressed file to patch file */
	if (fwrite(&ncf.crc, sizeof(ncf.crc), 1, pf.pfile) != 1) {
		err_msg("Failed write of crc for new compressed file to %s", p_file);
	}

	/* Copying binary header for new compressed file to patch file */
	if (fwrite(&ncf.binheader, sizeof(binary_header_t), 1, pf.pfile) != 1) {
		err_msg("Failed write of binary header for new compressed file to %s", p_file);
	}

	/* Copying compression header for new compressed file to patch file */
	if (fwrite(ncf.compheader, ncf.compheader_size, 1, pf.pfile) != 1) {
		err_msg("Failed write of compression header for new compressed file to %s", p_file);
	}
}

/* Open and Parse New Compressed File. Read CRC, Binary Header, Compression Header */
void parse_new_comp_file(char *nc_file)
{
	int nbytes, rpos;

	/* Opening File and getting filesize */
	if (((ncf.fd = open(nc_file, O_RDONLY, 0)) < 0) || ((ncf.total_size = lseek(ncf.fd, 0, SEEK_END)) == -1) || (lseek(ncf.fd, 0, SEEK_SET) != 0)) {
		err_msg("%s", nc_file);
	}

	/* Reading checksum */
	nbytes = read(ncf.fd, &ncf.crc, sizeof(ncf.crc));
	if (nbytes != sizeof(ncf.crc)) {
		err_msg("CRC read failed for %s", nc_file);
	}

	/* Reading Binary Header */
	nbytes = read(ncf.fd, &ncf.binheader, sizeof(binary_header_t));
	if (nbytes != sizeof(binary_header_t)) {
		err_msg("Binary Header read failed for %s", nc_file);
	}

	/* Reading Compression Header size */
	nbytes = read(ncf.fd, &ncf.compheader_size, sizeof(ncf.compheader_size));
	if (nbytes != sizeof(ncf.compheader_size)) {
		err_msg("Compheader size read failed for %s", nc_file);
	}

	rpos = lseek(ncf.fd, -sizeof(ncf.compheader_size), SEEK_CUR);
	if (rpos != (sizeof(binary_header_t) + sizeof(ncf.crc))) {
		err_msg("Lseek failed to start of compression header for %s", nc_file);
	}

	/* Malloc for compheader (now that we know its size) */
	if ((ncf.compheader = malloc(ncf.compheader_size)) == NULL) {
		err_msg("malloc failed for compheader for %s", nc_file);
	}

	/* Read Compression Header */
	nbytes = read(ncf.fd, ncf.compheader, ncf.compheader_size);
	if (nbytes != ncf.compheader_size) {
		err_msg("Read failed for compheader for %s", nc_file);
	}
}

int main(int argc, char *argv[])
{
	int no_common_blocks = 0;	/* Represents lesser of (no. of blocks in oldfile, no. blocks in new file) */
	int no_extra_blocks = 0;	/* Represents no. of extra blocks in old/new file, whichever one has more */
	int bytes_to_read_ou, bytes_to_read_nu;	/* Bytes to read for blockwise diff from old/new uncomp. files */
	int no_blocks_ou, no_blocks_nu;	/* No. of blocks in old/new uncomp. files based on blocksize for compression */
	int rpos;

	/* Buffers needed by Bsdiff */
	char *db, *eb;				/* contents of diff, extra sections */
	u_char *old, *new;			/* contents of old, new files */
	off64_t *I, *V;				/* arrays used for suffix sort; I is ordering */

	/* Buffers for CRC calculation */
	char *block_crc_buffer;		/* Buffer for CRC calculation of blocks from new compressed file */
	char *pf_crc_buffer;		/* Buffer for CRC calculation of patch file */

	if (argc != 5) {
		err_msg("Create Patch for Compressed Binary (on device)\nusage:\n\n%s  old_uncompressed_file" "  new_uncompressed_file  new_compressed_file  patchfile\n", argv[0]);
	}

	/* Open and Parse metadata from New Compressed File */
	parse_new_comp_file(argv[3]);

	/* Open and Parse metadata from New Uncompressed File */
	parse_new_uncomp_file(argv[2]);

	/* Open and Parse metadata from Old Uncompressed File */
	parse_old_uncomp_file(argv[1]);

	/* Assign number of blocks for new uncompressed file; same value as in new compressed file */
	no_blocks_nu = ncf.compheader->sections - 1;

	/* Assign number of blocks for old uncompressed file */
	no_blocks_ou = (ouf.size_for_blocks_calc / ncf.compheader->blocksize);
	if (ouf.size_for_blocks_calc % ncf.compheader->blocksize) {
		no_blocks_ou++;
	}

	/* Create and initialize patch file */
	initialize_patch_file(argv[4]);

	/* Malloc for buffers */
	malloc_for_buffers(&db, &eb, &I, &V, &old, &new, &block_crc_buffer, &pf_crc_buffer);

	/* Analysis of number of blocks in old/new files */
	no_common_blocks = MIN(no_blocks_ou, no_blocks_nu);
	no_extra_blocks = (no_blocks_ou - no_common_blocks > 0 ? no_blocks_ou - no_common_blocks : no_blocks_nu - no_common_blocks);

	/* Diff for common/corresponding blocks */

	for (int index = 0; index < no_common_blocks; index++) {

		blockheader.block_number = index;
		blockheader.new_block_size_c = ncf.compheader->secoff[index + 1] - ncf.compheader->secoff[index];

		/* Calculate CRC of corresponding block from new "compressed" binary */
		calculate_block_crc_comp(block_crc_buffer, argv[3], index);

		/* Calculate bytes to read from files for current block diff */
		calculate_bytes_to_read_common_blocks(index, no_common_blocks, no_blocks_nu, no_blocks_ou, &bytes_to_read_ou, &bytes_to_read_nu);

		blockheader.new_block_size_u = bytes_to_read_nu;

		/* Copy blocks from old/new uncompressed files to old, new buffers */
		read_common_blocks_from_files(bytes_to_read_ou, bytes_to_read_nu, old, new, index, argv[1], argv[2]);

		/* If old/new blocks are same, Token = SKIP; else Token = MODIFY */
		if (bytes_to_read_nu == bytes_to_read_ou) {
			if (memcmp(old, new, bytes_to_read_ou) == 0) {
				blockheader.token = SKIP;
				goto diff_done_ocf;
			} else {
				blockheader.token = MODIFY;
			}
		} else {
			blockheader.token = MODIFY;
		}

		/* Skip space for blockheader in patch file ; Seek to location where diff data will be written */
		rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
		if (rpos != 0) {
			err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
		}

		/* Bsdiff for current block index */
		bsdiff(old, new, db, eb, I, V, bytes_to_read_ou, bytes_to_read_nu);

diff_done_ocf:

		/* Write block header */
		write_blockheader_common_blocks(argv[4]);
	}

	/* Diff for Extra Blocks */

	/*
	 * Delta for extra blocks
	 *
	 * 2 cases:
	 *      1) No. Blocks Old Uncomp. File < No. Blocks in New Uncomp. File => Token = ADD;
	 *      2) No. Blocks Old Uncomp. File > No. Blocks in New Uncomp. File => Token = REMOVE;
	 */

	if (no_blocks_ou < no_blocks_nu) {
		for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

			/* Get bytes to read for newfile for current block */
			calculate_bytes_to_read_extra_blocks(index, no_common_blocks, no_extra_blocks, &bytes_to_read_nu);

			/* Read block from new file */
			read_extra_blocks_from_files(bytes_to_read_nu, new, index, argv[2]);

			/* Assign Blockheader info */
			blockheader.block_number = index;
			blockheader.token = ADD;
			blockheader.new_block_size_c = ncf.compheader->secoff[index + 1] - ncf.compheader->secoff[index];
			blockheader.new_block_size_u = bytes_to_read_nu;
			blockheader.size_ctrl_block = 0;
			blockheader.size_diff_block = 0;

			/* Calculate CRC of corresponding block from new "compressed" binary */
			calculate_block_crc_comp(block_crc_buffer, argv[3], index);

			/*  Seek to location of diff data */
			rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
			if (rpos != 0) {
				err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
			}

			/* Write block to patch file at current location */
			write_extra_block(new);

			/* Write blockheader */
			write_blockheader_extra_blocks(argv[4]);

		}
	} else if (no_blocks_ou > no_blocks_nu) {

		for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

			/* Assign Blockheader info */
			blockheader.block_number = index;
			blockheader.token = REMOVE;
			blockheader.new_block_crc = 0;
			blockheader.new_block_size_c = 0;
			blockheader.new_block_size_u = 0;
			blockheader.size_ctrl_block = 0;
			blockheader.size_diff_block = 0;
			blockheader.size_extra_block = 0;

			/* Write blockheader to patch file */
			if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
				err_msg("fwrite - failed of blockheader to %s", argv[4]);
			}
		}
	}

	/* Update and Write Patch header */

	/* Seek to the beginning of patch file */
	if (fseek(pf.pfile, 0, SEEK_SET)) {
		err_msg("fseek %s", argv[4]);
	}

	/* Assign Patch header values */
	patchheader.old_file_compression_format = CONFIG_COMPRESSION_TYPE;
	patchheader.blocks_newfile = no_blocks_nu;
	patchheader.blocks_oldfile = no_blocks_ou;
	patchheader.blocksize_newfile = ncf.compheader->blocksize;
	patchheader.blocksize_oldfile = ncf.compheader->blocksize;
	patchheader.crc_newfile_size = sizeof(unsigned int);
	patchheader.binary_header_size = sizeof(binary_header_t);
	patchheader.comp_header_size = ncf.compheader_size;

	if (strlen(patch_signature) != DIFF_SIGNATURE_SIZE) {
		err_msg("Signature size should be %d bytes", DIFF_SIGNATURE_SIZE);
	}

	memcpy(patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE);

	/* Write Patch header to patch file */
	if (fwrite(&patchheader, sizeof(patchheader), 1, pf.pfile) != 1) {
		err_msg("fwrite(%s)", argv[4]);
	}

	/* Calculate Patch File CRC and Write to file */

	patch_file_crc_calculation(pf_crc_buffer, argv[4]);

	/* Free memory and close files */

	/* Close patch file */
	if (fclose(pf.pfile)) {
		err_msg("fclose : %s", argv[4]);
	}

	/* Free the memory we used */
	free(db);
	free(eb);
	free(I);
	free(V);
	free(old);
	free(new);
	free(ncf.compheader);

	/* Close input file if opened */
	if (ncf.fd > 0) {
		close(ncf.fd);
	}
	if (nuf.fd > 0) {
		close(nuf.fd);
	}
	if (ouf.fd > 0) {
		close(ouf.fd);
	}

	return 0;
}

#elif CONFIG_COMPRESSED_BINARY == 0	/* Create Patch for Uncompressed Binary */

/* Calculate crc for block from new uncomp. file.
 * CRC Value is assigned in blockheader.
 */
void calculate_block_crc_uncomp(u_char *new, int bytes_to_read_nu)
{
	unsigned int block_crc;

	/* Calculate CRC of corresponding block from new "uncompressed" binary */
	block_crc = 0;
	block_crc = crc32_z(block_crc, (const Bytef *)new, bytes_to_read_nu);
	blockheader.new_block_crc = block_crc;
}

/* Calculate bytes to read for extra blocks from new uncompressed file for block diff */
void calculate_bytes_to_read_extra_blocks(int block_index, int no_common_blocks, int no_extra_blocks, int *bytes_to_read_nu)
{
	/* Get bytes to read for newfile for current block */
	if (block_index != no_common_blocks + no_extra_blocks - 1) {
		*bytes_to_read_nu = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		*bytes_to_read_nu = (nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
	}
}

/* Calculate bytes to read for common blocks from old/new uncompressed files for block diff */
void calculate_bytes_to_read_common_blocks(int block_index, int no_common_blocks, int no_blocks_nu, int no_blocks_ou, int *bytes_to_read_ou, int *bytes_to_read_nu)
{
	/*
	 * Calculate bytes to read from files for current block diff
	 *
	 * Cases :
	 * - This is not the last common block.
	 * - This is last common block:
	 *      1.) Both files have same number of blocks
	 *      2.) New file has less blocks than old file
	 *      3.) New file has more blocks than old file
	 */
	if (block_index != no_common_blocks - 1) {
		*bytes_to_read_ou = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
		*bytes_to_read_nu = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
	} else {
		if (no_blocks_nu == no_blocks_ou) {
			*bytes_to_read_nu = (nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
			*bytes_to_read_ou = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		} else if (no_blocks_nu < no_blocks_ou) {
			*bytes_to_read_ou = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
			*bytes_to_read_nu = (nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		} else {
			*bytes_to_read_nu = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
			*bytes_to_read_ou = (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA != 0 ? ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA : CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		}
	}
}

/* Malloc for buffers */
void malloc_for_buffers(char **db, char **eb, off64_t **I, off64_t **V, u_char **old, u_char **new, char **pf_crc_buffer)
{
	/* Buffers for diff/extra sections; max size needed is blocksize */
	if (((*db = malloc(CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1)) == NULL) || ((*eb = malloc(CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1)) == NULL)) {
		err_msg("Malloc of db/eb failed");
	}

	/* Malloc for I/V buffers, used for suffix sort */
	if (((*I = malloc((CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1) * sizeof(off64_t))) == NULL) || ((*V = malloc((CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1) * sizeof(off64_t))) == NULL)) {
		err_msg("malloc for I/V buffers failed");
	}

	/* Malloc for old/new buffers into which blocks will be read */
	if (((*old = malloc((CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1) * sizeof(u_char))) == NULL) || ((*new = malloc((CONFIG_BLOCKSIZE_FOR_FOTA_DELTA + 1) * sizeof(u_char))) == NULL)) {
		err_msg("malloc for old/new buffers failed");
	}

	/* Malloc for buffer used in CRC calculation */
	if ((*pf_crc_buffer = malloc((CRC_BLOCKSIZE + 1) * sizeof(char))) == NULL) {
		err_msg("malloc failed for pf_crc_buffer");
	}
}

/* Open Patch File for write. Write CRC, Binary Header */
void initialize_patch_file_elf(char *p_file)
{
	int rpos;

	/* Create the patch file */
	if ((pf.pfile = fopen(p_file, "w+")) == NULL) {
		err_msg("%s", p_file);
	}

	/* Seek to position where we will write crc checksum for new uncompressed file */
	rpos = fseek(pf.pfile, sizeof(patchheader), SEEK_SET);
	if (rpos != 0) {
		err_msg("Seek failed to end of patch header for %s", p_file);
	}

	/* Copying crc */
	if (fwrite(&nuf.crc, sizeof(nuf.crc), 1, pf.pfile) != 1) {
		err_msg("Failed write of crc for new uncompressed file to %s", p_file);
	}

	/* Copying binary header */
	if (fwrite(&nuf.binheader, sizeof(nuf.binheader), 1, pf.pfile) != 1) {
		err_msg("Failed write of binary header for new uncompressed file to %s", p_file);
	}
}

/* Open Patch File for write */
void initialize_patch_file_kernel(char *p_file)
{
	int rpos;

	/* Create the patch file */
	if ((pf.pfile = fopen(p_file, "w+")) == NULL) {
		err_msg("%s", p_file);
	}

	/* Seek to position where we will write crc checksum for new uncompressed file */
	rpos = fseek(pf.pfile, sizeof(patchheader), SEEK_SET);
	if (rpos != 0) {
		err_msg("Seek failed to end of patch header for %s", p_file);
	}
}

int main(int argc, char *argv[])
{
	int no_common_blocks = 0;	/* Represents lesser of (no. of blocks in oldfile, no. blocks in new file) */
	int no_extra_blocks = 0;	/* Represents no. of extra blocks in old/new file, whichever one has more */
	int bytes_to_read_ou, bytes_to_read_nu;	/* Bytes to read for blockwise diff from old/new uncomp. files */
	int no_blocks_ou, no_blocks_nu;	/* No. of blocks in old/new uncomp. files based on blocksize for compression */
	int rpos;

	/* Buffers needed by Bsdiff */
	char *db, *eb;				/* contents of diff, extra sections */
	u_char *old, *new;			/* contents of old, new files */
	off64_t *I, *V;				/* arrays used for suffix sort; I is ordering */

	/* Buffers for CRC calculation */
	char *pf_crc_buffer;		/* Buffer for CRC calculation of patch file */

	if (argc != 5) {
		err_msg("Create patch for Uncompressed Binary (on device)\nusage:\n\n%s  elf/kernel old_uncompressed_file" "  new_uncompressed_file patchfile\n", argv[0]);
	}

	if (memcmp(argv[1], "elf", 3) == 0) {

		/* Open and Parse metadata from New Uncompressed File */
		parse_new_uncomp_file(argv[3]);

		/* Assigning number of blocks for new uncompressed file */
		no_blocks_nu = (nuf.size_for_blocks_calc / CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		if (nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA) {
			no_blocks_nu++;
		}

		/* Open and Parse metadata from old uncompressed File */
		parse_old_uncomp_file(argv[2]);

		/* Assigning number of blocks for old uncompressed file */
		no_blocks_ou = (ouf.size_for_blocks_calc / CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		if (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA) {
			no_blocks_ou++;
		}

		/* Create and initialize patch file */
		initialize_patch_file_elf(argv[4]);

		/* Malloc for buffers */
		malloc_for_buffers(&db, &eb, &I, &V, &old, &new, &pf_crc_buffer);

		/* Analysis of number of blocks in old/new files */
		no_common_blocks = MIN(no_blocks_ou, no_blocks_nu);
		no_extra_blocks = (no_blocks_ou - no_common_blocks > 0 ? no_blocks_ou - no_common_blocks : no_blocks_nu - no_common_blocks);

		/* Diff for common/corresponding blocks */

		for (int index = 0; index < no_common_blocks; index++) {

			blockheader.block_number = index;
			blockheader.new_block_size_c = 0;	/* Dummy value. Not going to used on patch side */

			/* Calculate bytes to read from files for current block diff */
			calculate_bytes_to_read_common_blocks(index, no_common_blocks, no_blocks_nu, no_blocks_ou, &bytes_to_read_ou, &bytes_to_read_nu);

			blockheader.new_block_size_u = bytes_to_read_nu;

			/* Copy blocks from old/new uncompressed files to old, new buffers */
			read_common_blocks_from_files(bytes_to_read_ou, bytes_to_read_nu, old, new, index, argv[2], argv[3]);

			/* CRC calculation and assignment for new uncompressed block */
			calculate_block_crc_uncomp(new, bytes_to_read_nu);

			/* If old/new blocks are same, Token = SKIP; else Token = MODIFY */
			if (bytes_to_read_nu == bytes_to_read_ou) {
				if (memcmp(old, new, bytes_to_read_ou) == 0) {
					blockheader.token = SKIP;
					goto diff_done_oufe;
				} else {
					blockheader.token = MODIFY;
				}
			} else {
				blockheader.token = MODIFY;
			}

			/* Skip space for blockheader; Seek to location of where diff data will be written */
			rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
			if (rpos != 0) {
				err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
			}

			/* Bsdiff for current block index */
			bsdiff(old, new, db, eb, I, V, bytes_to_read_ou, bytes_to_read_nu);

diff_done_oufe:

			/* Write block header */
			write_blockheader_common_blocks(argv[4]);
		}

		/* Diff for Extra Blocks */

		/*
		 * Delta for extra blocks
		 *
		 * 2 cases:
		 *      1) No. Blocks Old Uncomp. File < No. Blocks in New Uncomp. File => Token = ADD;
		 *      2) No. Blocks Old Uncomp. File > No. Blocks in New Uncomp. File => Token = REMOVE;
		 */

		if (no_blocks_ou < no_blocks_nu) {
			for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

				/* Get bytes to read for newfile for current block */
				calculate_bytes_to_read_extra_blocks(index, no_common_blocks, no_extra_blocks, &bytes_to_read_nu);

				read_extra_blocks_from_files(bytes_to_read_nu, new, index, argv[3]);

				/* Assign Blockheader info */
				blockheader.block_number = index;
				blockheader.token = ADD;
				blockheader.new_block_size_c = 0;	/* Dummy value. Not going to be used on patch file */
				blockheader.new_block_size_u = bytes_to_read_nu;
				blockheader.size_ctrl_block = 0;
				blockheader.size_diff_block = 0;

				/* Calculate CRC of corresponding block from new "uncompressed" binary */
				calculate_block_crc_uncomp(new, bytes_to_read_nu);

				/*  Seek to location of diff data */
				rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
				if (rpos != 0) {
					err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
				}

				write_extra_block(new);

				write_blockheader_extra_blocks(argv[4]);

			}
		} else if (no_blocks_ou > no_blocks_nu) {

			for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

				/* Assign Blockheader info */
				blockheader.block_number = index;
				blockheader.token = REMOVE;
				blockheader.new_block_crc = 0;
				blockheader.new_block_size_c = 0;	/* Dummy value. Not going to be used on patch side */
				blockheader.new_block_size_u = 0;
				blockheader.size_ctrl_block = 0;
				blockheader.size_diff_block = 0;
				blockheader.size_extra_block = 0;

				/* Write blockheader to patch file */
				if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
					err_msg("fwrite - failed of blockheader to %s", argv[4]);
				}
			}
		}

		/* Update and Write Patch header */

		/* Seek to the beginning of patch file */
		if (fseek(pf.pfile, 0, SEEK_SET)) {
			err_msg("fseek");
		}

		/* Assign Patch header values */
		patchheader.old_file_compression_format = COMP_TYPE_NONE;
		patchheader.blocks_newfile = no_blocks_nu;
		patchheader.blocks_oldfile = no_blocks_ou;
		patchheader.blocksize_newfile = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
		patchheader.blocksize_oldfile = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
		patchheader.crc_newfile_size = sizeof(unsigned int);
		patchheader.binary_header_size = sizeof(binary_header_t);
		patchheader.comp_header_size = 0;

		if (strlen(patch_signature) != DIFF_SIGNATURE_SIZE) {
			err_msg("Signature size should be %d bytes", DIFF_SIGNATURE_SIZE);
		}

		memcpy(patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE);

		/* Write Patch header to patch file */
		if (fwrite(&patchheader, sizeof(patchheader), 1, pf.pfile) != 1) {
			err_msg("fwrite(%s)", argv[4]);
		}

		/* Calculate Patch File CRC and Write to file */
		patch_file_crc_calculation(pf_crc_buffer, argv[4]);

		/* Free memory and close files */

		/* Close patch file */
		if (fclose(pf.pfile)) {
			err_msg("fclose : %s", argv[4]);
		}

		/* Free the memory we used */
		free(db);
		free(eb);
		free(I);
		free(V);
		free(old);
		free(new);

		/* Close input file if opened */
		if (ncf.fd > 0) {
			close(ncf.fd);
		}
		if (nuf.fd > 0) {
			close(nuf.fd);
		}
		if (ouf.fd > 0) {
			close(ouf.fd);
		}
	} else if (memcmp(argv[1], "kernel", 6) == 0) {

		/* Open and Parse metadata from New Uncompressed File */
		/* Opening file and getting filesize */
		if (((nuf.fd = open(argv[3], O_RDONLY, 0)) < 0) || ((nuf.total_size = lseek(nuf.fd, 0, SEEK_END)) == -1) || (lseek(nuf.fd, 0, SEEK_SET) != 0)) {
			err_msg("%s", argv[3]);
		}

		nuf.size_for_blocks_calc = nuf.total_size;

		/* Assigning number of blocks for new uncompressed file */
		no_blocks_nu = (nuf.size_for_blocks_calc / CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		if (nuf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA) {
			no_blocks_nu++;
		}

		/* Open and Parse metadata from old uncompressed File */
		/* Opening old uncompressed file and getting filesize */
		if (((ouf.fd = open(argv[2], O_RDONLY, 0)) < 0) || ((ouf.total_size = lseek(ouf.fd, 0, SEEK_END)) == -1) || (lseek(ouf.fd, 0, SEEK_SET) != 0)) {
			err_msg("%s", argv[2]);
		}

		ouf.size_for_blocks_calc = ouf.total_size;

		/* Assigning number of blocks for old uncompressed file */
		no_blocks_ou = (ouf.size_for_blocks_calc / CONFIG_BLOCKSIZE_FOR_FOTA_DELTA);
		if (ouf.size_for_blocks_calc % CONFIG_BLOCKSIZE_FOR_FOTA_DELTA) {
			no_blocks_ou++;
		}

		/* Create and initialize patch file */
		initialize_patch_file_kernel(argv[4]);

		/* Malloc for buffers */
		malloc_for_buffers(&db, &eb, &I, &V, &old, &new, &pf_crc_buffer);

		/* Analysis of number of blocks in old/new files */
		no_common_blocks = MIN(no_blocks_ou, no_blocks_nu);
		no_extra_blocks = (no_blocks_ou - no_common_blocks > 0 ? no_blocks_ou - no_common_blocks : no_blocks_nu - no_common_blocks);

		/* Diff for common/corresponding blocks */

		for (int index = 0; index < no_common_blocks; index++) {

			blockheader.block_number = index;
			blockheader.new_block_size_c = 0;	/* Dummy value. Not going to used on patch side */

			/* Calculate bytes to read from files for current block diff */
			calculate_bytes_to_read_common_blocks(index, no_common_blocks, no_blocks_nu, no_blocks_ou, &bytes_to_read_ou, &bytes_to_read_nu);

			blockheader.new_block_size_u = bytes_to_read_nu;

			/* Copy blocks from old/new uncompressed files to old, new buffers */
			read_common_blocks_from_files(bytes_to_read_ou, bytes_to_read_nu, old, new, index, argv[2], argv[3]);

			/* CRC calculation and assignment for new uncompressed block */
			calculate_block_crc_uncomp(new, bytes_to_read_nu);

			/* If old/new blocks are same, Token = SKIP; else Token = MODIFY */
			if (bytes_to_read_nu == bytes_to_read_ou) {
				if (memcmp(old, new, bytes_to_read_ou) == 0) {
					blockheader.token = SKIP;
					goto diff_done_oufk;
				} else {
					blockheader.token = MODIFY;
				}
			} else {
				blockheader.token = MODIFY;
			}

			/* Skip space for blockheader; Seek to location of where diff data will be written */
			rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
			if (rpos != 0) {
				err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
			}

			/* Bsdiff for current block index */
			bsdiff(old, new, db, eb, I, V, bytes_to_read_ou, bytes_to_read_nu);

diff_done_oufk:

			/* Write block header */
			write_blockheader_common_blocks(argv[4]);
		}

		/* Diff for Extra Blocks */

		/*
		 * Delta for extra blocks
		 *
		 * 2 cases:
		 *      1) No. Blocks Old Uncomp. File < No. Blocks in New Uncomp. File => Token = ADD;
		 *      2) No. Blocks Old Uncomp. File > No. Blocks in New Uncomp. File => Token = REMOVE;
		 */

		if (no_blocks_ou < no_blocks_nu) {
			for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

				/* Get bytes to read for newfile for current block */
				calculate_bytes_to_read_extra_blocks(index, no_common_blocks, no_extra_blocks, &bytes_to_read_nu);

				read_extra_blocks_from_files(bytes_to_read_nu, new, index, argv[3]);

				/* Assign Blockheader info */
				blockheader.block_number = index;
				blockheader.token = ADD;
				blockheader.new_block_size_c = 0;	/* Dummy value. Not going to be used on patch file */
				blockheader.new_block_size_u = bytes_to_read_nu;
				blockheader.size_ctrl_block = 0;
				blockheader.size_diff_block = 0;

				/* Calculate CRC of corresponding block from new "uncompressed" binary */
				calculate_block_crc_uncomp(new, bytes_to_read_nu);

				/*  Seek to location of diff data */
				rpos = fseek(pf.pfile, sizeof(blockheader), SEEK_CUR);
				if (rpos != 0) {
					err_msg("Lseek failed to location of start of diff data - %s", argv[4]);
				}

				write_extra_block(new);

				write_blockheader_extra_blocks(argv[4]);

			}
		} else if (no_blocks_ou > no_blocks_nu) {

			for (int index = no_common_blocks; index < no_common_blocks + no_extra_blocks; index++) {

				/* Assign Blockheader info */
				blockheader.block_number = index;
				blockheader.token = REMOVE;
				blockheader.new_block_crc = 0;
				blockheader.new_block_size_c = 0;	/* Dummy value. Not going to be used on patch side */
				blockheader.new_block_size_u = 0;
				blockheader.size_ctrl_block = 0;
				blockheader.size_diff_block = 0;
				blockheader.size_extra_block = 0;

				/* Write blockheader to patch file */
				if (fwrite(&blockheader, sizeof(blockheader), 1, pf.pfile) != 1) {
					err_msg("fwrite - failed of blockheader to %s", argv[4]);
				}
			}
		}

		/* Update and Write Patch header */

		/* Seek to the beginning of patch file */
		if (fseek(pf.pfile, 0, SEEK_SET)) {
			err_msg("fseek");
		}

		/* Assign Patch header values */
		patchheader.old_file_compression_format = COMPRESSION_TYPE_NONE;
		patchheader.blocks_newfile = no_blocks_nu;
		patchheader.blocks_oldfile = no_blocks_ou;
		patchheader.blocksize_newfile = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
		patchheader.blocksize_oldfile = CONFIG_BLOCKSIZE_FOR_FOTA_DELTA;
		patchheader.crc_newfile_size = 0;
		patchheader.binary_header_size = 0;
		patchheader.comp_header_size = 0;

		if (strlen(patch_signature) != DIFF_SIGNATURE_SIZE) {
			err_msg("Signature size should be %d bytes", DIFF_SIGNATURE_SIZE);
		}

		memcpy(patchheader.signature, patch_signature, DIFF_SIGNATURE_SIZE);

		/* Write Patch header to patch file */
		if (fwrite(&patchheader, sizeof(patchheader), 1, pf.pfile) != 1) {
			err_msg("fwrite(%s)", argv[4]);
		}

		/* Calculate Patch File CRC and Write to file */
		patch_file_crc_calculation(pf_crc_buffer, argv[4]);

		/* Free memory and close files */

		/* Close patch file */
		if (fclose(pf.pfile)) {
			err_msg("fclose : %s", argv[4]);
		}

		/* Free the memory we used */
		free(db);
		free(eb);
		free(I);
		free(V);
		free(old);
		free(new);

		/* Close input file if opened */
		if (nuf.fd > 0) {
			close(nuf.fd);
		}
		if (ouf.fd > 0) {
			close(ouf.fd);
		}

	}

	return 0;
}
#endif
