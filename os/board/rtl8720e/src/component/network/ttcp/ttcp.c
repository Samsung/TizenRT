#include "lwipconf.h"
#include "timers.h"
#include "main.h"

#include <platform_stdlib.h>

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#include "diag.h"
#ifndef printf
#define printf DBG_8195A
#endif
//#define printf prvDiagPrintf

#define printk(fmt, ...)		do{printf("\n\r");printf(fmt, ##__VA_ARGS__);}while(0)
#define timer_get_ms() 			(xTaskGetTickCount()*(1000/configTICK_RATE_HZ))
#define mdelay(x) 				vTaskDelay(x*configTICK_RATE_HZ/1000)

#define STACKSIZE				(512*2)
#define TTCP_MODE_TRANSMIT		0
#define TTCP_MODE_RECEIVE		1
#define TX_SENT_LOOP			0
#define TX_TIMEOUT_CB			1
#define TX_SENT_CB				2
#define TX_MODE					TX_SENT_LOOP
//#define TX_MODE					TX_TIMEOUT_CB
//#define TX_MODE					TX_SENT_CB
#define USE_TIMER_MSG			1
#define MSG_TIME				(10000*configTICK_RATE_HZ/1000)	// msec
#define USE_PAYLOAD				0
#define CHECK_RECEIVE_BUF		0
#define DESTROY_RECEIVE_TTCP	1

typedef void (ttcp_done_cb_t)(void *opaque, int result);

struct ttcp {
	/* options */
	struct ip_addr addr;	/* host */
	uint16_t port;			/* -p */
	uint32_t nbuf;			/* -n */
	uint32_t buflen;		/* -l */
	uint8_t mode;			/* -t */
	uint8_t verbose;		/* -v */
	uint8_t udp;			/* -u */
	uint8_t tos;			/* -u */

	/* common */
	uint16_t print_cnt;
	uint32_t start_time;
	uint32_t last_time;
	uint64_t left;
	uint64_t recved;
	uint64_t last_bytes;
	ttcp_done_cb_t *done_cb;
	void *opaque;
	uint32_t tid;
	xTimerHandle thdl;
	xTimerHandle thdl_msg;

	/* TCP specific */
	struct tcp_pcb *tpcb;
	struct tcp_pcb *lpcb;
	char *payload;
	xSemaphoreHandle xSemaphore;

	/* UDP specific */
	int udp_started;
	uint16_t udp_end_marker_left;
	struct udp_pcb *upcb;
};

#if configGENERATE_RUN_TIME_STATS
static char cBuffer[ 512 ];
#endif

#if CHECK_RECEIVE_BUF
int cnt = 0 ;
char c = 0;
static void
ttcp_check_buf(struct ttcp *ttcp, struct pbuf *p)
{
	char PBPreamble[] = "PCAUSA PCATTCP Pattern";   // 22 Bytes
	struct pbuf *q;
	char *cp;
	int i, len;

	printk("TTCP [%p]: p->tot_len = %d\n", ttcp, p->tot_len);

	for (q = p; q != NULL; q = q->next) {
		cp = q->payload;
		len = q->len;
//		printk("TTCP [%p]: q->len = %d\n", ttcp, q->len);
//		for(i=0;i<q->len;i++)
//			printf("0x%02x, ", cp[i]);
		if (cnt == ttcp->buflen && cnt > 22) {
			if (memcmp(cp, PBPreamble, 22)) {
				printk("TTCP [%p]: Preamble error !!\n", ttcp);
			}
			len -= 22;
			cnt -= 22;
			cp += 22;
		}
		while (cnt-- > 0 && len-- > 0) {
			while (!isprint((c & 0x7F))) {
				c++;
			}
			if (*cp++ != (c++ & 0x7F)) {
				printk("TTCP [%p]: cp(0x%02x) != c(0x%02x), cnt = %d !!\n", ttcp, *cp, c, cnt);
			}
		}
		if (cnt == 0) {
			cnt = ttcp->buflen;
		}
	}
	printk("\n");
}
#endif

static void
ttcp_print_stats(struct ttcp *ttcp, int done)
{
	uint32_t ms;
	uint32_t kb;
	uint64_t bytes = ttcp->mode == TTCP_MODE_TRANSMIT ? (((uint64_t)ttcp->nbuf * (uint64_t)ttcp->buflen) - ttcp->left) : ttcp->recved;
//	uint32_t bytes = ttcp->mode == TTCP_MODE_TRANSMIT ?  (((uint32_t)ttcp->nbuf * (uint32_t)ttcp->buflen) - ttcp->left) : ttcp->recved;

	if (ttcp->verbose) {
		printk("\n");
	}
	if (done) {
		ms = timer_get_ms() - ttcp->start_time;
//		printk("TTCP [%p]: %llu bytes processed, total %d ms, average speed: %lld.%lld KB/s (%s/%s)\n" , ttcp, bytes, ms, bytes / ms, bytes % ms, ttcp->udp ? "udp" : "tcp", ttcp->mode == TTCP_MODE_TRANSMIT ? "tx" : "rx");
		kb = bytes >> 10;	// division 1024
		printk("TTCP [%p]: %d KB processed, total %d ms, average speed: %d.%d KB/s (%s/%s)\n", ttcp, kb, ms, kb / (ms / 1000), kb % (ms / 1000),
			   ttcp->udp ? "udp" : "tcp", ttcp->mode == TTCP_MODE_TRANSMIT ? "tx" : "rx");
	} else {
		uint32_t bytes_32;
		uint32_t last_time = timer_get_ms();
		uint64_t last_bytes = bytes;
		ms = last_time - ttcp->last_time;
		kb = bytes >> 10; // division 1024
		bytes_32 = last_bytes - ttcp->last_bytes;
//		printk("TTCP [%p]: %llu bytes processed, new ms = %d, now speed: %lld.%lld KB/s (%s/%s)\n" , ttcp, last_bytes, timer_get_ms(), bytes / ms, bytes % ms, ttcp->udp ? "udp" : "tcp", ttcp->mode == TTCP_MODE_TRANSMIT ? "tx" : "rx");
		printk("TTCP [%p]: %d KB processed, new ms = %d, now speed: %d.%d KB/s (%s/%s)\n", ttcp, kb, timer_get_ms(), bytes_32 / ms, bytes_32 % ms,
			   ttcp->udp ? "udp" : "tcp", ttcp->mode == TTCP_MODE_TRANSMIT ? "tx" : "rx");
		ttcp->last_time = last_time;
		ttcp->last_bytes = last_bytes;
	}
	printf("\n\r[MEM] available heap %d\n\r", xPortGetFreeHeapSize());
}


static void
ttcp_destroy(struct ttcp *ttcp)
{
	if (ttcp->tpcb) {
		tcp_arg(ttcp->tpcb, NULL);
		tcp_sent(ttcp->tpcb, NULL);
		tcp_recv(ttcp->tpcb, NULL);
		tcp_err(ttcp->tpcb, NULL);
		tcp_close(ttcp->tpcb);
	}

	if (ttcp->lpcb) {
		tcp_arg(ttcp->lpcb, NULL);
		tcp_accept(ttcp->lpcb, NULL);
		tcp_close(ttcp->lpcb);
	}

	if (ttcp->upcb) {
		udp_disconnect(ttcp->upcb);
		udp_remove(ttcp->upcb);
	}

	if (ttcp->payload) {
		free(ttcp->payload);
	}

#if (TX_MODE == TX_TIMEOUT_CB)
	if (ttcp->thdl) {
		xTimerDelete(ttcp->thdl, 1000);
	}
#endif

#if USE_TIMER_MSG
	if (ttcp->thdl_msg) {
		xTimerDelete(ttcp->thdl_msg, 1000);
	}
#endif
	free(ttcp);
}


static void
ttcp_done(struct ttcp *ttcp, int result)
{
	if (result == 0) {
		ttcp_print_stats(ttcp, 1);
	}

	if (ttcp->done_cb) {
		ttcp->done_cb(ttcp->opaque, result);
	}

#if (TX_MODE == TX_TIMEOUT_CB)
	if (ttcp->thdl) {
		xTimerStop(ttcp->thdl, 5000);
	}
#endif

#if USE_TIMER_MSG
	if (ttcp->thdl_msg) {
		xTimerStop(ttcp->thdl_msg, 5000);
	}
#endif

	printk("TTCP [%p]: ttcp_done !!!\n", ttcp);
	if (ttcp->mode == TTCP_MODE_TRANSMIT) {
		ttcp_destroy(ttcp);
	}
#if DESTROY_RECEIVE_TTCP
	if (ttcp->mode == TTCP_MODE_RECEIVE) { // Not destroy in TTCP_MODE_RECEIVE , it can accept next client to connect.
		ttcp_destroy(ttcp);
	}
#endif
}

static void
tcp_timer_msg(xTimerHandle pxTimer)
{
	struct ttcp *ttcp = pvTimerGetTimerID(pxTimer);
	ttcp_print_stats(ttcp, 0);
#if configGENERATE_RUN_TIME_STATS
	vTaskGetRunTimeStats((signed char *) cBuffer);
	MONITOR_LOG(cBuffer);
#endif
}

static void
tcp_send_loop(struct ttcp *ttcp)
{
	err_t err;
	uint32_t len, o_len;
	uint32_t cnt = 0;

	while (xSemaphoreTake(ttcp->xSemaphore, 30000 / portTICK_RATE_MS) != pdTRUE) {	// wait 30,000 ms
		cnt++;
		printk("TTCP [%p]: tcp_send_loop timeout count %d.\n", ttcp, cnt);
	}
	if (ttcp->tpcb == NULL) {
		printk("TTCP [%p]: tcp_send_loop connect error !!!\n", ttcp);
		return;
	}
	while (ttcp->left) {
		if (tcp_sndbuf(ttcp->tpcb) == 0) {
			mdelay(1);
			continue;
		}
		len = ttcp->left;
		/* don't send more than we have in the payload */
		if (len > ttcp->buflen) {
			len = ttcp->buflen;
		}
		/* We cannot send more data than space available in the send buffer. */
		if (len > tcp_sndbuf(ttcp->tpcb)) {
			len = tcp_sndbuf(ttcp->tpcb);
		}
		o_len = len;
		while (1) {
			err = tcp_write(ttcp->tpcb, ttcp->payload, len, 0);
			if (err != ERR_MEM) {
				break;
			}
			len = len >> 1;
			if (len == 0) {
//				printk("TTCP [%p]: len = %d, snd_buf = %d, snd_queuelen= %d\n", ttcp, len, ttcp->tpcb->snd_buf, ttcp->tpcb->snd_queuelen);
				break;
			}
		}

		if (err == ERR_OK) {
			ttcp->left -= len;
		} else {
			mdelay(1);
			static uint32_t xxx = 0;
			xxx++;
			if ((xxx % 1000) == 0) {
				printk("TTCP [%p]: tcp_write failed, err = %d, o_len = %d, len = %d, snd_buf = %d, snd_queuelen= %d\n", ttcp, err, o_len, len, ttcp->tpcb->snd_buf,
					   ttcp->tpcb->snd_queuelen);
			}

		}
		if (ttcp->tpcb == NULL) {
			printk("TTCP [%p]: tcp_send_loop exit because connection error !!!\n", ttcp);
			return;
		}
	}

	if (ttcp->tpcb->snd_queuelen) {
		printk("TTCP [%p]: queuelen = %d\n", ttcp, ttcp->tpcb->snd_queuelen);
	}
#if(1)
	cnt = 1200;
	while (cnt) {
		/* all sent - empty queue */
		if (ttcp->tpcb->snd_queuelen) {
			mdelay(10);
		} else {
			break;
		}
		cnt--;
	}
	if (cnt == 0) {
		printk("TTCP [%p]: snd_queuelen is not empty !!! queuelen = %d\n", ttcp, ttcp->tpcb->snd_queuelen);
	}
#endif
	ttcp_done(ttcp, 0);
}

#if (TX_MODE == TX_TIMEOUT_CB || TX_MODE == TX_SENT_CB)
static void
tcp_send_data(struct ttcp *ttcp)
{
	err_t err;
	uint64_t len;

	len = ttcp->left;
	/* don't send more than we have in the payload */
	if (len > ttcp->buflen) {
		len = ttcp->buflen;
	}

	/* We cannot send more data than space available in the send
		buffer. */
	if (tcp_sndbuf(ttcp->tpcb) == 0) {
		mdelay(1);
//		printk("TTCP [%p]: tcp_write buf no space\n", ttcp);
#if (TX_MODE == TX_TIMEOUT_CB)
		xTimerStart(ttcp->thdl, 1);
#endif
		return;
	}
	if (len > tcp_sndbuf(ttcp->tpcb)) {
		len = tcp_sndbuf(ttcp->tpcb);
	}

	do {
		err = tcp_write(ttcp->tpcb, ttcp->payload, len, 0);
		if (err == ERR_MEM) {
			len /= 2;
		}
	} while (err == ERR_MEM && len > 1);

	if (err == ERR_OK) {
		ttcp->left -= len;
	} else {
		printk("TTCP [%p]: tcp_write failed\n", ttcp);
	}
#if (TX_MODE == TX_TIMEOUT_CB)
	xTimerStart(ttcp->thdl, 1); // ttcp->tid = timer_sched_timeout_cb(0, TIMEOUT_ONESHOT, tcp_timeout_cb, ttcp);
#endif
}
#endif

#if (TX_MODE == TX_TIMEOUT_CB)
static void
tcp_timeout_cb(xTimerHandle pxTimer)
{
	struct ttcp *ttcp = pvTimerGetTimerID(pxTimer);

	if (ttcp->left > 0) {
		tcp_send_data(ttcp);
		if (ttcp->verbose) {
			if (0) {
				if (ttcp->print_cnt % 1000 == 0) {
					ttcp_print_stats(ttcp, 0);
				}
			} else {
				printk(".");
				if (ttcp->print_cnt % 80 == 0) {
					printk("\n");
				}
			}
			ttcp->print_cnt++;
		}
		return;
	}

	/* all sent - empty queue */
	if (ttcp->tpcb->snd_queuelen) {
		mdelay(10);
//		printk("TTCP [%p]: snd queue not empty\n", ttcp);
		xTimerStart(ttcp->thdl, 1); // ttcp->tid = timer_sched_timeout_cb(0, TIMEOUT_ONESHOT, tcp_timeout_cb, ttcp);
	} else {
		ttcp_done(ttcp, 0);
	}
}
#endif

#if (TX_MODE == TX_SENT_CB)
static err_t
tcp_sent_cb(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	struct ttcp *ttcp = arg;

	if (ttcp->left > 0) {
		tcp_send_data(ttcp);
		if (ttcp->verbose) {
			printk(".");
			if (ttcp->print_cnt % 80 == 0) {
				printk("\n");
			}
			ttcp->print_cnt++;
		}
	} else if (pcb->snd_queuelen == 0) {
		ttcp_done(ttcp, 0);
	}
	return ERR_OK;
}
#endif	// TX_SENT_CB

static err_t
tcp_connect_cb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	/* To avoid gcc warnings */
	(void) err;
	(void) tpcb;

	struct ttcp *ttcp = arg;

	ttcp->left = (uint64_t)ttcp->nbuf * ttcp->buflen;
	ttcp->start_time = timer_get_ms();
	ttcp->last_time = ttcp->start_time;
	printk("TTCP [%p]:  start time = %d , connect !!! \n", ttcp, ttcp->start_time);

#if (TX_MODE == TX_TIMEOUT_CB)
	tcp_send_data(ttcp);
#elif (TX_MODE == TX_SENT_CB)
	tcp_sent(tpcb, tcp_sent_cb);
	tcp_send_data(ttcp);
#elif (TX_MODE == TX_SENT_LOOP)
	xSemaphoreGive(ttcp->xSemaphore);
#else
#error
#endif
	return ERR_OK;
}


static void
tcp_conn_err_cb(void *arg, err_t err)
{
	struct ttcp *ttcp = arg;

	printk("TTCP [%p]: connection error !!!\n", ttcp);

	ttcp->tpcb = NULL; /* free'd by lwip upon return */
#if (TX_MODE == TX_SENT_LOOP)
	if (ttcp->mode == TTCP_MODE_TRANSMIT) {
		xSemaphoreGive(ttcp->xSemaphore);
	}
#endif
	ttcp_done(ttcp, err);
}


static err_t
tcp_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	/* To avoid gcc warnings */
	(void) err;

	struct ttcp *ttcp = arg;

	/* p will be NULL when remote end is done */
	if (p == NULL) {
		ttcp_done(ttcp, 0);
		return ERR_OK;
	}
	/* for print_stats() */
	ttcp->recved += p->tot_len;
	if (ttcp->verbose) {
		if (0) {
			if (ttcp->print_cnt % 1000 == 0) {
				ttcp_print_stats(ttcp, 0);
			}
		} else {
			printk(".");
			if (ttcp->print_cnt % 80 == 0) {
				printk("\n");
			}
		}
		ttcp->print_cnt++;
	}
#if CHECK_RECEIVE_BUF
	ttcp_check_buf(ttcp, p);
#endif
	tcp_recved(pcb, p->tot_len);
	pbuf_free(p);
	return ERR_OK;
}


static err_t
tcp_accept_cb(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	/* To avoid gcc warnings */
	(void) err;

	struct ttcp *ttcp = arg;

	ttcp->tpcb = newpcb;
	ttcp->recved = 0;
	ttcp->start_time = timer_get_ms();
	ttcp->last_time = ttcp->start_time;
	printk("TTCP [%p]: start time = %d , accept !!!  \n", ttcp, ttcp->start_time);

	tcp_recv(ttcp->tpcb, tcp_recv_cb);
	tcp_err(ttcp->tpcb, tcp_conn_err_cb);
	return ERR_OK;
}

static int
tcp_start(struct ttcp *ttcp)
{
	ttcp->tpcb = tcp_new();
	if (ttcp->tpcb == NULL) {
		printk("TTCP [%p]: could not allocate pcb\n", ttcp);
		return -1;
	}
	// set tos, packet priority
	ttcp->tpcb->tos = ttcp->tos << 5;

#if USE_PAYLOAD
	ttcp->payload = malloc(ttcp->buflen);
	if (ttcp->payload == NULL) {
		printk("TTCP [%p]: could not allocate payload\n", ttcp);
		return -1;
	}
#endif

	tcp_arg(ttcp->tpcb,  ttcp);

#if USE_TIMER_MSG
	ttcp->thdl_msg = xTimerCreate((const char *) "MsgTimer", MSG_TIME, pdTRUE, ttcp, tcp_timer_msg);
	xTimerStart(ttcp->thdl_msg, 1000);
#endif
	if (ttcp->mode == TTCP_MODE_TRANSMIT) {
		tcp_err(ttcp->tpcb,  tcp_conn_err_cb);
		tcp_recv(ttcp->tpcb, tcp_recv_cb);
		if (tcp_connect(ttcp->tpcb, &ttcp->addr, ttcp->port, tcp_connect_cb) != ERR_OK) {
			printk("TTCP [%p]: tcp connect failed\n", ttcp);
			return -1;
		}
#if (TX_MODE == TX_TIMEOUT_CB)
		ttcp->thdl = xTimerCreate((const signed char *) "SendTimer", 1, pdFALSE, ttcp, tcp_timeout_cb);
#elif (TX_MODE == TX_SENT_CB)
#elif (TX_MODE == TX_SENT_LOOP)
		vSemaphoreCreateBinary(ttcp->xSemaphore);
		xSemaphoreTake(ttcp->xSemaphore, 0);
		tcp_send_loop(ttcp);
#else
#error
#endif
	} else {
		tcp_bind(ttcp->tpcb, IP_ADDR_ANY, ttcp->port);
		ttcp->lpcb = tcp_listen(ttcp->tpcb);
		if (ttcp->lpcb == NULL) {
			printk("TTCP [%p]: listen failed\n", ttcp);
			return -1;
		}
		tcp_accept(ttcp->lpcb, tcp_accept_cb);
	}
	return 0;
}

#if (TX_MODE == TX_TIMEOUT_CB)
static void
udp_send_data(struct ttcp *ttcp);

static void udp_timeout_cb(xTimerHandle pxTimer)
{
	struct ttcp *ttcp = pvTimerGetTimerID(pxTimer);
	udp_send_data(ttcp);
}
#endif

static int
udp_send_bytes(struct ttcp *ttcp, uint32_t len)
{
	struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	if (p == NULL) {
		printk("TTCP [%p]: could not allocate pbuf\n", ttcp);
		return -1;
	}

	if (udp_send(ttcp->upcb, p) != ERR_OK) {
		printk("TTCP [%p]: udp_send() failed\n", ttcp);
		pbuf_free(p);
		return -1;
	}

	pbuf_free(p);
	return 0;
}

static void
udp_send_data(struct ttcp *ttcp)
{
	/* send start marker first time */
	if (!ttcp->udp_started) {
		if (udp_send_bytes(ttcp, 4) == 0) {
			ttcp->udp_started = 1;
			ttcp->start_time = timer_get_ms();
			ttcp->last_time = ttcp->start_time;
		}
	}
	/* normal case */
	else if (ttcp->left) {
		/* send data */
		if (udp_send_bytes(ttcp, ttcp->buflen) == 0) {
			ttcp->left -= ttcp->buflen;
		}
	}
	/* end marker? */
	else if (ttcp->left == 0 && ttcp->udp_end_marker_left) {
		if (udp_send_bytes(ttcp, 4) == 0) {
			ttcp->udp_end_marker_left--;
		}
	}
	/* all end markers sent */
	else if (ttcp->left == 0) {
		ttcp_done(ttcp, 0);
		return;
	}
#if (TX_MODE == TX_TIMEOUT_CB)
	xTimerStart(ttcp->thdl, 1); // ttcp->tid = timer_sched_timeout_cb(0, TIMEOUT_ONESHOT, udp_timeout_cb, ttcp);
#endif
}

static void
udp_recv_cb(void *arg, struct udp_pcb *upcb, struct pbuf *p,
			struct ip_addr *addr, u16_t port)
{
	/* To avoid gcc warnings */
	(void) addr;
	(void) port;
	(void) upcb;

	struct ttcp *ttcp = arg;

	/* got start marker? we might lose this so if we get it just reset
	 * the timer
	 */
	if (!ttcp->udp_started && p->tot_len <= 4) {
		ttcp->start_time = timer_get_ms();
		ttcp->last_time = ttcp->start_time;
		ttcp->udp_started = 1;
		goto out;
	}

	/* after receiving at least 1 byte, check end marker
	 * don't check udp_started since we might have lost the start marker
	 */
	if (ttcp->recved && p->tot_len <= 4) {
		ttcp_done(ttcp, 0);
		goto out;
	}

	/* for print_stats() */
	ttcp->recved += p->tot_len;
	if (ttcp->verbose) {
		printk(".");
		if (ttcp->print_cnt % 80 == 0) {
			printk("\n");
		}
		ttcp->print_cnt++;
	}

out:
	pbuf_free(p);
}


static int
udp_start(struct ttcp *ttcp)
{
	ttcp->udp_end_marker_left = 5;
	ttcp->upcb = udp_new();
	if (ttcp->upcb == NULL) {
		printk("TTCP [%p]: could not allocate pcb\n", ttcp);
		return -1;
	}

	if (ttcp->mode == TTCP_MODE_TRANSMIT) {
		if (udp_connect(ttcp->upcb, &ttcp->addr, ttcp->port) != ERR_OK) {
			printk("TTCP [%p]: udp connect failed\n", ttcp);
			return -1;
		}
#if (TX_MODE == TX_TIMEOUT_CB)
		ttcp->thdl = xTimerCreate((const signed char *) "SendTimer", 1, pdFALSE, ttcp, udp_timeout_cb);
#endif
		udp_send_data(ttcp);
	} else {
		udp_recv(ttcp->upcb, (udp_recv_fn)udp_recv_cb, ttcp);
	}

	return 0;
}


int
ttcp_start(struct ip_addr addr, uint16_t port, void *opaque,
		   ttcp_done_cb_t *done_cb,
		   uint8_t mode, uint32_t nbuf, uint32_t buflen, uint8_t udp, uint8_t verbose, uint8_t tos)
{
	struct ttcp *ttcp;
	int status;

	if (mode != TTCP_MODE_TRANSMIT && mode != TTCP_MODE_RECEIVE) {
		printk("TTCP [-]: invalid mode\n");
		return -1;
	}

	if (nbuf == 0) {
		printk("TTCP [-]: invalid nbuf\n");
		return -1;
	}

	if (buflen == 0) {
		printk("TTCP [-]: invalid buflen\n");
		return -1;
	}

	ttcp = malloc(sizeof(struct ttcp));
	if (ttcp == NULL) {
		printk("TTCP [-]: could not allocate memory for ttcp\n");
		return -1;
	}
	memset(ttcp, 0, sizeof(struct ttcp));
	ttcp->addr = addr;
	ttcp->port = port;
	ttcp->nbuf = nbuf;
	ttcp->mode = mode;
	ttcp->left = (uint64_t)nbuf * buflen;
	ttcp->done_cb = done_cb;
	ttcp->opaque = opaque;
	ttcp->udp = udp;
	ttcp->verbose = verbose;
	ttcp->buflen = buflen;
	ttcp->tos = tos;
#if CHECK_RECEIVE_BUF
	cnt = ttcp->buflen;
#endif

	if (ttcp->mode == TTCP_MODE_TRANSMIT) {
		printk("TTCP [%p]: port=%d, buflen=%d, nbuf=%d (%s/tx)\n",
			   ttcp, ttcp->port, ttcp->buflen, ttcp->nbuf,
			   ttcp->udp ? "udp" : "tcp");
	} else {
		printk("TTCP [%p]: port=%d, buflen=%d (%s/rx)\n",
			   ttcp, ttcp->port, ttcp->buflen,
			   ttcp->udp ? "udp" : "tcp");
	}

	if (ttcp->udp) {
		status = udp_start(ttcp);
	} else {
		status = tcp_start(ttcp);
	}

	if (status) {
		goto fail;
	}

	return 0;

fail:
	ttcp_destroy(ttcp);
	return -1;
}

void ttcp_test_rx(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

	uint8_t mode = TTCP_MODE_RECEIVE;
	uint8_t verbose = 0;
	uint32_t buflen = 1024;
	uint32_t nbuf = 1024;
	uint16_t port = 5001;
	uint8_t udp = 0;
	uint8_t tos = 0;
	struct ip_addr addr = { 0 };

	printf("\n\rttcp rx thread created");
	ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
	vTaskDelete(NULL);
}

void ttcp_test_tx(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

	uint8_t mode = TTCP_MODE_TRANSMIT;
	uint8_t verbose = 0;
	uint32_t buflen = 2048;
	uint32_t nbuf = 1024; // 2147483647;
	uint16_t port = 5002;
	uint8_t udp = 0;
	uint8_t tos = 0;
	char *ip = "192.168.1.2";
	struct ip_addr addr = { 0 };

	printf("\n\rttcp tx thread created");
#if LWIP_VERSION_MAJOR >= 2
	ip_addr_set_ip4_u32(&addr, inet_addr(ip));
	if (!ip_addr_get_ip4_u32(&addr)) {
		printk("%s: address type error!", __FUNCTION__);
		return;
	}
#else
	addr.addr = inet_addr(ip);
	if (!addr.addr) {
		printk("%s: address type error!", __FUNCTION__);
		return;
	}
#endif
	ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
	vTaskDelete(NULL);
}

//  test_mode = 0: rx mode
//  test_mode = 1: tx mode
void ttcp_test(int test_mode)
{

	if (test_mode == 0) {
		printf("\n\r%s: ttcp_test_rx", __FUNCTION__);
//		if(sys_thread_new("ttcp_test_rx", ttcp_test_rx, NULL, STACKSIZE, tskIDLE_PRIORITY + 1 ) == NULL)
		if (xTaskCreate(ttcp_test_rx, "ttcp_test_rx", STACKSIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
			printf("\n\r%s xTaskCreate ttcp_test_rx failed", __FUNCTION__);
		}

	} else {
		printf("\n\r%s: ttcp_test_tx", __FUNCTION__);
//		if(sys_thread_new("ttcp_test_tx", ttcp_test_tx, NULL, STACKSIZE, tskIDLE_PRIORITY + 1 ) == NULL)
		if (xTaskCreate(ttcp_test_tx, "ttcp_test_tx", STACKSIZE, NULL, tskIDLE_PRIORITY + 2,
						NULL) != pdPASS) {	// if priority + 1, pcb->snd_queuelen in tcp_send_loop() will not be 0.
			printf("\n\r%s xTaskCreatet tcp_test_tx failed", __FUNCTION__);
		}

	}
}

/*--------------------------------------------------------
// PCATTCP.exe
// char usage[] = "Usage: PCATTCP -t/-r \n\
//         -l      length of bufs written to network (default 1024)\n\
//         -n      number of bufs written to network (default 1024)\n\
//         -p      port number to send to (default rx:5001, tx:5002)\n\
//         -u      udp\n\
//         -v      verbose\n\
//                 ip\n";
//--------------------------------------------------------
// ttcp
// receive mode  : ttcp r [length] , ex: ttcp r 1024 ; default port = 5001
// transmit mode : ttcp t [length] [number] [ip], ex: ttcp t 1024 2048 192.168.1.2 ; default port = 5002
// r + t mode    : ttcp rt [length] [number] [ip], ex: ttcp rt 1024 2048 192.168.1.2
//--------------------------------------------------------
*/
void cmd_ttcp(int argc, char **argv)
{
	uint8_t mode = TTCP_MODE_RECEIVE;
	uint8_t udp = 0;
	uint8_t tos = 0;	// 0~7
	uint8_t verbose = 0;
	uint32_t buflen = 1024;
	uint32_t nbuf = 1024;
	uint16_t port = 5001;
	int ret = -1;
	struct ip_addr addr = { 0 };

	if (argc == 3 && strcmp(argv[1], "r") == 0) {
		// receive
		buflen = atoi(argv[2]);
		ret = ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
	} else if (argc == 5 && strcmp(argv[1], "t") == 0) {
		// transmit
		mode = TTCP_MODE_TRANSMIT;
#if LWIP_VERSION_MAJOR >= 2
		ip_addr_set_ip4_u32(&addr, inet_addr(argv[4]));
		if (!ip_addr_get_ip4_u32(&addr)) {
			printk("%s: address type error!", __FUNCTION__);
			return;
		}
#else
		addr.addr = inet_addr(argv[4]);
		if (!addr.addr) {
			printk("%s: address type error!", __FUNCTION__);
			return;
		}
#endif
		port = 5002;
		buflen = atoi(argv[2]);
		nbuf = atoi(argv[3]);
		ret = ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
	} else if (argc == 5 && (strcmp(argv[1], "rt") == 0 || strcmp(argv[1], "tr") == 0)) {
		// receive
		buflen = atoi(argv[2]);
		ret = ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
		if (ret != 0) {
			printk("%s: ttcp_start rx fail!", __FUNCTION__);
		}
		// transmit
		mode = TTCP_MODE_TRANSMIT;
#if LWIP_VERSION_MAJOR >= 2
		ip_addr_set_ip4_u32(&addr, inet_addr(argv[4]));
		if (!ip_addr_get_ip4_u32(&addr)) {
			printk("%s: address type error!", __FUNCTION__);
			return;
		}
#else
		addr.addr = inet_addr(argv[4]);
		if (!addr.addr) {
			printk("%s: address type error!", __FUNCTION__);
			return;
		}
#endif
		port = 5002;
		nbuf = atoi(argv[3]);
		ret = ttcp_start(addr, port, NULL, NULL, mode, nbuf, buflen, udp, verbose, tos);
	} else {
		printk("%s: ttcp command format error!", __FUNCTION__);
		return;
	}
	if (ret != 0) {
		printk("%s: ttcp_start fail!", __FUNCTION__);
	}
}
