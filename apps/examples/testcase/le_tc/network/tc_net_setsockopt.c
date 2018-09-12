/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file tc_net_setsockopt.c
/// @brief Test Case Example for setsockopt() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"

/**
* @fn                   :tc_net_setsockopt_multicast_ttl_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_ttl_p(int s)
{
  int ret = -1;
  u8_t optval = 1;
  ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &optval, sizeof(optval));
  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_ttl_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_ttl_p: PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_multicast_ttl_loop_own_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_ttl_loop_own_p(int s)
{
  int ret = -1;
  u8_t loop = 1;
  ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_ttl_loop_own_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_ttl_loop_own_p: PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_multicast_ttl_loop_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_ttl_loop_p(int s)
{
  int ret = -1;
  u8_t loop = 250;
  ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_ttl_loop_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_ttl_loop_p: PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_multicast_if_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor, ipv4 addr
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_if_p(int s, const char *my_ipv4addr)
{
  int ret = -1;
  struct sockaddr_in interface_addr;
  interface_addr.sin_addr.s_addr = inet_addr(my_ipv4addr);

  ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, &interface_addr, sizeof(interface_addr));
  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_if_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_if_p: PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_multicast_add_group_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor, group ipv4 addr
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_add_group_p(int s,
    const char *group_ipv4addr)
{
  int ret = -1;
  struct ip_mreq mreq;

  mreq.imr_multiaddr.s_addr = inet_addr(group_ipv4addr);
  //mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  ret = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_add_group_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_add_group_p: PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_multicast_drop_group_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor, group ipv4 addr
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_multicast_drop_group_p(int s,
    const char *group_ipv4addr)
{
  int ret = -1;
  struct ip_mreq mreq;

  mreq.imr_multiaddr.s_addr = inet_addr(group_ipv4addr);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  ret = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));

  if (ret < 0) {
    printf("tc_net_setsockopt_multicast_add_group_p: SETSOCKOPT Multicast failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_multicast_add_group_p: PASS\n");
  nw_total_pass++;
}
*/

/**
   * @testcase		   :tc_net_setsockopt_multicast_tcp_nodelay_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_multicast_tcp_nodelay_p(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &option, sizeof(option));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_multicast_tcp_keepalive_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_multicast_tcp_keepalive_p(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPALIVE, &option, sizeof(option));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_multicast_tcp_keepidle_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_multicast_tcp_keepidle_p(int s)
{
	int ret = -1;
	int keepidle = 30;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_multicast_tcp_keepintvl_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_multicast_tcp_keepintvl_p(int s)
{
	int ret = -1;
	int keepintvl = 120;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_multicast_tcp_keepcnt_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_multicast_tcp_keepcnt_p(int s)
{
	int ret = -1;
	int keepcnt = 5;

	ret = setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_ip_tos_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_ip_tos_p(int s)
{
	int ret = -1;
	int optval = 1;

	ret = setsockopt(s, IPPROTO_IP, IP_TOS, &optval, sizeof(optval));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
* @fn                   :tc_net_setsockopt_ip_ttl_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_ip_ttl_p(int s)
{
  int ret = -1;
  int optval = 1;

  ret = setsockopt(s, IPPROTO_IP, IP_TTL, &optval, sizeof(optval));
  if (ret < 0) {
    printf("tc_net_setsockopt_ip_ttl_p FAIL: setopt SO_IP_TTL failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_ip_ttl_p PASS\n");
  nw_total_pass++;
}
*/
/**
* @fn                   :tc_net_setsockopt_no_check_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_no_check_p(int s)
{
  int ret = -1;
  int optval = 1;

  ret = setsockopt(s, SOL_SOCKET, SO_NO_CHECK, &optval, sizeof(optval));
  if (ret < 0) {
    printf("tc_net_setsockopt_no_check_p FAIL: setopt SO_NO_CHECK failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_no_check_p PASS\n");
  nw_total_pass++;
}
*/

/**
   * @testcase		   :tc_net_setsockopt_broadcast_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_broadcast_p(int s)
{
	int ret = 0;
	int optval = 1;				//broadcast value

	ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof optval);

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
* @fn                   :tc_net_setsockopt_sndtimo_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_sndtimo_p(int s)
{
  int ret = -1;
  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  ret = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
  if (ret < 0) {
    printf("tc_net_setsockopt_sndtimo_p FAIL: setopt SO_SNDTIMO failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_sndtimo_p PASS\n");
  nw_total_pass++;
}
*/

/**
   * @testcase		   :tc_net_setsockopt_rcvtimo_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_rcvtimo_p(int s)
{
	int ret = -1;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_reuseaddr_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_reuseaddr_p(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof option);

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

#if 0
/**
   * @testcase		   :tc_net_setsockopt_reuseport_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_reuseport_p(int s)
{
	int ret = -1;
	int option = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (char *)&option, sizeof option);

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}
#endif

/**
* @fn                   :tc_net_setsockopt_rcvbuf_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void
*/
/*
static void tc_net_setsockopt_rcvbuf_p(int s)
{
  int ret = -1;
  int size = 1000;

  ret = setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, size);
  if (ret < 0) {
    printf("tc_net_setsockopt_rcvbuf_p FAIL: setopt SO_RCVBUF failure \n");
    nw_total_fail++;
    RETURN_ERR;
  }

  printf("tc_net_setsockopt_rcvbuf_p PASS\n");
  nw_total_pass++;
}
*/

/**
   * @testcase		   :tc_net_setsockopt_keepalive_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_keepalive_p(int s)
{
	int ret = -1;
	int optval = 1;

	ret = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval);

	TC_ASSERT_GEQ("setsockopt", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
* @fn                   :tc_net_setsockopt_ipproto_ip_ip_pktinfo_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void

static void tc_net_setsockopt_ipproto_ip_ip_pktinfo_p(int s)
{
    int ret = -1;
    int optval=1;

    ret = setsockopt(s, IPPROTO_IP, IP_PKTINFO, &optval, sizeof(optval));;
    if (ret < 0) {
		printf("tc_net_setsockopt_ipproto_ip_ip_pktinfo_p FAIL: setopt KEEPALIVE failure\n");
		nw_total_fail++;
		RETURN_ERR;
    }

    printf("tc_net_setsockopt_ipproto_ip_ip_pktinfo_p PASS\n");
    nw_total_pass++;
}*/

/**
* @fn                   :tc_net_setsockopt_sol_socket_timestamping_p
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void

static void tc_net_setsockopt_sol_socket_timestamping_p(fd)
{
    int ret = -1;
    int optval=1;

    ret = setsockopt(fd, SOL_SOCKET, SO_TIMESTAMPING, &optval, sizeof(optval));
    if (ret < 0) {
		printf("tc_net_setsockopt_sol_socket_timestamping_p FAIL: setopt KEEPALIVE failure\n");
		nw_total_fail++;
		RETURN_ERR;
    }

    printf("tc_net_setsockopt_sol_socket_timestamping_p PASS\n");
    nw_total_pass++;
}*/

/**
   * @testcase		   :tc_net_setsockopt_bad_filedesc_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_setsockopt_bad_filedesc_n(void)
{
	int ret = -1;
	// int optval=1;

	ret = setsockopt(-1, SOL_SOCKET, 0, 0, 0);

	TC_ASSERT_EQ("setsockopt", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_invalid_filedesc_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_setsockopt_invalid_filedesc_n(void)
{
	int ret = -1;
	// int optval=1;

	ret = setsockopt(0, SOL_SOCKET, 0, 0, 0);

	TC_ASSERT_EQ("setsockopt", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_setsockopt_invalid_level_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :setsockopt()
   * @precondition	   :socket file descriptor
   * @postcondition	   :
   */
static void tc_net_setsockopt_invalid_level_n(int s)
{
	int ret = -1;
	//int optval=1;

	ret = setsockopt(s, -1, 0, 0, 0);

	TC_ASSERT_EQ("setsockopt", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
* @fn                   :tc_net_setsockopt_invalid_opt_name_n
* @brief                :
* @Scenario             :
* @API's covered        :setsockopt()
* Preconditions         :socket file descriptor
* Postconditions        :
* @return               :void

static void tc_net_setsockopt_invalid_opt_name_n(int s)
{
    int ret = -1;
    //int optval=1;

    ret = setsockopt(s, SOL_SOCKET, SCTP_AUTOCLOSE, 0, 0);
    if (ret != -1) {
		printf("tc_net_setsockopt_invalid_opt_name_n FAIL: setopt KEEPALIVE failure\n");
		nw_total_fail++;
		RETURN_ERR;
    }

    printf("tc_net_setsockopt_invalid_opt_name_n PASS\n");
    nw_total_pass++;
}*/

/****************************************************************************
 * Name: setsockopt()
 ****************************************************************************/

void net_setsockopt_main(void)
{
	int fd = -1;
	//char *mip = "239.255.255.255";
	//char *ip = "192.168.1.100";

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	//tc_net_setsockopt_invalid_opt_name_n(fd);
	tc_net_setsockopt_invalid_level_n(fd);
	tc_net_setsockopt_invalid_filedesc_n();
	tc_net_setsockopt_bad_filedesc_n();
	//tc_net_setsockopt_sol_socket_timestamping_p(fd);
	//tc_net_setsockopt_ipproto_ip_ip_pktinfo_p(fd);
	tc_net_setsockopt_keepalive_p(fd);
	//tc_net_setsockopt_rcvbuf_p(fd);
	//tc_net_setsockopt_reuseport_p(fd);
	tc_net_setsockopt_reuseaddr_p(fd);
	tc_net_setsockopt_rcvtimo_p(fd);
	//tc_net_setsockopt_sndtimo_p(fd);
	tc_net_setsockopt_broadcast_p(fd);
	//tc_net_setsockopt_no_check_p(fd);
	//tc_net_setsockopt_ip_ttl_p(fd);
	tc_net_setsockopt_ip_tos_p(fd);
	tc_net_setsockopt_multicast_tcp_keepcnt_p(fd);
	tc_net_setsockopt_multicast_tcp_keepintvl_p(fd);
	tc_net_setsockopt_multicast_tcp_keepidle_p(fd);
	tc_net_setsockopt_multicast_tcp_keepalive_p(fd);
	tc_net_setsockopt_multicast_tcp_nodelay_p(fd);
	//tc_net_setsockopt_multicast_drop_group_p(fd,mip);
	//tc_net_setsockopt_multicast_add_group_p(fd,mip);
	//tc_net_setsockopt_multicast_if_p(fd,ip);
	//tc_net_setsockopt_multicast_ttl_loop_p(fd);
	//tc_net_setsockopt_multicast_ttl_loop_own_p(fd);
	//tc_net_setsockopt_multicast_ttl_p(fd);

	close(fd);
	return;
}
