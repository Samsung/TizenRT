/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include <osdep_service.h>
#include <stdio.h>

/* For Smart, need to align malloc to 64bytes (cache line size of AP) for cache operations */
int align = 64;

void *rtw_vmalloc(u32 sz)
{
	void *pbuf = NULL;
	pbuf = kmm_memalign(align, sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	if (pbuf){
		DEBUG_SET_CALLER_ADDR(pbuf);
	}
#endif
	return pbuf;
}

void *rtw_zvmalloc(u32 sz)
{
	void *pbuf = NULL;

	pbuf = kmm_memalign(align, sz);
	if (pbuf){
		memset(pbuf, 0 ,sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(pbuf);
#endif
	}
	return pbuf;
}

void rtw_vmfree(u8 *pbuf, u32 sz)
{
	(void) sz;

	kmm_free(pbuf);
}

void *rtw_malloc(u32 sz)
{
	void *pbuf = NULL;
	pbuf = kmm_memalign(align, sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	if (pbuf){
		DEBUG_SET_CALLER_ADDR(pbuf);
	}
#endif
	return pbuf;
}

void *rtw_zmalloc(u32 sz)
{
	void *pbuf = NULL;

	pbuf = kmm_memalign(align, sz);
	if (pbuf){
		memset(pbuf, 0, sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(pbuf);
#endif
	}
	return pbuf;
}

void *rtw_calloc(u32 nelements, u32 elementSize)
{
	u32 sz = nelements * elementSize;
	void *pbuf = kmm_memalign(align, sz);
	if (pbuf){
		memset(pbuf, 0, sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(pbuf);
#endif
	}
	return pbuf;
}

void rtw_mfree(u8 *pbuf, u32 sz)
{
	(void) sz;

	kmm_free(pbuf);
}

void rtw_memcpy(void *dst, void *src, u32 sz)
{
	memcpy(dst, src, sz);
}

int rtw_memcmp(void *dst, void *src, u32 sz)
{
	if (!(memcmp(dst, src, sz))) {
		return 1;
	}

	return 0;
}

void rtw_memset(void *pbuf, int c, u32 sz)
{
	memset(pbuf, c, sz);
}

