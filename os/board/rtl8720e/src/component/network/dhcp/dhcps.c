
#include "dhcps.h"
#include "tcpip.h"
#include "wifi_constants.h"
#include "lwip_netconf.h"
//static struct dhcp_server_state dhcp_server_state_machine;
static uint8_t dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;
/* recorded the client MAC addr(default sudo mac) */
//static uint8_t dhcps_record_first_client_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
/* recorded transaction ID (default sudo id)*/
static uint8_t dhcp_recorded_xid[4] = {0xff, 0xff, 0xff, 0xff};

/* UDP Protocol Control Block(PCB) */
static struct udp_pcb *dhcps_pcb;
static struct udp_pcb *dns_server_pcb;

static struct ip_addr dhcps_send_broadcast_address;
static struct ip_addr dhcps_local_address;
static struct ip_addr dhcps_pool_start;
static struct ip_addr dhcps_pool_end;
static struct ip_addr dhcps_local_mask;
static struct ip_addr dhcps_local_gateway;
static struct ip_addr dhcps_network_id;
static struct ip_addr dhcps_subnet_broadcast;
static struct eth_addr dhcps_allocated_client_ethaddr;
static struct ip_addr dhcps_allocated_client_address;
static int dhcps_addr_pool_set = 0;
static struct ip_addr dhcps_addr_pool_start;
static struct ip_addr dhcps_addr_pool_end;
#if 1
static struct ip_addr dhcps_owned_first_ip;
static struct ip_addr dhcps_owned_last_ip;
static volatile uint8_t dhcps_num_of_available_ips;
#endif
static struct dhcp_msg *dhcp_message_repository;
static int dhcp_message_total_options_lenth;

/* allocated IP range */
static struct table  ip_table;
static struct ip_addr client_request_ip;
static uint8_t client_addr[6];

static xSemaphoreHandle dhcps_ip_table_semaphore;

static struct netif *dhcps_netif = NULL;
/**
  * @brief  latch the specific ip in the ip table.
  * @param  d the specific index
  * @retval None.
  */
#if (!IS_USE_FIXED_IP)
static void mark_ip_in_table(uint8_t d)
{
#if (debug_dhcps)
	printf("\r\nmark ip %d\r\n", d);
#endif
	xSemaphoreTake(dhcps_ip_table_semaphore, portMAX_DELAY);
	if (0 < d && d <= 32) {
		ip_table.ip_range[0] = MARK_RANGE1_IP_BIT(ip_table, d);
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[0] = 0x%x\r\n", ip_table.ip_range[0]);
#endif
	} else if (32 < d && d <= 64) {
		ip_table.ip_range[1] = MARK_RANGE2_IP_BIT(ip_table, (d - 32));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[1] = 0x%x\r\n", ip_table.ip_range[1]);
#endif
	} else if (64 < d && d <= 96) {
		ip_table.ip_range[2] = MARK_RANGE3_IP_BIT(ip_table, (d - 64));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[2] = 0x%x\r\n", ip_table.ip_range[2]);
#endif
	} else if (96 < d && d <= 128) {
		ip_table.ip_range[3] = MARK_RANGE4_IP_BIT(ip_table, (d - 96));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[3] = 0x%x\r\n", ip_table.ip_range[3]);
#endif
	} else if (128 < d && d <= 160) {
		ip_table.ip_range[4] = MARK_RANGE5_IP_BIT(ip_table, d);
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[4] = 0x%x\r\n", ip_table.ip_range[4]);
#endif
	} else if (160 < d && d <= 192) {
		ip_table.ip_range[5] = MARK_RANGE6_IP_BIT(ip_table, (d - 160));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[5] = 0x%x\r\n", ip_table.ip_range[5]);
#endif
	} else if (192 < d && d <= 224) {
		ip_table.ip_range[6] = MARK_RANGE7_IP_BIT(ip_table, (d - 192));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[6] = 0x%x\r\n", ip_table.ip_range[6]);
#endif
	} else if (224 < d) {
		ip_table.ip_range[7] = MARK_RANGE8_IP_BIT(ip_table, (d - 224));
#if (debug_dhcps)
		printf("\r\n ip_table.ip_range[7] = 0x%x\r\n", ip_table.ip_range[7]);
#endif
	} else {
		printf("\r\n Request ip over the range(1-128) \r\n");
	}
	xSemaphoreGive(dhcps_ip_table_semaphore);

}
#ifdef CONFIG_DHCPS_KEPT_CLIENT_INFO
static void save_client_addr(struct ip_addr *client_ip, uint8_t *hwaddr)
{
	int i, j, client_number;
	uint8_t invalid_ipaddr4;
	struct {
		int count;
		rtw_mac_t mac_list[AP_STA_NUM];
	} client_info;

#if LWIP_VERSION_MAJOR >= 2
	uint8_t d = (uint8_t)ip4_addr4(ip_2_ip4(client_ip));
#else
	uint8_t d = (uint8_t)ip4_addr4(client_ip);
#endif

	xSemaphoreTake(dhcps_ip_table_semaphore, portMAX_DELAY);

	for (i = 0; i < AP_STA_NUM; i++) {
		if ((ip_table.ip_addr4[i] == 0 || ip_table.ip_addr4[i] == d)) {
			ip_table.ip_addr4[i] = d;
			memcpy(ip_table.client_mac[i], hwaddr, 6);
			break;
		}
	}
	/* cache of ip_table is full,write the new mac&ip instead of an invalid pairs */
	if (i == AP_STA_NUM) {
		wifi_get_associated_client_list(&client_info, sizeof(client_info));
		for (j = 0; j < AP_STA_NUM; j++) {
			for (client_number = 0; client_number < client_info.count; client_number++) {
				if (memcmp(ip_table.client_mac[j], client_info.mac_list[client_number].octet, 6) == 0) {
					break;
				}
			}
			/* find an invalid mac&ip,update it with a new record */
			if (client_number == client_info.count) {
				invalid_ipaddr4 = ip_table.ip_addr4[j];
				ip_table.ip_range[invalid_ipaddr4 / 32] &= (~ BIT(invalid_ipaddr4 % 32 - 1)); //clear invalid ip_range
				ip_table.ip_addr4[j] = d;
				memcpy(ip_table.client_mac[j], hwaddr, 6);
				break;
			}
		}
	}

#if (debug_dhcps)
#if LWIP_VERSION_MAJOR >= 2
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(ip_2_ip4(client_ip)), ip4_addr2(ip_2_ip4(client_ip)), ip4_addr3(ip_2_ip4(client_ip)), ip4_addr4(ip_2_ip4(client_ip)),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#else
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(client_ip), ip4_addr2(client_ip), ip4_addr3(client_ip), ip4_addr4(client_ip),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#endif
#endif
	xSemaphoreGive(dhcps_ip_table_semaphore);
}

static uint8_t check_client_request_ip(struct ip_addr *client_req_ip, uint8_t *hwaddr)
{
	/* To avoid gcc warnings */
	(void) client_req_ip;

	int ip_addr4 = 0, i;

#if (debug_dhcps)
#if LWIP_VERSION_MAJOR >= 2
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(ip_2_ip4(client_req_ip)), ip4_addr2(ip_2_ip4(client_req_ip)), ip4_addr3(ip_2_ip4(client_req_ip)), ip4_addr4(ip_2_ip4(client_req_ip)),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#else
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(client_req_ip), ip4_addr2(client_req_ip), ip4_addr3(client_req_ip), ip4_addr4(client_req_ip),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#endif
#endif

	xSemaphoreTake(dhcps_ip_table_semaphore, portMAX_DELAY);

	for (i = 0; i < AP_STA_NUM; i++) {
		if (memcmp(ip_table.client_mac[i], hwaddr, 6) == 0) {
			uint8_t temp = ip_table.ip_addr4[i];
			if ((ip_table.ip_range[temp / 32] >> (temp % 32 - 1)) & 1) {
				ip_addr4 = temp;
				break;
			}
		}
	}

	xSemaphoreGive(dhcps_ip_table_semaphore);

	if (i == AP_STA_NUM) {
		ip_addr4 = 0;
	}

	return ip_addr4;
}

static uint8_t check_client_direct_request_ip(struct ip_addr *client_req_ip, uint8_t *hwaddr)
{
	int ip_addr4 = 0;

#if (debug_dhcps)
#if LWIP_VERSION_MAJOR >= 2
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(ip_2_ip4(client_req_ip)), ip4_addr2(ip_2_ip4(client_req_ip)), ip4_addr3(ip_2_ip4(client_req_ip)), ip4_addr4(ip_2_ip4(client_req_ip)),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#else
	printf("\r\n%s: ip %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", __func__,
		   ip4_addr1(client_req_ip), ip4_addr2(client_req_ip), ip4_addr3(client_req_ip), ip4_addr4(client_req_ip),
		   hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
#endif
#endif
#if LWIP_VERSION_MAJOR >= 2
	if ((ip4_addr1(ip_2_ip4(&dhcps_network_id)) != ip4_addr1(ip_2_ip4(client_req_ip))) ||
		(ip4_addr2(ip_2_ip4(&dhcps_network_id)) != ip4_addr2(ip_2_ip4(client_req_ip))) ||
		(ip4_addr3(ip_2_ip4(&dhcps_network_id)) != ip4_addr3(ip_2_ip4(client_req_ip))))
#else
	if ((ip4_addr1(&dhcps_network_id) != ip4_addr1(client_req_ip)) ||
		(ip4_addr2(&dhcps_network_id) != ip4_addr2(client_req_ip)) ||
		(ip4_addr3(&dhcps_network_id) != ip4_addr3(client_req_ip)))
#endif
	{
		ip_addr4 = 0;
		goto Exit;
	}

	// check if the requested ip is available
#if LWIP_VERSION_MAJOR >= 2
	ip_addr4 = ip4_addr4(ip_2_ip4(client_req_ip));
#else
	ip_addr4 = ip4_addr4(client_req_ip);
#endif
	if (ip_addr4 < DHCP_POOL_START || ip_addr4 > DHCP_POOL_END) {
		ip_addr4 = 0;
		goto Exit;
	}
	xSemaphoreTake(dhcps_ip_table_semaphore, portMAX_DELAY);

	for (int i = 0; i < AP_STA_NUM; i++) {
		if ((ip_table.ip_addr4[i] == ip_addr4 &&
			 !(ip_table.client_mac[i][0] == hwaddr[0] &&
			   ip_table.client_mac[i][1] == hwaddr[1] &&
			   ip_table.client_mac[i][2] == hwaddr[2] &&
			   ip_table.client_mac[i][3] == hwaddr[3] &&
			   ip_table.client_mac[i][4] == hwaddr[4] &&
			   ip_table.client_mac[i][5] == hwaddr[5]))) {
			ip_addr4 = 0; // the ip is used
			break;
		}
		// the ip is available or already allocated to this client
	}

	xSemaphoreGive(dhcps_ip_table_semaphore);

Exit:
	return ip_addr4;
}

void dump_client_table(void)
{
#if 0
	int i;
	uint8_t *p = NULL;
	printf("\r\nip_range: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
		   ip_table.ip_range[0], ip_table.ip_range[1], ip_table.ip_range[2], ip_table.ip_range[3],
		   ip_table.ip_range[4], ip_table.ip_range[5], ip_table.ip_range[6], ip_table.ip_range[7]);
	for (i = 0; i < AP_STA_NUM; i++) {
		p = ip_table.client_mac[i];
		printf("\r\nip_addr4 = %d; Client[%d]: 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x",
			   ip_table.ip_addr4[i], i, p[0], p[1], p[2], p[3], p[4], p[5]);
	}
	printf("\r\n");
#endif
}
#endif //CONFIG_DHCPS_KEPT_CLIENT_INFO
#endif

/**
  * @brief  get one usable ip from the ip table of dhcp server.
  * @param: None
  * @retval the usable index which represent the ip4_addr(ip) of allocated ip addr.
  */
#if (!IS_USE_FIXED_IP)
static uint8_t search_next_ip(void)
{
	uint8_t range_count, offset_count;
	uint8_t start, end;
	volatile uint8_t max_count;
	if (dhcps_addr_pool_set) {
#if LWIP_VERSION_MAJOR >= 2
		start = (uint8_t)ip4_addr4(ip_2_ip4(&dhcps_addr_pool_start));
		end = (uint8_t)ip4_addr4(ip_2_ip4(&dhcps_addr_pool_end));
#else
		start = (uint8_t)ip4_addr4(&dhcps_addr_pool_start);
		end = (uint8_t)ip4_addr4(&dhcps_addr_pool_end);
#endif
	} else {
		start = 0;
		end = 255;
	}
	xSemaphoreTake(dhcps_ip_table_semaphore, portMAX_DELAY);
	for (range_count = 0; range_count < (max_count = 8); range_count++) {
		for (offset_count = 0; offset_count < 32; offset_count++) {
			if ((((ip_table.ip_range[range_count] >> offset_count) & 0x01) == 0)
				&& (((range_count * 32) + (offset_count + 1)) >= start)
				&& (((range_count * 32) + (offset_count + 1)) <= end)) {
				xSemaphoreGive(dhcps_ip_table_semaphore);
				return ((range_count * 32) + (offset_count + 1));
			}
		}
	}
	xSemaphoreGive(dhcps_ip_table_semaphore);
	return 0;
}
#endif

/**
  * @brief  fill in the option field with message type of a dhcp message.
  * @param  msg_option_base_addr: the addr be filled start.
  *	    message_type: the type code you want to fill in
  * @retval the start addr of the next dhcp option.
  */
static uint8_t *add_msg_type(uint8_t *msg_option_base_addr, uint8_t message_type)
{
	uint8_t *option_start;
	msg_option_base_addr[0] = DHCP_OPTION_CODE_MSG_TYPE;
	msg_option_base_addr[1] = DHCP_OPTION_LENGTH_ONE;
	msg_option_base_addr[2] = message_type;
	option_start = msg_option_base_addr + 3;
	if (DHCP_MESSAGE_TYPE_NAK == message_type) {
		*option_start++ = DHCP_OPTION_CODE_END;
	}
	return option_start;
}


static uint8_t *fill_one_option_content(uint8_t *option_base_addr,
										uint8_t option_code, uint8_t option_length, void *copy_info)
{
	uint8_t *option_data_base_address;
	uint8_t *next_option_start_address = NULL;
	uint8_t copy_info_array[4] = {0};
	option_base_addr[0] = option_code;
	option_base_addr[1] = option_length;
	option_data_base_address = option_base_addr + 2;
	if (copy_info == NULL) {
		copy_info = copy_info_array;
	}
	switch (option_length) {
	case DHCP_OPTION_LENGTH_FOUR:
		memcpy(option_data_base_address, copy_info, DHCP_OPTION_LENGTH_FOUR);
		next_option_start_address = option_data_base_address + 4;
		break;
	case DHCP_OPTION_LENGTH_TWO:
		memcpy(option_data_base_address, copy_info, DHCP_OPTION_LENGTH_TWO);
		next_option_start_address = option_data_base_address + 2;
		break;
	case DHCP_OPTION_LENGTH_ONE:
		memcpy(option_data_base_address, copy_info, DHCP_OPTION_LENGTH_ONE);
		next_option_start_address = option_data_base_address + 1;
		break;
	}

	return next_option_start_address;
}

/**
  * @brief  fill in the needed content of the dhcp offer message.
  * @param  optptr  the addr which the tail of dhcp magic field.
  * @retval	0, add ok
  *			-1, add fail
  */
static int8_t add_offer_options(uint8_t *option_start_address)
{
	// Total minimum len = 6+6+6+6+6+6+4+3+1 = 44
	uint8_t *temp_option_addr = option_start_address;
	int max_addable_option_len = dhcp_message_total_options_lenth - 4 - 3;	// -magic-type

	if (option_start_address == NULL) {
		goto ERROR;
	}

	/* add DHCP options 1.
	The subnet mask option specifies the client's subnet mask */
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(option_start_address, DHCP_OPTION_CODE_SUBNET_MASK,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcps_local_mask);
	} else {
		goto ERROR;
	}

	/* add DHCP options 3 (i.e router(gateway)). The time server option
	specifies a list of RFC 868 [6] time servers available to the client. */
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_ROUTER,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcps_local_address);
	} else {
		goto ERROR;
	}

	/* add DHCP options 6 (i.e DNS).
	    The option specifies a list of DNS servers available to the client. */
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_DNS_SERVER,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcps_local_address);
	} else {
		goto ERROR;
	}

	/* add DHCP options 51.
	This option is used to request a lease time for the IP address. */
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_LEASE_TIME,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcp_option_lease_time);
	} else {
		goto ERROR;
	}

	/* add DHCP options 54.
	The identifier is the IP address of the selected server. */
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_SERVER_ID,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcps_local_address);
	} else {
		goto ERROR;
	}

	/* add DHCP options 28.
	This option specifies the broadcast address in use on client's subnet.*/
	if (temp_option_addr + 6 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_BROADCAST_ADDRESS,
						   DHCP_OPTION_LENGTH_FOUR, (void *)&dhcps_subnet_broadcast);
	} else {
		goto ERROR;
	}

	/* add DHCP options 26.
	This option specifies the Maximum transmission unit to use */
	if (temp_option_addr + 4 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_INTERFACE_MTU,
						   DHCP_OPTION_LENGTH_TWO, (void *) &dhcp_option_interface_mtu);//dhcp_option_interface_mtu_576);
	} else {
		goto ERROR;
	}

	/* add DHCP options 31.
	This option specifies whether or not the client should solicit routers */
	if (temp_option_addr + 3 - option_start_address <= max_addable_option_len) {
		temp_option_addr = fill_one_option_content(temp_option_addr, DHCP_OPTION_CODE_PERFORM_ROUTER_DISCOVERY,
						   DHCP_OPTION_LENGTH_ONE,	NULL);
	} else {
		goto ERROR;
	}

	// END
	if (temp_option_addr + 1 - option_start_address <= max_addable_option_len) {
		*temp_option_addr++ = DHCP_OPTION_CODE_END;
	} else {
		goto ERROR;
	}
	return 0;

ERROR:
	printf("\r\n[%s] error: add options fail !!", __func__);
	return -1;
}


/**
  * @brief  fill in common content of a dhcp message.
  * @param  m the pointer which point to the dhcp message store in.
  * @retval None.
  */
static void dhcps_initialize_message(struct dhcp_msg *dhcp_message_repository)
{

	dhcp_message_repository->op = DHCP_MESSAGE_OP_REPLY;
	dhcp_message_repository->htype = DHCP_MESSAGE_HTYPE;
	dhcp_message_repository->hlen = DHCP_MESSAGE_HLEN;
	dhcp_message_repository->hops = 0;
	memcpy((char *)dhcp_recorded_xid, (char *) dhcp_message_repository->xid,
		   sizeof(dhcp_message_repository->xid));
	dhcp_message_repository->secs = 0;

	memcpy((char *)dhcp_message_repository->yiaddr,
		   (char *)&dhcps_allocated_client_address,
		   sizeof(dhcp_message_repository->yiaddr));

	memset((char *)dhcp_message_repository->ciaddr, 0,
		   sizeof(dhcp_message_repository->ciaddr));
	memset((char *)dhcp_message_repository->siaddr, 0,
		   sizeof(dhcp_message_repository->siaddr));
	memset((char *)dhcp_message_repository->giaddr, 0,
		   sizeof(dhcp_message_repository->giaddr));
	memset((char *)dhcp_message_repository->sname,  0,
		   sizeof(dhcp_message_repository->sname));
	memset((char *)dhcp_message_repository->file,   0,
		   sizeof(dhcp_message_repository->file));
	memset((char *)dhcp_message_repository->options, 0,
		   dhcp_message_total_options_lenth);
	memcpy((char *)dhcp_message_repository->options, (char *)dhcp_magic_cookie,
		   sizeof(dhcp_magic_cookie));
}

/**
  * @brief  init and fill in  the needed content of dhcp offer message.
  * @param  packet_buffer packet buffer for UDP.
  * @retval None.
  */
static void dhcps_send_offer(struct pbuf *packet_buffer)
{
	uint8_t temp_ip = 0;
	struct pbuf *newly_malloc_packet_buffer = NULL;

	// newly malloc a longer pbuf for dhcp offer rather than using the short pbuf from dhcp discover
	newly_malloc_packet_buffer = pbuf_alloc(PBUF_TRANSPORT, DHCP_MSG_LEN + DHCP_OPTION_TOTAL_LENGTH_MAX, PBUF_RAM);
	if (newly_malloc_packet_buffer == NULL) {
		printf("\r\n[%s] error:  pbuf alloc fail !", __func__);
		return;
	}
	if (pbuf_copy(newly_malloc_packet_buffer, packet_buffer) != ERR_OK) {
		printf("\r\n[%s] error:  pbuf copy fail !", __func__);
		pbuf_free(newly_malloc_packet_buffer);
		return;
	}
	dhcp_message_total_options_lenth = DHCP_OPTION_TOTAL_LENGTH_MAX;
	dhcp_message_repository = (struct dhcp_msg *)newly_malloc_packet_buffer->payload;
#if (!IS_USE_FIXED_IP)
#ifdef CONFIG_DHCPS_KEPT_CLIENT_INFO
	temp_ip = check_client_request_ip(&client_request_ip, client_addr);
#endif
	/* create new client ip */
	if (temp_ip == 0) {
		temp_ip = search_next_ip();
	}
#if (debug_dhcps)
	printf("\r\n temp_ip = %d", temp_ip);
#endif
	if (temp_ip == 0) {
#if 0
		memset(&ip_table, 0, sizeof(struct table));
		mark_ip_in_table((uint8_t)ip4_addr4(&dhcps_local_address));
		printf("\r\n reset ip table!!\r\n");
#endif
		printf("\r\n No useable ip!!!!\r\n");
	}
#if LWIP_VERSION_MAJOR >= 2
	printf("\n\r[%d]DHCP assign ip = %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", \
		   (int)xTaskGetTickCount(), ip4_addr1(ip_2_ip4(&dhcps_network_id)), \
		   ip4_addr2(ip_2_ip4(&dhcps_network_id)), \
		   ip4_addr3(ip_2_ip4(&dhcps_network_id)), temp_ip, \
		   client_addr[0], client_addr[1], client_addr[2], \
		   client_addr[3], client_addr[4], client_addr[5]);
	IP4_ADDR(ip_2_ip4(&dhcps_allocated_client_address), (ip4_addr1(ip_2_ip4(&dhcps_network_id))),
			 ip4_addr2(ip_2_ip4(&dhcps_network_id)), ip4_addr3(ip_2_ip4(&dhcps_network_id)), temp_ip);
#else
	printf("\n\r[%d]DHCP assign ip = %d.%d.%d.%d, hwaddr 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\n", \
		   xTaskGetTickCount(), ip4_addr1(&dhcps_network_id), \
		   ip4_addr2(&dhcps_network_id), ip4_addr3(&dhcps_network_id), temp_ip, \
		   client_addr[0], client_addr[1], client_addr[2], \
		   client_addr[3], client_addr[4], client_addr[5]);
	IP4_ADDR(&dhcps_allocated_client_address, (ip4_addr1(&dhcps_network_id)),
			 ip4_addr2(&dhcps_network_id), ip4_addr3(&dhcps_network_id), temp_ip);
#endif
#endif
	dhcps_initialize_message(dhcp_message_repository);
	if (add_offer_options(add_msg_type(&dhcp_message_repository->options[4], DHCP_MESSAGE_TYPE_OFFER)) == 0) {
		if (dhcp_message_repository->flags == 0x0) {
			// unicast
			for (int i = 0; i < 6; i++) {
				dhcps_allocated_client_ethaddr.addr[i] = dhcp_message_repository->chaddr[i];
			}
#if ETHARP_SUPPORT_STATIC_ENTRIES
#if LWIP_VERSION_MAJOR >= 2
			etharp_add_static_entry(ip_2_ip4(&dhcps_allocated_client_address), &dhcps_allocated_client_ethaddr);
#else
			etharp_add_static_entry(&dhcps_allocated_client_address, &dhcps_allocated_client_ethaddr);
#endif
#endif
			udp_sendto_if(dhcps_pcb, newly_malloc_packet_buffer, &dhcps_allocated_client_address, DHCP_CLIENT_PORT, dhcps_netif);
#if ETHARP_SUPPORT_STATIC_ENTRIES
#if LWIP_VERSION_MAJOR >= 2
			etharp_remove_static_entry(ip_2_ip4(&dhcps_allocated_client_address));
#else
			etharp_remove_static_entry(&dhcps_allocated_client_address);
#endif
#endif
		} else
			// broadcast
		{
			udp_sendto_if(dhcps_pcb, newly_malloc_packet_buffer, &dhcps_send_broadcast_address, DHCP_CLIENT_PORT, dhcps_netif);
		}
	}

	pbuf_free(newly_malloc_packet_buffer);
}

/**
  * @brief  init and fill in  the needed content of dhcp nak message.
  * @param  packet buffer packet buffer for UDP.
  * @retval None.
  */
static void dhcps_send_nak(struct pbuf *packet_buffer)
{
	dhcp_message_repository = (struct dhcp_msg *)packet_buffer->payload;
	dhcps_initialize_message(dhcp_message_repository);
	add_msg_type(&dhcp_message_repository->options[4], DHCP_MESSAGE_TYPE_NAK);
	udp_sendto_if(dhcps_pcb, packet_buffer,
				  &dhcps_send_broadcast_address, DHCP_CLIENT_PORT, dhcps_netif);
}

/**
  * @brief  init and fill in  the needed content of dhcp ack message.
  * @param  packet buffer packet buffer for UDP.
  * @retval None.
  */
static err_t dhcps_send_ack(struct pbuf *packet_buffer)
{
	struct pbuf *newly_malloc_packet_buffer = NULL;
	err_t send_ret = ERR_OK;

	// newly malloc a longer pbuf for dhcp ack rather than using the short pbuf from dhcp request
	newly_malloc_packet_buffer = pbuf_alloc(PBUF_TRANSPORT, DHCP_MSG_LEN + DHCP_OPTION_TOTAL_LENGTH_MAX, PBUF_RAM);
	if (newly_malloc_packet_buffer == NULL) {
		printf("\r\n[%s] error:  pbuf alloc fail !", __func__);
		return ERR_BUF;
	}
	if (pbuf_copy(newly_malloc_packet_buffer, packet_buffer) != ERR_OK) {
		printf("\r\n[%s] error:  pbuf copy fail !", __func__);
		pbuf_free(newly_malloc_packet_buffer);
		return ERR_BUF;
	}
	dhcp_message_total_options_lenth = DHCP_OPTION_TOTAL_LENGTH_MAX;
	dhcp_message_repository = (struct dhcp_msg *)newly_malloc_packet_buffer->payload;
	dhcps_initialize_message(dhcp_message_repository);
	if (add_offer_options(add_msg_type(&dhcp_message_repository->options[4], DHCP_MESSAGE_TYPE_ACK)) == 0) {
		if (dhcp_message_repository->flags == 0x0) {
			// unicast
			for (int i = 0; i < 6; i++) {
				dhcps_allocated_client_ethaddr.addr[i] = dhcp_message_repository->chaddr[i];
			}
#if ETHARP_SUPPORT_STATIC_ENTRIES
#if LWIP_VERSION_MAJOR >= 2
			etharp_add_static_entry(ip_2_ip4(&dhcps_allocated_client_address), &dhcps_allocated_client_ethaddr);

#else
			etharp_add_static_entry(&dhcps_allocated_client_address, &dhcps_allocated_client_ethaddr);
#endif
#endif
			send_ret = udp_sendto_if(dhcps_pcb, newly_malloc_packet_buffer, &dhcps_allocated_client_address, DHCP_CLIENT_PORT, dhcps_netif);
#if ETHARP_SUPPORT_STATIC_ENTRIES
#if LWIP_VERSION_MAJOR >= 2
			etharp_remove_static_entry(ip_2_ip4(&dhcps_allocated_client_address));

#else
			etharp_remove_static_entry(&dhcps_allocated_client_address);
#endif
#endif
		} else
			// broadcast
		{
			send_ret = udp_sendto_if(dhcps_pcb, newly_malloc_packet_buffer, &dhcps_send_broadcast_address, DHCP_CLIENT_PORT, dhcps_netif);
		}
	}
	pbuf_free(newly_malloc_packet_buffer);
	return send_ret;
}

/**
  * @brief  according by the input message type to reflect the correspond state.
  * @param  option_message_type the input server state
  * @retval the server state which already transfer to.
  */
uint8_t dhcps_handle_state_machine_change(uint8_t option_message_type)
{
	switch (option_message_type) {
	case DHCP_MESSAGE_TYPE_DECLINE:
#if (debug_dhcps)
		printf("\r\nget message DHCP_MESSAGE_TYPE_DECLINE\n");
#endif
		dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;
		break;
	case DHCP_MESSAGE_TYPE_DISCOVER:
#if (debug_dhcps)
		printf("\r\nget message DHCP_MESSAGE_TYPE_DISCOVER\n");
#endif
		if (dhcp_server_state_machine == DHCP_SERVER_STATE_IDLE) {
			dhcp_server_state_machine = DHCP_SERVER_STATE_OFFER;
		}
		break;
	case DHCP_MESSAGE_TYPE_REQUEST:
#if (debug_dhcps)
		printf("\r\n[%d]get message DHCP_MESSAGE_TYPE_REQUEST\n", xTaskGetTickCount());
#endif
#if (!IS_USE_FIXED_IP)
#if (debug_dhcps)
		printf("\r\ndhcp_server_state_machine=%d", dhcp_server_state_machine);
#if LWIP_VERSION_MAJOR >= 2
		printf("\r\ndhcps_allocated_client_address=%d.%d.%d.%d",
			   ip4_addr1(ip_2_ip4(&dhcps_allocated_client_address)),
			   ip4_addr2(ip_2_ip4(&dhcps_allocated_client_address)),
			   ip4_addr3(ip_2_ip4(&dhcps_allocated_client_address)),
			   ip4_addr4(ip_2_ip4(&dhcps_allocated_client_address)));
		printf("\r\nclient_request_ip=%d.%d.%d.%d\n",
			   ip4_addr1(ip_2_ip4(&client_request_ip)),
			   ip4_addr2(ip_2_ip4(&client_request_ip)),
			   ip4_addr3(ip_2_ip4(&client_request_ip)),
			   ip4_addr4(ip_2_ip4(&client_request_ip)));
#else
		printf("\r\ndhcps_allocated_client_address=%d.%d.%d.%d",
			   ip4_addr1(&dhcps_allocated_client_address),
			   ip4_addr2(&dhcps_allocated_client_address),
			   ip4_addr3(&dhcps_allocated_client_address),
			   ip4_addr4(&dhcps_allocated_client_address));
		printf("\r\nclient_request_ip=%d.%d.%d.%d\n",
			   ip4_addr1(&client_request_ip),
			   ip4_addr2(&client_request_ip),
			   ip4_addr3(&client_request_ip),
			   ip4_addr4(&client_request_ip));
#endif
#endif

		// for renew
		{
#if 0
			if (((*(uint32_t *)((void *)dhcp_message_repository->ciaddr)) != 0) && ((*(uint32_t *)((void *)&client_request_ip)) == 0)) {
				memcpy(&client_request_ip, dhcp_message_repository->ciaddr, sizeof(client_request_ip));
			}
#endif
#if 1
			//void * temp_repo = dhcp_message_repository->ciaddr;
			uint32_t temp_repo;
			memcpy(&temp_repo, dhcp_message_repository->ciaddr, sizeof(uint32_t));
			void *temp_ip = &client_request_ip;
			if ((temp_repo != 0) && ((*(uint32_t *)(temp_ip)) == 0)) {
				memcpy(&client_request_ip, dhcp_message_repository->ciaddr, sizeof(client_request_ip));
			}
#endif
		}

		if (dhcp_server_state_machine == DHCP_SERVER_STATE_OFFER) {
			uint8_t ip_addr4 = check_client_direct_request_ip(&client_request_ip, client_addr);

			if (ip_addr4 > 0) {
#if LWIP_VERSION_MAJOR >= 2
				IP4_ADDR(ip_2_ip4(&dhcps_allocated_client_address), (ip4_addr1(ip_2_ip4(&dhcps_network_id))),
						 ip4_addr2(ip_2_ip4(&dhcps_network_id)), ip4_addr3(ip_2_ip4(&dhcps_network_id)), ip_addr4);
#else
				IP4_ADDR(&dhcps_allocated_client_address, (ip4_addr1(&dhcps_network_id)),
						 ip4_addr2(&dhcps_network_id), ip4_addr3(&dhcps_network_id), ip_addr4);
#endif
				dhcp_server_state_machine = DHCP_SERVER_STATE_ACK;
				break;
			}

#if LWIP_VERSION_MAJOR >= 2
			if (ip4_addr4(ip_2_ip4(&dhcps_allocated_client_address)) != 0)
#else
			if (ip4_addr4(&dhcps_allocated_client_address) != 0)
#endif
			{
				if (memcmp((void *)&dhcps_allocated_client_address, (void *)&client_request_ip, 4) == 0) {
					dhcp_server_state_machine = DHCP_SERVER_STATE_ACK;
				} else {
					dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
				}
			} else {
				dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
			}
#ifdef CONFIG_DHCPS_KEPT_CLIENT_INFO
		} else if (dhcp_server_state_machine == DHCP_SERVER_STATE_IDLE) {
			uint8_t ip_addr4 = check_client_direct_request_ip(&client_request_ip, client_addr);

			if (ip_addr4 > 0) {
#if LWIP_VERSION_MAJOR >= 2
				IP4_ADDR(ip_2_ip4(&dhcps_allocated_client_address), (ip4_addr1(ip_2_ip4(&dhcps_network_id))),
						 ip4_addr2(ip_2_ip4(&dhcps_network_id)), ip4_addr3(ip_2_ip4(&dhcps_network_id)), ip_addr4);
#else
				IP4_ADDR(&dhcps_allocated_client_address, (ip4_addr1(&dhcps_network_id)),
						 ip4_addr2(&dhcps_network_id), ip4_addr3(&dhcps_network_id), ip_addr4);
#endif
				dhcp_server_state_machine = DHCP_SERVER_STATE_ACK;
			} else {
				dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
			}
#endif
		} else {
			dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
		}
#else
		if (!(dhcp_server_state_machine == DHCP_SERVER_STATE_ACK ||
			  dhcp_server_state_machine == DHCP_SERVER_STATE_NAK)) {
			dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
		}
#endif
		break;
	case DHCP_MESSAGE_TYPE_RELEASE:
		printf("get message DHCP_MESSAGE_TYPE_RELEASE\n");
		dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;
		break;
	}

	return dhcp_server_state_machine;
}
/**
  * @brief  parse the dhcp message option part.
  * @param  optptr: the addr of the first option field.
  *         len: the total length of all option fields.
  * @retval dhcp server state.
  */
static uint8_t dhcps_handle_msg_options(uint8_t *option_start, int16_t total_option_length)
{

	int16_t option_message_type = 0;
	uint8_t *option_end = option_start + total_option_length;
	//dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;

	/* begin process the dhcp option info */
	while (option_start < option_end) {
		switch ((uint8_t)*option_start) {
		case DHCP_OPTION_CODE_MSG_TYPE:
			option_message_type = *(option_start + 2); // 2 => code(1)+lenth(1)
			break;
		case DHCP_OPTION_CODE_REQUEST_IP_ADDRESS :
#if IS_USE_FIXED_IP
			if (memcmp((char *)&dhcps_allocated_client_address,
					   (char *)option_start + 2, 4) == 0) {
				dhcp_server_state_machine = DHCP_SERVER_STATE_ACK;
			} else {
				dhcp_server_state_machine = DHCP_SERVER_STATE_NAK;
			}
#else
			memcpy((char *)&client_request_ip, (char *)option_start + 2, 4);
#endif
			break;
		}
		// calculate the options offset to get next option's base addr
		option_start += option_start[1] + 2; // optptr[1]: length value + (code(1)+ Len(1))
	}
	return dhcps_handle_state_machine_change(option_message_type);
}

/**
  * @brief  get message from buffer then check whether it is dhcp related or not.
  *         if yes , parse it more to undersatnd the client's request.
  * @param  same as recv callback function definition
  * @retval if message is dhcp related then return dhcp server state,
  *	    otherwise return 0
  */
static uint8_t dhcps_check_msg_and_handle_options(struct pbuf *packet_buffer)
{
	int dhcp_message_option_offset;
	dhcp_message_repository = (struct dhcp_msg *)packet_buffer->payload;
	dhcp_message_option_offset = ((int)dhcp_message_repository->options
								  - (int)packet_buffer->payload);
	dhcp_message_total_options_lenth = (packet_buffer->len
										- dhcp_message_option_offset);
	memcpy(client_addr, dhcp_message_repository->chaddr, 6);
	/* check the magic number,if correct parse the content of options */
	if (memcmp((char *)dhcp_message_repository->options,
			   (char *)dhcp_magic_cookie, sizeof(dhcp_magic_cookie)) == 0) {
		return dhcps_handle_msg_options(&dhcp_message_repository->options[4],
										(dhcp_message_total_options_lenth - 4));
	}

	return 0;
}


/**
  * @brief  handle imcoming dhcp message and response message to client
  * @param  same as recv callback function definition
  * @retval None
  */
static void dhcps_receive_udp_packet_handler(void *arg, struct udp_pcb *udp_pcb,
		struct pbuf *udp_packet_buffer, struct ip_addr *sender_addr, uint16_t sender_port)
{
	/* To avoid gcc warnings */
	(void) sender_addr;
	(void) arg;
	err_t send_result = ERR_OK;

	int16_t total_length_of_packet_buffer;
	struct pbuf *merged_packet_buffer = NULL;

	dhcp_message_repository = (struct dhcp_msg *)udp_packet_buffer->payload;
	if (udp_packet_buffer == NULL) {
		printf("\n\r Error!!!! System doesn't allocate any buffer \n\r");
		return;
	}
	if (sender_port == DHCP_CLIENT_PORT) {
		if (netif_get_idx(ip_current_input_netif()) == 0) {
			pbuf_free(udp_packet_buffer);
			return;
		}
		total_length_of_packet_buffer = udp_packet_buffer->tot_len;
		if (udp_packet_buffer->next != NULL) {
			merged_packet_buffer = pbuf_coalesce(udp_packet_buffer,
												 PBUF_TRANSPORT);
			if ((merged_packet_buffer->tot_len != total_length_of_packet_buffer) || (merged_packet_buffer == udp_packet_buffer)) {
				pbuf_free(merged_packet_buffer);
				return;
			}
			udp_packet_buffer = merged_packet_buffer;
		}
		switch (dhcps_check_msg_and_handle_options(udp_packet_buffer)) {
		case  DHCP_SERVER_STATE_OFFER:
#if (debug_dhcps)
			printf("%s DHCP_SERVER_STATE_OFFER\n", __func__);
#endif
			dhcps_send_offer(udp_packet_buffer);
			break;
		case DHCP_SERVER_STATE_ACK:
#if (debug_dhcps)
			printf("%s DHCP_SERVER_STATE_ACK\n", __func__);
#endif
			/*retry 20 times for alloc skb while softap TX UDP packet*/
			for (int retry = 1; retry < 20; retry++) {
				send_result = dhcps_send_ack(udp_packet_buffer);
				if (send_result != ERR_OK) {
					vTaskDelay(10);//ms
				} else {
					break;
				}
			}
#if (!IS_USE_FIXED_IP)
#if LWIP_VERSION_MAJOR >= 2
			mark_ip_in_table((uint8_t)ip4_addr4(ip_2_ip4(&dhcps_allocated_client_address)));
#else
			mark_ip_in_table((uint8_t)ip4_addr4(&dhcps_allocated_client_address));
#endif
#ifdef CONFIG_DHCPS_KEPT_CLIENT_INFO
			save_client_addr(&dhcps_allocated_client_address, client_addr);
			memset(&client_request_ip, 0, sizeof(client_request_ip));
			memset(&client_addr, 0, sizeof(client_addr));
			memset(&dhcps_allocated_client_address, 0, sizeof(dhcps_allocated_client_address));
#if (debug_dhcps)
			dump_client_table();
#endif
#endif
#endif
			dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;
			break;
		case DHCP_SERVER_STATE_NAK:
#if (debug_dhcps)
			printf("%s DHCP_SERVER_STATE_NAK\n", __func__);
#endif
			dhcps_send_nak(udp_packet_buffer);
			dhcp_server_state_machine = DHCP_SERVER_STATE_IDLE;
			break;
		case DHCP_OPTION_CODE_END:
#if (debug_dhcps)
			printf("%s DHCP_OPTION_CODE_END\n", __func__);
#endif
			break;
		}
	}

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(udp_pcb);

	/* Free the packet buffer */
	if (merged_packet_buffer != NULL) {
		pbuf_free(merged_packet_buffer);
	} else {
		pbuf_free(udp_packet_buffer);
	}
}

void dhcps_set_addr_pool(int addr_pool_set, struct ip_addr *addr_pool_start, struct ip_addr *addr_pool_end)
{
	//uint8_t *ip;
	if (addr_pool_set) {
		dhcps_addr_pool_set = 1;

		memcpy(&dhcps_addr_pool_start, addr_pool_start,
			   sizeof(struct ip_addr));
		//ip = &dhcps_addr_pool_start;
		//ip[3] = 100;
		memcpy(&dhcps_addr_pool_end, addr_pool_end,
			   sizeof(struct ip_addr));
		//ip = &dhcps_addr_pool_end;
		//ip[3] = 200;
	} else {
		dhcps_addr_pool_set = 0;
	}
}


#define DOMAIN_NAME "amebaiot.com"
const char *domain_name = DOMAIN_NAME;
uint8_t domain_name_buf[sizeof(DOMAIN_NAME) + 1];

static void dnss_receive_udp_packet_handler(
	void *arg,
	struct udp_pcb *udp_pcb,
	struct pbuf *udp_packet_buffer,
	struct ip_addr *sender_addr,
	uint16_t sender_port)
{
	/* To avoid gcc warnings */
	(void) arg;

	struct dns_hdr *hdr = (struct dns_hdr *) udp_packet_buffer->payload;

	if (memcmp((uint8_t *) hdr + sizeof(struct dns_hdr), domain_name_buf, sizeof(domain_name_buf)) == 0) {
		printf("\n\r query %s \n\r", domain_name);

		struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 20, PBUF_RAM);

		if (p) {
			struct dns_hdr *rsp_hdr = (struct dns_hdr *) p->payload;
			rsp_hdr->id = hdr->id;
			rsp_hdr->flags1 = 0x85;
			rsp_hdr->flags2 = 0x80;
			rsp_hdr->numquestions = PP_HTONS(1);
			rsp_hdr->numanswers = PP_HTONS(1);
			rsp_hdr->numauthrr = PP_HTONS(0);
			rsp_hdr->numextrarr = PP_HTONS(0);
			memcpy((uint8_t *) rsp_hdr + sizeof(struct dns_hdr), domain_name_buf, sizeof(domain_name_buf));
			*(uint16_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf)) = PP_HTONS(1);
			*(uint16_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 2) = PP_HTONS(1);
			*((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 4) = 0xc0;
			*((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 5) = 0x0c;
			*(uint16_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 6) = PP_HTONS(1);
			*(uint16_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 8) = PP_HTONS(1);
			*(uint32_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 10) = PP_HTONL(0);
			*(uint16_t *)((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 14) = PP_HTONS(4);
			memcpy((uint8_t *) rsp_hdr + sizeof(struct dns_hdr) + sizeof(domain_name_buf) + 16, (void *)&dhcps_local_address, 4);

			udp_sendto(udp_pcb, p, sender_addr, sender_port);
			pbuf_free(p);
		}
	} else {
		struct dns_hdr *dns_rsp;

		/*
			printf("\n%s: Receive DNS query,ip = %d.%d.%d.%d, port = %d(%x)\n",__func__,\
					sender_addr->addr & 0xff,\
					sender_addr->addr>>8 & 0xff,\
					sender_addr->addr>>16 & 0xff,\
					sender_addr->addr>>24 & 0xff,\
					sender_port,sender_port);
		*/

		dns_rsp = (struct dns_hdr *) udp_packet_buffer->payload;

		dns_rsp->flags1 |= 0x80; // 0x80 : Response;
		dns_rsp->flags2 = 0x05;  //0x05 : Reply code (Query Refused)

		udp_sendto(udp_pcb, udp_packet_buffer, sender_addr, sender_port);
	}

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(udp_pcb);

	/* Free the packet buffer */
	pbuf_free(udp_packet_buffer);
}


void dns_server_init(struct netif *pnetif)
{
	/* To avoid gcc warnings */
	(void) pnetif;

	// convert domain name
	memset(domain_name_buf, 0, sizeof(domain_name_buf));
	uint8_t *ptr = domain_name_buf;
	unsigned int idx, len = 0;
	for (idx = 0; idx < strlen(domain_name) + 1; idx ++) {
		if ((domain_name[idx] != '.') && (domain_name[idx] != 0)) {
			ptr[1 + len] = domain_name[idx];
			len ++;
		} else {
			*ptr = len;
			ptr += (1 + len);
			len = 0;
		}
	}

	if (dns_server_pcb != NULL) {
		udp_remove(dns_server_pcb);
		dns_server_pcb = NULL;
	}

	dns_server_pcb = udp_new();
	if (dns_server_pcb == NULL) {
		printf("\n\r Error!!!upd_new error \n\r");
		return;
	}

	udp_bind(dns_server_pcb, IP_ADDR_ANY, DNS_SERVER_PORT);
	udp_recv(dns_server_pcb, (udp_recv_fn)dnss_receive_udp_packet_handler, NULL);
}

void dns_server_deinit(void)
{
	if (dns_server_pcb != NULL) {
		udp_remove(dns_server_pcb);
		dns_server_pcb = NULL;
	}
}

/**
  * @brief  Initialize dhcp server.
  * @param  None.
  * @retval None.
  * Note, for now,we assume the server latch ip 192.168.1.1 and support dynamic
  *       or fixed IP allocation.
  */
void dhcps_init(struct netif *pnetif)
{
	uint8_t *ip;
//	printf("dhcps_init,wlan:%c\n\r",pnetif->name[1]);
#ifdef CONFIG_DHCPS_KEPT_CLIENT_INFO
	memset(&ip_table, 0, sizeof(struct table));
//	int i = 0;
//	for(i=0; i< DHCPS_MAX_CLIENT_NUM+2; i++)
//		memset(ip_table.client_mac[i], 0, 6);
//	dump_client_table();
#endif

	dhcps_netif = pnetif;

	if (dhcps_pcb != NULL) {
		udp_remove(dhcps_pcb);
		dhcps_pcb = NULL;
	}

	dhcps_pcb = udp_new();
	if (dhcps_pcb == NULL) {
		printf("\n\r Error!!!upd_new error \n\r");
		return;
	}
#if LWIP_VERSION_MAJOR >= 2
	IP4_ADDR(ip_2_ip4(&dhcps_send_broadcast_address), 255, 255, 255, 255);
#else
	IP4_ADDR(&dhcps_send_broadcast_address, 255, 255, 255, 255);
#endif
	/* get net info from net interface */

	memcpy(&dhcps_local_address, &pnetif->ip_addr,
		   sizeof(struct ip_addr));
	memcpy(&dhcps_local_mask, &pnetif->netmask,
		   sizeof(struct ip_addr));

	memcpy(&dhcps_local_gateway, &pnetif->gw,
		   sizeof(struct ip_addr));

	/* calculate the usable network ip range */
#if LWIP_VERSION_MAJOR >= 2
	ip4_addr_set_u32(ip_2_ip4(&dhcps_network_id), (ip_addr_get_ip4_u32(netif_ip_addr4(pnetif)) & (ip_addr_get_ip4_u32(netif_ip_netmask4(pnetif)))));
	ip4_addr_set_u32(ip_2_ip4(&dhcps_subnet_broadcast), (ip4_addr_get_u32(ip_2_ip4(&dhcps_network_id)) | ~(ip_addr_get_ip4_u32(netif_ip_netmask4(pnetif)))));
#if 1
	ip4_addr_set_u32(ip_2_ip4(&dhcps_owned_first_ip), htonl(ntohl(ip4_addr_get_u32(ip_2_ip4(&dhcps_network_id))) + 1));
	ip4_addr_set_u32(ip_2_ip4(&dhcps_owned_last_ip), htonl(ntohl(ip4_addr_get_u32(ip_2_ip4(&dhcps_subnet_broadcast))) - 1));
	dhcps_num_of_available_ips = (ntohl(ip4_addr_get_u32(ip_2_ip4(&dhcps_owned_last_ip))) - ntohl(ip4_addr_get_u32(ip_2_ip4(&dhcps_owned_first_ip)))) + 1;
#endif
#else
	dhcps_network_id.addr = ((pnetif->ip_addr.addr) &
							 (pnetif->netmask.addr));

	dhcps_subnet_broadcast.addr = ((dhcps_network_id.addr |
									~(pnetif->netmask.addr)));
#if 1
	dhcps_owned_first_ip.addr = htonl((ntohl(dhcps_network_id.addr) + 1));
	dhcps_owned_last_ip.addr = htonl(ntohl(dhcps_subnet_broadcast.addr) - 1);
	dhcps_num_of_available_ips = ((ntohl(dhcps_owned_last_ip.addr)
								   - ntohl(dhcps_owned_first_ip.addr)) + 1);
#endif
#endif

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
#if IP_SOF_BROADCAST
	dhcps_pcb->so_options |= SOF_BROADCAST;
#endif /* IP_SOF_BROADCAST */
#endif

#if IS_USE_FIXED_IP
#if LWIP_VERSION_MAJOR >= 2
	IP4_ADDR(ip_2_ip4(&dhcps_allocated_client_address), ip4_addr1(ip_2_ip4(&dhcps_local_address))
			 , ip4_addr2(ip_2_ip4(&dhcps_local_address)), ip4_addr3(ip_2_ip4(&dhcps_local_address)),
			 (ip4_addr4(ip_2_ip4(&dhcps_local_address))) + 1);
#else
	IP4_ADDR(&dhcps_allocated_client_address, ip4_addr1(&dhcps_local_address)
			 , ip4_addr2(&dhcps_local_address), ip4_addr3(&dhcps_local_address),
			 (ip4_addr4(&dhcps_local_address)) + 1);
#endif
#else
	if (dhcps_ip_table_semaphore != NULL) {
		vSemaphoreDelete(dhcps_ip_table_semaphore);
		dhcps_ip_table_semaphore = NULL;
	}
	dhcps_ip_table_semaphore = xSemaphoreCreateMutex();

	//dhcps_ip_table = (struct ip_table *)(pvPortMalloc(sizeof(struct ip_table)));
	memset(&ip_table, 0, sizeof(struct table));
#if LWIP_VERSION_MAJOR >= 2
	mark_ip_in_table((uint8_t)ip4_addr4(ip_2_ip4(&dhcps_local_address)));
	mark_ip_in_table((uint8_t)ip4_addr4(ip_2_ip4(&dhcps_local_gateway)));
#else
	mark_ip_in_table((uint8_t)ip4_addr4(&dhcps_local_address));
	mark_ip_in_table((uint8_t)ip4_addr4(&dhcps_local_gateway));
#endif
#if 0
	for (i = 1; i < ip4_addr4(&dhcps_local_address); i++) {
		mark_ip_in_table(i);
	}
#endif
#endif
#if LWIP_VERSION_MAJOR >= 2
	if (ip4_addr_get_u32(ip_2_ip4(&dhcps_addr_pool_start)) == 0 && ip4_addr_get_u32(ip_2_ip4(&dhcps_addr_pool_end)) == 0)
#else
	if (dhcps_addr_pool_start.addr == 0 && dhcps_addr_pool_end.addr == 0)
#endif
	{
		memcpy(&dhcps_pool_start, &dhcps_local_address, sizeof(struct ip_addr));
		ip = (uint8_t *)&dhcps_pool_start;
		ip[3] = DHCP_POOL_START;
		memcpy(&dhcps_pool_end, &dhcps_local_address, sizeof(struct ip_addr));
		ip = (uint8_t *)&dhcps_pool_end;
		ip[3] = DHCP_POOL_END;
		dhcps_set_addr_pool(1, &dhcps_pool_start, &dhcps_pool_end);
	}
	udp_bind(dhcps_pcb, IP_ADDR_ANY, DHCP_SERVER_PORT);
	udp_recv(dhcps_pcb, (udp_recv_fn)dhcps_receive_udp_packet_handler, NULL);

	//DNS server init
	dns_server_init(pnetif);

}

void dhcps_deinit(void)
{
	if (dhcps_pcb != NULL) {
		udp_remove(dhcps_pcb);
		dhcps_pcb = NULL;
	}
	if (dhcps_ip_table_semaphore != NULL) {
		vSemaphoreDelete(dhcps_ip_table_semaphore);
		dhcps_ip_table_semaphore = NULL;
	}
	//DNS server deinit
	dns_server_deinit();
}
