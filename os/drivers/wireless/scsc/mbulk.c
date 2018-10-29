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
#include <sys/types.h>

#include "mbulk.h"
#include "utils_misc.h"
#include "dev.h"

#define MBULK_DEBUG

/* mbulk descriptor is aligned to 64 bytes considering the host processor's
 * cache line size
 */
#define MBULK_ALIGN                              (64)
#define MBULK_IS_ALIGNED(s)                      (((uintptr_t)(s) & (MBULK_ALIGN - 1)) == 0)
#define MBULK_SZ_ROUNDUP(s)                      round_up(s, MBULK_ALIGN)

/* a magic number to allocate the remaining buffer to the bulk buffer
 * in a segment. Used in chained mbulk allocation.
 */
#define MBULK_DAT_BUFSZ_REQ_BEST_MAGIC  ((u32)(-2))

pthread_mutex_t mbulk_pool_lock;

static inline void mbulk_debug(struct mbulk *m)
{
	(void)m;					/* may be unused */

	pr_debug("m->next %p: %d\n", &m->next, m->next);
	pr_debug("m->flag %p: %d\n", &m->flag, m->flag);
	pr_debug("m->clas %p: %d\n", &m->clas, m->clas);
	pr_debug("m->pid  %p: %d\n", &m->pid, m->pid);
	pr_debug("m->refcnt %p: %d\n", &m->refcnt, m->refcnt);
	pr_debug("m->dat_bufsz %p: %d\n", &m->dat_bufsz, m->dat_bufsz);
	pr_debug("m->sig_bufsz %p: %d\n", &m->sig_bufsz, m->sig_bufsz);
	pr_debug("m->len %p: %d\n", &m->len, m->len);
	pr_debug("m->head %p: %d\n", &m->head, m->head);
	pr_debug("m->chain_next_offset %p: %d\n", &m->chain_next_offset, m->chain_next_offset);
}

/* mbulk pool */
struct mbulk_pool {
	bool valid;							  /** is valid */
	u8 pid;								  /** pool id */
	struct mbulk *free_list;			  /** head of free segment list */
	int free_cnt;						  /** current number of free segments */
	int usage[MBULK_CLASS_MAX];			  /** statistics of usage per mbulk clas*/
	char *base_addr;					  /** base address of the pool */
	char *end_addr;						  /** exclusive end address of the pool */
	mbulk_len_t seg_size;				  /** segment size in bytes "excluding" struct mbulk */
	u8 guard;							  /** pool guard **/
#ifdef MBULK_DEBUG
	int tot_seg_num;					  /** total number of segments in this pool */
#endif
};

/* get a segment from a pool */
static inline struct mbulk *mbulk_pool_get(struct mbulk_pool *pool, enum mbulk_class clas)
{
	struct mbulk *m;
	u8 guard;

	if (pool == NULL) {
		return NULL;
	}

	pthread_mutex_lock(&mbulk_pool_lock);

	guard = pool->guard;
	m = pool->free_list;

	if (m == NULL || pool->free_cnt <= guard) {	/* guard */
		pthread_mutex_unlock(&mbulk_pool_lock);
		return NULL;
	}

	pool->free_cnt--;
	pool->usage[clas]++;

	pool->free_list = m->next;

	memset(m, 0, sizeof(*m));
	m->pid = pool->pid;
	m->clas = clas;

	pthread_mutex_unlock(&mbulk_pool_lock);
	return m;
}

/* put a segment to a pool */
static inline void mbulk_pool_put(struct mbulk_pool *pool, struct mbulk *m)
{
	if (m->flag == MBULK_F_FREE) {
		return;
	}

	pthread_mutex_lock(&mbulk_pool_lock);
	pool->usage[m->clas]--;
	pool->free_cnt++;

	m->flag = MBULK_F_FREE;
	m->next = pool->free_list;
	pool->free_list = m;
	pthread_mutex_unlock(&mbulk_pool_lock);
}

/** mbulk pool configuration */
struct mbulk_pool_config {
	mbulk_len_t seg_sz;		/** segment size "excluding" struct mbulk */
	int seg_num;			/** number of segments. If -1, all remaining space is used */
};

/** mbulk pools */
static struct mbulk_pool mbulk_pools[MBULK_POOL_ID_MAX];

#ifdef CONFIG_SLSI_WLAN_STATS
void mbulk_pool_stats(struct slsi_dev *sdev)
{
	sdev->dp_stats.mbulk_free_data = mbulk_pools[MBULK_CLASS_FROM_HOST_DAT].free_cnt;
	sdev->dp_stats.mbulk_free_ctrl = mbulk_pools[MBULK_CLASS_FROM_HOST_CTL].free_cnt;
}
#endif

/**
 * allocate a mbulk segment from the pool
 *
 * Note that the refcnt would be zero if \dat_bufsz is zero, as there is no
 * allocated bulk data.
 * If \dat_bufsz is \MBULK_DAT_BUFSZ_REQ_BEST_MAGIC, then this function
 * allocates all remaining buffer space to the bulk buffer.
 *
 */
static struct mbulk *mbulk_seg_generic_alloc(struct mbulk_pool *pool, enum mbulk_class clas, size_t sig_bufsz, size_t dat_bufsz)
{
	struct mbulk *m;

	if (pool == NULL) {
		return NULL;
	}

	/* get a segment from the pool */
	m = mbulk_pool_get(pool, clas);
	if (m == NULL) {
		return NULL;
	}

	/* signal buffer */
	m->sig_bufsz = (mbulk_len_t)sig_bufsz;
	if (sig_bufsz) {
		m->flag = MBULK_F_SIG;
	}

	/* data buffer.
	 * Note that data buffer size can be larger than the requested.
	 */
	m->head = m->sig_bufsz;
	if (dat_bufsz == 0) {
		m->dat_bufsz = 0;
		m->refcnt = 0;
	} else if (dat_bufsz == MBULK_DAT_BUFSZ_REQ_BEST_MAGIC) {
		m->dat_bufsz = pool->seg_size - m->sig_bufsz;
		m->refcnt = 1;
	} else {
		m->dat_bufsz = (mbulk_len_t)dat_bufsz;
		m->refcnt = 1;
	}

	return m;
}

/**
 * Allocate a bulk buffer with an in-lined signal buffer
 *
 * A mbulk segment is allocated from the given the pool, if its size
 * meeting the requested size.
 *
 */
struct mbulk *mbulk_with_signal_alloc_by_pool(u8 pool_id, u16 colour, enum mbulk_class clas, size_t sig_bufsz_req, size_t dat_bufsz)
{
	struct mbulk_pool *pool;
	size_t sig_bufsz;
	size_t tot_bufsz;
	struct mbulk *m_ret;

	/* data buffer should be aligned */
	sig_bufsz = MBULK_SIG_BUFSZ_ROUNDUP(sizeof(struct mbulk) + sig_bufsz_req) - sizeof(struct mbulk);

	if (pool_id >= MBULK_POOL_ID_MAX) {
		pr_err("Invalid pool_id: %d\n", pool_id);
		return NULL;
	}
	pool = &mbulk_pools[pool_id];

	if (!pool->valid) {
		pr_err("Invalid pool!\n");
		return NULL;
	}

	/* check if this pool meets the size */
	tot_bufsz = sig_bufsz + dat_bufsz;
	if (dat_bufsz != MBULK_DAT_BUFSZ_REQ_BEST_MAGIC && pool->seg_size < tot_bufsz) {
		return NULL;
	}

	m_ret = mbulk_seg_generic_alloc(pool, clas, sig_bufsz, dat_bufsz);
	/* Colour the mbulk */
	if (m_ret) {
		/* Use pool id for coding vif and peer_id */
		m_ret->pid = m_ret->pid | (colour & 0xfe);
		/* Code AC queue at the [7:6] bits */
		m_ret->clas = m_ret->clas | ((colour & 0x300) >> 2);
	}
	return m_ret;
}

struct mbulk_pool *mbulk_get_pool(u8 pool_id)
{
	if (pool_id >= MBULK_POOL_ID_MAX) {
		pr_err("Invalid pool ID: %d\n", pool_id);
		return NULL;
	}

	return &mbulk_pools[pool_id];
}

/**
 * add a memory zone to a mbulk pool list
 *
 */
int mbulk_pool_add(u8 pool_id, char *base, char *end, size_t seg_size, u8 guard)
{
	struct mbulk_pool *pool;
	struct mbulk *next;
	char *tmpNext = NULL;
	size_t byte_per_block;

	pool = mbulk_get_pool(pool_id);
	if (pool == NULL) {
		return -EINVAL;
	}

	if (!MBULK_IS_ALIGNED(base)) {
		pr_err("base address is not aligned\n");
		return -EINVAL;
	}

	/* total required memory per block */
	byte_per_block = MBULK_SZ_ROUNDUP(sizeof(struct mbulk) + seg_size);

	/* init pool structure */
	memset(pool, 0, sizeof(struct mbulk_pool));
	pool->pid = pool_id;
	pool->base_addr = base;
	pool->end_addr = end;
	pool->seg_size = (mbulk_len_t)(byte_per_block - sizeof(struct mbulk));
	pool->guard = guard;

	/* allocate segments */
	next = (struct mbulk *)base;
	while (((char *)next + byte_per_block) <= end) {
		memset(next, 0, sizeof(struct mbulk));
		next->pid = pool_id;

		/* add to the free list */
		next->next = pool->free_list;
		next->flag = MBULK_F_FREE;
		pool->free_list = next;
#ifdef MBULK_DEBUG
		pool->tot_seg_num++;
#endif
		pool->free_cnt++;

		tmpNext = ((char *)next + byte_per_block);
		next = (struct mbulk *)tmpNext;
	}

	pool->valid = (pool->free_cnt) ? true : false;

	return 0;
}

/**
 * add mbulk pools in MIF address space
 */
void mbulk_pool_dump(u8 pool_id, int max_cnt)
{
	struct mbulk_pool *pool;
	struct mbulk *m;
	int cnt = max_cnt;

	pool = &mbulk_pools[pool_id];
	m = pool->free_list;
	while (m != NULL && cnt--) {
		m = m->next;
	}
}

/**
 * free a mbulk in the virtual host
 */
void mbulk_free_virt_host(struct mbulk *m)
{
	u8 pool_id;
	struct mbulk_pool *pool;

	if (m == NULL) {
		return;
	}

	/* Remove colour */
	pool_id = m->pid & 0x1;

	pool = &mbulk_pools[pool_id];
	if (!pool->valid) {
		pr_err("Invalid pool.\n");
		return;
	}

	/* put to the pool */
	mbulk_pool_put(pool, m);
}

void mbulk_pool_init(void)
{
	pthread_mutex_init(&mbulk_pool_lock, NULL);
}
