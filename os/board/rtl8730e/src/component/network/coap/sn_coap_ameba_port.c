#include "sn_coap_ameba_port.h"


///////////////////////////////////////////ramdom function///////////////////////////////////////////
uint16_t randLIB_get_16bit(){

	unsigned char rnd[2];
	rtw_get_random_bytes(rnd,2);

	return ( ((uint16_t)rnd[0] << 8) | ((uint16_t)rnd[1]) );
}

///////////////////////////////////////////memory allocation///////////////////////////////////////////

void* coap_malloc(uint16_t size) {
	return pvPortMalloc(size);
}

void coap_free(void* addr) {
	vPortFree(addr);
}

void* coap_calloc(uint32_t size){

	if(size == 0)
	{
		return NULL;
	}

	void *p = pvPortMalloc(size);

	if(p){
		memset(p,0,size);
	}

	return p;
}

///////////////////////////////////////////init CoAP protocol///////////////////////////////////////////
struct coap_s * coap_protocol_init(	uint8_t (*coap_tx_callback)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *),
									int8_t (*coap_rx_callback)(sn_coap_hdr_s *, sn_nsdl_addr_s *, void *)){
	return sn_coap_protocol_init(&coap_malloc, &coap_free, coap_tx_callback, coap_rx_callback);
}

///////////////////////////////////////////send and receive///////////////////////////////////////////

int coap_sock_open(){

	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	#if COAP_DBG_EN
	if(socket_fd < 0){
		tr_debug("ERROR: open socket");
	}
	#endif

	return socket_fd;
}

int coap_sock_close(int socket_fd){
	int r = close(socket_fd);

	#if COAP_DBG_EN
	if (r < 0)
	{
		tr_debug("ERROR: close socket");
	}
	#endif

	return r;
}

int coap_sendto(char *to_address, uint16_t port, int socket, sn_coap_hdr_s *coap_msg_hdr){

	// Calculate the CoAP message size, allocate the memory and build the message

	uint16_t message_len = sn_coap_builder_calc_needed_packet_data_size(coap_msg_hdr);

	uint8_t* message_ptr = (uint8_t*)coap_malloc(message_len);

	sn_coap_builder(message_ptr, coap_msg_hdr);

	// Send CoAP message

	struct sockaddr_in internet_address;

	memset((char *) &internet_address,0,sizeof(internet_address));

	internet_address.sin_family = AF_INET;
	internet_address.sin_port = htons(port);
	//internet_address.sin_addr.s_addr = htonl(INADDR_ANY);

	struct hostent *host_entry;

	host_entry = gethostbyname(to_address);
	if(!host_entry){
		tr_debug("DNS failed!");
		coap_free(message_ptr);
		return -1;
	}
	memcpy((void *) &internet_address.sin_addr, (void *) host_entry->h_addr, host_entry->h_length);

	int n = sendto(socket, message_ptr, message_len, 0, (struct sockaddr *)&internet_address, sizeof(internet_address));

	#if COAP_DBG_EN
	if(n<0){
		tr_debug("ERROR: send message");
	}else{
		tr_debug("Send '%s' %d bytes",to_address,n);
	}
	#endif

	coap_free(message_ptr);

	return n;
}

int coap_recv(int socket, struct sockaddr_in *from_address, void *buf, uint32_t buf_size){

	#if COAP_DBG_EN
	if(buf == NULL){
		tr_debug("ERROR: receive buffer == NULL");
		return -1;
	}
	#endif

	uint32_t addr_len = sizeof(struct sockaddr_in);

	return recvfrom(socket, buf, buf_size, 0, (struct sockaddr *) from_address, (socklen_t *)&addr_len);
}

/////////////////////////////////////////print header///////////////////////////////////////////
void coap_print_hdr(sn_coap_hdr_s* parsed_hdr){

	char code[] = "0.00";
	int cat = (parsed_hdr->msg_code & 0xe0) >> 5;
	int dd = parsed_hdr->msg_code & 0x1f;
	sprintf(code, "%d.%2x", cat, dd);

	printf("\ttoken_len:            %d\n", parsed_hdr->token_len);
	if(parsed_hdr->token_ptr)
		printf("\ttoken_ptr:            %s\n", parsed_hdr->token_ptr);
	else
		printf("\ttoken_ptr:            (null)\n");
	printf("\tcoap_status:          %d ", parsed_hdr->coap_status);
	switch(parsed_hdr->coap_status){

		case COAP_STATUS_OK:
		printf("COAP_STATUS_OK\n");
		break;

		case COAP_STATUS_PARSER_ERROR_IN_HEADER:
		printf("COAP_STATUS_PARSER_ERROR_IN_HEADER\n");
		break;

		case COAP_STATUS_PARSER_DUPLICATED_MSG:
		printf("COAP_STATUS_PARSER_DUPLICATED_MSG\n");
		break;

		case COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING:
		printf("COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING\n");
		break;

		case COAP_STATUS_PARSER_BLOCKWISE_ACK:
		printf("COAP_STATUS_PARSER_BLOCKWISE_ACK\n");
		break;

		case COAP_STATUS_PARSER_BLOCKWISE_MSG_REJECTED:
		printf("COAP_STATUS_PARSER_BLOCKWISE_MSG_REJECTED\n");
		break;

		case COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED:
		printf("COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED\n");
		break;

		case COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED:
		printf("COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED\n");
		break;

		default:
		printf("\n");
	}

	printf("\tmsg_code:             %s ", code);
	switch(parsed_hdr->msg_code){

		case COAP_MSG_CODE_EMPTY:
		printf("COAP_MSG_CODE_EMPTY\n");
		break;

		case COAP_MSG_CODE_REQUEST_GET:
		printf("COAP_MSG_CODE_REQUEST_GET\n");
		break;

		case COAP_MSG_CODE_REQUEST_POST:
		printf("COAP_MSG_CODE_REQUEST_POST\n");
		break;

		case COAP_MSG_CODE_REQUEST_PUT:
		printf("COAP_MSG_CODE_REQUEST_PUT\n");
		break;

		case COAP_MSG_CODE_REQUEST_DELETE:
		printf("COAP_MSG_CODE_REQUEST_DELETE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_CREATED:
		printf("COAP_MSG_CODE_RESPONSE_CREATED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_DELETED:
		printf("COAP_MSG_CODE_RESPONSE_DELETED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_VALID:
		printf("COAP_MSG_CODE_RESPONSE_VALID\n");
		break;

		case COAP_MSG_CODE_RESPONSE_CHANGED:
		printf("COAP_MSG_CODE_RESPONSE_CHANGED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_CONTENT:
		printf("COAP_MSG_CODE_RESPONSE_CONTENT\n");
		break;

		case COAP_MSG_CODE_RESPONSE_CONTINUE:
		printf("COAP_MSG_CODE_RESPONSE_CONTINUE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_BAD_REQUEST:
		printf("COAP_MSG_CODE_RESPONSE_BAD_REQUEST\n");
		break;

		case COAP_MSG_CODE_RESPONSE_UNAUTHORIZED:
		printf("COAP_MSG_CODE_RESPONSE_UNAUTHORIZED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_BAD_OPTION:
		printf("COAP_MSG_CODE_RESPONSE_BAD_OPTION\n");
		break;

		case COAP_MSG_CODE_RESPONSE_FORBIDDEN:
		printf("COAP_MSG_CODE_RESPONSE_FORBIDDEN\n");
		break;

		case COAP_MSG_CODE_RESPONSE_NOT_FOUND:
		printf("COAP_MSG_CODE_RESPONSE_NOT_FOUND\n");
		break;

		case COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED:
		printf("COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE:
		printf("COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE:
		printf("COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED:
		printf("COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE:
		printf("COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT:
		printf("COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT\n");
		break;

		case COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR:
		printf("COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR\n");
		break;

		case COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED:
		printf("COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED\n");
		break;

		case COAP_MSG_CODE_RESPONSE_BAD_GATEWAY:
		printf("COAP_MSG_CODE_RESPONSE_BAD_GATEWAY\n");
		break;

		case COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE:
		printf("COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE\n");
		break;

		case COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT:
		printf("COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT\n");
		break;

		case COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED:
		printf("COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED\n");
		break;

		default:
		printf("\n");
	}

	printf("\tmsg_type:             %2x ", parsed_hdr->msg_type);
	switch(parsed_hdr->msg_type){

		case COAP_MSG_TYPE_CONFIRMABLE:
		printf("COAP_MSG_TYPE_CONFIRMABLE\n");
		break;

		case COAP_MSG_TYPE_NON_CONFIRMABLE:
		printf("COAP_MSG_TYPE_NON_CONFIRMABLE\n");
		break;

		case COAP_MSG_TYPE_ACKNOWLEDGEMENT:
		printf("COAP_MSG_TYPE_ACKNOWLEDGEMENT\n");
		break;

		case COAP_MSG_TYPE_RESET:
		printf("COAP_MSG_TYPE_RESET\n");
		break;

		default:
		printf("\n");
	}


	printf("\tcontent_format:       %d ", parsed_hdr->content_format);
	switch(parsed_hdr->content_format){
		case COAP_CT_NONE:
		printf("COAP_CT_NONE\n");
		break;

		case COAP_CT_TEXT_PLAIN:
		printf("COAP_CT_TEXT_PLAIN\n");
		break;

		case COAP_CT_LINK_FORMAT:
		printf("COAP_CT_LINK_FORMAT\n");
		break;

		case COAP_CT_XML:
		printf("COAP_CT_XML\n");
		break;

		case COAP_CT_OCTET_STREAM:
		printf("COAP_CT_OCTET_STREAM\n");
		break;

		case COAP_CT_EXI:
		printf("COAP_CT_EXI\n");
		break;

		case COAP_CT_JSON:
		printf("COAP_CT_JSON\n");
		break;

		case COAP_CT__MAX:
		printf("COAP_CT__MAX\n");
		break;

		default:
		printf("\n");
	}

	printf("\tmsg_id:               %d\n", parsed_hdr->msg_id);
	printf("\turi_path_len:         %d\n", parsed_hdr->uri_path_len);	
	if(parsed_hdr->uri_path_ptr)
		printf("\turi_path_ptr:         %s\n", parsed_hdr->uri_path_ptr);
	else
		printf("\turi_path_ptr:         (null)\n");	
		printf("\tpayload_len:          %d\n", parsed_hdr->payload_len);		
	if(parsed_hdr->payload_ptr)
		printf("\tpayload_ptr:          %s\n", parsed_hdr->payload_ptr);
	else
		printf("\tpayload_ptr:          (null)\n");
		//printf("\toptions_list_ptr:     %p\n", parsed_hdr->options_list_ptr);
}