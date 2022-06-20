/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __SKBUFF_H__
#define __SKBUFF_H__

struct  sk_buff_head {
	struct list_head	*next, *prev;
	unsigned int 		qlen;
};

#ifdef CONFIG_TRACE_SKB
#define	TRACE_SKB_DEPTH			8
#endif

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;		/* Next buffer in list */
	struct sk_buff		*prev;		/* Previous buffer in list */

	struct sk_buff_head	*list;		/* List we are on */
	unsigned char		*head;		/* Head of buffer */
	unsigned char		*data;		/* Data head pointer */
	unsigned char		*tail;		/* Tail pointer	*/
	unsigned char		*end;		/* End pointer */
	void	*dev;		/* Device we arrived on/are leaving by */
	unsigned int 		len;		/* Length of actual data */
#ifdef CONFIG_TRACE_SKB
	unsigned int		liston[TRACE_SKB_DEPTH];	/* Trace the Lists we went through */
	const char		*funcname[TRACE_SKB_DEPTH];
	unsigned int		list_idx;	/* Trace the List we are on */
#endif

	int 			dyalloc_flag;

};

unsigned char *skb_put(struct sk_buff *skb, unsigned int len);
unsigned char *skb_pull(struct sk_buff *skb, unsigned int len);
void skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_assign_buf(struct sk_buff *skb, unsigned char *buf, unsigned int len);
unsigned char *skb_tail_pointer(const struct sk_buff *skb);
void skb_set_tail_pointer(struct sk_buff *skb, const int offset);
unsigned char *skb_end_pointer(const struct sk_buff *skb);

void init_skb_pool(void);
void init_skb_data_pool(void);

struct sk_buff *dev_alloc_skb(unsigned int length, unsigned int reserve_len);
void kfree_skb(struct sk_buff *skb);


#endif //__SKBUFF_H__

