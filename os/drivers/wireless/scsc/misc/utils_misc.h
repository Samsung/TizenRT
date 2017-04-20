/******************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 *****************************************************************************/

#ifndef SLSI_UTILS_MISC_H__
#define SLSI_UTILS_MISC_H__
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/irq.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>
#include <math.h>
#include <tinyara/compiler.h>
#include <arch/chip/irq.h>
#include <tinyara/kmalloc.h>
#include "utils_scsc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __bitwise
#define __force         __attribute__((force))
#define __aligned(x)    __attribute__((aligned(x)))

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, 64)

#define DECLARE_BITMAP(name, bits) \
	unsigned long name[BITS_TO_LONGS(bits)]

#define wmb() __asm__ __volatile__ ("" : : : "memory")
/* MAC Address Length */
#define ETH_ALEN        6
#ifndef ETH_P_WAPI
#define ETH_P_WAPI 0x88b4
#endif
#define ETH_P_IP   0x0800
#define ETH_P_ARP  0x0806

#define SSID_MAX_LEN 32

struct firmware {
	size_t size;
	const u8 *data;
	//struct page **pages;

	/* firmware loader private fields */
	void *priv;
};

static inline void *ERR_PTR(long error)
{
	return (void *)error;
}
/****************************************************************************
 * Private Functions
 ****************************************************************************/ static inline void set_bit(int nr, const long unsigned int *addr)
{
	uint8_t mask = BIT_MASK(nr);
	uint8_t *p = ((uint8_t *) addr) + BIT_BYTE(nr);

	*p |= mask;
}
static inline void clear_bit(int nr, const long unsigned int *addr)
{
	uint8_t mask = BIT_MASK(nr);
	uint8_t *p = ((uint8_t *) addr) + BIT_BYTE(nr);

	*p &= ~mask;
}

static inline int test_bit(int nr, const long unsigned int *addr)
{
	return 1 & (addr[BIT_BYTE(nr)] >> (nr & (BITS_PER_BYTE - 1)));
}

/*We can use ndbg,nlldbg,... defined under CONFIG_DEBUG_NET.
 * face compiler errors when we enable the CONFIG.*/

#define pr_err(_msg, args ...)   \
	lldbg(_msg, ## args)
#define pr_warn(_msg, args ...)  \
	lldbg(_msg, ## args)
#define pr_info(_msg, args ...)  \
	lldbg(_msg, ## args)
#define pr_debug(_msg, args ...)  \
	lldbg(_msg, ## args)

#define dev_info(dev, tag, args ...) \
	do { \
		UNUSED(dev); \
		lldbg(tag, ##args); \
	} while (0)

#define dev_err(dev, tag, args ...) \
	do { \
		UNUSED(dev); \
		lldbg(tag, ##args); \
	} while (0)

#define dev_warn(dev, tag, args ...) \
	do { \
		UNUSED(dev); \
		lldbg(tag, ##args); \
	} while (0)
#define dev_dbg(dev, tag, args ...) \
	do { \
		UNUSED(dev); \
		lldbg(tag, ##args); \
	} while (0)
/* spinlock related */
#ifndef CONFIG_SPINLOCK
#define spin_lock_irqsave(a, b) \
	UNUSED(a);  \
	b = irqsave();
#define spin_unlock_irqrestore(a, b) \
	UNUSED(a); \
	 irqrestore((irqstate_t)b);
#define spin_lock_init(a) UNUSED(a);
#endif							//CONFIG_SPINLOCK
#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

/**
 * bitmap_copy_le - copy a bitmap, putting the bits into little-endian order.
 * @dst:   destination buffer
 * @src:   bitmap to copy
 * @nbits: number of bits in the bitmap
 *
 * Require nbits % BITS_PER_LONG == 0.
 */

#ifdef CONFIG_LONG_IS_NOT_INT
#define BITS_PER_LONG 64
#else
#define BITS_PER_LONG 32
#endif
static inline void bitmap_zero(unsigned long *dst, int nbits)
{
	if (small_const_nbits(nbits)) {
		*dst = 0UL;
	} else {
		int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
		memset(dst, 0, len);
	}
}

static inline void bitmap_copy_le(void *dst, const unsigned long *src, int nbits)
{
	unsigned long *d = dst;
	int i;

	for (i = 0; i < nbits / BITS_PER_LONG; i++) {
		d[i] = cpu_to_le32(src[i]);
	}
}
#define __round_mask(x, y) ((__typeof__(x))((y) - 1))
#define round_up(x, y) ((((x) - 1) | __round_mask(x, y)) + 1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))
/**
 * get_first_bit - find first set bit in word
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long get_first_bit(unsigned long elem)
{
	int num = 0;

#if BITS_PER_LONG == 64
	if ((elem & 0xffffffff) == 0) {
		elem >>= 32;
		num += 32;
	}
#endif
	if ((elem & 0xffff) == 0) {
		elem >>= 16;
		num += 16;
	}
	if ((elem & 0xff) == 0) {
		elem >>= 8;
		num += 8;
	}
	if ((elem & 0xf) == 0) {
		elem >>= 4;
		num += 4;
	}
	if ((elem & 0x3) == 0) {
		elem >>= 2;
		num += 2;
	}
	if ((elem & 0x1) == 0) {
		num += 1;
	}

	return num;
}

#define min(x,y) ({ \
        typeof(x) _x = (x);     \
        typeof(y) _y = (y);     \
        (void) (&_x == &_y);    \
        _x < _y ? _x : _y; })

#define max(x,y) ({ \
        typeof(x) _x = (x);     \
        typeof(y) _y = (y);     \
        (void) (&_x == &_y);    \
        _x > _y ? _x : _y; })

static unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
{
	unsigned long idx;

	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx]) {
			return min(idx * BITS_PER_LONG + get_first_bit(addr[idx]), size);
		}
	}

	return size;
}
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
/*
 * This is a common helper function for find_next_bit and
 * find_next_zero_bit.  The difference is the "invert" argument, which
 * is XORed with each fetched word before searching it for one bits.
 */
static unsigned long _find_next_bit(const unsigned long *addr, unsigned long nbits, unsigned long start, unsigned long invert)
{
	unsigned long tmp;

	if (!nbits || start >= nbits) {
		return nbits;
	}

	tmp = addr[start / BITS_PER_LONG] ^ invert;

	/* Handle 1st word. */
	tmp &= BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits) {
			return nbits;
		}

		tmp = addr[start / BITS_PER_LONG] ^ invert;
	}

	return min(start + get_first_bit(tmp), nbits);
}

/*
 * Find the next set bit in a memory region.
 */
static unsigned long find_next_bit(const unsigned long *addr, unsigned long size, unsigned long offset)
{
	return _find_next_bit(addr, size, offset, 0UL);
}

#define for_each_set_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size));            \
	     (bit) < (size);                                    \
	     (bit) = find_next_bit((addr), (size), (bit) + 1))
#define ffz(x)  get_first_bit(~(x))
static inline unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
	unsigned long idx;

	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx] != ~0UL) {
			return min(idx * BITS_PER_LONG + ffz(addr[idx]), size);
		}
	}

	return size;
}

#ifdef __cplusplus
}
#endif
/*
 * SLSI doubly linked llist implementation.
 *
 */

struct slsi_dlist_head {
	struct slsi_dlist_head *lnext, *lprev;
};

/*__slsi_dlist_add -
 * Insert a new entry between two known consecutive entries.
 */
static inline void __slsi_dlist_add(struct slsi_dlist_head *new_node, struct slsi_dlist_head *lprev, struct slsi_dlist_head *lnext)
{
	new_node->lprev = lprev;
	lprev->lnext = new_node;
	lnext->lprev = new_node;
	new_node->lnext = lnext;
}

/**
 * slsi_dlist_add - add a new entry  after the specified head
 * @new_node: new entry to be added
 * @list_head: llist head to add it after
 *
 * Suitable for implementing stacks.
 */
static inline void slsi_dlist_add(struct slsi_dlist_head *new_node, struct slsi_dlist_head *list_head)
{
	__slsi_dlist_add(new_node, list_head, list_head->lnext);
}

/*
 * Delete a llist entry by  linking the given nodes  i.e  lprev/lnext entries
 * point to each other.
 *
 */
static inline void __slsi_dlist_del(struct slsi_dlist_head *lprev, struct slsi_dlist_head *lnext)
{
	lnext->lprev = lprev;
	lprev->lnext = lnext;
}

/**
 * slsi_dlist_del - deletes entry from llist.
 * @entry: the element to delete from the llist.
 * Note: llist_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void slsi_dlist_del(struct slsi_dlist_head *entry)
{
	__slsi_dlist_del(entry->lprev, entry->lnext);
	entry->lnext = NULL;
	entry->lprev = NULL;
}

/**
 * slsi_dllist_add_tail - add a new entry at end of the list
 * @new_node: new entry to be added
 * @list_head: llist head to add it before
 *
 * This is useful for implementing queues.
 */
static inline void slsi_dlist_add_tail(struct slsi_dlist_head *new_node, struct slsi_dlist_head *list_head)
{
	__slsi_dlist_add(new_node, list_head->lprev, list_head);
}

/**
 * slsi_dlist_empty - returns whether a llist is empty
 * @head: Head of the dlist.
 */
static inline int slsi_dlist_empty(const struct slsi_dlist_head *head)
{
	return head->lnext == head;
}

static inline void prefetch(__attribute__((unused))
							const void *x)
{
	;
}

#define container_of(ptr, type, member) ({				\
		const typeof(((type*)0)->member) *__mptr = (ptr);	\
		(type *)((char *)__mptr - offsetof(type, member));})

/**
 * dlist_entry - get the struct for this entry
 * @ptr:    the &struct slsi_dlist_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slsi_dlist_struct within the struct.
 */
#define slsi_dlist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * slsi_dlist_for_each_entry    -   iterate over dlist of given type
 * @pos:    the type * to use as a loop counter.
 * @head:   the head for your dlist.
 * @member: the name of the slsi_dlist_struct within the struct.
 */
#define slsi_dlist_for_each_entry(pos, head, member)                \
	for (pos = slsi_dlist_entry((head)->lnext, typeof(*pos), member),   \
	     prefetch(pos->member.lnext);            \
	     &pos->member != (head);                     \
	     pos = slsi_dlist_entry(pos->member.lnext, typeof(*pos), member),    \
	     prefetch(pos->member.lnext))

/**
 * slsi_dlist_for_each_safe -   iterate over a llist safe against removal of llist entry
 * @pos:    the &struct slsi_dlist_head to use as a loop counter.
 * @n:      another &struct slsi_dlist_head to use as temporary storage
 * @head:   the head for your llist.
 */
#define slsi_dlist_for_each_safe(pos, n, head) \
	for (pos = (head)->lnext, n = pos->lnext; pos != (head); \
	     pos = n, n = pos->lnext)

/**
 * slsi_dlist_for_each_entry_safe - iterate over llist of given type safe against removal of llist entry
 * @pos:    the type * to use as a loop counter.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your llist.
 * @member: the name of the llist_struct within the struct.
 */
#define  slsi_dlist_for_each_entry_safe(pos, n, head, member)           \
	for (pos = slsi_dlist_entry((head)->lnext, typeof(*pos), member), \
	     n = slsi_dlist_entry(pos->member.lnext, typeof(*pos), member);  \
	     &pos->member != (head);                     \
	     pos = n, n = slsi_dlist_entry(n->member.lnext, typeof(*n), member))

#define SLSI_DLIST_HEAD_INIT(name) { &(name), &(name) }

#define SLSI_INIT_DLIST_HEAD(ptr) do { \
		(ptr)->lnext = (ptr); (ptr)->lprev = (ptr); \
} while (0)

#endif							/* SLSI_UTILS_MISC_H__ */
