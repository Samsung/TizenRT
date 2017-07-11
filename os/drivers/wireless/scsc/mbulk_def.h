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

#ifndef __MBULK_DEF_H__
#define __MBULK_DEF_H__

#include <scsc/scsc_mifram.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils_scsc.h"

/**
 * mbulk
 *
 * The mbulk supports ....
 */

/* MIF_HIP_CFG_BLUK_BUFFER_ALIGNE = 4
 * Hence, data buffer is always aligned to 4.
 */
#define MBULK_SIG_BUFSZ_ROUNDUP(sig_bufsz)   round_up(sig_bufsz, 4)

/**
 * bulk memory descriptor, managing semgments allocated from mbulk pools
 *
 * mbulk is defined in public header, to avoid the function call dummy operations...
 * Privae variables staring with __ should be not be referred by the client
 * codes.
 *
 * mbulk structure is shared with the host. Hence the structure should be packed.
 * data buffer size in byte
 *
 * This could be duplicated as the buffer size can be calculated from
 * pool block size. There are two reasons why the data buffer size is
 * set in the descritpor:
 * - if mbulk comes from the host, the firmware is not aware of its pool
 *   configuration
 * - if the data buffer is less than the block size in the pool, it's more
 *   cache efficient to flush only the data buffer area
 *   <Q> probably the firwmare has to invalidate the entire block ?
 *
 */
typedef u16 mbulk_len_t;
struct mbulk {
	struct mbulk *next;					/** next mbulk */
	u8 flag;							/** mbulk flags */
	enum mbulk_class clas;				/** bulk buffer classification */
	u8 pid;								/** mbulk pool id */
	u8 refcnt;							/** reference counter of bulk buffer */
	mbulk_len_t dat_bufsz;				/** data buffer size in byte */
	mbulk_len_t sig_bufsz;				/** signal buffer size in byte */
	mbulk_len_t len;					/** valid data length */
	mbulk_len_t head;					/** start offset of data after mbulk structure */
	ptrdiff_t chain_next_offset;		/** chain next mbulk offset */
} STRUCT_PACKED;

/* mbulk flags */
#define MBULK_F_SIG                 (1 << 0) /** has a signal after mbulk descriptor*/
#define MBULK_F_READONLY            (1 << 1) /** is read-only */
#define MBULK_F_OBOUND              (1 << 2) /** other CPU(host) owns the buffer */
#define MBULK_F_FREE                (1 << 5) /** mbulk alreay freed */
#define MBULK_F_CHAIN_HEAD          (1 << 6) /** is an head in scatter/gather chain */
#define MBULK_F_CHAIN               (1 << 7) /** is scatter/gather chain */

/* Define max number of chained mbulk */
#define MBULK_MAX_CHAIN              16
/* the buffer address just after mbulk descriptor */
#define MBULK_SEG_B(m)              ((uintptr_t)(m) + sizeof(struct mbulk))

/* raw buffer size of mbulk segment including struct mbulk */
#define MBULK_SEG_RAW_BUFSZ(m)      (sizeof(struct mbulk) + (m)->sig_bufsz + (m)->dat_bufsz)

/* operations against "a" mbulk segment. */
#define MBULK_SEG_NEXT(m)           ((m)->next_offset)
#define MBULK_SEG_REFCNT(m)         ((m)->refcnt)
#define MBULK_SEG_HAS_SIGNAL(m)     ((m)->flag & MBULK_F_SIG)
#define MBULK_SEG_CHAIN_NEXT(m)     ((m)->chain_next_offset)
#define MBULK_SEG_IS_CHAIN_HEAD(m)  ((m)->flag & MBULK_F_CHAIN_HEAD)
#define MBULK_SEG_IS_CHAINED(m)     ((m)->flag & MBULK_F_CHAIN)

#define MBULK_SEG_IS_READONLY(m)    ((m)->flag & MBULK_F_READONLY)
#define MBULK_SEG_SET_READONLY(m)   ((m)->flag |= MBULK_F_READONLY)

#define MBULK_SEG_DAT(m)            ((char *)MBULK_SEG_B(m) + (m)->head)
#define MBULK_SEG_DAT_AT(m, off)     ((char *)MBULK_SEG_B(m) + (m)->head + (mbulk_len_t)(off))
#define MBULK_SEG_DAT_BUFSIZE(m)    ((size_t)((m)->dat_bufsz))
#define MBULK_SEG_SIG_BUFSIZE(m)    ((size_t)((m)->sig_bufsz))
#define MBULK_SEG_LEN(m)            ((m)->len)
#define MBULK_SEG_HEADROOM(m)       ((size_t)((m)->head - (m)->sig_bufsz))
#define MBULK_SEG_TAILROOM(m)       ((size_t)((m)->dat_bufsz - (MBULK_SEG_HEADROOM(m) + (m)->len)))

/**
 * Reserve a headroom in the buffer
 *
 */
static inline bool mbulk_seg_reserve_head(struct mbulk *m, size_t headroom)
{
	//BUG_ON(!(m->len == 0 && m->dat_bufsz >= headroom));
	m->head += (mbulk_len_t) headroom;
	return true;
}

static inline bool mbulk_seg_adjust_range(struct mbulk *m, size_t headroom, size_t len)
{
	//BUG_ON(!(m->dat_bufsz >= (headroom + len)));
	m->head = m->sig_bufsz + (mbulk_len_t) headroom;
	m->len = (mbulk_len_t) len;
	return true;
}

static inline bool mbulk_seg_prepend_head(struct mbulk *m, size_t more)
{
	//BUG_ON(!(MBULK_SEG_HEADROOM(m) >= more));
	m->head -= (mbulk_len_t) more;
	m->len += (mbulk_len_t) more;
	return true;
}

static inline bool mbulk_seg_append_tail(struct mbulk *m, size_t more)
{
	//BUG_ON(!(MBULK_SEG_TAILROOM(m) >= more));
	m->len += (mbulk_len_t) more;
	return true;
}

static inline bool mbulk_seg_trim_head(struct mbulk *m, size_t less)
{
	m->head += (mbulk_len_t) less;
	m->len = (m->len <= (mbulk_len_t) less) ? 0 : (m->len - (mbulk_len_t) less);
	return true;
}

static inline bool mbulk_seg_trim_tail(struct mbulk *m, size_t less)
{
	//BUG_ON(!(m->len >= (mbulk_len_t)less));
	m->len -= (mbulk_len_t) less;
	return true;
}

/**
 * free the bulk buffer of a segment
 *
 * Simply decrement the bulk reference counter and put to the pool if
 * it is zero. Note that the signal buffer is not affected.
 *
 */
void mbulk_seg_free(struct mbulk *m);

/**
 * duplicate the bulk buffer of a mbulk segment
 *
 * This is used to share the read-only bulk buffer. The reference counter is
 * increased by one each time it is duplicated.
 *
 */
struct mbulk *mbulk_seg_duplicate(struct mbulk *m);

/**
 * clone the bulk buffer of a mbulk segment
 *
 * A separate mbulk segment is created and the data is copied to it.
 * If \copy_sig is true, then the signal is copied as well.
 *
 */
struct mbulk *mbulk_seg_clone(const struct mbulk *m, enum mbulk_class clas, bool copy_sig);

#endif /*__MBULK_DEF_H__*/
