#include "freertos_service.h"
#include "task.h"
#include "main.h"
#include "osdep_service.h"

#include <lwipconf.h>
#include <platform_stdlib.h>
#if defined(CONFIG_AS_INIC_AP)
#define BSD_STACK_SIZE		    1024
#else
#define BSD_STACK_SIZE		    512
#endif
#define DEFAULT_PORT            5001
#define DEFAULT_TIME            10
#define SERVER_BUF_SIZE         1500
#define CLIENT_BUF_SIZE         1460
#define KB                      1024
#define MB                      1048576//1024*1024
#define DEFAULT_UDP_BANDWIDTH   16777216 // 16MByes => 16*1024*1024 Bytes = 128 Mbits
#define DEFAULT_REPORT_INTERVAL 0xffffffff
#define DEFAULT_UDP_TOS_VALUE   1 // AC_BE
#define SUBSTREAM_FLAG 0x0100

//Max number of streaming
#define MULTI_STREAM_NUM 10

struct stream_id_t {
	uint8_t id_used;
	uint8_t  terminate;
};


struct iperf_data_t {
	uint64_t total_size;
	uint64_t bandwidth;
	xTaskHandle task;
	int  server_fd;
	int  client_fd;
	uint32_t buf_size;
	uint32_t time;
	uint32_t report_interval;
	uint16_t port;
	uint8_t  server_ip[16];
	uint8_t  role; // 'c' for client and 's' for server
	uint8_t  protocol; // 'u' for udp and 't' for tcp
	uint8_t  tos_value;
	int8_t   stream_id;
	//uint8_t  terminate;
	uint8_t  bidirection;
	uint16_t  is_sub_stream; //Byte[1] : this client stream is created by bidirection parameter, Byte[0] : main stream id
};

struct iperf_tcp_client_hdr {
	uint32_t flags;
	uint32_t numThreads;
	uint32_t mPort;
	uint32_t bufferlen;
	uint32_t mWinband;
	uint32_t mAmount;
};

struct iperf_udp_datagram {
	uint32_t id;
	uint32_t tv_sec;
	uint32_t tv_usec;
};

struct iperf_udp_client_hdr {
	uint32_t id;
	uint32_t tv_sec;
	uint32_t tv_usec;
	uint32_t flags;
	uint32_t numThreads;
	uint32_t mPort;
	uint32_t bufferlen;
	uint32_t mWinband;
	uint32_t mAmount;
};

struct iperf_udp_server_hdr {
	uint32_t flags;
	uint32_t total_len1;
	uint32_t total_len2;
	uint32_t stop_sec;
	uint32_t stop_usec;
	uint32_t error_cnt;
	uint32_t outorder_cnt;
	uint32_t datagrams;
	uint32_t jitter1;
	uint32_t jitter2;
};

uint8_t g_is_iperf_init = 0;
struct stream_id_t g_stream_id[MULTI_STREAM_NUM];

_mutex g_tptest_log_mutex = NULL;
_mutex g_tptest_mutex = NULL;

#define tptest_res_log(...)       do { \
                                    if(NULL != g_tptest_log_mutex){\
                                        rtw_mutex_get(&g_tptest_log_mutex); \
                                        printf(__VA_ARGS__); \
                                        rtw_mutex_put(&g_tptest_log_mutex); \
                                    }\
                                    else{\
                                        printf(__VA_ARGS__); \
                                    }\
                                }while(0)

static void iperf_test_handler(void *param);

void iperf_init(void)
{
	int i = 0;

	if (g_is_iperf_init == 0) {
		if (NULL == g_tptest_mutex) {
			rtw_mutex_init(&g_tptest_mutex);
		}
		if (NULL == g_tptest_log_mutex) {
			rtw_mutex_init(&g_tptest_log_mutex);
		}
		for (i = 0; i < MULTI_STREAM_NUM; i++) {
			g_stream_id[i].id_used = 0;
			g_stream_id[i].terminate = 0;
		}
		g_is_iperf_init = 1;
	}

	return;
}

void iperf_deinit(void)
{
	int i = 0;

	if (g_is_iperf_init == 1) {
		if (g_tptest_mutex) {
			rtw_mutex_free(&g_tptest_mutex);
			g_tptest_mutex = NULL;
		}
		if (g_tptest_log_mutex) {
			rtw_mutex_free(&g_tptest_log_mutex);
			g_tptest_log_mutex = NULL;
		}
		for (i = 0; i < MULTI_STREAM_NUM; i++) {
			g_stream_id[i].id_used = 0;
			g_stream_id[i].terminate = 0;
		}
		g_is_iperf_init = 0;
	}

	return;
}

// Note : Need to use mutex protect
// stream id range : 0 ~ MULTI_STREAM_NUM
struct iperf_data_t *init_stream_data(uint8_t protocol, uint8_t role)
{
	int8_t i = 0, stream_id = -1;
	struct iperf_data_t *stream_data = NULL;

	for (i = 0; i < MULTI_STREAM_NUM; i++) {
		if (g_stream_id[i].id_used == 0) {
			g_stream_id[i].id_used = 1;
			stream_id = i;
			break;
		}
	}

	if (stream_id == -1) {
		tptest_res_log("\n\r[ERROR] stream reach limit(%d)!\n", MULTI_STREAM_NUM);
		return NULL;
	}

	stream_data = pvPortMalloc(sizeof(struct iperf_data_t));
	if (!stream_data) {
		g_stream_id[stream_id].id_used = 0;
		tptest_res_log("\n\r[ERROR] stream_data allocate failed!\n");
		return NULL;
	}

	memset(stream_data, 0, sizeof(struct iperf_data_t));
	stream_data->stream_id = stream_id;
	stream_data->role = role;
	stream_data->protocol = protocol;
	stream_data->port = DEFAULT_PORT;
	stream_data->report_interval = DEFAULT_REPORT_INTERVAL;
	stream_data->time = DEFAULT_TIME;

	if (role == 'c') {
		stream_data->buf_size = CLIENT_BUF_SIZE;
	} else if (role == 's') {
		stream_data->buf_size = SERVER_BUF_SIZE;
	}

	if (protocol == 'u') {
		stream_data->bandwidth = DEFAULT_UDP_BANDWIDTH;
		stream_data->tos_value = DEFAULT_UDP_TOS_VALUE;
	}
	return stream_data;
}

// Note : Need to use mutex protect when calling the function
void free_stream_data(struct iperf_data_t *stream_data)
{
	if (stream_data) {
		g_stream_id[stream_data->stream_id].id_used = 0;
		g_stream_id[stream_data->stream_id].terminate = 0;
		vPortFree(stream_data);
		stream_data = NULL;
	}
	return;
}

uint64_t km_parser(char *buf, int len)
{
	uint64_t ret = 0;
	int keyword_num = 0;
	char num_str[17] = "\0";
	uint64_t num;

	if (len > 16) {
		return ret;
	}

	while ((buf[keyword_num] != '\0') && (keyword_num < len)) {
		if ((buf[keyword_num] == 'k') || (buf[keyword_num] == 'K')) {
			strncpy(num_str, buf, keyword_num);
			num = atol(num_str);
			ret = num * KB;
			break;
		} else if ((buf[keyword_num] == 'm') || (buf[keyword_num] == 'M')) {
			strncpy(num_str, buf, keyword_num);
			num = atol(num_str);
			ret = num * MB;
			break;
		}
		keyword_num++;
		if (keyword_num == len) {
			strncpy(num_str, buf, keyword_num);
			num = atol(num_str);
			ret = num;
			break;
		}
	}
	return ret;
}

int tcp_client_func(struct iperf_data_t iperf_data)
{
	struct sockaddr_in  ser_addr;
	uint32_t            i = 0;
	uint32_t            start_time, end_time, bandwidth_time, report_start_time;
	uint64_t            total_size = 0, bandwidth_size = 0, report_size = 0;
	struct iperf_tcp_client_hdr client_hdr;
	char *tcp_client_buffer = NULL;

	// for internal test
	iperf_data.bandwidth = 0;

	tcp_client_buffer = pvPortMalloc(iperf_data.buf_size);
	if (!tcp_client_buffer) {
		tptest_res_log("\n\r[ERROR] %s: Alloc buffer failed", __func__);
		goto exit2;
	}

	//filling the buffer
	for (i = 0; i < iperf_data.buf_size; i++) {
		tcp_client_buffer[i] = (char)(i % 10);
	}

	//create socket
	if ((iperf_data.client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Create TCP socket failed", __func__);
		goto exit2;
	}

	//initialize value in dest
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(iperf_data.port);
	ser_addr.sin_addr.s_addr = inet_addr((char const *)iperf_data.server_ip);

	tptest_res_log("\n\r%s: Server IP=%s, port=%d", __func__, iperf_data.server_ip, iperf_data.port);
	tptest_res_log("\n\r%s: Create socket fd = %d", __func__, iperf_data.client_fd);

	//Connecting to server
	if (connect(iperf_data.client_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Connect to server failed", __func__);
		goto exit1;
	}
	tptest_res_log("\n\r%s: Connect to server successfully", __func__);

	// For "iperf -d" command, send first packet with iperf client header
	if (iperf_data.bidirection) {
		client_hdr.flags = 0x01000080;
		client_hdr.numThreads = 0x01000000;
		client_hdr.mPort = htonl(iperf_data.port);
		client_hdr.bufferlen = 0;
		client_hdr.mWinband = 0;
		client_hdr.mAmount = htonl(~(iperf_data.time * 100) + 1);
		if (send(iperf_data.client_fd, (char *) &client_hdr, sizeof(client_hdr), 0) <= 0) {
			tptest_res_log("\n\r[ERROR] %s: TCP client send data error", __func__);
			goto exit1;
		}
	}

	if (iperf_data.total_size == 0) {
		start_time = xTaskGetTickCount();
		end_time = start_time;
		bandwidth_time = start_time;
		report_start_time = start_time;
		while (((end_time - start_time) <= (configTICK_RATE_HZ * iperf_data.time)) && (!g_stream_id[iperf_data.stream_id].terminate)) {
			if (send(iperf_data.client_fd, tcp_client_buffer, iperf_data.buf_size, 0) <= 0) {
				tptest_res_log("\n\r[ERROR] %s: TCP client send data error", __func__);
				goto exit1;
			}
			total_size += iperf_data.buf_size;
			bandwidth_size += iperf_data.buf_size;
			report_size += iperf_data.buf_size;
			end_time = xTaskGetTickCount();

			if ((iperf_data.bandwidth != 0) && (bandwidth_size >= iperf_data.bandwidth) && ((end_time - bandwidth_time) < (configTICK_RATE_HZ * 1))) {
				vTaskDelay(configTICK_RATE_HZ * 1 - (end_time - bandwidth_time));
				end_time = xTaskGetTickCount();
				bandwidth_time = end_time;
				bandwidth_size = 0;
			}

			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rtcp_c: id[%d] Send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				bandwidth_time = end_time;
				report_size = 0;
				bandwidth_size = 0;
			}
		}
	} else {
		start_time = xTaskGetTickCount();
		end_time = start_time;
		bandwidth_time = start_time;
		report_start_time = start_time;
		while ((total_size < iperf_data.total_size) && (!g_stream_id[iperf_data.stream_id].terminate)) {
			if (send(iperf_data.client_fd, tcp_client_buffer, iperf_data.buf_size, 0) <= 0) {
				tptest_res_log("\n\r[ERROR] %s: TCP client send data error", __func__);
				goto exit1;
			}
			total_size += iperf_data.buf_size;
			bandwidth_size += iperf_data.buf_size;
			report_size += iperf_data.buf_size;
			end_time = xTaskGetTickCount();

			if ((iperf_data.bandwidth != 0) && (bandwidth_size >= iperf_data.bandwidth) && ((end_time - bandwidth_time) < (configTICK_RATE_HZ * 1))) {
				vTaskDelay(configTICK_RATE_HZ * 1 - (end_time - bandwidth_time));
				end_time = xTaskGetTickCount();
				bandwidth_time = end_time;
				bandwidth_size = 0;
			}

			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rtcp_c: id[%d] Send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				bandwidth_time = end_time;
				report_size = 0;
				bandwidth_size = 0;
			}
		}
	}

	if (iperf_data.is_sub_stream && SUBSTREAM_FLAG) {
		//This stream is created by bidirectional parameter
		tptest_res_log("\n\rtcp_c: [END] id[%d] Bidirection Totally send %d KBytes in %d ms, %d Kbits/sec", iperf_data.is_sub_stream & 0xff, (int)(total_size / KB),
					   (int)(end_time - start_time),
					   (int)((total_size * 8) / (end_time - start_time)));
	} else {
		tptest_res_log("\n\rtcp_c: [END] id[%d] Totally send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(total_size / KB),
					   (int)(end_time - start_time),
					   (int)((total_size * 8) / (end_time - start_time)));
	}

exit1:
	closesocket(iperf_data.client_fd);
exit2:
	tptest_res_log("\n\r%s: Close client socket", __func__);
	if (tcp_client_buffer) {
		vPortFree(tcp_client_buffer);
		tcp_client_buffer = NULL;
	}

	return 0;
}

int tcp_server_func(struct iperf_data_t iperf_data)
{
	struct sockaddr_in   ser_addr, client_addr;
	int                  addrlen = sizeof(struct sockaddr_in);
	int                  n = 1;
	int                  recv_size = 0;
	uint64_t             total_size = 0, report_size = 0;
	uint32_t             start_time, report_start_time, end_time;
	struct iperf_tcp_client_hdr client_hdr;
	char *tcp_server_buffer = NULL;
	struct iperf_data_t *tcp_client_data = NULL;

	tcp_server_buffer = pvPortMalloc(iperf_data.buf_size);
	if (!tcp_server_buffer) {
		tptest_res_log("\n\r[ERROR] %s: Alloc buffer failed", __func__);
		goto exit4;
	}

	//create socket
	if ((iperf_data.server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Create socket failed", __func__);
		goto exit4;
	}

	tptest_res_log("\n\r%s: Create socket fd = %d", __func__, iperf_data.server_fd);

	setsockopt(iperf_data.server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *) &n, sizeof(n));

	//initialize structure dest
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(iperf_data.port);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// binding the TCP socket to the TCP server address
	if (bind(iperf_data.server_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Bind socket failed", __func__);
		goto exit3;
	}
	tptest_res_log("\n\r%s: Bind socket successfully", __func__);

	//Make it listen to socket with max 20 connections
	if (listen(iperf_data.server_fd, 20) != 0) {
		tptest_res_log("\n\r[ERROR] %s: Listen socket failed", __func__);
		goto exit3;
	}
	tptest_res_log("\n\r%s: Listen port %d", __func__, iperf_data.port);

//Restart:
	if ((iperf_data.client_fd = accept(iperf_data.server_fd, (struct sockaddr *)&client_addr, (u32_t *)&addrlen)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Accept TCP client socket error!", __func__);
		goto exit3;
	}
	tptest_res_log("\n\r%s: Accept connection successfully", __func__);

	recv_size = recv(iperf_data.client_fd, tcp_server_buffer, iperf_data.buf_size, 0);
	if (!iperf_data.bidirection) { //Server
		//parser the amount of tcp iperf setting
		memcpy(&client_hdr, tcp_server_buffer, sizeof(client_hdr));
		if (ntohl(client_hdr.flags) == 0x80000001) { //bi-direction, create client to send packets back
			rtw_mutex_get(&g_tptest_mutex);
			tcp_client_data = init_stream_data('t', 'c');
			rtw_mutex_put(&g_tptest_mutex);
			if (tcp_client_data == NULL) {
				tptest_res_log("\n\r[ERROR] init_stream_data failed!\n");
				goto exit2;
			}
			if (client_hdr.mAmount != 0) {
				client_hdr.mAmount = ntohl(client_hdr.mAmount);
				if (client_hdr.mAmount > 0x7fffffff) {
					client_hdr.mAmount = (~(client_hdr.mAmount) + 1) / 100;
					tcp_client_data->time = client_hdr.mAmount;
				} else {
					tcp_client_data->total_size = client_hdr.mAmount;
				}
			}
			strncpy((char *)tcp_client_data->server_ip, inet_ntoa(client_addr.sin_addr), (strlen(inet_ntoa(client_addr.sin_addr))));
			tcp_client_data->port = ntohl(client_hdr.mPort);
			tcp_client_data->buf_size = CLIENT_BUF_SIZE;
			tcp_client_data->report_interval = DEFAULT_REPORT_INTERVAL;
			tcp_client_data->is_sub_stream = SUBSTREAM_FLAG | iperf_data.stream_id;

			if (xTaskCreate(iperf_test_handler, "iperf_test_handler", BSD_STACK_SIZE, (void *) tcp_client_data, tskIDLE_PRIORITY + 1 + PRIORITIE_OFFSET,
							&tcp_client_data->task) != pdPASS) {
				tptest_res_log("\n\rTCP ERROR: Create TCP client task failed.");
				goto exit1;
			}
		}
	}

	start_time = xTaskGetTickCount();
	end_time = start_time;
	report_start_time = start_time;
	while (!g_stream_id[iperf_data.stream_id].terminate) {
		recv_size = recv(iperf_data.client_fd, tcp_server_buffer, iperf_data.buf_size, 0);  //MSG_DONTWAIT   MSG_WAITALL
		if (recv_size < 0) {
			tptest_res_log("\n\r[ERROR] %s: Receive data failed", __func__);
			goto exit2;
		} else if (recv_size == 0) {
			//tptest_res_log("\n\r%s: [END] Totally receive %d KBytes in %d ms, %d Kbits/sec",__func__, (uint32_t) (total_size/KB),(uint32_t) (end_time-start_time),((uint32_t) (total_size*8)/(end_time - start_time)));
			//total_size=0;
			//close(iperf_data.client_fd);
			//goto Restart;
			break;
		}
		end_time = xTaskGetTickCount();
		total_size += recv_size;
		report_size += recv_size;
		if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
			tptest_res_log("\n\rtcp_s: id[%d] Receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
						   (int)((report_size * 8) / (end_time - report_start_time)));
			report_start_time = end_time;
			report_size = 0;
		}
	}

	tptest_res_log("\n\rtcp_s: [END] id[%d] Totally receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(total_size / KB),
				   (int)(end_time - start_time),
				   (int)((total_size * 8) / (end_time - start_time)));

exit1:
	rtw_mutex_get(&g_tptest_mutex);
	free_stream_data(tcp_client_data);
	rtw_mutex_put(&g_tptest_mutex);

exit2:
	// close the connected socket after receiving from connected TCP client
	close(iperf_data.client_fd);

exit3:
	// close the listening socket
	close(iperf_data.server_fd);

exit4:
	if (tcp_server_buffer) {
		vPortFree(tcp_server_buffer);
		tcp_server_buffer = NULL;
	}
	return 0;
}

int udp_client_func(struct iperf_data_t iperf_data)
{
	struct sockaddr_in  ser_addr;
	uint32_t			i = 0;
	int                 addrlen = sizeof(struct sockaddr_in);
	uint32_t            start_time, end_time, bandwidth_time, report_start_time;
	uint64_t            total_size = 0, bandwidth_size = 0, report_size = 0;
	struct iperf_udp_client_hdr client_hdr = {0};
	u32_t now;
	uint32_t id_cnt = 0;
	int tos_value = (int)iperf_data.tos_value;// fix optlen check fail issue in lwip_setsockopt_impl
	extern int			skbbuf_used_num;
	extern int			skbdata_used_num;
	extern int			max_local_skb_num;
	extern int			max_skb_buf_num;
	char *udp_client_buffer = NULL;

	udp_client_buffer = pvPortMalloc(iperf_data.buf_size);
	if (!udp_client_buffer) {
		tptest_res_log("\n\r[ERROR] %s: Alloc buffer failed", __func__);
		goto exit2;
	}

	//filling the buffer
	for (i = 0; i < iperf_data.buf_size; i++) {
		udp_client_buffer[i] = (char)(i % 10);
	}

	//create socket
	if ((iperf_data.client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Create UDP socket failed", __func__);
		goto exit2;
	}

	//initialize value in dest
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(iperf_data.port);
	ser_addr.sin_addr.s_addr = inet_addr((char const *)iperf_data.server_ip);

	tptest_res_log("\n\r%s: Server IP=%s, port=%d", __func__, iperf_data.server_ip, iperf_data.port);
	tptest_res_log("\n\r%s: Create socket fd = %d", __func__, iperf_data.client_fd);

	if (setsockopt(iperf_data.client_fd, IPPROTO_IP, IP_TOS, &tos_value, sizeof(tos_value)) != 0) {
		tptest_res_log("\n\r[ERROR] %s: Set sockopt failed", __func__);
		goto exit1;
	}

	client_hdr.numThreads = htonl(0x00000001);
	client_hdr.mPort = htonl(iperf_data.port);
	client_hdr.bufferlen = 0;
	client_hdr.mWinband = htonl(iperf_data.bandwidth);

	if (iperf_data.bidirection) {
		client_hdr.id = 0;
		client_hdr.tv_sec = 0;
		client_hdr.tv_usec = 0;
		client_hdr.flags = htonl(0x80000001);
		client_hdr.mAmount = htonl(~(iperf_data.time * 100) + 1);
		memcpy(udp_client_buffer, &client_hdr, sizeof(client_hdr));
	}

	if (iperf_data.total_size == 0) {
		start_time = xTaskGetTickCount();
		end_time = start_time;
		bandwidth_time = start_time;
		report_start_time = start_time;
		client_hdr.mAmount = htonl(~(iperf_data.time * 100) + 1);
		while (((end_time - start_time) <= (configTICK_RATE_HZ * iperf_data.time)) && (!g_stream_id[iperf_data.stream_id].terminate)) {
			now = xTaskGetTickCount();
			client_hdr.id = htonl(id_cnt);
			client_hdr.tv_sec  = htonl(now / 1000);
			client_hdr.tv_usec = htonl((now % 1000) * 1000);
			memcpy(udp_client_buffer, &client_hdr, sizeof(client_hdr));
			if (sendto(iperf_data.client_fd, udp_client_buffer, iperf_data.buf_size, 0, (struct sockaddr *)&ser_addr, addrlen) < 0) {
				//Add delay to avoid consuming too much CPU when data link layer is busy
				vTaskDelay(2);
			} else {
				total_size += iperf_data.buf_size;
				bandwidth_size += iperf_data.buf_size;
				report_size += iperf_data.buf_size;
				// increase id_cnt only send success
				id_cnt++;
			}
			end_time = xTaskGetTickCount();
			if ((bandwidth_size >= iperf_data.bandwidth) && ((end_time - bandwidth_time) < (configTICK_RATE_HZ * 1))) {
				vTaskDelay(configTICK_RATE_HZ * 1 - (end_time - bandwidth_time));
				end_time = xTaskGetTickCount();
				bandwidth_time = end_time;
				bandwidth_size = 0;
			}

			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rudp_c: id[%d] Send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				bandwidth_time = end_time;
				report_size = 0;
				bandwidth_size = 0;
			}
		}
	} else {
		start_time = xTaskGetTickCount();
		end_time = start_time;
		bandwidth_time = start_time;
		report_start_time = start_time;
		client_hdr.mAmount = htonl(iperf_data.total_size);
		while ((total_size < iperf_data.total_size) && (!g_stream_id[iperf_data.stream_id].terminate)) {
			now = xTaskGetTickCount();
			client_hdr.id = htonl(id_cnt);
			client_hdr.tv_sec  = htonl(now / 1000);
			client_hdr.tv_usec = htonl((now % 1000) * 1000);
			memcpy(udp_client_buffer, &client_hdr, sizeof(client_hdr));
			while ((skbdata_used_num > (max_skb_buf_num - 5)) || (skbbuf_used_num > (max_local_skb_num - 5))) {
				vTaskDelay(1);
			}
			if (sendto(iperf_data.client_fd, udp_client_buffer, iperf_data.buf_size, 0, (struct sockaddr *)&ser_addr, addrlen) < 0) {
				//tptest_res_log("\n\r[ERROR] %s: UDP client send data error",__func__);
			} else {
				total_size += iperf_data.buf_size;
				bandwidth_size += iperf_data.buf_size;
				report_size += iperf_data.buf_size;
				// increase id_cnt only send success
				id_cnt++;
			}
			end_time = xTaskGetTickCount();
			if ((bandwidth_size >= iperf_data.bandwidth) && ((end_time - bandwidth_time) < (configTICK_RATE_HZ * 1))) {
				vTaskDelay(configTICK_RATE_HZ * 1 - (end_time - bandwidth_time));
				end_time = xTaskGetTickCount();
				bandwidth_time = end_time;
				bandwidth_size = 0;
			}

			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rudp_c: id[%d] Send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				bandwidth_time = end_time;
				report_size = 0;
				bandwidth_size = 0;
			}
		}
	}

	if (iperf_data.is_sub_stream && SUBSTREAM_FLAG) {
		//This stream is created by bidirectional parameter
		tptest_res_log("\n\rudp_c: [END] id[%d] Bidirection Totally send %d KBytes in %d ms, %d Kbits/sec", iperf_data.is_sub_stream & 0xff, (int)(total_size / KB),
					   (int)(end_time - start_time),
					   (int)((total_size * 8) / (end_time - start_time)));
	} else {
		tptest_res_log("\n\rudp_c: [END] id[%d] Totally send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(total_size / KB),
					   (int)(end_time - start_time),
					   (int)((total_size * 8) / (end_time - start_time)));
	}

	// send a final terminating datagram
	i = 0;
	int rc;
	fd_set readSet;
	struct timeval timeout;
	uint32_t stop_ms;
	uint64_t total_len;

	now = xTaskGetTickCount();
	client_hdr.id = htonl(-id_cnt);
	client_hdr.tv_sec  = htonl(now / 1000);
	client_hdr.tv_usec = htonl((now % 1000) * 1000);
	memcpy(udp_client_buffer, &client_hdr, sizeof(client_hdr));

	while (i < 10) {
		i++;
		sendto(iperf_data.client_fd, udp_client_buffer, iperf_data.buf_size, 0, (struct sockaddr *)&ser_addr, addrlen);

		// wait until the socket is readable, or our timeout expires
		FD_ZERO(&readSet);
		FD_SET(iperf_data.client_fd, &readSet);
		timeout.tv_sec  = 0;
		timeout.tv_usec = 250000; // quarter second, 250 ms
		rc = select(iperf_data.client_fd + 1, &readSet, NULL, NULL, &timeout);
		if (rc == -1) {
			break;
		}

		if (rc == 0) {
			// select timed out
			continue;
		} else {
			// socket ready to read
			rc = read(iperf_data.client_fd, udp_client_buffer, iperf_data.buf_size);
			if (rc < 0) {
				break;
			}
			if (rc >= (int)(sizeof(struct iperf_udp_datagram) + sizeof(struct iperf_udp_server_hdr))) {
				struct iperf_udp_datagram *UDP_Hdr;
				struct iperf_udp_server_hdr *hdr;

				UDP_Hdr = (struct iperf_udp_datagram *) udp_client_buffer;
				hdr = (struct iperf_udp_server_hdr *)(UDP_Hdr + 1);
				tptest_res_log("\n\r%s: Server Report", __func__);
				if ((ntohl(hdr->flags) & 0x80000000) != 0) {
					stop_ms = ntohl(hdr->stop_sec) * 1000 + ntohl(hdr->stop_usec) / 1000;
					total_len = (((uint64_t) ntohl(hdr->total_len1)) << 32) + ntohl(hdr->total_len2);
					tptest_res_log("\n\rudp_c: [END] id[%d] Totally send %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(total_len / KB), (int)stop_ms,
								   (int)(total_len * 8 / stop_ms));
				}
			}
			break;
		}
	}
exit1:
	close(iperf_data.client_fd);
exit2:
	tptest_res_log("\n\r%s: Close client socket", __func__);
	if (udp_client_buffer) {
		vPortFree(udp_client_buffer);
		udp_client_buffer = NULL;
	}
	return 0;
}

int udp_server_func(struct iperf_data_t iperf_data)
{
	struct sockaddr_in   ser_addr, client_addr;
	int                  addrlen = sizeof(struct sockaddr_in);
	int                  n = 1;
	int 				 datagram_id;
	uint32_t             start_time, report_start_time, end_time;
	int                  recv_size = 0;
	uint64_t             total_size = 0, report_size = 0;
	struct iperf_udp_client_hdr client_hdr;
	uint8_t time_boundary = 0, size_boundary = 0;
	char *udp_server_buffer = NULL;
	struct iperf_data_t *udp_client_data = NULL;

	udp_server_buffer = pvPortMalloc(iperf_data.buf_size);
	if (!udp_server_buffer) {
		tptest_res_log("\n\r[ERROR] %s: Alloc buffer failed", __func__);
		goto exit3;
	}

	//create socket
	if ((iperf_data.server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Create socket failed", __func__);
		goto exit3;
	}
	tptest_res_log("\n\r%s: Create socket fd = %d, port = %d", __func__, iperf_data.server_fd, iperf_data.port);

	setsockopt(iperf_data.server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *) &n, sizeof(n));

	//initialize structure dest
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(iperf_data.port);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// binding the UDP socket to the UDP server address
	if (bind(iperf_data.server_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0) {
		tptest_res_log("\n\r[ERROR] %s: Bind socket failed", __func__);
		goto exit2;
	}

	tptest_res_log("\n\r%s: Bind socket successfully", __func__);

	//wait for first packet to start
	recv_size = recvfrom(iperf_data.server_fd, udp_server_buffer, iperf_data.buf_size, 0, (struct sockaddr *) &client_addr, (u32_t *)&addrlen);
	total_size += recv_size;
	report_size += recv_size;
	start_time = xTaskGetTickCount();
	report_start_time = start_time;
	end_time = start_time;
	if (!iperf_data.bidirection) { //Server
		//parser the amount of udp iperf setting
		memcpy(&client_hdr, udp_server_buffer, sizeof(client_hdr));
		printf("\nclient_hdr.mAmount = %x\n", (unsigned int)client_hdr.mAmount);

		if (client_hdr.mAmount != 0) {
			client_hdr.mAmount = ntohl(client_hdr.mAmount);
			if (client_hdr.mAmount > 0x7fffffff) {
				client_hdr.mAmount = (~(client_hdr.mAmount) + 1) / 100;
				time_boundary = 1;
			} else {
				size_boundary = 1;
			}
		} else {
			printf("\nNo boundary\n");
			//set receive timeout
			int recv_timeout = 500;

#if defined(LWIP_SO_SNDRCVTIMEO_NONSTANDARD) && (LWIP_SO_SNDRCVTIMEO_NONSTANDARD == 0)	//lwip 2.0.2
			struct timeval timeout;
			timeout.tv_sec  = recv_timeout / 1000;
			timeout.tv_usec = (recv_timeout % 1000) * 1000;
			setsockopt(iperf_data.server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#else	//lwip 1.4.1
			setsockopt(iperf_data.server_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
#endif
		}
		if (ntohl(client_hdr.flags) == 0x80000001) { //bi-direction, create client to send packets back
			rtw_mutex_get(&g_tptest_mutex);
			udp_client_data = init_stream_data('u', 'c');
			rtw_mutex_put(&g_tptest_mutex);
			if (udp_client_data == NULL) {
				tptest_res_log("\n\r[ERROR] init_stream_data failed!\n");
				goto exit2;
			}
			if (time_boundary) {
				udp_client_data->time = client_hdr.mAmount;
			} else if (size_boundary) {
				udp_client_data->total_size = client_hdr.mAmount;
			}
			strncpy((char *)udp_client_data->server_ip, inet_ntoa(client_addr.sin_addr), (strlen(inet_ntoa(client_addr.sin_addr))));
			udp_client_data->port = ntohl(client_hdr.mPort);
			udp_client_data->bandwidth = ntohl(client_hdr.mWinband);
			udp_client_data->buf_size = CLIENT_BUF_SIZE;
			udp_client_data->tos_value = DEFAULT_UDP_TOS_VALUE;
			udp_client_data->report_interval = DEFAULT_REPORT_INTERVAL;
			udp_client_data->is_sub_stream = SUBSTREAM_FLAG | iperf_data.stream_id;
			if (xTaskCreate(iperf_test_handler, "iperf_test_handler", BSD_STACK_SIZE, (void *) udp_client_data, tskIDLE_PRIORITY + 1 + PRIORITIE_OFFSET,
							&udp_client_data->task) != pdPASS) {
				tptest_res_log("\r\nUDP ERROR: Create UDP client task failed.");
				goto exit1;
			}
		}
	} else { //listener
		if (iperf_data.total_size) {
			client_hdr.mAmount = iperf_data.total_size;
			size_boundary = 1;
		} else if (iperf_data.time) {
			client_hdr.mAmount = iperf_data.time;
			time_boundary = 1;
		}
	}

	if (time_boundary) {
		while (((end_time - start_time) <= (configTICK_RATE_HZ * client_hdr.mAmount))  && (!g_stream_id[iperf_data.stream_id].terminate)) {
			recv_size = recvfrom(iperf_data.server_fd, udp_server_buffer, iperf_data.buf_size, 0, (struct sockaddr *) &client_addr, (u32_t *)&addrlen);
			if (recv_size < 0) {
				tptest_res_log("\n\r[ERROR] %s: Receive data failed", __func__);
				goto exit1;
			}

			// ack data to client
			// Not send ack to prevent send fail due to limited skb, but it will have warning at iperf client
			//sendto(server_fd,udp_server_buffer,ret,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
			datagram_id = ntohl(((struct iperf_udp_datagram *) udp_server_buffer)->id);
			if (datagram_id < 0) {
				sendto(iperf_data.server_fd, udp_server_buffer, 0, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
				g_stream_id[iperf_data.stream_id].terminate = 1;
			}

			end_time = xTaskGetTickCount();
			total_size += recv_size;
			report_size += recv_size;
			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rudp_s: id[%d] Receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				report_size = 0;
			}
		}
	} else if (size_boundary) {
		while ((total_size < client_hdr.mAmount) && (!g_stream_id[iperf_data.stream_id].terminate)) {
			recv_size = recvfrom(iperf_data.server_fd, udp_server_buffer, iperf_data.buf_size, 0, (struct sockaddr *) &client_addr, (u32_t *)&addrlen);
			if (recv_size < 0) {
				tptest_res_log("\n\r[ERROR] %s: Receive data failed", __func__);
				goto exit1;
			}

			// ack data to client
			// Not send ack to prevent send fail due to limited skb, but it will have warning at iperf client
			//sendto(server_fd,udp_server_buffer,ret,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
			datagram_id = ntohl(((struct iperf_udp_datagram *) udp_server_buffer)->id);
			if (datagram_id < 0) {
				sendto(iperf_data.server_fd, udp_server_buffer, 0, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
				g_stream_id[iperf_data.stream_id].terminate = 1;
			}

			end_time = xTaskGetTickCount();
			total_size += recv_size;
			report_size += recv_size;
			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rudp_s: id[%d] Receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				report_size = 0;
			}
		}
	} else {
		while (!g_stream_id[iperf_data.stream_id].terminate) {
			recv_size = recvfrom(iperf_data.server_fd, udp_server_buffer, iperf_data.buf_size, 0, (struct sockaddr *) &client_addr, (u32_t *)&addrlen);
			if (recv_size < 0) {
				tptest_res_log("\n\r%s: Receive data timeout", __func__);
				goto exit1;
			}

			// ack data to client
			// Not send ack to prevent send fail due to limited skb, but it will have warning at iperf client
			//sendto(server_fd,udp_server_buffer,ret,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
			datagram_id = ntohl(((struct iperf_udp_datagram *) udp_server_buffer)->id);
			if (datagram_id < 0) {
				sendto(iperf_data.server_fd, udp_server_buffer, 0, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
				g_stream_id[iperf_data.stream_id].terminate = 1;
			}

			end_time = xTaskGetTickCount();
			total_size += recv_size;
			report_size += recv_size;
			if ((iperf_data.report_interval != DEFAULT_REPORT_INTERVAL) && ((end_time - report_start_time) >= (configTICK_RATE_HZ * iperf_data.report_interval))) {
				tptest_res_log("\n\rudp_s: id[%d] Receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(report_size / KB), (int)(end_time - report_start_time),
							   (int)((report_size * 8) / (end_time - report_start_time)));
				report_start_time = end_time;
				report_size = 0;
			}
		}
	}

exit1:
	tptest_res_log("\n\rudp_s: [END] id[%d] Totally receive %d KBytes in %d ms, %d Kbits/sec", iperf_data.stream_id, (int)(total_size / KB),
				   (int)(end_time - start_time),
				   (int)((uint64_t)(total_size * 8) / (end_time - start_time)));


	rtw_mutex_get(&g_tptest_mutex);
	free_stream_data(udp_client_data);
	rtw_mutex_put(&g_tptest_mutex);

exit2:
	// close the listening socket
	close(iperf_data.server_fd);

exit3:
	if (udp_server_buffer) {
		vPortFree(udp_server_buffer);
		udp_server_buffer = NULL;
	}
	return 0;
}

static void iperf_test_handler(void *param)
{
	/* To avoid gcc warnings */
	struct iperf_data_t *stream_data = (struct iperf_data_t *) param;

	vTaskDelay(100);
	if (stream_data->protocol == 'u') {
		if (stream_data->role == 'c') {
			tptest_res_log("\n\rStart UDP client! id = [%d]", stream_data->stream_id);
			udp_client_func(stream_data[0]);
			tptest_res_log("\n\rUDP client stopped!");
		} else if (stream_data->role == 's') {
			tptest_res_log("\n\rStart UDP server! id = [%d]", stream_data->stream_id);
			udp_server_func(stream_data[0]);
			tptest_res_log("\n\rUDP server stopped!");
		}
	} else if (stream_data->protocol == 't') {
		if (stream_data->role == 'c') {
			tptest_res_log("\n\rStart TCP client! id = [%d]", stream_data->stream_id);
			tcp_client_func(stream_data[0]);
			tptest_res_log("\n\rTCP client stopped!");
		} else if (stream_data->role == 's') {
			tptest_res_log("\n\rStart TCP server! id = [%d]", stream_data->stream_id);
			tcp_server_func(stream_data[0]);
			tptest_res_log("\n\rTCP server stopped!");
		}
	} else {
		tptest_res_log("\n\r[ERROR] Wrong data");
	}

#if defined(INCLUDE_uxTaskGetStackHighWaterMark) && (INCLUDE_uxTaskGetStackHighWaterMark == 1)
	tptest_res_log("\n\rMin available stack size of %s = %d * %d bytes", __FUNCTION__, uxTaskGetStackHighWaterMark(NULL), sizeof(portBASE_TYPE));
#endif

	vTaskDelete(NULL);
}

void cmd_iperf(int argc, char **argv)
{
	int argv_count = 2;
	uint8_t stream_id;
	struct iperf_data_t *stream_data = NULL;
	struct iperf_data_t *stream_data_s = NULL;
	uint8_t protocol = 0;

	iperf_init();
	rtw_mutex_get(&g_tptest_mutex);

	if (argc < 2) {
		goto exit;
	}

	if (strncmp(argv[0], "tcp", 3) == 0) {
		protocol = 't';
	} else if (strncmp(argv[0], "udp", 3) == 0) {
		protocol = 'u';
	} else {
		goto exit;
	}

	while (argv_count <= argc) {
		//first operation
		if (argv_count == 2) {
			if (strcmp(argv[argv_count - 1], "-s") == 0) {
				stream_data = init_stream_data(protocol, 's');
				if (stream_data == NULL) {
					tptest_res_log("\n\r[ERROR] init_stream_data failed!\n");
					goto exit;
				}
				argv_count++;
			} else if (strcmp(argv[argv_count - 1], "stop") == 0) {
				if (argc == 3) {
					stream_id = atoi(argv[2]);
					g_stream_id[stream_id].terminate = 1;
					rtw_mutex_put(&g_tptest_mutex);
					return;
				} else {
					goto exit;
				}
			} else if (strcmp(argv[argv_count - 1], "-c") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data = init_stream_data(protocol, 'c');
				if (stream_data == NULL) {
					tptest_res_log("\n\r[ERROR] init_stream_data failed!\n");
					goto exit;
				}
				strncpy((char *)stream_data->server_ip, argv[2], sizeof(stream_data->server_ip) - 1);
				stream_data->server_ip[sizeof(stream_data->server_ip) - 1] = '\0';
				argv_count += 2;
			} else {
				goto exit;
			}
		} else {
			if ((strcmp(argv[argv_count - 1], "-b") == 0) && (stream_data->protocol == 'u')) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				if (stream_data->role == 'c') {
					stream_data->bandwidth = km_parser(argv[argv_count], strlen(argv[argv_count]));
					stream_data->bandwidth = stream_data->bandwidth / 8; //bits to Bytes
				} else {
					goto exit;
				}
				argv_count += 2;
			} else if ((strcmp(argv[argv_count - 1], "-d") == 0) && (stream_data->role == 'c')) {
				stream_data->bidirection = 1;
				argv_count += 1;
			} else if (strcmp(argv[argv_count - 1], "-i") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data->report_interval = (uint32_t) atoi(argv[argv_count]);
				argv_count += 2;
			} else if (strcmp(argv[argv_count - 1], "-l") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data->buf_size = atoi(argv[argv_count]);
				argv_count += 2;
			} else if (strcmp(argv[argv_count - 1], "-n") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data->time = 0;
				stream_data->total_size = km_parser(argv[argv_count], strlen(argv[argv_count]));
				argv_count += 2;
			} else if (strcmp(argv[argv_count - 1], "-p") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data->port = (uint16_t) atoi(argv[argv_count]);
				argv_count += 2;
			}
#if CONFIG_WLAN
			else if (strcmp(argv[argv_count - 1], "-S") == 0) { //for wmm test
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				if (stream_data->role == 'c') {
					if (atoi(argv[argv_count]) >= 0 && atoi(argv[argv_count]) <= 255) {
						stream_data->tos_value = (uint8_t) atoi(argv[argv_count]);
					} else {
						goto exit;
					}
				} else {
					goto exit;
				}
				argv_count += 2;
			}
#endif
			else if (strcmp(argv[argv_count - 1], "-t") == 0) {
				if (argc < (argv_count + 1)) {
					goto exit;
				}
				stream_data->time = atoi(argv[argv_count]);
				argv_count += 2;
			} else {
				goto exit;
			}
		}
	}

	if (stream_data->role == 's') {
		if (xTaskCreate(iperf_test_handler, "iperf_test_handler", BSD_STACK_SIZE, (void *) stream_data, tskIDLE_PRIORITY + 2 + PRIORITIE_OFFSET,
						&stream_data->task) != pdPASS) {
			tptest_res_log("\r\nUDP ERROR: Create UDP server task failed.");
			goto exit;
		}
	} else if (stream_data->role == 'c') {
		if (stream_data->bidirection == 1) {
			stream_data_s = init_stream_data(protocol, 's');
			if (stream_data_s == NULL) {
				tptest_res_log("\n\r[ERROR] init_stream_data failed!\n");
				goto exit;
			}
			stream_data_s->bidirection = 1;
			stream_data_s->port = stream_data->port;
			stream_data_s->time = stream_data->time;
			stream_data_s->total_size = stream_data->total_size;
			if (xTaskCreate(iperf_test_handler, "iperf_test_handler", BSD_STACK_SIZE, (void *) stream_data_s, tskIDLE_PRIORITY + 2 + PRIORITIE_OFFSET,
							&stream_data_s->task) != pdPASS) {
				tptest_res_log("\r\nUDP ERROR: Create UDP server task failed.");
				goto exit;
			}
		}
		if (xTaskCreate(iperf_test_handler, "iperf_test_handler", BSD_STACK_SIZE, (void *) stream_data, tskIDLE_PRIORITY + 1 + PRIORITIE_OFFSET,
						&stream_data->task) != pdPASS) {
			tptest_res_log("\r\nUDP ERROR: Create UDP client task failed.");
			vTaskDelete(stream_data_s->task);
			goto exit;
		}
	}

	rtw_mutex_put(&g_tptest_mutex);
	return;

exit:
	free_stream_data(stream_data);
	free_stream_data(stream_data_s);
	rtw_mutex_put(&g_tptest_mutex);

	if (strncmp(argv[0], "tcp", 3) == 0) {
		printf("\n\r[ATWT] Command format ERROR!\n");
		printf("\n\r[ATWT] Usage: ATWT=[-s|-c,host|stop],[options]\n");
		printf("\n\r   Client/Server:\n");
		printf("  \r	 stop  #		terminate specific stream id\n");
		printf("  \r	 -i    #		seconds between periodic bandwidth reports\n");
		printf("  \r	 -l    #		length of buffer to read or write (default 1460 Bytes)\n");
		printf("  \r	 -p    #		server port to listen on/connect to (default 5001)\n");
		printf("\n\r   Server specific:\n");
		printf("  \r	 -s 			run in server mode\n");
		printf("\n\r   Client specific:\n");
		printf("  \r	 -c    <host>	run in client mode, connecting to <host>\n");
		printf("  \r	 -d 			Do a bidirectional test simultaneously\n");
		printf("  \r	 -t    #		time in seconds to transmit for (default 10 secs)\n");
		printf("  \r	 -n    #[KM]	number of bytes to transmit (instead of -t)\n");
		printf("\n\r   Example:\n");
		printf("  \r	 ATWT=-s,-p,5002\n");
		printf("  \r	 ATWT=-c,192.168.1.2,-t,100,-p,5002\n");
	} else if (strncmp(argv[0], "udp", 3) == 0) {
		printf("\n\r[ATWU] Command format ERROR!\n");
		printf("\n\r[ATWU] Usage: ATWU=[-s|-c,host|stop][options]\n");
		printf("\n\r   Client/Server:\n");
		printf("  \r     stop  #        terminate specific stream id\n");
		printf("  \r     -i    #        seconds between periodic bandwidth reports\n");
		printf("  \r     -l    #        length of buffer to read or write (default 1460 Bytes)\n");
		printf("  \r     -p    #        server port to listen on/connect to (default 5001)\n");
		printf("\n\r   Server specific:\n");
		printf("  \r     -s             run in server mode\n");
		printf("\n\r   Client specific:\n");
		printf("  \r     -b    #[KM]    for UDP, bandwidth to send at in bits/sec (default 1 Mbit/sec)\n");
		printf("  \r     -c    <host>   run in client mode, connecting to <host>\n");
		printf("  \r     -d             Do a bidirectional test simultaneously\n");
		printf("  \r     -t    #        time in seconds to transmit for (default 10 secs)\n");
		printf("  \r     -n    #[KM]    number of bytes to transmit (instead of -t)\n");
#if CONFIG_WLAN
		printf("  \r     -S    #        set the IP 'type of service'\n");
#endif
		printf("\n\r   Example:\n");
		printf("  \r     ATWU=-s,-p,5002\n");
		printf("  \r     ATWU=-c,192.168.1.2,-t,100,-p,5002\n");
	} else {
		printf("\n\rATWT/ATWU Command ERROR!\n");
	}
	return;
}

