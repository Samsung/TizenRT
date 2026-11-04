/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
//#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#ifdef _OPENWRT_
#include <sys/time.h>
#endif
#include <time.h>
//#include <sys/socket.h>
//#include <sys/ioctl.h>
#include <sys/stat.h>
//#include <linux/if.h>
//#include <netinet/in.h>
//#include <netinet/ip.h>
//#include <netinet/ip_icmp.h>
//#include <arpa/inet.h>
//#include <ifaddrs.h>
#include <stdint.h>
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

#include "vendor_specific.h"
#include "utils.h"
#include "controlapp_eloop.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <tinyara/mm/mm.h>
#include <os/mem.h>
#include <os/os.h>
#include "bk_netif_addr.h"

/* Log */
int stdout_level = LOG_LEVEL_ERROR;
int syslog_level = LOG_LEVEL_INFO;

/* multiple VAPs */
int interface_count = 0;
int configured_interface_count = 0;
struct interface_info interfaces[8];
int band_mbssid_cnt[16];
struct interface_info* default_interface;
static struct loopback_info loopback = {.sock = -1};

#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
int use_openwrt_wpad = 0;
#endif

#if 0
int strnlen(const char *s, int maxlen)
{
	int i = 0;

	while (s[i++] != '\0' && (i < maxlen))
		;
	return i;
}
#endif

void send_continuous_loopback_packet(void *eloop_ctx, void *sock_ctx);

void debug_print_timestamp(void) {
    time_t rawtime;
    struct tm *info;
    char buffer[32];

    time(&rawtime);
    info = localtime(&rawtime);
    if (info) {
        strftime(buffer, sizeof(buffer), "%b %d %H:%M:%S", info);
    }
    printf("%s ", buffer);
}

void indigo_logger(int level, const char *fmt, ...) {
    char *format, *log_type;
    int maxlen;
#if CONTROLAPPC_LINUX_API
    int priority;
#endif
    va_list ap;

    maxlen = strlen(fmt) + 100;
    format = os_malloc(maxlen);
    if (!format) {
		bk_printf("%s %d\n", __func__, __LINE__);
        return;
    }

    switch (level) {
    case LOG_LEVEL_DEBUG_VERBOSE:
        log_type = "debugverbose";
        break;
    case LOG_LEVEL_DEBUG:
        log_type = "debug";
        break;
    case LOG_LEVEL_INFO:
        log_type = "info";
        break;
    case LOG_LEVEL_NOTICE:
        log_type = "notice";
        break;
    case LOG_LEVEL_WARNING:
        log_type = "warning";
        break;
    default:
        log_type = "info";
        break;
    }

    snprintf(format, maxlen, "controlappc.%8s  %s", log_type, fmt);

    if (level >= stdout_level) {
		// bk_printf("%s %d\n", __func__, __LINE__);
        //debug_print_timestamp();
        va_start(ap, fmt);
        vsprintf(format, fmt, ap);
        va_end(ap);
		bk_printf(format);
        bk_printf("\n");
    }

#if CONTROLAPPC_LINUX_API
    if (level >= stdout_level) {
        switch (level) {
        case LOG_LEVEL_DEBUG_VERBOSE:
        case LOG_LEVEL_DEBUG:
                priority = LOG_DEBUG;
                break;
        case LOG_LEVEL_INFO:
                priority = LOG_INFO;
                break;
        case LOG_LEVEL_NOTICE:
                priority = LOG_NOTICE;
                break;
        case LOG_LEVEL_WARNING:
                priority = LOG_WARNING;
                break;
        default:
                priority = LOG_INFO;
                break;
        }
        va_start(ap, fmt);
        vsyslog(priority, format, ap);
        va_end(ap);
    }
#endif
	if (format)
		os_free(format);
}

#ifdef CONTROLAPPC_LINUX_API
/* System */
int pipe_command(char *buffer, int buffer_size, char *cmd, char *parameter[]) {
    int pipefds[2], len;
    pid_t pid;

    if (pipe(pipefds) == -1){
        indigo_logger(LOG_LEVEL_ERROR, "Failed to create the pipe");
        return -1;
    }

    pid = fork();
    if (pid == -1) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to fork");
        return -1;
    }

    if (pid == 0) {
        // Replace stdout with the write end of the pipe
        dup2(pipefds[1], STDOUT_FILENO);
        // Close read to pipe, in child
        close(pipefds[0]);
        execv(cmd, parameter);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefds[1]);
        len = read(pipefds[0], buffer, buffer_size);
        indigo_logger(LOG_LEVEL_DEBUG_VERBOSE, "Pipe system call= %s, Return length= %d, result= %s", cmd, len, buffer);
        close(pipefds[0]);
        wait(NULL); /* Parent waits for the child to terminate */
    }
    return len;
}
#endif

char* read_file(char *fn) {
    struct stat st;
    int fd, size;
    char *buffer = NULL;

    memset(&st, 0, sizeof(struct stat));
    stat(fn, &st);
    size = st.st_size;

    fd = open(fn, O_RDONLY);
    if (fd) {
        buffer = (char*)os_malloc(sizeof(char)*(size+1));
        memset(buffer, 0, size+1);
        read(fd, buffer, size);
        close(fd);
    }
    return buffer;
}

int write_file(char *fn, char *buffer, int len) {
    int fd;

    fd = open(fn, O_CREAT | O_WRONLY | O_TRUNC);
    if (fd > 0) {
        (void)write(fd, buffer, len);
        close(fd);
        return 0;
    }
    return -1;
}

int append_file(char *fn, char *buffer, int len) {
    int fd;

    fd = open(fn, O_CREAT | O_WRONLY | O_APPEND);
    if (fd > 0) {
        (void)write(fd, buffer, len);
        close(fd);
        return 0;
    }
    return -1;
}

/* strrstr(), reversed strstr(), is not available in some compilers. Here is the implementation. */
static char* indigo_strrstr(char *input, const char *token) {
    char *result = NULL, *p = NULL;

    if (*token == '\0') {
        return (char *) input;
    }

    while (1) {
        p = strstr(input, token);
        if (p == NULL)
            break;
        result = p;
        input = p + 1;
    }

    return result;
}

/* Loopback */
int loopback_socket = -1;
static beken_thread_t loopback_server_thread_handle = NULL;
static beken_semaphore_t loopback_server_ready = NULL;
static volatile int loopback_server_stop_requested;
volatile int g_qt_loopback_fast_echo_enabled;
volatile unsigned short g_qt_loopback_fast_echo_port;
volatile unsigned char *g_qt_loopback_fast_echo_buf;
volatile unsigned short g_qt_loopback_fast_echo_buf_len;

#define CONTROLAPPC_PSRAM_HEAP_INDEX 1
#define CONTROLAPPC_LOOPBACK_STACK_SIZE 2048

struct loopback_server_context {
    char local_ip[64];
    char local_port[16];
    int timeout;
    int result;
};

static struct loopback_server_context loopback_server_ctx;

static unsigned char *qt_loopback_buffer_alloc(size_t size, int *from_psram)
{
    unsigned char *buffer = NULL;

    if (from_psram) {
        *from_psram = 0;
    }

#if defined(CONFIG_PSRAM_AS_SYS_MEMORY) && defined(CONFIG_MM_KERNEL_HEAP) && \
    CONFIG_KMM_NHEAPS > CONTROLAPPC_PSRAM_HEAP_INDEX
    buffer = (unsigned char *)kmm_malloc_at(CONTROLAPPC_PSRAM_HEAP_INDEX, size);
    if (buffer) {
        if (from_psram) {
            *from_psram = 1;
        }
        return buffer;
    }
#endif

    return (unsigned char *)os_malloc(size);
}

static void qt_loopback_buffer_free(unsigned char *buffer, int from_psram)
{
    if (!buffer) {
        return;
    }

#if defined(CONFIG_PSRAM_AS_SYS_MEMORY) && defined(CONFIG_MM_KERNEL_HEAP) && \
    CONFIG_KMM_NHEAPS > CONTROLAPPC_PSRAM_HEAP_INDEX
    if (from_psram) {
        kmm_free(buffer);
        return;
    }
#else
    (void)from_psram;
#endif

    os_free(buffer);
}

static void loopback_server_receive_message(int sock, void *eloop_ctx, void *sock_ctx) {
    struct sockaddr_storage from;
    unsigned char buffer[BUFFER_LEN];
    int len;
	socklen_t fromlen;

    fromlen = sizeof(from);
    len = recvfrom(sock, buffer, BUFFER_LEN, 0, (struct sockaddr *) &from, &fromlen);
    if (len < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Loopback server recvfrom[server] error");
        return ;
    }

    indigo_logger(LOG_LEVEL_INFO, "Loopback server received length = %d", len);

    len = sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (struct sockaddr *)&from, sizeof(from));

    indigo_logger(LOG_LEVEL_INFO, "Loopback server echo back length = %d", len);
}

static void loopback_server_timeout(void *eloop_ctx, void *timeout_ctx) {
    int s = (intptr_t)eloop_ctx;
    controlapp_eloop_unregister_read_sock(s);
    close(s);
    loopback_socket = -1;
    indigo_logger(LOG_LEVEL_INFO, "Loopback server stops");
}

static void loopback_server_thread(beken_thread_arg_t arg)
{
    int s = -1;
    int len;
    int echo_len;
    time_t start_time;
    socklen_t fromlen;
    socklen_t addr_len;
    struct timeval tv;
    struct sockaddr_in addr;
    struct sockaddr_storage from;
    unsigned char *buffer = NULL;
    unsigned char *fast_echo_buffer = NULL;
    int buffer_from_psram = 0;
    int fast_echo_buffer_from_psram = 0;

    (void)arg;
    loopback_server_ctx.result = -1;

    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        printf("[QT] loopback server socket failed errno=%d\n", errno);
        goto ready;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (loopback_server_ctx.local_ip[0]) {
        addr.sin_addr.s_addr = inet_addr(loopback_server_ctx.local_ip);
    }

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("[QT] loopback server bind %s failed errno=%d\n",
               loopback_server_ctx.local_ip, errno);
        close(s);
        s = -1;
        goto ready;
    }

    addr_len = sizeof(addr);
    if (getsockname(s, (struct sockaddr *)&addr, &addr_len) < 0) {
        printf("[QT] loopback server getsockname failed errno=%d\n", errno);
        close(s);
        s = -1;
        goto ready;
    }

    buffer = qt_loopback_buffer_alloc(BUFFER_LEN, &buffer_from_psram);
    if (!buffer) {
        printf("[QT] loopback server buffer alloc failed len=%u\n",
               (unsigned int)BUFFER_LEN);
        close(s);
        s = -1;
        goto ready;
    }

    fast_echo_buffer = qt_loopback_buffer_alloc(1514, &fast_echo_buffer_from_psram);
    if (!fast_echo_buffer) {
        printf("[QT] loopback fast echo buffer alloc failed len=1514\n");
        qt_loopback_buffer_free(buffer, buffer_from_psram);
        close(s);
        s = -1;
        goto ready;
    }

    sprintf(loopback_server_ctx.local_port, "%d", ntohs(addr.sin_port));
    loopback_socket = s;
    g_qt_loopback_fast_echo_buf = fast_echo_buffer;
    g_qt_loopback_fast_echo_buf_len = 1514;
    g_qt_loopback_fast_echo_port = ntohs(addr.sin_port);
    g_qt_loopback_fast_echo_enabled = 1;
    loopback_server_ctx.result = 0;
    printf("[QT] loopback server start: ip=%s port=%s\n",
           loopback_server_ctx.local_ip, loopback_server_ctx.local_port);
    sta_send_gratuitous_arp();
    rtos_delay_milliseconds(100);

ready:
    if (loopback_server_ready) {
        rtos_set_semaphore(&loopback_server_ready);
    }
    if (s < 0) {
        loopback_server_thread_handle = NULL;
        rtos_delete_thread(NULL);
        return;
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    start_time = time(NULL);

    while (!loopback_server_stop_requested) {
        if (loopback_server_ctx.timeout > 0 &&
            time(NULL) - start_time >= loopback_server_ctx.timeout) {
            break;
        }

        fromlen = sizeof(from);
        len = recvfrom(s, buffer, BUFFER_LEN, 0, (struct sockaddr *)&from, &fromlen);
        if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            printf("[QT] loopback server recv failed errno=%d\n", errno);
            continue;
        }

        echo_len = sendto(s, (const char *)buffer, len, MSG_CONFIRM,
                          (struct sockaddr *)&from, fromlen);
        if (echo_len != len) {
            printf("[QT] loopback server echo failed len=%d ret=%d errno=%d\n",
                   len, echo_len, errno);
        }
    }

    close(s);
    loopback_socket = -1;
    g_qt_loopback_fast_echo_enabled = 0;
    g_qt_loopback_fast_echo_port = 0;
    g_qt_loopback_fast_echo_buf = NULL;
    g_qt_loopback_fast_echo_buf_len = 0;
    qt_loopback_buffer_free(fast_echo_buffer, fast_echo_buffer_from_psram);
    qt_loopback_buffer_free(buffer, buffer_from_psram);
    loopback_server_thread_handle = NULL;
    printf("[QT] loopback server stop\n");
    rtos_delete_thread(NULL);
}

int loopback_server_start(char *local_ip, char *local_port, int timeout) {
#ifdef BEKEN_API
    bk_err_t ret;

    if (loopback_server_thread_handle || loopback_socket >= 0) {
        loopback_server_stop();
    }

    memset(&loopback_server_ctx, 0, sizeof(loopback_server_ctx));
    if (local_ip) {
        snprintf(loopback_server_ctx.local_ip, sizeof(loopback_server_ctx.local_ip),
                 "%s", local_ip);
    }
    loopback_server_ctx.timeout = timeout;
    loopback_server_stop_requested = 0;

    if (rtos_init_semaphore(&loopback_server_ready, 1) != kNoErr) {
        return -1;
    }

    ret = rtos_create_thread(&loopback_server_thread_handle,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "qt_loopback",
                             (beken_thread_function_t)loopback_server_thread,
                             CONTROLAPPC_LOOPBACK_STACK_SIZE,
                             (beken_thread_arg_t)0);
    if (ret != kNoErr) {
        rtos_deinit_semaphore(&loopback_server_ready);
        loopback_server_ready = NULL;
        loopback_server_thread_handle = NULL;
        return -1;
    }

    ret = rtos_get_semaphore(&loopback_server_ready, 3000);
    rtos_deinit_semaphore(&loopback_server_ready);
    loopback_server_ready = NULL;

    if (ret != kNoErr || loopback_server_ctx.result != 0) {
        return -1;
    }

    strcpy(local_port, loopback_server_ctx.local_port);
    return 0;
#else
    int s = 0;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

   /* Open UDP socket */
    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open server socket");
        return -1;
    }

    /* Bind specific IP */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (local_ip) {
        addr.sin_addr.s_addr = inet_addr(local_ip);
    }

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to bind server socket");
        close(s);
        return -1;
    }

    if (getsockname(s, (struct sockaddr *)&addr, &len) == -1) {
        indigo_logger(LOG_LEVEL_INFO, "Failed to get socket port number");
        close(s);
        return -1;
    } else {
        indigo_logger(LOG_LEVEL_INFO, "loopback server port number %d\n", ntohs(addr.sin_port));
        sprintf(local_port, "%d", ntohs(addr.sin_port));
    }

    /* Register to eloop and ready for the socket event */
    if (controlapp_eloop_register_read_sock(s, loopback_server_receive_message, NULL, NULL)) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to initiate ControlAppC");
        return -1;
    }
    loopback_socket = s;
    controlapp_eloop_register_timeout(timeout, 0, loopback_server_timeout, (void*)(intptr_t)s, NULL);
    indigo_logger(LOG_LEVEL_INFO, "Loopback Client starts ip %s port %s", local_ip, local_port);

    return 0;
#endif
}

int loopback_server_stop() {
#ifdef BEKEN_API
    int wait_ms = 0;

    loopback_server_stop_requested = 1;
    g_qt_loopback_fast_echo_enabled = 0;
    g_qt_loopback_fast_echo_port = 0;
    g_qt_loopback_fast_echo_buf = NULL;
    g_qt_loopback_fast_echo_buf_len = 0;
    while ((loopback_server_thread_handle || loopback_socket >= 0) && wait_ms < 1500) {
        rtos_delay_milliseconds(20);
        wait_ms += 20;
    }
    return 0;
#else
    if (loopback_socket >= 0) {
        controlapp_eloop_cancel_timeout(loopback_server_timeout, (void*)(intptr_t)loopback_socket, NULL);
        controlapp_eloop_unregister_read_sock(loopback_socket);
        close(loopback_socket);
        loopback_socket = -1;
    }
    return 0;
#endif
}

int loopback_server_status() {
    return loopback_socket >= 0;
}

unsigned short icmp_checksum(unsigned short *buf, int size)
{
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buf;
        buf++;
        size -= 2;
    }
    if (size == 1)
        sum += *(unsigned char *)buf;
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

void setup_icmphdr(u_int8_t type, u_int8_t code, u_int16_t id,
    u_int16_t seq, struct icmphdr *icmphdr, int packet_size)
{
    memset(icmphdr, 0, sizeof(struct icmphdr));
    icmphdr->type = type;
    icmphdr->code = code;
    icmphdr->un.echo.id = id;
    icmphdr->un.echo.sequence = seq;
    icmphdr->checksum = icmp_checksum((unsigned short *)icmphdr, packet_size);
}

void send_one_loopback_icmp_packet(struct loopback_info *info) {
    int n;
    char server_reply[1600];
    struct in_addr insaddr;
    struct icmphdr *icmphdr, *recv_icmphdr;
    struct iphdr *recv_iphdr;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(info->target_ip);

    icmphdr = (struct icmphdr *)&info->message;

    info->pkt_sent++;
    setup_icmphdr(ICMP_ECHO, 0, 0, info->pkt_sent, icmphdr, info->pkt_size);

    n = sendto(info->sock, (char *)info->message, info->pkt_size, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n < 0) {
        indigo_logger(LOG_LEVEL_WARNING, "Send failed on icmp packet %d", info->pkt_sent);
        goto done;
    }
    indigo_logger(LOG_LEVEL_INFO, "Packet %d: Send icmp %d bytes data to ip %s",
                  info->pkt_sent, n, info->target_ip);

    memset(&server_reply, 0, sizeof(server_reply));
    n = recv(info->sock, server_reply, sizeof(server_reply), 0);
    if (n < 0) {
        indigo_logger(LOG_LEVEL_WARNING, "recv failed on icmp packet %d", info->pkt_sent);
        goto done;
    } else {
        recv_iphdr = (struct iphdr *)server_reply;
        recv_icmphdr = (struct icmphdr *)(server_reply + (recv_iphdr->ihl << 2));
        insaddr.s_addr = recv_iphdr->saddr;

        if (!strcmp(info->target_ip, inet_ntoa(insaddr)) && recv_icmphdr->type == ICMP_ECHOREPLY) {
            indigo_logger(LOG_LEVEL_INFO, "icmp echo reply from %s, Receive echo %d bytes data", info->target_ip, n - 20);
            info->pkt_rcv++;
        } else {
            indigo_logger(LOG_LEVEL_INFO, "Received packet is not the ICMP reply from the DUT");
        }
    }

done:
    controlapp_eloop_register_timeout(0, info->rate * 1000000, send_continuous_loopback_packet, info, NULL);
}

void send_one_loopback_udp_packet(struct loopback_info *info) {
    char server_reply[1600];
    ssize_t recv_len = 0, send_len = 0;

    memset(&server_reply, 0, sizeof(server_reply));

    info->pkt_sent++;
    send_len = send(info->sock, info->message, strlen(info->message), 0);
    if (send_len < 0) {
        indigo_logger(LOG_LEVEL_INFO, "Send failed on packet %d", info->pkt_sent);
        // In case Tool doesn't send stop or doesn't receive stop
        if (info->pkt_sent < 1000)
            controlapp_eloop_register_timeout(0, info->rate*1000000, send_continuous_loopback_packet, info, NULL);
        return;
    }
    indigo_logger(LOG_LEVEL_INFO, "Packet %d: Send loopback %d bytes data",
            info->pkt_sent, send_len);

    recv_len = recv(info->sock, server_reply, sizeof(server_reply), 0);
    if (recv_len < 0) {
        indigo_logger(LOG_LEVEL_INFO, "recv failed on packet %d", info->pkt_sent);
        // In case Tool doesn't send stop or doesn't receive stop
        if (info->pkt_sent < 1000)
            controlapp_eloop_register_timeout(0, info->rate*1000000, send_continuous_loopback_packet, info, NULL);
        return;
    }
    info->pkt_rcv++;
    indigo_logger(LOG_LEVEL_INFO, "Receive echo %d bytes data", recv_len);

    controlapp_eloop_register_timeout(0, info->rate*1000000, send_continuous_loopback_packet, info, NULL);
}

void send_continuous_loopback_packet(void *eloop_ctx, void *sock_ctx) {
    struct loopback_info *info = (struct loopback_info *)eloop_ctx;

    if (info->pkt_type == DATA_TYPE_ICMP) {
        send_one_loopback_icmp_packet(info);
    } else {
        send_one_loopback_udp_packet(info);
    }
}

/* Stop to send continuous loopback data */
int stop_loopback_data(int *pkt_sent)
{
    if (loopback.sock < 0)
        return 0;

    controlapp_eloop_cancel_timeout(send_continuous_loopback_packet, &loopback, NULL);
    close(loopback.sock);
    loopback.sock = -1;
    if (pkt_sent)
        *pkt_sent = loopback.pkt_sent;

    return loopback.pkt_rcv;
}

int send_udp_data(char *target_ip, int target_port, int packet_count, int packet_size, double rate) {
    int s = 0, i = 0;
    struct sockaddr_in addr;
    int pkt_sent = 0, pkt_rcv = 0;
    char message[1600], server_reply[1600], ifname[32];
    ssize_t recv_len = 0, send_len = 0;
    struct timeval timeout;

    /* Open UDP socket */
    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open socket");
        return -1;
    }

    if (rate < 1) {
        timeout.tv_sec = 0;
        timeout.tv_usec = rate * 1000000;
    } else {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
    }
    snprintf(ifname, sizeof(ifname), "%s", get_wireless_interface());
    if (net_bind_socket_to_iface(s, ifname) < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "failed to bind the interface %s", ifname);
        close(s);
        return -1;
    }

    setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (target_ip) {
        addr.sin_addr.s_addr = inet_addr(target_ip);
    }
    addr.sin_port = htons(target_port);

    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Connect failed. Error");
        close(s);
        return -1;
    }

    indigo_logger(LOG_LEVEL_INFO, "packet_count %d rate %lf\n",
                  packet_count, rate);

    /* Continuous data case: reply OK and use eloop timeout to send data */
    if (packet_count == -1) {
        loopback.sock = s;
        loopback.pkt_type = DATA_TYPE_UDP;
        loopback.rate = rate;
        loopback.pkt_sent = loopback.pkt_rcv = 0;
        memset(loopback.message, 0, sizeof(loopback.message));
        for (i = 0; (i < packet_size) && (i < sizeof(loopback.message)); i++)
            loopback.message[i] = 0x0A;
        controlapp_eloop_register_timeout(0, 0, send_continuous_loopback_packet, &loopback, NULL);
        indigo_logger(LOG_LEVEL_INFO, "Send continuous loopback data to ip %s port %u",
                      target_ip, target_port);
        return 0;
    }

    memset(message, 0, sizeof(message));
    for (i = 0; (i < packet_size) && (i < sizeof(message)); i++)
        message[i] = 0x0A;

    for (pkt_sent = 1; pkt_sent <= packet_count; pkt_sent++) {
        memset(&server_reply, 0, sizeof(server_reply));

        send_len = send(s, message, strlen(message), 0);
        if (send_len < 0) {
            indigo_logger(LOG_LEVEL_INFO, "Send failed on packet %d", pkt_sent);
#ifdef CONTROLAPPC_LINUX_API
            usleep(rate*1000000);
#else
			rtos_delay_milliseconds(rate*1000);
#endif
            continue;
        }
        indigo_logger(LOG_LEVEL_INFO, "Packet %d: Send loopback %d bytes data to ip %s port %u",
                      pkt_sent, send_len, target_ip, target_port);

        recv_len = recv(s, server_reply, sizeof(server_reply), 0);
        if (recv_len < 0) {
            indigo_logger(LOG_LEVEL_INFO, "recv failed on packet %d", pkt_sent);
            if (rate > 1)
#ifdef CONTROLAPPC_LINUX_API
                usleep((rate-1)*1000000);
#else
				rtos_delay_milliseconds((rate-1)*1000);
#endif
            continue;
        }
        pkt_rcv++;
#ifdef CONTROLAPPC_LINUX_API
        usleep(rate*1000000);
#else
		rtos_delay_milliseconds(rate*1000);
#endif

        indigo_logger(LOG_LEVEL_INFO, "Receive echo %d bytes data", recv_len);
    }
    close(s);

    return pkt_rcv;
}

int send_icmp_data(char *target_ip, int packet_count, int packet_size, double rate)
{
    int n, sock, i;
    char buf[1600], server_reply[1600], ifname[32];
    struct sockaddr_in addr;
    struct in_addr insaddr;
    struct icmphdr *icmphdr, *recv_icmphdr;
    struct iphdr *recv_iphdr;
    struct timeval timeout;
    int pkt_sent = 0, pkt_rcv = 0;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0) {
        return -1;
	}

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(target_ip);

    if (rate < 1) {
        timeout.tv_sec = 0;
        timeout.tv_usec = rate * 1000000;
    } else {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
    }

    snprintf(ifname, sizeof(ifname), "%s", get_wireless_interface());
    if (net_bind_socket_to_iface(sock, ifname) < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "failed to bind the interface %s", ifname);
        close(sock);
        return -1;
    }

    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    /* Continuous data case: reply OK and use eloop timeout to send data */
    if (packet_count == -1) {
        memset(&loopback, 0, sizeof(loopback));
        loopback.sock = sock;
        loopback.pkt_type = DATA_TYPE_ICMP;
        loopback.rate = rate;
        loopback.pkt_size = packet_size;
        snprintf(loopback.target_ip, sizeof(loopback.target_ip), "%s", target_ip);
        for (i = sizeof(struct icmphdr); (i < packet_size) && (i < sizeof(loopback.message)); i++)
            loopback.message[i] = 0x0A;
        controlapp_eloop_register_timeout(0, 0, send_continuous_loopback_packet, &loopback, NULL);
        indigo_logger(LOG_LEVEL_INFO, "Send continuous loopback data to ip %s", loopback.target_ip);
        return 0;
    }

    icmphdr = (struct icmphdr *)&buf;
    memset(&buf, 0, sizeof(buf));
    for (i = sizeof(struct icmphdr); (i < packet_size) && (i < sizeof(buf)); i++)
        buf[i] = 0x0A;

    for (pkt_sent = 1; pkt_sent <= packet_count; pkt_sent++) {
        memset(&server_reply, 0, sizeof(server_reply));
        setup_icmphdr(ICMP_ECHO, 0, 0, pkt_sent, icmphdr, packet_size);

        n = sendto(sock, (char *)buf, packet_size, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0) {
            indigo_logger(LOG_LEVEL_WARNING, "Send failed on icmp packet %d", pkt_sent);
#ifdef CONTROLAPPC_LINUX_API
            usleep(rate * 1000000);
#else
			rtos_delay_milliseconds(rate*1000);
#endif
            continue;
        }
        indigo_logger(LOG_LEVEL_INFO, "Packet %d: Send icmp %d bytes data to ip %s",
                      pkt_sent, n, target_ip);

        n = recv(sock, server_reply, sizeof(server_reply), 0);
        if (n < 0) {
            indigo_logger(LOG_LEVEL_WARNING, "recv failed on icmp packet %d", pkt_sent);
            if (rate > 1)
#ifdef CONTROLAPPC_LINUX_API
				usleep((rate-1)*1000000);
#else
				rtos_delay_milliseconds((rate-1)*1000);
#endif
            continue;
        } else {
            recv_iphdr = (struct iphdr *)server_reply;
            recv_icmphdr = (struct icmphdr *)(server_reply + (recv_iphdr->ihl << 2));
            insaddr.s_addr = recv_iphdr->saddr;

            if (!strcmp(target_ip, inet_ntoa(insaddr)) && recv_icmphdr->type == ICMP_ECHOREPLY) {
                /* IP header 20 bytes */
                indigo_logger(LOG_LEVEL_INFO, "icmp echo reply from %s, Receive echo %d bytes data", target_ip, n - 20);
                pkt_rcv++;
            } else {
                indigo_logger(LOG_LEVEL_INFO, "Received packet is not the ICMP reply from the DUT");
            }
        }
#ifdef CONTROLAPPC_LINUX_API
		usleep(rate * 1000000);
#else
		rtos_delay_milliseconds(rate*1000);
#endif
    }

    close(sock);
    return pkt_rcv;
}

#ifdef _TEST_PLATFORM_
int send_broadcast_arp(char *target_ip, int *send_count, int rate) {
    char buffer[S_BUFFER_LEN];
    FILE *fp;
    int recv = 0;

#ifdef _OPENWRT_
    snprintf(buffer, sizeof(buffer), "arping -I %s %s -c %d -b | grep broadcast", get_wireless_interface(), target_ip, *send_count);
#else
    snprintf(buffer, sizeof(buffer), "arping -i %s %s -c %d -W %d | grep packet", get_wireless_interface(), target_ip, *send_count, rate);
#endif
    fp = popen(buffer, "r");
    if (fp == NULL)
        return 0;
#ifdef _OPENWRT_
    //Format: Sent 3 probe(s) (3 broadcast(s))
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%*s %d", send_count);
    //Format: Received 0 reply (0 request(s), 0 broadcast(s))
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%*s %d", &recv);
#else
    //arping output format: 1 packets transmitted, 1 packets received,   0% unanswered (0 extra)
    fscanf(fp, "%d %*s %*s %d", send_count, &recv);
#endif
    indigo_logger(LOG_LEVEL_INFO, "ARP TEST - send: %d recv: %d", *send_count, recv );
    pclose(fp);

    return recv;
}
#endif

//ToDo
int find_interface_ip(char *ipaddr, int ipaddr_len, char *name) {
#if 0
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr = NULL;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, name) == 0) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            if (ipaddr) {
                strcpy(ipaddr, addr);
            }
            return 1;
        }
    }
    freeifaddrs(ifap);
#endif
    return 0;
}

//ToDo
int get_mac_address(char *buffer, int size, char *interface) {
#if 0
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (fd <= 0) {
        goto done;
    }
    strcpy(s.ifr_name, interface);
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
            (char)s.ifr_addr.sa_data[0]&0x00ff, (char)s.ifr_addr.sa_data[1]&0x00ff, (char)s.ifr_addr.sa_data[2]&0x00ff,
            (char)s.ifr_addr.sa_data[3]&0x00ff, (char)s.ifr_addr.sa_data[4]&0x00ff, (char)s.ifr_addr.sa_data[5]&0x00ff);
        close(fd);
        return 0;
    }
    close(fd);
done:
#endif

    return 1;
}

int set_mac_address(char *ifname, char *mac) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];
    /* If the system doesn't support ip command, please use ifconfig. E.g., */
    /* sprintf(cmd, "ifconfig %s hw ether %s", ifname, mac_addr) */
    sprintf(cmd, "ip link set dev %s address %s", ifname, mac);
    return system(cmd);
#else
	return 0;
#endif
}

int bridge_created = 0;

int is_bridge_created() {
    return bridge_created;
}

void bridge_init(char *br) {
#ifdef CONTROLAPPC_LINUX_API
    /* Create bridge for multiple VAPs */
    if (configured_interface_count >= 2) {
        create_bridge(br);
        add_all_wireless_interface_to_bridge(br);
    }
#endif
}

int create_bridge(char *br) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];

    /* Create new bridge */
    sprintf(cmd, "brctl addbr %s", br);
    system(cmd);

    /* Bring up bridge */
    control_interface(br, "up");
#endif

    bridge_created = 1;

    return 0;
}

int add_interface_to_bridge(char *br, char *ifname) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];

    /* Reset IP address */
    reset_interface_ip(ifname);

    /* Add interface to bridge */
    sprintf(cmd, "brctl addif %s %s", br, ifname);
    system(cmd);
    printf("%s\n", cmd);
#endif
    return 0;
}

int reset_bridge(char *br) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];

    /* Bring down bridge */
    control_interface(br, "down");
    sprintf(cmd, "brctl delbr %s", br);
    system(cmd);
#endif
    bridge_created = 0;

    return 0;
}

int add_wireless_interface(char *ifname) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];

    sprintf(cmd, "iw dev %s interface add %s type managed", get_wireless_interface(), ifname);
    system(cmd);
#endif
    return 0;
}

int delete_wireless_interface(char *ifname) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];

    sprintf(cmd, "iw dev %s del", ifname);
    system(cmd);
#endif
    return 0;
}

int control_interface(char *ifname, char *op) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];
    /* If the system doesn't support ip command, please use ifconfig. E.g., */
    /* sprintf(cmd, "ifconfig %s %s", ifname, op); */
    sprintf(cmd, "ip link set %s %s", ifname, op);
    system(cmd);
#endif
    return 0;
}

int set_interface_ip(char *ifname, char *ip) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];
    /* If the system doesn't support ip command, please use ifconfig. */
    /* Please also update the caller to use netmask instead of CIDR. E.g., */
    /* sprintf(cmd, "ifconfig %s %s", ifname, ip); */
    sprintf(cmd, "ip addr add %s dev %s", ip, ifname);
    system(cmd);
#endif

    return 0;
}

//ToDo
int reset_interface_ip(char *ifname) {
#ifdef CONTROLAPPC_LINUX_API
    char cmd[S_BUFFER_LEN];
    /* If the system doesn't support ip command, please use ifconfig. E.g., */
    /* sprintf(cmd, "ifconfig %s 0.0.0.0", ifname); */
    sprintf(cmd, "ip addr flush dev %s", ifname);
    return system(cmd);
#endif
	return 0;
}

#ifdef _TEST_PLATFORM_
void detect_del_arp_entry(char *ip) {
    char buffer[S_BUFFER_LEN];
    char res_ip[32], res_dev[16], res_inf[16];
    FILE *fp;

    snprintf(buffer, sizeof(buffer), "ip neigh show %s", ip);
    fp = popen(buffer, "r");
    if (fp == NULL)
        return;

    if (NULL == fgets(buffer, sizeof(buffer), fp)) {
    } else if (3 == sscanf(buffer, "%31s %15s %15s", res_ip, res_dev, res_inf)) {
        if (!strcmp(res_ip, ip) && !strcmp(res_dev, "dev")) {
            indigo_logger(LOG_LEVEL_INFO, "Delete existing ARP entry: %s", ip);
            snprintf(buffer, sizeof(buffer), "ip neigh del %s %s %s", res_ip, res_dev, res_inf);
            system(buffer);
        } else {
            indigo_logger(LOG_LEVEL_INFO, "Format mismatch?: %s %s %s\n", res_ip, res_dev, res_inf);
        }
    }
    pclose(fp);

    return;
}
#endif

int add_all_wireless_interface_to_bridge(char *br) {
    int i;

    for (i = 0; i < interface_count; i++) {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER) {
            control_interface(interfaces[i].ifname, "up");
            add_interface_to_bridge(br, interfaces[i].ifname);
        }
    }

    return 0;
}

/* Environment */
int service_port = SERVICE_PORT_DEFAULT;

char hapd_exec_file[64];
char hapd_full_exec_path[64] = HAPD_EXEC_FILE_DEFAULT;
char hapd_ctrl_path[64] = HAPD_CTRL_PATH_DEFAULT;
char hapd_full_ctrl_path[128];
char hapd_global_ctrl_path[64] = HAPD_GLOBAL_CTRL_PATH_DEFAULT;
char hapd_conf_file[64] = HAPD_CONF_FILE_DEFAULT;
int hostapd_debug_level = DEBUG_LEVEL_DISABLE;

char wpas_exec_file[64];
char wpas_full_exec_path[64] = WPAS_EXEC_FILE_DEFAULT;
char wpas_ctrl_path[64] = WPAS_CTRL_PATH_DEFAULT;
char wpas_full_ctrl_path[128];
char wpas_global_ctrl_path[64] = WPAS_GLOBAL_CTRL_PATH_DEFAULT;
char wpas_conf_file[64] = WPAS_CONF_FILE_DEFAULT;
int wpas_debug_level = DEBUG_LEVEL_DISABLE;

struct interface_info* assign_wireless_interface_info(struct bss_identifier_info *bss) {
    int i;

    for (i = 0; i < interface_count; i++) {
        if ((interfaces[i].band == bss->band) &&
             (interfaces[i].identifier == UNUSED_IDENTIFIER)) {
            configured_interface_count++;
            interfaces[i].identifier = bss->identifier;
            interfaces[i].mbssid_enable = bss->mbssid_enable;
            interfaces[i].transmitter = bss->transmitter;
            if (bss->mbssid_enable) {
                interfaces[i].hapd_bss_id = band_mbssid_cnt[bss->band];
                band_mbssid_cnt[bss->band]++;
            }
            memset(interfaces[i].hapd_conf_file, 0, sizeof(interfaces[i].hapd_conf_file));
            return &interfaces[i];
        }
            snprintf(interfaces[i].hapd_conf_file, sizeof(interfaces[i].hapd_conf_file),
                     "%s/hostapd_%s.conf", HAPD_CONF_FILE_DEFAULT_PATH, interfaces[i].ifname);
    }

    return NULL;
}

struct interface_info* get_wireless_interface_info(int band, int identifier) {
    int i;

    for (i = 0; i < interface_count; i++) {
        if ((interfaces[i].band == band) &&
             ((interfaces[i].identifier != UNUSED_IDENTIFIER) &&
              (interfaces[i].identifier == identifier))) {
            return &interfaces[i];
        }
    }

    return NULL;
}

int get_debug_level(int value) {
    if (value == 0) {
        return DEBUG_LEVEL_DISABLE;
    } else if (value == 1) {
        return DEBUG_LEVEL_BASIC;
    }
    return DEBUG_LEVEL_ADVANCED;
}

/* get hostapd's file name */
char* get_hapd_exec_file() {
    return hapd_exec_file;
}

/* parse hostapd full path and set hostapd's file name */
int set_hapd_exec_file(char* path) {
    char *ptr = indigo_strrstr(path, "/");

    if (ptr) {
        strcpy(hapd_exec_file, ptr+1);
    } else {
        strcpy(hapd_exec_file, path);
    }
    return 0;
}

/* get hostapd's full path */
char* get_hapd_full_exec_path() {
    return hapd_full_exec_path;
}

/* set hostapd's full path */
int set_hapd_full_exec_path(char* path) {
    memset(hapd_full_exec_path, 0, sizeof(hapd_full_exec_path));
    snprintf(hapd_full_exec_path, sizeof(hapd_full_exec_path), "%s", path);

    set_hapd_exec_file(hapd_full_exec_path);
    return 0;
}

char* get_hapd_ctrl_path_by_id(struct interface_info* wlan) {
    memset(hapd_full_ctrl_path, 0, sizeof(hapd_full_ctrl_path));
    if (wlan) {
        sprintf(hapd_full_ctrl_path, "%s/%s", hapd_ctrl_path, wlan->ifname);
    }
    else {
        sprintf(hapd_full_ctrl_path, "%s/%s", hapd_ctrl_path, get_default_wireless_interface_info());
    }
    printf("hapd_full_ctrl_path: %s, wlan %p\n", hapd_full_ctrl_path, wlan);
    return hapd_full_ctrl_path;
}

char* get_hapd_ctrl_path() {
    memset(hapd_full_ctrl_path, 0, sizeof(hapd_full_ctrl_path));
    sprintf(hapd_full_ctrl_path, "%s/%s", hapd_ctrl_path, get_default_wireless_interface_info());
    return hapd_full_ctrl_path;
}

int set_hapd_ctrl_path(char* path) {
    memset(hapd_ctrl_path, 0, sizeof(hapd_ctrl_path));
    snprintf(hapd_ctrl_path, sizeof(hapd_ctrl_path), "%s", path);
    return 0;
}

char* get_hapd_global_ctrl_path() {
    return hapd_global_ctrl_path;
}

int set_hapd_global_ctrl_path(char* path) {
    memset(hapd_global_ctrl_path, 0, sizeof(hapd_global_ctrl_path));
    snprintf(hapd_global_ctrl_path, sizeof(hapd_global_ctrl_path), "%s", path);
    return 0;
}

char* get_hapd_conf_file() {
    return hapd_conf_file;
}

int set_hapd_conf_file(char* path) {
    memset(hapd_conf_file, 0, sizeof(hapd_conf_file));
    snprintf(hapd_conf_file, sizeof(hapd_conf_file), "%s", path);
    return 0;
}

void set_hostapd_debug_level(int level) {
    hostapd_debug_level = level;
}

char* get_hostapd_debug_arguments() {
    if (hostapd_debug_level == DEBUG_LEVEL_ADVANCED) {
        return "-dddK";
    } else if (hostapd_debug_level == DEBUG_LEVEL_BASIC) {
        return "-dK";
    }
    return "";
}

char* get_wpas_exec_file() {
    return wpas_exec_file;
}

int set_wpas_exec_file(char* path) {
    char *ptr = indigo_strrstr(path, "/");
    if (ptr) {
        strcpy(wpas_exec_file, ptr+1);
    } else {
        strcpy(wpas_exec_file, path);
    }
    return 0;
}

char* get_wpas_full_exec_path() {
    return wpas_full_exec_path;
}

int set_wpas_full_exec_path(char* path) {
    memset(wpas_full_exec_path, 0, sizeof(wpas_full_exec_path));
    snprintf(wpas_full_exec_path, sizeof(wpas_full_exec_path), "%s", path);

    set_wpas_exec_file(wpas_full_exec_path);
    return 0;
}

char* get_wpas_ctrl_path() {
    memset(wpas_full_ctrl_path, 0, sizeof(wpas_full_ctrl_path));
    sprintf(wpas_full_ctrl_path, "%s/%s", wpas_ctrl_path, get_default_wireless_interface_info());
    return wpas_full_ctrl_path;
}

int set_wpas_ctrl_path(char* path) {
    snprintf(wpas_ctrl_path, sizeof(wpas_ctrl_path), "%s", path);
    return 0;
}

char* get_wpas_global_ctrl_path() {
    return wpas_global_ctrl_path;
}

int set_wpas_global_ctrl_path(char* path) {
    snprintf(wpas_global_ctrl_path, sizeof(wpas_global_ctrl_path), "%s", path);
    return 0;
}

char* get_wpas_conf_file() {
    return wpas_conf_file;
}

int set_wpas_conf_file(char* path) {
    memset(wpas_conf_file, 0, sizeof(wpas_conf_file));
    snprintf(wpas_conf_file, sizeof(wpas_conf_file), "%s", path);
    return 0;
}

void set_wpas_debug_level(int level) {
    wpas_debug_level = level;
}

char* get_wpas_debug_arguments() {
    if (wpas_debug_level == DEBUG_LEVEL_ADVANCED) {
        return "-dddK";
    } else if (wpas_debug_level == DEBUG_LEVEL_BASIC) {
        return "-dK";
    }
    return "";
}

int add_wireless_interface_info(int band, int bssid, char *name) {
    interfaces[interface_count].band = band;
    interfaces[interface_count].bssid = -1;
    interfaces[interface_count].identifier = UNUSED_IDENTIFIER;
    strcpy(interfaces[interface_count++].ifname, name);
    return 0;
}

int show_wireless_interface_info() {
    int i;
    char *band = BAND_24GHZ;
    indigo_logger(LOG_LEVEL_INFO, "interface_count=%d", interface_count);

    for (i = 0; i < interface_count; i++) {
        if (interfaces[i].band == BAND_24GHZ) {
            band = "2.4GHz";
        } else if (interfaces[i].band == BAND_5GHZ) {
            band = "5GHz";
        } else if (interfaces[i].band == BAND_6GHZ) {
            band = "6GHz";
        }

        indigo_logger(LOG_LEVEL_INFO, "Interface Name: %s, Band: %s, identifier %d",
            interfaces[i].ifname, band, interfaces[i].identifier);
    }
    return 0;
}

int parse_wireless_interface_info(char *info) {
    char *token = NULL;
    char *delimit = ",";

    token = strtok(info, delimit);

    while(token != NULL) {
        if (strncmp(token, "2:", 2) == 0) {
            add_wireless_interface_info(BAND_24GHZ, -1, token+2);
        } else if (strncmp(token, "5:", 2) == 0) {
            add_wireless_interface_info(BAND_5GHZ, -1, token+2);
        } else if (strncmp(token, "6:", 2) == 0) {
            add_wireless_interface_info(BAND_6GHZ, -1, token+2);
        } else {
            return -1;
        }
        token = strtok(NULL, delimit);
    }

    return 0;
}

char* get_default_wireless_interface_info() {
    int i;
    for (i = 0; i < interface_count; i++) {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER) {
            return interfaces[i].ifname;
        }
    }
    if (default_interface) {
        return default_interface->ifname;
    }
    else
        return interfaces[0].ifname;
}

void set_default_wireless_interface_info(int band) {
    int i;

    for (i = 0; i < interface_count; i++) {
        if (interfaces[i].band == band) {
            default_interface = &interfaces[i];
            indigo_logger(LOG_LEVEL_DEBUG, "Set default_interface %s", default_interface->ifname);
            break;
        }
    }
}

void reset_default_wireless_interface_info() {
    default_interface = NULL;
}

/* Parse BSS IDENTIFIER TLV */
void parse_bss_identifier(int bss_identifier, struct bss_identifier_info* bss) {
    bss->band = bss_identifier & 0x0F;
    bss->identifier = (bss_identifier & 0xF0) >> 4;
    bss->mbssid_enable = (bss_identifier & 0x100) >> 8;
    bss->transmitter = (bss_identifier & 0x200) >> 9;
    return;
}

int clear_interfaces_resource() {
    int i, ret = 0;
    for (i = 0; i < interface_count; i++)
    {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER) {
            interfaces[i].identifier = UNUSED_IDENTIFIER;
        }
    }
    configured_interface_count = 0;
    memset(band_mbssid_cnt, 0, sizeof(band_mbssid_cnt));

    return ret;
}

void iterate_all_wlan_interfaces(void (*callback_fn)(void *)) {
    int i;
    for (i = 0; i < interface_count; i++)
    {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER) {
            callback_fn((void *)&interfaces[i]);
        }
    }

    return ;
}

/* This API is useful only when for provisioning multiple VAPs */
int is_band_enabled(int band) {
    int i;
    for (i = 0; i < interface_count; i++)
    {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER &&
                interfaces[i].band == band) {
            return 1;
        }
    }
    return 0;
}


char* get_all_hapd_conf_files(int *swap_hapd) {
    int i, valid_id_cnt = 0;
    static char conf_files[128];

    memset(conf_files, 0, sizeof(conf_files));
    for (i = 0; i < interface_count; i++) {
        if (interfaces[i].identifier != UNUSED_IDENTIFIER) {
#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
            if (use_openwrt_wpad && !interfaces[i].mbssid_enable) {
                *swap_hapd = 1;
                continue;
            } else if (!use_openwrt_wpad && interfaces[i].mbssid_enable) {
                continue;
            }
#endif
            valid_id_cnt++;
            strncat(conf_files, interfaces[i].hapd_conf_file, strlen(interfaces[i].hapd_conf_file));
            strcat(conf_files, " ");
        }
    }
    if (valid_id_cnt)
        return conf_files;
    else
        return hapd_conf_file;
}

char* get_wireless_interface() {
    return get_default_wireless_interface_info();
}

int set_wireless_interface(char *name) {
    memset(interfaces, 0, sizeof(interfaces));
    interface_count = 0;

    if (strstr(name, ":") || strstr(name, ",")) {
        return parse_wireless_interface_info(name);
    } else {
#ifdef _LAPTOP_
        add_wireless_interface_info(BAND_24GHZ, -1, name);
        add_wireless_interface_info(BAND_5GHZ, -1, name);
#else
        return -1;
#endif
    }
    return 0;
}

int get_service_port() {
    return service_port;
}

int set_service_port(int port) {
    service_port = port;
    return 0;
}

/* Channel functions */
struct channel_info band_24[] = { {1, 2412}, {2, 2417}, {3, 2422}, {4, 2427}, {5, 2432}, {6, 2437}, {7, 2442}, {8, 2447}, {9, 2452}, {10, 2457}, {11, 2462} };
struct channel_info band_5[] = { {36, 5180}, {40, 5200}, {44, 5220}, {48, 5240}, {52, 5260}, {56, 5280}, {60, 5300}, {64, 5320}, {100, 5500}, {104, 5520}, {108, 5540},
                                 {112, 5560}, {116, 5580}, {120, 5600}, {124, 5620}, {128, 5640}, {132, 5660}, {136, 5680}, {140, 5700}, {144, 5720}, {149, 5745},
                                 {153, 5765}, {157, 5785}, {161, 5805}, {165, 8525} };

int verify_band_from_freq(int freq, int band) {
    struct channel_info *info = NULL;
    int i, size = 0;

    if (band == BAND_24GHZ) {
        info = band_24;
        size = sizeof(band_24)/sizeof(struct channel_info);
    } else if (band == BAND_5GHZ) {
        info = band_5;
        size = sizeof(band_5)/sizeof(struct channel_info);
    } else if (band == BAND_6GHZ) {
        if (freq >= (5950 + 5*1) && freq <= (5950 + 5*233))
            return 0;
        else
            return -1;
    }

    for (i = 0; i < size; i++) {
        if (freq == info[i].freq) {
            return 0;
        }
    }

    return -1;
}

int get_center_freq_index(int channel, int width) {
    if (width == 1) {
        if (channel >= 36 && channel <= 48) {
            return 42;
        } else if (channel <= 64) {
            return 58;
        } else if (channel >= 100 && channel <= 112) {
            return 106;
        } else if (channel <= 128) {
            return 122;
        } else if (channel <= 144) {
            return 138;
        } else if (channel >= 149 && channel <= 161) {
            return 155;
        }
    } else if (width == 2) {
        if (channel >= 36 && channel <= 64) {
            return 50;
        } else if (channel >= 36 && channel <= 64) {
            return 114;
        }
    }
    return 0;
}

int get_6g_center_freq_index(int channel, int width) {
    int chwidth, i;

    if (width == 1) {
        chwidth = 80;
    } else if (width == 2) {
        chwidth = 160;
    } else {
        return channel;
    }

    for (i=1; i<233; i+=chwidth/5) {
        if (channel >= i && channel < i + chwidth/5)
            return i + (chwidth - 20)/10;
    }

	// error
	return 0;
}

int is_ht40plus_chan(int chan) {
    if (chan == 36 || chan == 44 || chan == 52 || chan == 60 ||
        chan == 100 || chan == 108 || chan == 116 || chan == 124 ||
        chan == 132 || chan == 140 || chan == 149 || chan == 157)
        return 1;
    else
        return 0;
}

int is_ht40minus_chan(int chan) {
    if (chan == 40 || chan == 48 || chan == 56 || chan == 64 ||
        chan == 104 || chan == 112 || chan == 120 || chan == 128 ||
        chan == 136 || chan == 144 || chan == 153 || chan == 161)
        return 1;
    else
        return 0;
}

/* String operation */
#ifdef CONTROLAPPC_LINUX_API
size_t strlcpy(char *dest, const char *src, size_t siz) {
	const char *s = src;
	size_t left = siz;

	if (left) {
		/* Copy string up to the maximum size of the dest buffer */
		while (--left != 0) {
			if ((*dest++ = *s++) == '\0')
				break;
		}
	}

	if (left == 0) {
		/* Not enough room for the string; force NUL-termination */
		if (siz != 0)
			*dest = '\0';
		while (*s++)
			; /* determine total src string length */
	}

	return s - src - 1;
}
#endif /* CONTROLAPPC_LINUX_API */

int get_key_value(char *value, char *buffer, char *token) {
    char *ptr = NULL, *endptr = NULL;
    char _token[S_BUFFER_LEN];

    if (!value || !buffer || !token) {
        return -1;
    }

    memset(_token, 0, sizeof(_token));
    sprintf(_token, "\n%s=", token);
    ptr = strstr(buffer, _token);
    if (!ptr) {
        sprintf(_token, "%s=", token);
        if (strncmp(buffer, _token, strlen(_token)) == 0) {
            ptr = buffer;
        }
    }

    if (!ptr) {
        return -1;
    }

    ptr += strlen(_token);
    endptr = strstr(ptr, "\n");
    if (endptr) {
        strncpy(value, ptr, endptr - ptr);
    } else {
        strcpy(value, ptr);
    }

    return 0;
}

/*
 *       These were generated with: openssl x509 -outform der -in $pemname | openssl dgst -sha256
 *       "rsa_server1_w1_fi.pem": "a7407d995678712bb7adb4e7a75e89674aba363dea0b8308c63b006329b0de2d",
 *       "rsa_server1ALT_w1_fi.pem": "79a9d7273368bee41566f79ae9fc84119f7c963cf8cfac5984e2e0adaeafb112",
 *       "rsa_server2_w1_fi.pem": "8d0e00b924e30f4595ae7f5ef9f1346e2c3f343dfb1caf1429b3bb6b32a1bf03",
 *       "rsa_server4_w1_fi.pem": "2703264d2d06727be661752ff5b57e85f842dc74e18aaa03316e7b2d08db6260",
 */
void get_server_cert_hash(char *pem_file, char *buffer) {
#define NUM_ITEMS 4

    char file[NUM_ITEMS][32] = {
        "rsa_server1_w1_fi.pem",
        "rsa_server1ALT_w1_fi.pem",
        "rsa_server2_w1_fi.pem",
        "rsa_server4_w1_fi.pem"};
    char hash[NUM_ITEMS][128] = {"a7407d995678712bb7adb4e7a75e89674aba363dea0b8308c63b006329b0de2d",
                                 "79a9d7273368bee41566f79ae9fc84119f7c963cf8cfac5984e2e0adaeafb112",
                                 "8d0e00b924e30f4595ae7f5ef9f1346e2c3f343dfb1caf1429b3bb6b32a1bf03",
                                 "2703264d2d06727be661752ff5b57e85f842dc74e18aaa03316e7b2d08db6260"};
    int i = 0;

    for(i = 0; i< NUM_ITEMS; i++) {
        if (strcmp(file[i], pem_file) == 0) {
            sprintf(buffer, "hash://server/sha256/%s", hash[i]);
        }
    }
}

int insert_wpa_network_config(char *config) {
    FILE *f_ptr, *f_tmp_ptr;
    char *path = get_wpas_conf_file();
    char *tmp_path = "/tmp/wpa_supplicant_tmp1.conf";
    char *target_str = "}"; /* get the last line in network profile */
    char buffer[S_BUFFER_LEN];

    f_ptr = fopen(path, "r");
    f_tmp_ptr = fopen(tmp_path, "w");

    if (f_ptr == NULL || f_tmp_ptr == NULL) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open the files");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));
    while ((fgets(buffer, S_BUFFER_LEN, f_ptr)) != NULL) {
        if (strstr(buffer, target_str) != NULL) {
            indigo_logger(LOG_LEVEL_DEBUG,
                "insert config: %s into the wpa_supplicant conf.", config);
            fputs(config, f_tmp_ptr);
        }

        fputs(buffer, f_tmp_ptr);
    }

    fclose(f_ptr);
    fclose(f_tmp_ptr);

    /* replace original file with new file */
    remove(path);
    rename(tmp_path, path);
    return 0;
}

void remove_pac_file(char *path) {
    char pac_path[S_BUFFER_LEN];

    memset(pac_path, 0, sizeof(pac_path));
    if (!strlen(path)) {
        return;
    } else if (path[0] != '/') {
        snprintf(pac_path, sizeof(pac_path), "/%s", path);
    } else {
        snprintf(pac_path, sizeof(pac_path), "%s", path);
    }

    indigo_logger(LOG_LEVEL_INFO, "Remove PAC file: [%s]", pac_path);

    if(access(pac_path, F_OK) == 0) {
        unlink(pac_path);
    }
}

/* HTTP post request */
/* Internal. Generate random string for the boundary */
static void rand_string(char *str, int size) {
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0, key = 0;

    memset(str, 0, size);
    srand(time(NULL));
    for (i = 0; i < size; i++) {
        key = rand() % (int)(sizeof(charset)-1);
        str[i] = charset[key];
    }
}

/* Internal. Generate the boundary */
static void random_boundary(char *boundary, int size) {
    memset(boundary, '-', size);
    boundary[size-1] = '\0';
    rand_string(&boundary[size - (16+1)], 16);
}

/* Internal. Generate HTTP header for the multipart POST */
static char* http_header_multipart(char *path, char *host, int port, int content_length, char *boundary) {
    char *buffer = NULL;

    buffer = (char*)os_malloc(sizeof(char)*256);
    sprintf(buffer,
        "POST %s HTTP/1.0\r\n" \
        "Host: %s:%d\r\n" \
        "User-Agent: ControlAppC\r\n" \
        "Accept: */*\r\n" \
        "Content-Length: %d\r\n" \
        "Connection: close\r\n" \
        "Content-Type: multipart/form-data; boundary=%s\r\n\r\n",
        path,
        host,
        port,
        content_length,
        boundary
    );

    return buffer;
}

/* Internal. Generate HTTP body for uploaded file */
static char* http_body_multipart(char *boundary, char *param_name, char *file_name) {
    char *buffer = NULL, *file_content = NULL;
    int body_size = 0, file_size = 0;
    struct stat st;
    char *file_ptr = NULL;

    /* Get the file size and content */
    memset(&st, 0, sizeof(st));
    stat(file_name, &st);
    file_size = st.st_size;
    if (file_size == 0) {
        return buffer;
    }
    file_content = read_file(file_name);

    /* Fill the body buffer */
    body_size = file_size + 256;
    buffer = (char*)os_malloc(body_size*sizeof(char));
    memset(buffer, 0, body_size);

    file_ptr = indigo_strrstr(file_name, "/");
    if (file_ptr) {
        file_ptr += 1;
    } else {
        file_ptr = file_name;
    }
    sprintf(buffer,
        "--%s\r\n" \
        "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n" \
        "Content-Type: text/plain\r\n\r\n" \
        "%s\r\n\r\n" \
        "--%s--",
        boundary,
        param_name,
        file_ptr,
        file_content,
        boundary
    );
    os_free(file_content);
    return buffer;
}

/* Internal. Create HTTP socket */
static int http_socket(char *host, int port) {
    int socketfd = 0;
    struct sockaddr_in server_addr;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        close(socketfd);
        return -1;
    }

    return socketfd;
}

/*  Upload log by specifying the host, port, path, and the local file name */
int http_file_post(char *host, int port, char *path, char *file_name) {
    int socketfd = 0, numbytes = 0;
    char *header = NULL, *body = NULL;
    char boundary[64];
    char response[10240];

    /* Generate boundary, header and body */
    random_boundary(boundary, 41);
    /* Parameter name needs to match with CompletedFileUpload in API */
    if (!strcmp(path, HAPD_UPLOAD_API))
        body = http_body_multipart(boundary, "hostApdLogFile", file_name);
    else if (!strcmp(path, WPAS_UPLOAD_API))
        body = http_body_multipart(boundary, "wpasLogFile", file_name);
    else {
        indigo_logger(LOG_LEVEL_ERROR, "Tool doesn't support %s ?", path);
        goto done;
    }
    /* Return if body is NULL */
    if (body == NULL) {
        goto done;
    }

    header = http_header_multipart(path, host, port, strlen(body), boundary);

    socketfd = http_socket(host, port);
    if (send(socketfd, header, strlen(header), 0) == -1){
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open HTTP socket");
        goto done;
    }

    if (send(socketfd, body, strlen(body), 0) == -1){
        indigo_logger(LOG_LEVEL_ERROR, "Failed to upload file");
        goto done;
    }

    while ((numbytes=recv(socketfd, response, sizeof(response), 0)) > 0) {
        response[numbytes] = '\0';
        indigo_logger(LOG_LEVEL_DEBUG, "Server response: %s", response);
    }
    indigo_logger(LOG_LEVEL_INFO, "Upload completes");

done:
    if (header) {
        os_free(header);
    }
    if (body) {
        os_free(body);
    }
    if (socketfd) {
        close(socketfd);
    }

	return 0;
}
