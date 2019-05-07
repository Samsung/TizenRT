/*---------------------------------------------------------------
 * Copyright (c) 1999,2000,2001,2002,2003
 * The Board of Trustees of the University of Illinois
 * All Rights Reserved.
 *---------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software (Iperf) and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 *
 * Redistributions of source code must retain the above
 * copyright notice, this list of conditions and
 * the following disclaimers.
 *
 *
 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimers in the documentation and/or other materials
 * provided with the distribution.
 *
 *
 * Neither the names of the University of Illinois, NCSA,
 * nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without
 * specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ________________________________________________________________
 * National Laboratory for Applied Network Research
 * National Center for Supercomputing Applications
 * University of Illinois at Urbana-Champaign
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________
 *
 * Listener.cpp
 * by Mark Gates <mgates@nlanr.net>
 * &  Ajay Tirumala <tirumala@ncsa.uiuc.edu>
 * -------------------------------------------------------------------
 * Listener sets up a socket listening on the server host. For each
 * connected socket that accept() returns, this creates a Server
 * socket and spawns a thread for it.
 *
 * Changes to the latest version. Listener will run as a daemon
 * Multicast Server is now Multi-threaded
 * -------------------------------------------------------------------
 * headers
 * uses
 *   <stdlib.h>
 *   <stdio.h>
 *   <string.h>
 *   <errno.h>
 *
 *   <sys/types.h>
 *   <unistd.h>
 *
 *   <netdb.h>
 *   <netinet/in.h>
 *   <sys/socket.h>
 * ------------------------------------------------------------------- */


#define HEADERS()
#include <arpa/inet.h>  // TizenRT
#include <limits.h>
#include "headers.h"
#include "Listener.hpp"
#include "SocketAddr.h"
#include "PerfSocket.hpp"
#include "List.h"
#include "util.h"
#include "version.h"
#include "Locale.h"
#include "SocketAddr.h"

#if (defined HAVE_SSM_MULTICAST) && (defined HAVE_NET_IF_H)
#include <net/if.h>
#endif
/* -------------------------------------------------------------------
 * Stores local hostname and socket info.
 * ------------------------------------------------------------------- */

Listener::Listener( thread_Settings *inSettings ) {

    mClients = inSettings->mThreads;
    mBuf = NULL;
    /*
     * These thread settings are stored in three places
     *
     * 1) Listener thread
     * 2) Reporter Thread (per the ReportSettings())
     * 3) Server thread
     */
    mSettings = inSettings;

    // alloc and initialize the buffer (mBuf) used for packet reads()
    mBuf = new char[((mSettings->mBufLen > SIZEOF_MAXHDRMSG) ? mSettings->mBufLen : SIZEOF_MAXHDRMSG)];
    FAIL_errno( mBuf == NULL, "No memory for buffer\n", mSettings );
    /*
     *  Perform listener threads length checks
     */
    if (isUDP(mSettings)) {
	if (!isCompat(inSettings) && (mSettings->mBufLen < SIZEOF_UDPHDRMSG)) {
	    fprintf(stderr, warn_len_too_small_peer_exchange, "Listener",  mSettings->mBufLen, SIZEOF_UDPHDRMSG);
	}
	if (mSettings->mBufLen < (int) sizeof( UDP_datagram ) ) {
	    mSettings->mBufLen = sizeof( UDP_datagram );
	    fprintf( stderr, warn_buffer_too_small, "Listener", mSettings->mBufLen );
	}
    } else {
	if (!isCompat(mSettings) && (mSettings->mBufLen < SIZEOF_TCPHDRMSG)) {
	    fprintf(stderr, warn_len_too_small_peer_exchange, "Listener", mSettings->mBufLen, SIZEOF_TCPHDRMSG);
	}
    }
    // Now hang the listening on the socket
    Listen( );

    ReportSettings( inSettings );
} // end Listener

/* -------------------------------------------------------------------
 * Delete memory (buffer).
 * ------------------------------------------------------------------- */
Listener::~Listener() {
    if ( mSettings->mSock != INVALID_SOCKET ) {
        int rc = close( mSettings->mSock );
        WARN_errno( rc == SOCKET_ERROR, "listener close" );
        mSettings->mSock = INVALID_SOCKET;
    }
    DELETE_ARRAY( mBuf );
} // end ~Listener

/* -------------------------------------------------------------------
 * Listens for connections and starts Servers to handle data.
 * For TCP, each accepted connection spawns a Server thread.
 * For UDP, handle all data in this thread for Win32 Only, otherwise
 *          spawn a new Server thread.
 * ------------------------------------------------------------------- */
void Listener::Run( void ) {
#if 0 // ifdef WIN32 removed to allow Windows to use multi-threaded UDP server
    if ( isUDP( mSettings ) && !isSingleUDP( mSettings ) ) {
        UDPSingleServer();
    } else
#else
#ifdef sun
    if ( ( isUDP( mSettings ) &&
           isMulticast( mSettings ) &&
           !isSingleUDP( mSettings ) ) ||
         isSingleUDP( mSettings ) ) {
        UDPSingleServer();
    } else
#else
    if ( isSingleUDP( mSettings ) ) {
        UDPSingleServer();
    } else
#endif
#endif
    {
        bool client = false, UDP = isUDP( mSettings ), mCount = (mSettings->mThreads != 0);
        thread_Settings *tempSettings = NULL;
        Iperf_ListEntry *exist, *listtemp;
        client_hdr* hdr = ( UDP ? (client_hdr*) (((UDP_datagram*)mBuf) + 1) :
                                  (client_hdr*) mBuf);

        if ( mSettings->mHost != NULL ) {
            client = true;
            SockAddr_remoteAddr( mSettings );
        }
        Settings_Copy( mSettings, &server );
        server->mThreadMode = kMode_Server;

        // Accept each packet,
        // If there is no existing client, then start
        // a new thread to service the new client
        // The listener runs in a single thread
        // Thread per client model is followed
        do {
            // Get a new socket
            Accept( server );
            if ( server->mSock == INVALID_SOCKET ) {
                break;
            }
            if ( sInterupted != 0 ) {
		// In the case of -r, ignore the clients alarm
		if (
#if HAVE_DECL_SIGALRM
sInterupted == SIGALRM
#else
0
#endif
		    ) {
		    sInterupted = 0;
		} else {
		    close( server->mSock );
		    break;
		}
            }

            // Reset Single Client Stuff
            if ( isSingleClient( mSettings ) && clients == NULL ) {
                mSettings->peer = server->peer;
                mClients--;
                client = true;
                // Once all the server threads exit then quit
                // Must keep going in case this client sends
                // more streams
                if ( mClients == 0 ) {
                    thread_release_nonterm( 0 );
                    mClients = 1;
                }
            }
            // Verify that it is allowed
            if ( client ) {
                if ( !SockAddr_Hostare_Equal( (sockaddr*) &mSettings->peer,
                                              (sockaddr*) &server->peer ) ) {
                    // Not allowed try again
                    close( server->mSock );
                    if ( isUDP( mSettings ) ) {
                        mSettings->mSock = -1;
                        Listen();
                    }
                    continue;
                }
            }
            // Check for exchange of test information and also determine v2.0.5 vs 2.0.10+
            if ( !isCompat( mSettings )) {
                if (ReadClientHeader(hdr) < 0) {
		    close( server->mSock );
		    continue;
		}
		// The following will set the tempSettings to NULL if
		// there is no need for the Listener to start a client
                Settings_GenerateClientSettings( server, &tempSettings, hdr );
            } else {
	        tempSettings = NULL;
	    }
            if ( tempSettings != NULL ) {
                client_init( tempSettings );
                if ( tempSettings->mMode == kTest_DualTest ) {
#ifdef HAVE_THREAD
                    server->runNow =  tempSettings;
#else
                    server->runNext = tempSettings;
#endif
                } else {
                    server->runNext =  tempSettings;
                }
            }

            // Create an entry for the connection list
            listtemp = new Iperf_ListEntry;
            memcpy(listtemp, &server->peer, sizeof(iperf_sockaddr));
            listtemp->next = NULL;

            // See if we need to do summing
            Mutex_Lock( &clients_mutex );
            exist = Iperf_hostpresent( &server->peer, clients);

            if ( exist != NULL ) {
                // Copy group ID
                listtemp->holder = exist->holder;
                server->multihdr = exist->holder;
            } else {
                Mutex_Lock( &groupCond );
                groupID--;
                listtemp->holder = InitMulti( server, groupID );
                server->multihdr = listtemp->holder;
                Mutex_Unlock( &groupCond );
            }


	    // Perform L2 setup if needed
	    if (isUDP(mSettings) && (isL2LengthCheck(mSettings) || isL2LengthCheck(server))) {
		if (L2_setup() < 0) {
		    // L2 not allowed, abort this server try
		    delete server;
		    mSettings->mSock = -1;
		}
	    }
            // Store entry in connection list
	    if (mSettings->mSock > 0)
		Iperf_pushback( listtemp, &clients );
            Mutex_Unlock( &clients_mutex );

            //
	    // Everything is now ready to start the server
	    //
#if defined(WIN32) && defined(HAVE_THREAD)
            if ( UDP && mSettings->mSock > 0) {
                // WIN32 does bad UDP handling so run single threaded
                if ( server->runNow != NULL ) {
                    thread_start( server->runNow );
                }
                server_spawn( server );
                if ( server->runNext != NULL ) {
                    thread_start( server->runNext );
                }
            } else
#endif
		{
		    if (mSettings->mSock > 0)
			thread_start( server );
		}
	    // create a new socket for the Listener thread now that server thread
	    // is handling the current one
            if ( UDP ) {
                mSettings->mSock = -1;
                Listen( );
            }

            // Prep for next connection
            if ( !isSingleClient( mSettings ) ) {
                mClients--;
            }
            Settings_Copy( mSettings, &server );
            server->mThreadMode = kMode_Server;
        } while ( !sInterupted && (!mCount || ( mCount && mClients > 0 )) );

        Settings_Destroy( server );
    }
} // end Run

/* -------------------------------------------------------------------
 * Setup a socket listening on a port.
 * For TCP, this calls bind() and listen().
 * For UDP, this just calls bind().
 * If inLocalhost is not null, bind to that address rather than the
 * wildcard server address, specifying what incoming interface to
 * accept connections on.
 * ------------------------------------------------------------------- */
void Listener::Listen( ) {
    int rc;

    SockAddr_localAddr( mSettings );

    // create an AF_INET socket for the accepts
    // for the case of L2 testing and UDP, a new AF_PACKET
    // will be created to supercede this one
    int type = (isUDP( mSettings )  ?  SOCK_DGRAM  :  SOCK_STREAM);
    int domain = (SockAddr_isIPv6( &mSettings->local ) ?
#ifdef HAVE_IPV6
		  AF_INET6
#else
		  AF_INET
#endif
		  : AF_INET);

#ifdef WIN32
    if ( SockAddr_isMulticast( &mSettings->local ) ) {
	// Multicast on Win32 requires special handling
	mSettings->mSock = WSASocket( domain, type, 0, 0, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF );
	WARN_errno( mSettings->mSock == INVALID_SOCKET, "socket" );

    } else
#endif
	{
	    mSettings->mSock = socket( domain, type, 0 );
	    WARN_errno( mSettings->mSock == INVALID_SOCKET, "socket" );
	}
    SetSocketOptions( mSettings );

    // reuse the address, so we can run if a former server was killed off
    int boolean = 1;
    Socklen_t len = sizeof(boolean);
    setsockopt( mSettings->mSock, SOL_SOCKET, SO_REUSEADDR, (char*) &boolean, len );

    // bind socket to server address
#ifdef WIN32
    if ( SockAddr_isMulticast( &mSettings->local ) ) {
	// Multicast on Win32 requires special handling
	rc = WSAJoinLeaf( mSettings->mSock, (sockaddr*) &mSettings->local, mSettings->size_local,0,0,0,0,JL_BOTH);
	WARN_errno( rc == SOCKET_ERROR, "WSAJoinLeaf (aka bind)" );
    } else
#endif
	{
	    rc = bind( mSettings->mSock, (sockaddr*) &mSettings->local, mSettings->size_local );
	    FAIL_errno( rc == SOCKET_ERROR, "bind", mSettings );
	}
    // listen for connections (TCP only).
    // use large (INT_MAX) backlog allowing multiple simultaneous connections
    if ( !isUDP( mSettings ) ) {
	rc = listen( mSettings->mSock, INT_MAX );
	WARN_errno( rc == SOCKET_ERROR, "listen" );
    }

#ifndef WIN32
    // if multicast, join the group
    if ( SockAddr_isMulticast( &mSettings->local ) ) {
#ifdef HAVE_MULTICAST
	McastJoin( );
#else
	fprintf(stderr, "Multicast not supported");
#endif // HAVE_MULTICAST
    }
#endif
} // end Listen

/* -------------------------------------------------------------------
 * Joins the multicast group or source and group (SSM S,G)
 *
 * taken from: https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.hale001/ipv6d0141001708.htm
 *
 * Multicast function	                                        IPv4	                   IPv6	                Protocol-independent
 * ==================                                           ====                       ====                 ====================
 * Level of specified option on setsockopt()/getsockopt()	IPPROTO_IP	           IPPROTO_IPV6	IPPROTO_IP or IPPROTO_IPV6
 * Join a multicast group	                                IP_ADD_MEMBERSHIP          IPV6_JOIN_GROUP	MCAST_JOIN_GROUP
 * Leave a multicast group or leave all sources of that
 *   multicast group	                                        IP_DROP_MEMBERSHIP	   IPV6_LEAVE_GROUP	MCAST_LEAVE_GROUP
 * Select outbound interface for sending multicast datagrams	IP_MULTICAST_IF	IPV6_MULTICAST_IF	NA
 * Set maximum hop count	                                IP_MULTICAST_TTL	   IPV6_MULTICAST_HOPS	NA
 * Enable multicast loopback	                                IP_MULTICAST_LOOP	   IPV6_MULTICAST_LOOP	NA
 * Join a source multicast group	                        IP_ADD_SOURCE_MEMBERSHIP   NA	                MCAST_JOIN_SOURCE_GROUP
 * Leave a source multicast group	                        IP_DROP_SOURCE_MEMBERSHIP  NA	                MCAST_LEAVE_SOURCE_GROUP
 * Block data from a source to a multicast group	        IP_BLOCK_SOURCE   	   NA	                MCAST_BLOCK_SOURCE
 * Unblock a previously blocked source for a multicast group	IP_UNBLOCK_SOURCE	   NA	                MCAST_UNBLOCK_SOURCE
 *
 *
 * Reminder:  The os will decide which version of IGMP or MLD to use.   This may be controlled by system settings, e.g.:
 *
 * [rmcmahon@lvnvdb0987:~/Code/ssm/iperf2-code] $ sysctl -a | grep mld | grep force
 * net.ipv6.conf.all.force_mld_version = 0
 * net.ipv6.conf.default.force_mld_version = 0
 * net.ipv6.conf.lo.force_mld_version = 0
 * net.ipv6.conf.eth0.force_mld_version = 0
 *
 * [rmcmahon@lvnvdb0987:~/Code/ssm/iperf2-code] $ sysctl -a | grep igmp | grep force
 * net.ipv4.conf.all.force_igmp_version = 0
 * net.ipv4.conf.default.force_igmp_version = 0
 * net.ipv4.conf.lo.force_igmp_version = 0
 * net.ipv4.conf.eth0.force_igmp_version = 0
 *
 * ------------------------------------------------------------------- */

void Listener::McastJoin( ) {
    // This is the older mulitcast join code.  Both SSM and binding the
    // an interface requires the newer socket options.  Using the older
    // code here will maintain compatiblity with previous iperf versions
    if (!isSSMMulticast(mSettings) && !mSettings->mIfrname) {
	if ( !SockAddr_isIPv6( &mSettings->local ) ) {

	    struct ip_mreq mreq;

	    memcpy( &mreq.imr_multiaddr, SockAddr_get_in_addr( &mSettings->local ),
		    sizeof(mreq.imr_multiaddr));

	    mreq.imr_interface.s_addr = htonl( INADDR_ANY );

	    int rc = setsockopt( mSettings->mSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				 (char*) &mreq, sizeof(mreq));
	    WARN_errno( rc == SOCKET_ERROR, "multicast join" );

	} else {
#ifdef HAVE_IPV6_MULTICAST
	    struct ipv6_mreq mreq;

	    memcpy( &mreq.ipv6mr_multiaddr, SockAddr_get_in6_addr( &mSettings->local ),
		    sizeof(mreq.ipv6mr_multiaddr));

	    mreq.ipv6mr_interface = 0;

	    int rc = setsockopt( mSettings->mSock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
				 (char*) &mreq, sizeof(mreq));
	    WARN_errno( rc == SOCKET_ERROR, "multicast v6 join" );
#else
	    fprintf(stderr, "Unfortunately, IPv6 multicast is not supported on this platform\n");
#endif
	}
    } else {
#ifdef HAVE_SSM_MULTICAST
	// Here it's either an SSM S,G multicast join or a *,G with an interface specifier
	// Use the newer socket options when these are specified
	socklen_t socklen = sizeof(struct sockaddr_storage);
	int iface=0;
	int rc;

#ifdef HAVE_NET_IF_H
	/* Set the interface or any */
	if (mSettings->mIfrname) {
	    iface = if_nametoindex(mSettings->mIfrname);
	    FAIL_errno(!iface, "mcast if_nametoindex",mSettings);
	} else {
	    iface = 0;
	}
#endif

        if (isIPV6(mSettings)) {
#ifdef HAVE_IPV6_MULTICAST
	    if (mSettings->mSSMMulticastStr) {
		struct group_source_req group_source_req;
		struct sockaddr_in6 *group;
		struct sockaddr_in6 *source;

		memset(&group_source_req, 0, sizeof(struct group_source_req));

		group_source_req.gsr_interface = iface;
		group=(struct sockaddr_in6*)&group_source_req.gsr_group;
		source=(struct sockaddr_in6*)&group_source_req.gsr_source;
		source->sin6_family = AF_INET6;
		group->sin6_family = AF_INET6;
		/* Set the group */
		rc=getsockname(mSettings->mSock,(struct sockaddr *)group, &socklen);
		FAIL_errno( rc == SOCKET_ERROR, "mcast join source group getsockname",mSettings );
		group->sin6_port = 0;    /* Ignored */

		/* Set the source, apply the S,G */
		rc=inet_pton(AF_INET6, mSettings->mSSMMulticastStr,&source->sin6_addr);
		FAIL_errno( rc != 1, "mcast v6 join source group pton",mSettings );
		source->sin6_port = 0;    /* Ignored */
#ifdef HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN
		source->sin6_len = group->sin6_len;
#endif
		rc = -1;
#if HAVE_DECL_MCAST_JOIN_SOURCE_GROUP
		rc = setsockopt(mSettings->mSock,IPPROTO_IPV6,MCAST_JOIN_SOURCE_GROUP, &group_source_req,
			    sizeof(group_source_req));
#endif
		FAIL_errno( rc == SOCKET_ERROR, "mcast v6 join source group",mSettings);
	    } else {
		struct group_req group_req;
		struct sockaddr_in6 *group;

		memset(&group_req, 0, sizeof(struct group_req));

		group_req.gr_interface = iface;
		group=(struct sockaddr_in6*)&group_req.gr_group;
		group->sin6_family = AF_INET6;
		/* Set the group */
		rc=getsockname(mSettings->mSock,(struct sockaddr *)group, &socklen);
		FAIL_errno( rc == SOCKET_ERROR, "mcast v6 join group getsockname",mSettings );
		group->sin6_port = 0;    /* Ignored */
		rc = -1;
#if HAVE_DECL_MCAST_JOIN_GROUP
		rc = setsockopt(mSettings->mSock,IPPROTO_IPV6,MCAST_JOIN_GROUP, &group_req,
				sizeof(group_source_req));
#endif
		FAIL_errno( rc == SOCKET_ERROR, "mcast v6 join group",mSettings);
	    }
#else
	    fprintf(stderr, "Unfortunately, IPv6 multicast is not supported on this platform\n");
#endif
	} else {
	    if (mSettings->mSSMMulticastStr) {
		struct sockaddr_in *group;
		struct sockaddr_in *source;

		// Fill out both structures because we don't which one will succeed
		// and both may need to be tried
#ifdef HAVE_STRUCT_IP_MREQ_SOURCE
		struct ip_mreq_source imr;
		memset (&imr, 0, sizeof (imr));
#endif
#ifdef HAVE_STRUCT_GROUP_SOURCE_REQ
		struct group_source_req group_source_req;
		memset(&group_source_req, 0, sizeof(struct group_source_req));
		group_source_req.gsr_interface = iface;
		group=(struct sockaddr_in*)&group_source_req.gsr_group;
		source=(struct sockaddr_in*)&group_source_req.gsr_source;
#else
		struct sockaddr_in imrgroup;
		struct sockaddr_in imrsource;
		group = &imrgroup;
		source = &imrsource;
#endif
		source->sin_family = AF_INET;
		group->sin_family = AF_INET;
		/* Set the group */
		rc=getsockname(mSettings->mSock,(struct sockaddr *)group, &socklen);
		FAIL_errno( rc == SOCKET_ERROR, "mcast join source group getsockname",mSettings );
		group->sin_port = 0;    /* Ignored */

		/* Set the source, apply the S,G */
		rc=inet_pton(AF_INET,mSettings->mSSMMulticastStr,&source->sin_addr);
		FAIL_errno(rc != 1, "mcast join source pton",mSettings );
#ifdef HAVE_STRUCT_SOCKADDR_IN_SIN_LEN
		source->sin_len = group->sin_len;
#endif
		source->sin_port = 0;    /* Ignored */
		rc = -1;

#if HAVE_DECL_MCAST_JOIN_SOURCE_GROUP
		rc = setsockopt(mSettings->mSock,IPPROTO_IP,MCAST_JOIN_SOURCE_GROUP, &group_source_req,
				sizeof(group_source_req));
#endif

#if HAVE_DECL_IP_ADD_SOURCE_MEMBERSHIP
#ifdef HAVE_STRUCT_IP_MREQ_SOURCE
		// Some operating systems will have MCAST_JOIN_SOURCE_GROUP but still fail
		// In those cases try the IP_ADD_SOURCE_MEMBERSHIP
		if (rc < 0) {
#ifdef HAVE_STRUCT_IP_MREQ_SOURCE_IMR_MULTIADDR_S_ADDR
		    imr.imr_multiaddr = ((const struct sockaddr_in *)group)->sin_addr;
		    imr.imr_sourceaddr = ((const struct sockaddr_in *)source)->sin_addr;
#else
		    // Some Android versions declare mreq_source without an s_addr
		    imr.imr_multiaddr = ((const struct sockaddr_in *)group)->sin_addr.s_addr;
		    imr.imr_sourceaddr = ((const struct sockaddr_in *)source)->sin_addr.s_addr;
#endif
		    rc = setsockopt (mSettings->mSock, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)(&imr), sizeof (imr));
		}
#endif
#endif // HAVE_DECL_IP_ADD_SOURCE_MEMBERSHIP
		FAIL_errno( rc == SOCKET_ERROR, "mcast join source group",mSettings);
	    } else {
		struct group_req group_req;
		struct sockaddr_in *group;

		memset(&group_req, 0, sizeof(struct group_req));

		group_req.gr_interface = iface;
		group=(struct sockaddr_in*)&group_req.gr_group;
		group->sin_family = AF_INET;
		/* Set the group */
		rc=getsockname(mSettings->mSock,(struct sockaddr *)group, &socklen);
		FAIL_errno( rc == SOCKET_ERROR, "mcast join group getsockname",mSettings );
		group->sin_port = 0;    /* Ignored */
		rc = -1;
#if HAVE_DECL_MCAST_JOIN_GROUP
		rc = setsockopt(mSettings->mSock,IPPROTO_IP,MCAST_JOIN_GROUP, &group_req,
				sizeof(group_source_req));
#endif
		FAIL_errno( rc == SOCKET_ERROR, "mcast join group",mSettings);
	    }
	}
#else
	fprintf(stderr, "Unfortunately, SSM is not supported on this platform\n");
	exit(-1);
#endif
    }
}
// end McastJoin

int Listener::L2_setup (void) {
#if defined(HAVE_LINUX_FILTER_H) && defined(HAVE_AF_PACKET)
    //
    //  Supporting parallel L2 UDP threads is a bit tricky.  Below are some notes as to why and the approach used.
    //
    //  The primary issues for UDP are:
    //
    //  1) We want the listener thread to hand off the flow to a server thread and not be burdened by that flow
    //  2) For -P support, the listener thread neads to detect new flows which will share the same UDP port
    //     and UDP is stateless
    //
    //  The listener thread needs to detect new traffic flows and hand them to a new server thread, and then
    //  rehang a listen/accept.  For standard iperf the "flow routing" is done using connect() per the ip quintuple.
    //  The OS will then route established connected flows to the socket descriptor handled by a server thread and won't
    //  burden the listener thread with these packets.
    //
    //  For L2 verification, we have to create a two sockets that will exist for the life of the flow.  A
    //  new packet socket (AF_PACKET) will receive L2 frames and bypasses
    //  the OS network stack.  The original AF_INET socket will still send up packets
    //  to the network stack.
    //
    //  When using packet sockets there is inherent packet duplication, the hand off to a server
    //  thread is not so straight forward as packets will continue being sent up to the listener thread
    //  (technical problem is that packet sockets do not support connect() which binds the IP quintuple as the
    //  forwarding key) Since the Listener uses recvfrom(), there is no OS mechanism to detect new flows nor
    //  to drop packets.  The listener can't listen on quintuple based connected flows because the client's source
    //  port is unknown.  Therefore the Listener thread will continue to receive packets from all established
    //  flows sharing the same dst port which will impact CPU utilization and hence performance.
    //
    //  The technique used to address this is to open an AF_PACKET socket and leave the AF_INET socket open.
    //  (This also aligns with BSD based systems)  The original AF_INET socket will remain in the (connected)
    //  state so the network stack has it's connected state.  A cBPF is then used to cause the kernel to fast drop
    //  those packets.  A cBPF is set up to drop such packets.  The test traffic will then only come over the
    //  packet (raw) socket and not the  AF_INET socket. If we were to try to close the original AF_INET socket
    //  (vs leave it open w/the fast drop cBPF) then the existing traffic will be sent up by the network stack
    //  to he Listener thread, flooding it with packets, again something we want to avoid.
    //
    //  On the packet (raw) socket itself, we do two more things to better handle performance
    //
    //  1)  Use a full quintuple cBPF allowing the kernel to filter packets (allow) per the quintuple
    //  2)  Use the packet fanout option to assign a CBPF to a socket and hence to a single server thread minimizing
    //      duplication (reduce all cBPF's filtering load)
    //
    struct sockaddr *p = (sockaddr *)&server->peer;
    struct sockaddr *l = (sockaddr *)&server->local;
    int rc = 0;

    //
    // Establish a packet (raw) socket to be used by the server thread giving it full L2 packets
    //
    struct sockaddr s;
    socklen_t len = sizeof(s);
    getpeername(mSettings->mSock, &s, &len);
    if (isIPV6(server)) {
	server->mSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IPV6));
	WARN_errno(server->mSock == INVALID_SOCKET, "ip6 packet socket (AF_PACKET)");
	server->l4offset = IPV6HDRLEN + sizeof(struct ether_header);
    } else {
	server->mSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	WARN_errno(server->mSock == INVALID_SOCKET, "ip packet socket (AF_PACKET)");
	unsetIPV6(server);
	server->l4offset = sizeof(struct iphdr) + sizeof(struct ether_header);
    }
    // Didn't get a valid socket, return now
    if (server->mSock < 0) {
	return server->mSock;
    }
    // More per thread settings based on using a packet socket
    server->l4payloadoffset = server->l4offset + sizeof(struct udphdr);
    server->recvflags = MSG_TRUNC;
    // The original AF_INET socket only exists to keep the connected state
    // in the OS for this flow. Fast drop packets there as
    // now packets will use the AF_PACKET (raw) socket
    // Also, store the original AF_INET socket descriptor so it can be
    // closed in the Server's destructor.  (Note: closing the
    // socket descriptors will also free the cBPF.)
    //
    server->mSockDrop = mSettings->mSock;
    rc = SockAddr_Drop_All_BPF(mSettings->mSock);
    WARN_errno( rc == SOCKET_ERROR, "l2 all drop bpf");

    // Now optimize packet flow up the raw socket
    // Establish the flow BPF to forward up only "connected" packets to this raw socket
    if (l->sa_family == AF_INET6) {
#ifdef HAVE_IPV6
	struct in6_addr *v6peer = SockAddr_get_in6_addr(&server->peer);
	struct in6_addr *v6local = SockAddr_get_in6_addr(&server->local);
	if (isIPV6(server)) {
	    rc = SockAddr_v6_Connect_BPF(server->mSock, v6local, v6peer, ((struct sockaddr_in6 *)(l))->sin6_port, ((struct sockaddr_in6 *)(p))->sin6_port);
	    WARN_errno( rc == SOCKET_ERROR, "l2 connect ipv6 bpf");
	} else {
	    // This is an ipv4 address in a v6 family (structure), just pull the lower 32 bits for the v4 addr
	    rc = SockAddr_v4_Connect_BPF(server->mSock, (uint32_t) v6local->s6_addr32[3], (uint32_t) v6peer->s6_addr32[3], ((struct sockaddr_in6 *)(l))->sin6_port, ((struct sockaddr_in6 *)(p))->sin6_port);
	    WARN_errno( rc == SOCKET_ERROR, "l2 v4in6 connect ip bpf");
	}
#else
	fprintf(stderr, "Unfortunately, IPv6 is not supported on this platform\n");
	return -1;
#endif /* HAVE_IPV6 */
    } else {
	rc = SockAddr_v4_Connect_BPF(server->mSock, ((struct sockaddr_in *)(l))->sin_addr.s_addr, ((struct sockaddr_in *)(p))->sin_addr.s_addr, ((struct sockaddr_in *)(l))->sin_port, ((struct sockaddr_in *)(p))->sin_port);
	WARN_errno( rc == SOCKET_ERROR, "l2 connect ip bpf");
    }
    if (rc < 0)
	return -1;
    else
	return 1;
#else
    fprintf(stderr, "Client requested --l2checks but not supported on this platform\n");
    return -1;
#endif
}
/* -------------------------------------------------------------------
 * Sets the Multicast TTL for outgoing packets.
 * ------------------------------------------------------------------- */

void Listener::McastSetTTL( int val ) {
#ifdef HAVE_MULTICAST
    if ( !SockAddr_isIPv6( &mSettings->local ) ) {
        int rc = setsockopt( mSettings->mSock, IPPROTO_IP, IP_MULTICAST_TTL,
                             (char*) &val, sizeof(val));
        WARN_errno( rc == SOCKET_ERROR, "multicast ttl" );
    }
#ifdef HAVE_IPV6_MULTICAST
      else {
        int rc = setsockopt( mSettings->mSock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
                             (char*) &val, sizeof(val));
        WARN_errno( rc == SOCKET_ERROR, "multicast ttl" );
    }
#endif
#endif
}
// end McastSetTTL

/* -------------------------------------------------------------------
 * After Listen() has setup mSock, this will block
 * until a new connection arrives or until the -t value occurs
 * ------------------------------------------------------------------- */

void Listener::Accept( thread_Settings *server ) {

    server->size_peer = sizeof(iperf_sockaddr);
    // Handles interupted accepts. Returns the newly connected socket.
    server->mSock = INVALID_SOCKET;

    bool mMode_Time = isServerModeTime( mSettings ) && !isDaemon( mSettings );
    // setup termination variables
    if ( mMode_Time ) {
	mEndTime.setnow();
	mEndTime.add( mSettings->mAmount / 100.0 );
	if (!setsock_blocking(mSettings->mSock, 0)) {
	    WARN(1, "Failed setting socket to non-blocking mode");
	}
    }

    while ( server->mSock == INVALID_SOCKET) {
	if (mMode_Time) {
	    struct timeval t1;
	    gettimeofday( &t1, NULL );
	    if (mEndTime.before( t1)) {
		break;
	    }
	    struct timeval timeout;
	    timeout.tv_sec = mSettings->mAmount / 100;
	    timeout.tv_usec = (mSettings->mAmount % 100) * 10000;
	    fd_set set;
	    FD_ZERO(&set);
	    FD_SET(mSettings->mSock, &set);
	    if (select( mSettings->mSock + 1, &set, NULL, NULL, &timeout) <= 0) {
		break;
	    }
	}
	if ( isUDP( server ) ) {
	    int rc;
	    /* ------------------------------------------------------------------------
	     * Do the equivalent of an accept() call for UDP sockets. This waits
	     * on a listening UDP socket until we get a datagram.
	     * ------------------------------------------------------------------- ----*/
	    Iperf_ListEntry *exist;
	    // Preset the server socket to INVALID, hang recvfrom on the Listener's socket
	    // The INVALID socket is used to keep the while loop going
	    server->mSock = INVALID_SOCKET;
	    rc = recvfrom( mSettings->mSock, mBuf, mSettings->mBufLen, 0,
			       (struct sockaddr*) &server->peer, &server->size_peer );
	    FAIL_errno( rc == SOCKET_ERROR, "recvfrom", mSettings );
	    if (sInterupted != 0) {
		server->mSock = INVALID_SOCKET;
	    } else {
		Mutex_Lock( &clients_mutex );
		// Handle connection for UDP sockets.
		exist = Iperf_present( &server->peer, clients);
		if ( exist == NULL ) {
		    // We have a new UDP flow so let's start the
		    // process to handle it and in a new server thread (yet to be created)
		    server->mSock = mSettings->mSock;
		    // This connect() will allow the OS to only
		    // send packets with the ip quintuple up to the server
		    // socket and, hence, to the server thread (yet to be created)
		    // This connect() routing is only supported with AF_INET or AF_INET6 sockets,
		    // e.g. AF_PACKET sockets can't do this.  We'll handle packet sockets later
		    // All UDP accepts here will use AF_INET.  This is intentional and needed
		    int rc = connect( server->mSock, (struct sockaddr*) &server->peer,
				      server->size_peer );
		    FAIL_errno( rc == SOCKET_ERROR, "connect UDP", mSettings );
		} else {
		    // This isn't a new flow so just ignore the packet
		    // and continue with the while loop
		    // printf("Debug: drop packet on sock %d\n",mSettings->mSock);
		    server->mSock = INVALID_SOCKET;
		}
		Mutex_Unlock( &clients_mutex );
	    }
	} else {
	    // accept a TCP  connection
	    server->mSock = accept( mSettings->mSock,  (sockaddr*) &server->peer, &server->size_peer );
	    if ( server->mSock == INVALID_SOCKET &&
#if WIN32
		 WSAGetLastError() == WSAEINTR
#else
		 errno == EINTR
#endif
		) {
		break;
	    }
	}
    }
    if (server->mSock != INVALID_SOCKET) {
	if (!setsock_blocking(server->mSock, 1)) {
	    WARN(1, "Failed setting socket to blocking mode");
	}
    }
    server->size_local = sizeof(iperf_sockaddr);
    getsockname( server->mSock, (sockaddr*) &server->local, &server->size_local );
} // end Accept

void Listener::UDPSingleServer( ) {

    bool client = false, UDP = isUDP( mSettings ), mCount = (mSettings->mThreads != 0);
    thread_Settings *tempSettings = NULL;
    Iperf_ListEntry *exist, *listtemp;
    int rc;
    int32_t datagramID;
    client_hdr* hdr = ( UDP ? (client_hdr*) (((UDP_datagram*)mBuf) + 1) :
                              (client_hdr*) mBuf);
    ReportStruct *reportstruct = new ReportStruct();
    FAIL_errno( reportstruct == NULL, "No memory for report structure\n", mSettings );
    bool mMode_Time = isServerModeTime( mSettings ) && !isDaemon( mSettings );
    // setup termination variables
    if ( mMode_Time ) {
	mEndTime.setnow();
	mEndTime.add( mSettings->mAmount / 100.0 );
    }

    if ( mSettings->mHost != NULL ) {
        client = true;
        SockAddr_remoteAddr( mSettings );
    }
    Settings_Copy( mSettings, &server );
    server->mThreadMode = kMode_Server;


    // Accept each packet,
    // If there is no existing client, then start
    // a new report to service the new client
    // The listener runs in a single thread
    Mutex_Lock( &clients_mutex );
    do {
        // Get next packet
        while ( sInterupted == 0) {
            server->size_peer = sizeof( iperf_sockaddr );

	    if (mMode_Time) {
		struct timeval t1;
		gettimeofday( &t1, NULL );
		if (mEndTime.before( t1)) {
		    sInterupted = 1;
		    break;
		}
		struct timeval timeout;
		timeout.tv_sec = mSettings->mAmount / 100;
		timeout.tv_usec = (mSettings->mAmount % 100) * 10000;
		fd_set set;
		FD_ZERO(&set);
		FD_SET(mSettings->mSock, &set);
		if (select( mSettings->mSock + 1, &set, NULL, NULL, &timeout) <= 0) {
		    sInterupted = 1;
		    break;
		}
	    }

            rc = recvfrom( mSettings->mSock, mBuf, mSettings->mBufLen, 0,
                           (struct sockaddr*) &server->peer, &server->size_peer );
            WARN_errno( rc == SOCKET_ERROR, "recvfrom" );
            if ( rc == SOCKET_ERROR ) {
                return;
            }


            // Handle connection for UDP sockets.
            exist = Iperf_present( &server->peer, clients);
            datagramID = ntohl( ((UDP_datagram*) mBuf)->id );
            if ( datagramID >= 0 ) {
                if ( exist != NULL ) {
                    // read the datagram ID and sentTime out of the buffer
                    reportstruct->packetID = datagramID;
                    reportstruct->sentTime.tv_sec = ntohl( ((UDP_datagram*) mBuf)->tv_sec  );
                    reportstruct->sentTime.tv_usec = ntohl( ((UDP_datagram*) mBuf)->tv_usec );

                    reportstruct->packetLen = rc;
                    gettimeofday( &(reportstruct->packetTime), NULL );

                    ReportPacket( exist->server->reporthdr, reportstruct );
                } else {
                    Mutex_Lock( &groupCond );
                    groupID--;
                    server->mSock = -groupID;
                    Mutex_Unlock( &groupCond );
                    server->size_local = sizeof(iperf_sockaddr);
                    getsockname( mSettings->mSock, (sockaddr*) &server->local, \
                                 &server->size_local );
                    break;
                }
            } else {
                if ( exist != NULL ) {
                    // read the datagram ID and sentTime out of the buffer
                    reportstruct->packetID = -datagramID;
                    reportstruct->sentTime.tv_sec = ntohl( ((UDP_datagram*) mBuf)->tv_sec  );
                    reportstruct->sentTime.tv_usec = ntohl( ((UDP_datagram*) mBuf)->tv_usec );

                    reportstruct->packetLen = rc;
                    gettimeofday( &(reportstruct->packetTime), NULL );

                    ReportPacket( exist->server->reporthdr, reportstruct );
                    // stop timing
                    gettimeofday( &(reportstruct->packetTime), NULL );
                    CloseReport( exist->server->reporthdr, reportstruct );

                    if (rc > (int) (sizeof(UDP_datagram) + sizeof(server_hdr))) {
                        UDP_datagram *UDP_Hdr;
                        server_hdr *hdr;

                        UDP_Hdr = (UDP_datagram*) mBuf;
                        Transfer_Info *stats = GetReport( exist->server->reporthdr );
                        hdr = (server_hdr*) (UDP_Hdr+1);

                        hdr->base.flags        = htonl( HEADER_VERSION1 );
#ifdef HAVE_INT64_T
			hdr->base.total_len1   = htonl( (long) (stats->TotalLen >> 32) );
#else
			hdr->base.total_len1   = htonl(0x0);
#endif
                        hdr->base.total_len2   = htonl( (long) (stats->TotalLen & 0xFFFFFFFF) );
                        hdr->base.stop_sec     = htonl( (long) stats->endTime );
                        hdr->base.stop_usec    = htonl( (long)((stats->endTime - (long)stats->endTime) \
                                                          * rMillion));
                        hdr->base.error_cnt    = htonl( stats->cntError );
                        hdr->base.outorder_cnt = htonl( stats->cntOutofOrder );
                        hdr->base.datagrams    = htonl( stats->cntDatagrams );
                        hdr->base.jitter1      = htonl( (long) stats->jitter );
                        hdr->base.jitter2      = htonl( (long) ((stats->jitter - (long)stats->jitter) \
                                                           * rMillion) );
                    }
                    EndReport( exist->server->reporthdr );
                    exist->server->reporthdr = NULL;
                    Iperf_delete( &(exist->server->peer), &clients );
                } else if (rc > (int) (sizeof(UDP_datagram) + sizeof(server_hdr))) {
                    UDP_datagram *UDP_Hdr;
                    server_hdr *hdr;

                    UDP_Hdr = (UDP_datagram*) mBuf;
                    hdr = (server_hdr*) (UDP_Hdr+1);
                    hdr->base.flags = htonl( 0 );
                }
                sendto( mSettings->mSock, mBuf, mSettings->mBufLen, 0, \
                        (struct sockaddr*) &server->peer, server->size_peer);
            }
        }
        if ( server->mSock == INVALID_SOCKET ) {
            break;
        }
        if ( sInterupted != 0 ) {
            close( server->mSock );
            break;
        }
        // Reset Single Client Stuff
        if ( isSingleClient( mSettings ) && clients == NULL ) {
            mSettings->peer = server->peer;
            mClients--;
            client = true;
            // Once all the server threads exit then quit
            // Must keep going in case this client sends
            // more streams
            if ( mClients == 0 ) {
                thread_release_nonterm( 0 );
                mClients = 1;
            }
        }
        // Verify that it is allowed
        if ( client ) {
            if ( !SockAddr_Hostare_Equal( (sockaddr*) &mSettings->peer, \
                                          (sockaddr*) &server->peer ) ) {
                // Not allowed try again
                connect( mSettings->mSock,
                         (sockaddr*) &server->peer,
                         server->size_peer );
                close( mSettings->mSock );
                mSettings->mSock = -1;
                Listen( );
                continue;
            }
        }

        // Create an entry for the connection list
        listtemp = new Iperf_ListEntry;
        memcpy(listtemp, &server->peer, sizeof(iperf_sockaddr));
        listtemp->server = server;
        listtemp->next = NULL;

        // See if we need to do summing
        exist = Iperf_hostpresent( &server->peer, clients);

        if ( exist != NULL ) {
            // Copy group ID
            listtemp->holder = exist->holder;
            server->multihdr = exist->holder;
        } else {
            Mutex_Lock( &groupCond );
            groupID--;
            listtemp->holder = InitMulti( server, groupID );
            server->multihdr = listtemp->holder;
            Mutex_Unlock( &groupCond );
        }

        // Store entry in connection list
        Iperf_pushback( listtemp, &clients );

        if ( !isCompat( mSettings ) && !isMulticast( mSettings ) ) {
            Settings_GenerateClientSettings( server, &tempSettings,
                                              hdr );
        } else {
	    tempSettings = NULL;
	}

        if ( tempSettings != NULL ) {
            client_init( tempSettings );
            if ( tempSettings->mMode == kTest_DualTest ) {
#ifdef HAVE_THREAD
                thread_start( tempSettings );
#else
                server->runNext = tempSettings;
#endif
            } else {
                server->runNext =  tempSettings;
            }
        }
        InitReport(server);
        PostFirstReport(server);

        // Prep for next connection
        if ( !isSingleClient( mSettings ) ) {
            mClients--;
        }
        Settings_Copy( mSettings, &server );
        server->mThreadMode = kMode_Server;
    } while ( !sInterupted && (!mCount || ( mCount && mClients > 0 )) );
    Mutex_Unlock( &clients_mutex );

    Settings_Destroy( server );
}

int Listener::ReadClientHeader(client_hdr *hdr ) {
    uint32_t flags = 0;
    int testflags = 0;
    if (isUDP(mSettings)) {
	flags = ntohl(hdr->base.flags);
	if (flags & HEADER_SEQNO64B) {
	  setSeqNo64b(server);
	}
	if ((flags & HEADER_UDPTESTS) != 0) {
	    testflags = ntohs(hdr->udp.testflags);
	    // Handle stateless flags
#ifdef HAVE_ISOCHRONOUS
	    if ((testflags & HEADER_UDP_ISOCH) != 0) {
		setIsochronous(server);
	    }
#endif
	    if ((testflags & HEADER_L2ETHPIPV6) != 0) {
		setIPV6(server);
	    } else {
		unsetIPV6(server);
	    }
	    if ((testflags & HEADER_L2LENCHECK) != 0) {
		setL2LengthCheck(server);
	    }
	    reporter_peerversion(server, ntohl(hdr->udp.version_u), ntohl(hdr->udp.version_l));
	}
    } else {
	int n, len;
	char *p = (char *)hdr;
	len = sizeof(client_hdr_v1);
	int sorcvtimer = 0;
	// sorcvtimer units microseconds convert to that
	// minterval double, units seconds
	// mAmount integer, units 10 milliseconds
	// divide by two so timeout is 1/2 the interval
	if (mSettings->mInterval) {
	    sorcvtimer = (int) (mSettings->mInterval * 1e6) / 2;
	} else if (isModeTime(mSettings)) {
	    sorcvtimer = (mSettings->mAmount * 1000) / 2;
	}
	if (sorcvtimer > 0) {
#ifdef WIN32
	    // Windows SO_RCVTIMEO uses ms
	    DWORD timeout = (double) sorcvtimer / 1e3;
#else
	    struct timeval timeout;
	    timeout.tv_sec = sorcvtimer / 1000000;
	    timeout.tv_usec = sorcvtimer % 1000000;
#endif // WIN32
	    if (setsockopt( mSettings->mSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0 ) {
		WARN_errno( mSettings->mSock == SO_RCVTIMEO, "socket" );
	    }
	}
	// Read the headers but don't pull them from the queue in order to
	// preserve server thread accounting, i.e. these exchanges will
	// be part of traffic accounting
	if ((n = recvn(server->mSock, p, 4, MSG_PEEK)) == 4) {
	    flags = ntohl(hdr->base.flags);
	    len=0;
	    if ((flags & HEADER_EXTEND) != 0) {
		len = sizeof(client_hdr);
	    } else if ((flags & HEADER_VERSION1) != 0) {
		len = sizeof(client_hdr_v1);
	    } else if ((flags & HEADER_TIMESTAMP) != 0 ) {
		setTripTime(server);
	    }
	    if (len && ((n = recvn(server->mSock, p, len, MSG_PEEK)) != len)) {
		return -1;
	    }
	}
    }
    // Handle flags that require an ack back to the client
    if ((flags & HEADER_EXTEND) != 0 ) {
	reporter_peerversion(server, ntohl(hdr->extend.version_u), ntohl(hdr->extend.version_l));
	//  Extended header successfully read. Ack the client with our version info now
	if (!isMulticast(mSettings)) {
	    ClientHeaderAck();
	}
    }
    return 0;
}

int Listener::ClientHeaderAck(void) {
    client_hdr_ack ack;
    int sotimer = 0;
    int optflag;
    ack.typelen.type  = htonl(CLIENTHDRACK);
    ack.typelen.length = htonl(sizeof(client_hdr_ack));
    ack.flags = 0;
    ack.reserved1 = 0;
    ack.reserved2 = 0;
    ack.version_u = htonl(IPERF_VERSION_MAJORHEX);
    ack.version_l = htonl(IPERF_VERSION_MINORHEX);
    int rc = 1;
    // This is a version 2.0.10 or greater client
    // write back to the client so it knows the server
    // version
    if (!isUDP(server)) {
	// sotimer units microseconds convert
	if (server->mInterval) {
	    sotimer = (int) ((server->mInterval * 1e6) / 4);
	} else if (isModeTime(server)) {
	    sotimer = (int) ((server->mAmount * 1000) / 4);
	}
	if (sotimer > HDRXACKMAX) {
	    sotimer = HDRXACKMAX;
	} else if (sotimer < HDRXACKMIN) {
	    sotimer = HDRXACKMIN;
	}
#ifdef WIN32
	// Windows SO_RCVTIMEO uses ms
	DWORD timeout = (double) sotimer / 1e3;
#else
	struct timeval timeout;
	timeout.tv_sec = sotimer / 1000000;
	timeout.tv_usec = sotimer % 1000000;
#endif
	if ((rc = setsockopt( server->mSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout))) < 0 ) {
	    WARN_errno( rc < 0, "setsockopt SO_SNDTIMEO");
	}
	optflag=1;
	// Disable Nagle to reduce latency of this intial message
	if ((rc = setsockopt( server->mSock, IPPROTO_TCP, TCP_NODELAY, (char *)&optflag, sizeof(int))) < 0 ) {
	    WARN_errno(rc < 0, "tcpnodelay" );
	}
    }
    if (isUDP(server) && (server->mBufLen < (int) sizeof(client_hdr_ack))) {
        fprintf( stderr, warn_len_too_small_peer_exchange, "Server", server->mBufLen, sizeof(client_hdr_ack));
    }
    if ((rc = send(server->mSock, (const char*)&ack, sizeof(client_hdr_ack),0)) < 0) {
	WARN_errno( rc <= 0, "send_ack" );
	rc = 0;
    }
    // Re-nable Nagle
    optflag=0;
    if (!isUDP( server ) && (rc = setsockopt( server->mSock, IPPROTO_TCP, TCP_NODELAY, (char *)&optflag, sizeof(int))) < 0 ) {
	WARN_errno(rc < 0, "tcpnodelay" );
    }
    return rc;
}
