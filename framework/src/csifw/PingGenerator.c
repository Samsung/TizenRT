#include "csifw/PingGenerator.h"
#include <netdb.h>
#include <lwip/prot/icmp.h>
#include <pthread.h>

extern COLLECT_STATE g_service_state;
extern CONNECTION_STATE g_nw_state;
static int gSocket = -1;
static struct sockaddr *gSocketAddr;
static struct addrinfo *gResult;
static unsigned int gPingInterval;
static pthread_t gPingThread;
static g_ping_count;

void* pingThreadFun(void *vargp);
static CSIFW_RES ping_generator_open_socket();
static CSIFW_RES ping_generator_close_socket();
static int setIcmp4Config(struct addrinfo *hints);
static bool gStopPingThread;

#define PING_TH_NAME "csifw_ping"

CSIFW_RES ping_generator_change_interval(unsigned int pingInterval)
{
	gPingInterval = pingInterval;
	CSIFW_LOGI("Ping interval set to %u MS", gPingInterval);
	return CSIFW_OK;
}

static u16_t standard_chksum(void *dataptr, u16_t len)
{
	u32_t acc;
	u16_t src;
	u8_t *octetptr;

	acc = 0;
	/* dataptr may be at odd or even addresses */
	octetptr = (u8_t *) dataptr;
	while (len > 1) {
		/* declare first octet as most significant
		   thus assume network order, ignoring host order */
		src = (*octetptr) << 8;
		octetptr++;
		/* declare second octet as least significant */
		src |= (*octetptr);
		octetptr++;
		acc += src;
		len -= 2;
	}
	if (len > 0) {
		/* accumulate remaining octet */
		src = (*octetptr) << 8;
		acc += src;
	}
	/* add deferred carry bits */
	acc = (acc >> 16) + (acc & 0x0000ffffUL);
	if ((acc & 0xffff0000UL) != 0) {
		acc = (acc >> 16) + (acc & 0x0000ffffUL);
	}
	/* This maybe a little confusing: reorder sum using htons()
	   instead of ntohs() since it has a little less call overhead.
	   The caller must invert bits for Internet sum ! */
	return htons((u16_t) acc);
}

static void ping_prepare_echo(struct icmp_echo_hdr *p_iecho, u16_t len)
{
	size_t i = 0;
	int icmp_hdrlen = sizeof(struct icmp_echo_hdr);
	ICMPH_CODE_SET(p_iecho, 0);
	p_iecho->id = 0xAFAF; //PING_ID;
	p_iecho->seqno = htons(1);

	/* fill the additional data buffer with some data */
	for (i = icmp_hdrlen; i < len; i++) {
		((char *)p_iecho)[i] = (char)i;
	}

	ICMPH_TYPE_SET(p_iecho, ICMP_ECHO);
	p_iecho->chksum = 0;
	p_iecho->chksum = ~standard_chksum(p_iecho, len);

}


static CSIFW_RES ping_send(struct icmp_echo_hdr *p_iecho)
{
	g_ping_count++;
	if(!(g_ping_count % 1000)) CSIFW_LOGD("ping_send(): ping sent %d times", g_ping_count);
    
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;

	ret = sendto(gSocket, p_iecho, icmplen, 0, gSocketAddr, addrlen);
	if (ret <= 0) {
		CSIFW_LOGE("sendto() return val is %d, errno: %d", ret, errno);
		perror("sendto");
		return CSIFW_ERROR;
	}

	return CSIFW_OK;
}

void* pingThreadFun(void *vargp)
{
	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;
	struct icmp_echo_hdr *p_iecho = NULL;

	/*	ToDo: Check this repetitive malloc */
	p_iecho = (struct icmp_echo_hdr *)malloc(icmplen);
	if (!p_iecho) {
		CSIFW_LOGE("ping_send: fail to allocate memory %zu", icmplen);
		ping_generator_close_socket();
		return NULL;
	}
	ping_prepare_echo(p_iecho, (u16_t)icmplen);

    while (!gStopPingThread) {
		usleep(gPingInterval);
		if (ping_send(p_iecho) == CSIFW_OK) {
		} else {
			CSIFW_LOGE("[T] ping_process: sendto error(%d)", errno);
		}
	}
	if (p_iecho) {
		free(p_iecho);
	}
	CSIFW_LOGD("[PING Thread] THREAD EXIT");
	return NULL;
}

CSIFW_RES ping_generator_start(void)
{
	if (gPingInterval == 0) {
		CSIFW_LOGD("Ping interval is 0 ==> NO PING START", gPingInterval);
		return CSIFW_OK;
	}
	CSIFW_RES res;
	g_ping_count = 0;
	gSocketAddr = NULL;
	gResult = NULL;
	gSocket = -1;
	gPingThread = 0;
	
	CSIFW_LOGI("Thread created and values initialized");
	// step1: open socket
	res = ping_generator_open_socket();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Start collect failed as socket open failed");
		return res;
	}

	// step2: thread create and values init
	gStopPingThread = false;
	if (pthread_create(&gPingThread, NULL, pingThreadFun, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data collect thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gPingThread, PING_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting ping thread name, error_no: %d", get_errno());
	}

	//Thread created now set other variables

	CSIFW_LOGD("[ping_state] started");
	return CSIFW_OK;
}

CSIFW_RES ping_generator_stop(CSIFW_REASON reason)
{
	if (gPingInterval == 0) { // means ping never started
		CSIFW_LOGI("Ping interval is 0 ==> PING STOP NOT REQUIRED", gPingInterval);
		return CSIFW_OK;
	}
	gStopPingThread = true;
	pthread_join(gPingThread, NULL);
	CSIFW_LOGI("Ping thread stopped");

	if (ping_generator_close_socket() != CSIFW_OK) {
		CSIFW_LOGE("[DC ping_state] socket close failed");
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("[ping_state] stopped(%d)", g_service_state);
	return CSIFW_OK;
}

static int setIcmp4Config(struct addrinfo *hints)
{
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
	return CSIFW_OK;
}

static CSIFW_RES ping_generator_open_socket()
{
	char ipv4_address[4];
	char ipv4_buf[16];
	const char *tAddr;
	if (netlib_get_ipv4addr("wlan0", ipv4_address) == 0) {
			snprintf(ipv4_buf, 16, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1], ipv4_address[2], 1);
	}
	
	struct timeval tv;
	struct timespec ping_time;
	struct addrinfo hints;
	struct addrinfo *rp = NULL;

	/* write information for getaddrinfo() */
	if (setIcmp4Config(&hints) != CSIFW_OK) {
		CSIFW_LOGE("ping_process: invalid IP address");
		return CSIFW_ERROR;
	}
	tAddr = ipv4_buf;
	CSIFW_LOGI("PING %s (%s)bytes of data.", tAddr, tAddr);

	/* get address information */
	if (getaddrinfo(tAddr, NULL, &hints, &gResult) != 0) {
		CSIFW_LOGE("ping_process: fail to get addrinfo");
		return CSIFW_ERROR;
	}
	/* try to find valid socket with address information */
	for (rp = gResult; rp != NULL; rp = rp->ai_next) {
		if ((gSocket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			/* continue if open is failed */
			continue;
		} else {
			/* success */
			CSIFW_LOGI("SOCKET SET!!!");
			break;
		}
	}
	CSIFW_LOGI("ping_socket: %d", gSocket);
	if (rp == NULL) {
		/* opening socket is totally failed */
		CSIFW_LOGE("ping_process: fail to create raw socket");
		ping_generator_close_socket();
		return CSIFW_ERROR;
	}

	/* copy the socket pointer we found */
	gSocketAddr = rp->ai_addr;
	if (!gSocketAddr)
	{
		CSIFW_LOGE("ping_send: invalid rp->ai_addr");
		ping_generator_close_socket();
		return CSIFW_ERROR;
	}

	if (gSocketAddr->sa_family != AF_INET) {
		CSIFW_LOGE("ping_send: invalid family");
		ping_generator_close_socket();
		return CSIFW_ERROR;
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	if (setsockopt(gSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != ERR_OK) {
		CSIFW_LOGE("ping_process: setsockopt error");
		ping_generator_close_socket();
		return CSIFW_ERROR;
	}
	CSIFW_LOGI("setsockopt Done");
	return CSIFW_OK;
}

static CSIFW_RES ping_generator_close_socket()
{
	if (gResult) {
		freeaddrinfo(gResult);
		CSIFW_LOGI("freeaddrinfo Done");
		}

	if (gSocket >= 0) {
		if (close(gSocket) < 0) {
			CSIFW_LOGE("close Socket failed");
		}
		gSocket = -1;
	}
	CSIFW_LOGI("Ping generator socket closed");
	return CSIFW_OK;
}
