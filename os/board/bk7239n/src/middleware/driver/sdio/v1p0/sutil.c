#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_uart.h"
#include "bk_sdio.h"
#include "sdio.h"
//#include "ll.h"
#include "sutil.h"
#include <os/mem.h>

//#include "rwnx_config.h"

#if defined(CONFIG_SDIO) || defined(CONFIG_SDIO_TRANS)
void su_init(SDIO_PTR sdio_ptr)
{
	UINT32 i;
	SDIO_NODE_PTR node_ptr;

#ifdef SDIO_PREALLOC
	UINT8 *buff_ptr;
	UINT32 buf_size = 1600;
#endif

	INIT_LIST_HEAD(&sdio_ptr->free_nodes);

	for (i = 0; i < CELL_COUNT; i ++) {
		node_ptr = &sdio_ptr->snode[i];

		node_ptr->callback = NULLPTR;
		node_ptr->Lparam   = NULL;
		node_ptr->Rparam   = NULL;
		node_ptr->addr     = 0;
		node_ptr->node_list.next = NULLPTR;
		node_ptr->node_list.prev = NULLPTR;

#ifdef SDIO_PREALLOC
		buff_ptr = (UINT8 *)os_malloc(CONFIG_MSDU_RESV_HEAD_LENGTH + buf_size
									  + CONFIG_MSDU_RESV_TAIL_LEN
									  + MALLOC_MAGIC_LEN);
		BK_ASSERT(buff_ptr);
		node_ptr->orig_addr = buff_ptr;
#endif

		su_push_node(&sdio_ptr->free_nodes, node_ptr);
	}
}

void su_push_node(LIST_HEADER_T *head, SDIO_NODE_PTR node)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	list_add_tail(&node->node_list, head);
	GLOBAL_INT_RESTORE();
}

SDIO_NODE_PTR su_pop_node(LIST_HEADER_T *head)
{
	LIST_HEADER_T *tmp;
	LIST_HEADER_T *pos;
	SDIO_NODE_PTR node;

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	node = NULLPTR;
	list_for_each_safe(pos, tmp, head) {
		list_del(pos);
		node = list_entry(pos, SDIO_NODE_T, node_list);

		break;
	}

	GLOBAL_INT_RESTORE();


	return node;
}

UINT32 su_get_node_count(LIST_HEADER_T *head)
{
	UINT32 count = 0;
	LIST_HEADER_T *tmp;
	LIST_HEADER_T *pos;

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	list_for_each_safe(pos, tmp, head) {
		count ++;
	}


	GLOBAL_INT_RESTORE();
	return count;
}

UINT32 su_align_power2(UINT32 size)
{
	UINT32 i = 1;

	while (i < size)
		i <<= 1;

	return i;
}
#endif
// EOF

