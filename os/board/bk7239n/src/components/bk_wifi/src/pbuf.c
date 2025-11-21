#include "lwip/pbuf.h"
//#include <os/mem.h>

#if (CONFIG_FULLY_HOSTED || CONFIG_SEMI_HOSTED)
struct pbuf *pbuf_alloc(int dummy0, uint32_t len, int dummy1)
{
	struct pbuf *p = wifi_zalloc(len + sizeof(*p));
	if (!p)
		return 0;

	p->next = 0;
	p->len = len;
	p->total_len = len;
	return p;
}

int pbuf_concat(struct pbuf *h, struct pbuf *t)
{
	struct pbuf *temp = h;

	if (!h || !t)
		return -1;

	while (1) {
		if (temp->next == 0) {
			temp->next = t;
			t->next = 0;
			h->total_len += t->len;	/* update header total_len */
			break;
		}
		temp = temp->next;
	}

	return 0;
}

void pbuf_free(struct pbuf *h)
{
	wifi_os_free(h);
}

int pbuf_free_all(struct pbuf *h)
{
	struct pbuf *t;

	while (h) {
		t = h->next;
		pbuf_free(h);
		h = t;
	}

	return 0;
}

void pbuf_ref(struct pbuf *p)
{

}
#endif

