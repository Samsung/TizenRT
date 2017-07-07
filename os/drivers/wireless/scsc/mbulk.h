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

#ifndef __MBULK_H__
#define __MBULK_H__

/**
 * mbulk(bulk memory) API
 *
 * This header file describes APIs of the bulk memory management.
 * The diagram below is an example of a mbulk buffer with one
 * segment (i.e. not a chained mbulk).
 *
 *            sig_bufsz
 *           |<-------->|
 *           |          |<--------- dat_bufsz ---------->|
 *    +--------------------------------------------------+
 *    | mbulk| signal   |          bulk buffer           |
 *    +-------------------------+---------------+--------+
 *           |          |       |  valid data   |        |
 *           |          |       |<--------+---->|        |
 *           |          |       | mbulk_tlen(m) |        |
 *           |          |<----->|          |    |<------>|
 *           | mbulk_headroom(m)|          | mbulk_tailroom(m)
 *           |                  |          |
 *           |                  |-- off -->|
 *           v                  v          |
 *    mbulk_get_sig(m)      mbulk_dat(m)   |
 *                                         v
 *                                 mbulk_dat_at(m,off)
 *
 * In general, all clients are supposed to use only mbulk_xxx() APIs (but not
 * mbulk_seg_xxx() APIs), as they can handle S/G chained mbulk as well.
 * But as of now, specially in Condor, S/G chained mbulk is not supported,
 * which means the most of mbulk_xxx() would be wrappers of mbulk_seg_xxx().
 *
 * An in-lined signal buffer can be allocated along with a mbulk buffer.
 * There is no direct life-cycle relationship between the signal and the
 * associated mbulk in this case, which means that the signal buffer should be
 * de-allocated independently of the mbulk buffer.
 *
 */

/**
 * bulk buffer descriptor
 */
struct mbulk;

/**
 * mbulk pool ID
 */
#define MBULK_POOL_ID_MIN        (0)
#define MBULK_POOL_ID_MAX_FW     (4)	/* 0 ~ 3 used for firmware pools */
#define MBULK_POOL_ID_MAX        (MBULK_POOL_ID_MAX_FW)

/* Max number of mbulk to be freed by the Host */
#define MBULK_MAX_HOST_TO_FREE          2

/**
 * mbulk buffer classification
 *
 * Note that PACKED attribute is added to enum definition so that
 * compiler assigns the smallest integral type (u8).
 */
enum mbulk_class {
	MBULK_CLASS_FROM_HOST_DAT = 0,
	MBULK_CLASS_FROM_HOST_CTL = 1,
	MBULK_CLASS_FROM_RADIO = 2,
	MBULK_CLASS_CONTRL = 3,
	MBULK_CLASS_DEBUG = 4,
	MBULK_CLASS_OTHERS = 5,
	MBULK_CLASS_MAX
} STRUCT_PACKED;

/**
 * The private definition of mbulk structure is included here
 * so that its members can be directly accessed, and the access
 * codes can be in-lined by the compiler.
 * But client codes are not supposed to directly refer to mbulk
 * members, nor use mbulk_seg_xxx() functions. Only modules handling
 * mbulk scatter/gather chain would directly use mulk_seg_xxx() APIs.
 */
#include "mbulk_def.h"

/**
 * Get the bulk data reference counter
 *
 * After a bulk buffer with non-zero data buffer size is created,
 * the reference counter is set to one. Each time it is duplicated,
 * its reference counter would be increased.
 *
 * Note that the reference counter is initialized to zero if a signal
 * is created from mbulk pool but with zero data buffer size, as there
 * is no data buffer.
 */
static inline int mbulk_refcnt(const struct mbulk *m)
{
	return MBULK_SEG_REFCNT(m);
}

/**
 * Get the bulk data buffer size
 *
 */
static inline int mbulk_buffer_size(const struct mbulk *m)
{
	return MBULK_SEG_DAT_BUFSIZE(m);
}

/**
 * Check if mbulk has an in-lined signal buffer
 *
 */
static inline bool mbulk_has_signal(const struct mbulk *m)
{
	return MBULK_SEG_HAS_SIGNAL(m);
}

/**
 * Set mbulk to be read-only
 */
static inline void mbulk_set_readonly(struct mbulk *m)
{
	MBULK_SEG_SET_READONLY(m);
}

/**
 * is mbulk read-only
 */
static inline bool mbulk_is_readonly(const struct mbulk *m)
{
	return MBULK_SEG_IS_READONLY(m);
}

/**
 * check if mbulk is a scatter/gather chained buffer
 *
 */
static inline bool mbulk_is_sg(const struct mbulk *m)
{
	return MBULK_SEG_IS_CHAIN_HEAD(m);
}

/**
 * check if mbulk is a part of scatter/gather chained buffer
 *
 */
static inline bool mbulk_is_chained(const struct mbulk *m)
{
	return MBULK_SEG_IS_CHAINED(m);
}

/**
 * Allocate a bulk buffer with an in-lined signal buffer
 *
 * Only one mbulk segment is used for allocation starting from the
 * mbulk pool with the smallest segment size. If no segment fitting
 * the requested size, then return NULL without trying to create
 * a chained buffer.
 *
 */
struct mbulk *mbulk_with_signal_alloc(enum mbulk_class clas, size_t sig_bufsz, size_t dat_bufsz);
/**
 * Allocate a bulk buffer with an in-lined signal buffer
 *
 * A mbulk segment is allocated from the given the pool, if its size
 * meeting the requested size.
 *
 */
struct mbulk *mbulk_with_signal_alloc_by_pool(u8 pool_id, u16 colour, enum mbulk_class clas, size_t sig_bufsz, size_t dat_bufsz);
/**
 * Get a signal buffer address
 *
 * Given a mbulk buffer, returns a signal buffer address.
 *
 * @param m             mbulk
 * @return              in-lined signal buffer
 */
static inline void *mbulk_get_seg(const struct mbulk *m)
{
	return (void *)MBULK_SEG_B(m);
}

/**
 * Get a signal buffer address
 *
 * Given a mbulk buffer, returns a signal buffer address if any in-lined
 * signal buffer.
 *
 */
static inline void *mbulk_get_signal(const struct mbulk *m)
{
	bool ret = false;

	ret = mbulk_has_signal(m);

	return ret ? mbulk_get_seg(m) : NULL;
}

/**
 * Allocate a bulk buffer
 *
 * Only one mbulk segment is used for allocation starting from the
 * mbulk pool with the smallest segment size. If no segment fitting
 * the requested size, then return NULL without trying to create
 * a chained buffer.
 *
 */
static inline struct mbulk *mbulk_alloc(enum mbulk_class clas, size_t dat_bufsz)
{
	return mbulk_with_signal_alloc(clas, 0, dat_bufsz);
}

/**
 * free mbulk buffer
 *
 * After checking the bulk reference counter, this function return the buffer
 * to the mbulk pool if it is zero. Note that this doesn't free the in-lined
 * signal buffer.
 */
static inline void mbulk_free(struct mbulk *m)
{
	mbulk_seg_free(m);
}

/**
 * get bulk buffer address for read or write access
 *
 * The address is the buffer address after the headroom in the mbulk segment.
 * Note that this function can only be used to access the data in the same
 * segment, including a segment in the mbulk chain (for example, to access
 * the 802.11 header of A-MSDU).
 *
 */
static inline void *mbulk_dat_rw(const struct mbulk *m)
{
	//BUG_ON(MBULK_SEG_IS_READONLY(m));
	return MBULK_SEG_DAT(m);
}

/**
 * get bulk buffer address for read-only
 *
 * The address is the buffer address after the headroom in the mbulk segment.
 * Note that this function can only be used to access the data in the same
 * segment, including a segment in the mbulk chain (for example, to access
 * the 802.11 header of A-MSDU).
 *
 */
static inline const void *mbulk_dat_r(const struct mbulk *m)
{
	return (const void *)MBULK_SEG_DAT(m);
}

/**
 * get bulk buffer address at the offset for read or write access
 *
 */
static inline void *mbulk_dat_at_rw(const struct mbulk *m, size_t off)
{
	//BUG_ON(MBULK_SEG_IS_READONLY(m));
	return MBULK_SEG_DAT_AT(m, off);
}

/**
 * get bulk buffer address at the offset for read access
 *
 */
static inline /*const */ void *mbulk_dat_at_r(const struct mbulk *m, size_t off)
{
	return (/*const */ void *)MBULK_SEG_DAT_AT(m, off);
}

/**
 * get valid data length
 *
 */
static inline size_t mbulk_tlen(const struct mbulk *m)
{
	return MBULK_SEG_LEN(m);
}

/**
 * get headroom
 *
 */
static inline size_t mbulk_headroom(const struct mbulk *m)
{
	return MBULK_SEG_HEADROOM(m);
}

static inline size_t mbulk_tailroom(const struct mbulk *m)
{
	return MBULK_SEG_TAILROOM(m);
}

/**
 * reserve headroom
 *
 * Note this API should be called right after mbulk is created or the valid
 * data length is zero.
 *
 */
static inline bool mbulk_reserve_head(struct mbulk *m, size_t headroom)
{
	return mbulk_seg_reserve_head(m, headroom);
}

/**
 * adjust the valid data range
 *
 * headroom would be placed after the signal buffer (or mbuf descriptor if
 * no in-lined signal), and the valid data length is set to \len.
 *
 */
static inline bool mbulk_adjust_range(struct mbulk *m, size_t headroom, size_t len)
{
	return mbulk_seg_adjust_range(m, headroom, len);
}

/**
 * extend the data range at the head
 *
 * The headroom would be reduced, and the data range is extended.
 * To prepend data in the head, the headroom should have been reserved before.
 *
 */
static inline bool mbulk_prepend_head(struct mbulk *m, size_t more)
{
	return mbulk_seg_prepend_head(m, more);
}

/**
 * extend the data at the tail
 *
 * Data range is expanded towards the tail.
 *
 */
static inline bool mbulk_append_tail(struct mbulk *m, size_t more)
{
	return mbulk_seg_append_tail(m, more);
}

/**
 * trim data at the head
 *
 * The headroom would be increased, and the valid data range is reduced
 * accordingly.
 *
 */
static inline bool mbulk_trim_head(struct mbulk *m, size_t less)
{
	return mbulk_seg_trim_head(m, less);
}

/**
 * trim data at the tail
 *
 * The data length would be reduced.
 *
 */
static inline bool mbulk_trim_tail(struct mbulk *m, size_t less)
{
	return mbulk_seg_trim_tail(m, less);
}

/**
 * duplicate a mbulk
 *
 * There is no data copy. but the referece counter of the orignal mbulk is
 * increased  by one.
 *
 */
static inline struct mbulk *mbulk_duplicate(struct mbulk *m)
{
	return mbulk_seg_duplicate(m);
}

/**
 * clone a mbulk
 *
 * New mbulk buffer is created, and contents are copied. The signal is copied
 * only when \copy_sig is TRUE.
 *
 */
static inline struct mbulk *mbulk_clone(const struct mbulk *m, enum mbulk_class clas, bool copy_sig)
{
	return mbulk_seg_clone(m, clas, copy_sig);
}

/**
 * allocate a signal buffer from mbulk pool
 *
 */
void *msignal_alloc(size_t sig_sz);

/**
 * free a signal buffer created from mbulk pool
 *
 */
void msignal_free(void *sig);

/**
 * get mbulk descriptor given a signal buffer address
 *
 */
struct mbulk *msignal_to_mbulk(void *sig);

/**
 * get next chained mbulk in a scatter/gathered list
 */
static inline scsc_mifram_ref mbulk_chain_next(struct mbulk *m)
{
	return MBULK_SEG_CHAIN_NEXT(m);
}

/**
 * init mbulk library
 */
/*extern void init_mbulk(void);*/
void init_mbulk(void *mem, size_t pool_size);

/**
 * add a memory zone to a mbulk pool list
 *
 */
int mbulk_pool_add(u8 pool_id, char *base, char *end, size_t buf_size, u8 guard);
/**
 * check sanity of a mbulk pool
 */
void mbulk_pool_check_sanity(u8 pool_id);

/**
 * configure the handler which returning the buffer to the host
 */
void mbulk_set_handler_return_host_mbulk(void (*free_host_buf)(struct mbulk *m));

/**
 * free a mbulk in the virtual host
 */
void mbulk_free_virt_host(struct mbulk *m);
void mbulk_pool_dump(u8 pool_id, int max_cnt);

void mbulk_pool_init(void);
#endif /*__MBULK_H__*/
