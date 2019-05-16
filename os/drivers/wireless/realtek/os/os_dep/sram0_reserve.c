/* Important: Please link this file to SRAM memory
 */
 
#include <osdep_service.h>

/* NOTE: struct size must be a 2's power! */
typedef struct _MemChunk
{
	struct _MemChunk *next;
	int size;
} MemChunk;

typedef MemChunk heap_buf_t;

// A heap
typedef struct Heap
{
	struct _MemChunk *FreeList;     ///< Head of the free list
} Heap;

/**
 * Utility macro to allocate a heap of size \a size.
 *
 * \param name Variable name for the heap.
 * \param size Heap size in bytes.
 */
#define HEAP_DEFINE_BUF(name, size) \
	heap_buf_t name[((size) + sizeof(heap_buf_t) - 1) / sizeof(heap_buf_t)]

#define ROUND_UP2(x, pad) (((x) + ((pad) - 1)) & ~((pad) - 1))

#define RESERVED_HEAP_SIZE	(20*1024)	// Important: Must at least 20kbytes for WPS

static struct Heap g_reserved_heap;

static HEAP_DEFINE_BUF(reserved_heap, RESERVED_HEAP_SIZE);

static int g_heap_inited=0;
static	_lock	heap_lock;

static void reserved_heap_init(void)
{
	/* Initialize heap with a single big chunk */
	g_reserved_heap.FreeList = (MemChunk *)&reserved_heap;
	g_reserved_heap.FreeList->next = NULL;
	g_reserved_heap.FreeList->size = sizeof(reserved_heap);

	g_heap_inited = 1;
	rtw_spinlock_init(&heap_lock);
}

static void *reserved_heap_allocmem(int size)
{
	MemChunk *chunk, *prev;
	struct Heap* h = &g_reserved_heap;
	_irqL 	irqL;

	rtw_enter_critical(&heap_lock, &irqL);

	if(!g_heap_inited)	reserved_heap_init();

	/* Round size up to the allocation granularity */
	size = ROUND_UP2(size, sizeof(MemChunk));

	/* Handle allocations of 0 bytes */
	if (!size)
		size = sizeof(MemChunk);

	/* Walk on the free list looking for any chunk big enough to
	 * fit the requested block size.
	 */
	for (prev = (MemChunk *)&h->FreeList, chunk = h->FreeList;
		chunk;
		prev = chunk, chunk = chunk->next)
	{
		if (chunk->size >= size)
		{
			if (chunk->size == size)
			{
				/* Just remove this chunk from the free list */
				prev->next = chunk->next;

				rtw_exit_critical(&heap_lock, &irqL);

				return (void *)chunk;
			}
			else
			{
				/* Allocate from the END of an existing chunk */
				chunk->size -= size;

				rtw_exit_critical(&heap_lock, &irqL);

				return (void *)((uint8_t *)chunk + chunk->size);
			}
		}
	}

	rtw_exit_critical(&heap_lock, &irqL);

	return NULL; /* fail */
}


static void reserved_heap_freemem(void *mem, int size)
{
	MemChunk *prev;
	struct Heap* h = &g_reserved_heap;
	_irqL 	irqL;

	rtw_enter_critical(&heap_lock, &irqL);
	
	if(!g_heap_inited)	reserved_heap_init();

	/* Round size up to the allocation granularity */
	size = ROUND_UP2(size, sizeof(MemChunk));

	/* Handle allocations of 0 bytes */
	if (!size)
		size = sizeof(MemChunk);

	/* Special cases: first chunk in the free list or memory completely full */
	if (((uint8_t *)mem) < ((uint8_t *)h->FreeList) || !h->FreeList)
	{
		/* Insert memory block before the current free list head */
		prev = (MemChunk *)mem;
		prev->next = h->FreeList;
		prev->size = size;
		h->FreeList = prev;
	}
	else /* Normal case: not the first chunk in the free list */
	{
		/*
		 * Walk on the free list. Stop at the insertion point (when mem
		 * is between prev and prev->next)
		 */
		prev = h->FreeList;
		while (prev->next < (MemChunk *)mem && prev->next)
			prev = prev->next;

		/* Should it be merged with previous block? */
		if (((uint8_t *)prev) + prev->size == ((uint8_t *)mem))
		{
			/* Yes */
			prev->size += size;
		}
		else /* not merged with previous chunk */
		{
			MemChunk *curr = (MemChunk*)mem;

			/* insert it after the previous node
			 * and move the 'prev' pointer forward
			 * for the following operations
			 */
			curr->next = prev->next;
			curr->size = size;
			prev->next = curr;

			/* Adjust for the following test */
			prev = curr;
		}
	}

	/* Also merge with next chunk? */
	if (((uint8_t *)prev) + prev->size == ((uint8_t *)prev->next))
	{
		prev->size += prev->next->size;
		prev->next = prev->next->next;
	}
	
	rtw_exit_critical(&heap_lock, &irqL);
}

int sram0_reserve_free_size(void)
{
	int free_mem = 0;
	struct Heap* h = &g_reserved_heap;
	_irqL 	irqL;
	MemChunk *chunk;

	rtw_enter_critical(&heap_lock, &irqL);
	
	if(!g_heap_inited)	reserved_heap_init();
	
	for (chunk = h->FreeList; chunk; chunk = chunk->next)
		free_mem += chunk->size;

	rtw_exit_critical(&heap_lock, &irqL);
	return free_mem;
}

void *sram0_reserve_malloc(int size)
{
	int *mem;

	size += sizeof(int);
	if ((mem = (int*)reserved_heap_allocmem(size))){
		*mem++ = size;
	}

	return mem;
}

void *sram0_reserve_calloc(int num, int size)
{
	void *mem;

	if ((mem = sram0_reserve_malloc(num * size)))
		memset(mem, 0, num * size);

	return mem;
}

void sram0_reserve_free(void *mem)
{
	int *_mem = (int *)mem;

	if (_mem)
	{
		--_mem;
		reserved_heap_freemem(_mem, *_mem);
	}
}


