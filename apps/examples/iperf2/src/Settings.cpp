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
 * Settings.cpp
 * by Mark Gates <mgates@nlanr.net>
 * & Ajay Tirumala <tirumala@ncsa.uiuc.edu>
 * -------------------------------------------------------------------
 * Stores and parses the initial values for all the global variables.
 * -------------------------------------------------------------------
 * headers
 * uses
 *   <stdlib.h>
 *   <stdio.h>
 *   <string.h>
 *
 *   <unistd.h>
 * ------------------------------------------------------------------- */

#define HEADERS()

#include "headers.h"
#include "Settings.hpp"
#include "Locale.h"
#include "SocketAddr.h"
#include "util.h"
#include "version.h"
#include "gnu_getopt.h"
#ifdef HAVE_ISOCHRONOUS
#include "isochronous.hpp"
#include "pdfs.h"
#endif

static int reversetest = 0;
static int udphistogram = 0;
static int l2checks = 0;
static int incrdstip = 0;
static int txstarttime = 0;
static int fqrate = 0;
static int triptime = 0;
#ifdef HAVE_ISOCHRONOUS
static int burstipg = 0;
static int burstipg_set = 0;
static int isochronous = 0;
#endif

void Settings_Interpret( char option, const char *optarg, thread_Settings *mExtSettings );
// apply compound settings after the command line has been fully parsed
void Settings_ModalOptions( thread_Settings *mExtSettings );


/* -------------------------------------------------------------------
 * command line options
 *
 * The option struct essentially maps a long option name (--foobar)
 * or environment variable ($FOOBAR) to its short option char (f).
 * ------------------------------------------------------------------- */
#define LONG_OPTIONS()

const struct option long_options[] =
{
{"singleclient",     no_argument, NULL, '1'},
{"bandwidth",  required_argument, NULL, 'b'},
{"client",     required_argument, NULL, 'c'},
{"dualtest",         no_argument, NULL, 'd'},
{"enhancedreports",   no_argument, NULL, 'e'},
{"format",     required_argument, NULL, 'f'},
{"help",             no_argument, NULL, 'h'},
{"interval",   required_argument, NULL, 'i'},
{"len",        required_argument, NULL, 'l'},
{"print_mss",        no_argument, NULL, 'm'},
{"num",        required_argument, NULL, 'n'},
{"output",     required_argument, NULL, 'o'},
{"port",       required_argument, NULL, 'p'},
{"tradeoff",         no_argument, NULL, 'r'},
{"server",           no_argument, NULL, 's'},
{"time",       required_argument, NULL, 't'},
{"udp",              no_argument, NULL, 'u'},
{"version",          no_argument, NULL, 'v'},
{"window",     required_argument, NULL, 'w'},
{"reportexclude", required_argument, NULL, 'x'},
{"reportstyle",required_argument, NULL, 'y'},
{"realtime",         no_argument, NULL, 'z'},

// more esoteric options
{"bind",       required_argument, NULL, 'B'},
{"compatibility",    no_argument, NULL, 'C'},
{"daemon",           no_argument, NULL, 'D'},
{"file_input", required_argument, NULL, 'F'},
{"ssm-host", required_argument, NULL, 'H'},
{"stdin_input",      no_argument, NULL, 'I'},
{"mss",        required_argument, NULL, 'M'},
{"nodelay",          no_argument, NULL, 'N'},
{"listenport", required_argument, NULL, 'L'},
{"parallel",   required_argument, NULL, 'P'},
#ifdef WIN32
{"remove",           no_argument, NULL, 'R'},
#else
{"reverse",          no_argument, NULL, 'R'},
#endif
{"tos",        required_argument, NULL, 'S'},
{"ttl",        required_argument, NULL, 'T'},
{"single_udp",       no_argument, NULL, 'U'},
{"ipv6_domain",      no_argument, NULL, 'V'},
{"suggest_win_size", no_argument, NULL, 'W'},
{"peer-detect",      no_argument, NULL, 'X'},
{"linux-congestion", required_argument, NULL, 'Z'},
{"udp-histogram", optional_argument, &udphistogram, 1},
{"l2checks", no_argument, &l2checks, 1},
{"incr-dstip", no_argument, &incrdstip, 1},
{"txstart-time", required_argument, &txstarttime, 1},
{"fq-rate", required_argument, &fqrate, 1},
{"trip-time", no_argument, &triptime, 1},
#ifdef HAVE_ISOCHRONOUS
{"ipg", required_argument, &burstipg, 1},
{"isochronous", optional_argument, &isochronous, 1},
#endif
#ifdef WIN32
{"reverse", no_argument, &reversetest, 1},
#endif
{0, 0, 0, 0}
};

#define ENV_OPTIONS()

const struct option env_options[] =
{
{"IPERF_IPV6_DOMAIN",      no_argument, NULL, 'V'},
{"IPERF_SINGLECLIENT",     no_argument, NULL, '1'},
{"IPERF_BANDWIDTH",  required_argument, NULL, 'b'},
{"IPERF_CLIENT",     required_argument, NULL, 'c'},
{"IPERF_DUALTEST",         no_argument, NULL, 'd'},
{"IPERF_ENHANCEDREPORTS",  no_argument, NULL, 'e'},
{"IPERF_FORMAT",     required_argument, NULL, 'f'},
// skip help
{"IPERF_INTERVAL",   required_argument, NULL, 'i'},
{"IPERF_LEN",        required_argument, NULL, 'l'},
{"IPERF_PRINT_MSS",        no_argument, NULL, 'm'},
{"IPERF_NUM",        required_argument, NULL, 'n'},
{"IPERF_PORT",       required_argument, NULL, 'p'},
{"IPERF_TRADEOFF",         no_argument, NULL, 'r'},
{"IPERF_SERVER",           no_argument, NULL, 's'},
{"IPERF_TIME",       required_argument, NULL, 't'},
{"IPERF_UDP",              no_argument, NULL, 'u'},
// skip version
{"TCP_WINDOW_SIZE",  required_argument, NULL, 'w'},
{"IPERF_REPORTEXCLUDE", required_argument, NULL, 'x'},
{"IPERF_REPORTSTYLE",required_argument, NULL, 'y'},

// more esoteric options
{"IPERF_BIND",       required_argument, NULL, 'B'},
{"IPERF_COMPAT",           no_argument, NULL, 'C'},
{"IPERF_DAEMON",           no_argument, NULL, 'D'},
{"IPERF_FILE_INPUT", required_argument, NULL, 'F'},
{"IPERF_STDIN_INPUT",      no_argument, NULL, 'I'},
{"IPERF_MSS",        required_argument, NULL, 'M'},
{"IPERF_NODELAY",          no_argument, NULL, 'N'},
{"IPERF_LISTENPORT", required_argument, NULL, 'L'},
{"IPERF_PARALLEL",   required_argument, NULL, 'P'},
{"IPERF_TOS",        required_argument, NULL, 'S'},
{"IPERF_TTL",        required_argument, NULL, 'T'},
{"IPERF_SINGLE_UDP",       no_argument, NULL, 'U'},
{"IPERF_SUGGEST_WIN_SIZE", required_argument, NULL, 'W'},
{"IPERF_PEER_DETECT", required_argument, NULL, 'X'},
{"IPERF_CONGESTION_CONTROL",  required_argument, NULL, 'Z'},
{0, 0, 0, 0}
};

#define SHORT_OPTIONS()

const char short_options[] = "1b:c:def:hi:l:mn:o:p:rst:uvw:x:y:zB:CDF:H:IL:M:NP:RS:T:UVWXZ:";

/* -------------------------------------------------------------------
 * defaults
 * ------------------------------------------------------------------- */
#define DEFAULTS()

const long kDefault_UDPRate = 1024 * 1024; // -u  if set, 1 Mbit/sec
const int  kDefault_UDPBufLen = 1470;      // -u  if set, read/write 1470 bytes
// v4: 1470 bytes UDP payload will fill one and only one ethernet datagram (IPv4 overhead is 20 bytes)
const int  kDefault_UDPBufLenV6 = 1450;      // -u  if set, read/write 1470 bytes
// v6: 1450 bytes UDP payload will fill one and only one ethernet datagram (IPv6 overhead is 40 bytes)
const int kDefault_TCPBufLen = 128 * 1024; // TCP default read/write size
/* -------------------------------------------------------------------
 * Initialize all settings to defaults.
 * ------------------------------------------------------------------- */

void Settings_Initialize( thread_Settings *main ) {
    // Everything defaults to zero or NULL with
    // this memset. Only need to set non-zero values
    // below.
    memset( main, 0, sizeof(thread_Settings) );
    main->mSock = INVALID_SOCKET;
    main->mReportMode = kReport_Default;
    // option, defaults
    main->flags         = FLAG_MODETIME | FLAG_STDOUT; // Default time and stdout
    main->flags_extend  = 0x0;           // Default all extend flags to off
    //main->mUDPRate      = 0;           // -b,  offered (or rate limited) load (both UDP and TCP)
    main->mUDPRateUnits = kRate_BW;
    //main->mHost         = NULL;        // -c,  none, required for client
    main->mMode         = kTest_Normal;  // -d,  mMode == kTest_DualTest
    main->mFormat       = 'a';           // -f,  adaptive bits
    // skip help                         // -h,
    //main->mBufLenSet  = false;         // -l,
    main->mBufLen       = kDefault_TCPBufLen; // -l,  Default to TCP read/write size
    //main->mInterval     = 0;           // -i,  ie. no periodic bw reports
    //main->mPrintMSS   = false;         // -m,  don't print MSS
    // mAmount is time also              // -n,  N/A
    //main->mOutputFileName = NULL;      // -o,  filename
    main->mPort         = 5001;          // -p,  ttcp port
    main->mBindPort     = 0;             // -B,  default port for bind
    // mMode    = kTest_Normal;          // -r,  mMode == kTest_TradeOff
    main->mThreadMode   = kMode_Unknown; // -s,  or -c, none
    main->mAmount       = 1000;          // -t,  10 seconds
    // mUDPRate > 0 means UDP            // -u,  N/A, see kDefault_UDPRate
    // skip version                      // -v,
    //main->mTCPWin       = 0;           // -w,  ie. don't set window

    // more esoteric options
    //main->mLocalhost    = NULL;        // -B,  none
    //main->mCompat     = false;         // -C,  run in Compatibility mode
    //main->mDaemon     = false;         // -D,  run as a daemon
    //main->mFileInput  = false;         // -F,
    //main->mFileName     = NULL;        // -F,  filename
    //main->mStdin      = false;         // -I,  default not stdin
    //main->mListenPort   = 0;           // -L,  listen port
    //main->mMSS          = 0;           // -M,  ie. don't set MSS
    //main->mNodelay    = false;         // -N,  don't set nodelay
    //main->mThreads      = 0;           // -P,
    //main->mRemoveService = false;      // -R,
    //main->mTOS          = 0;           // -S,  ie. don't set type of service
    main->mTTL          = -1;            // -T,  link-local TTL
    //main->mDomain     = kMode_IPv4;    // -V,
    //main->mSuggestWin = false;         // -W,  Suggest the window size.

} // end Settings

void Settings_Copy( thread_Settings *from, thread_Settings **into ) {
    *into = new thread_Settings;
    memcpy( *into, from, sizeof(thread_Settings) );
    if ( from->mHost != NULL ) {
        (*into)->mHost = new char[ strlen(from->mHost) + 1];
        strcpy( (*into)->mHost, from->mHost );
    }
    if ( from->mOutputFileName != NULL ) {
        (*into)->mOutputFileName = new char[ strlen(from->mOutputFileName) + 1];
        strcpy( (*into)->mOutputFileName, from->mOutputFileName );
    }
    if ( from->mLocalhost != NULL ) {
        (*into)->mLocalhost = new char[ strlen(from->mLocalhost) + 1];
        strcpy( (*into)->mLocalhost, from->mLocalhost );
    }
    if ( from->mFileName != NULL ) {
        (*into)->mFileName = new char[ strlen(from->mFileName) + 1];
        strcpy( (*into)->mFileName, from->mFileName );
    }
    if ( from->mUDPHistogramStr != NULL ) {
	(*into)->mUDPHistogramStr = new char[ strlen(from->mUDPHistogramStr) + 1];
        strcpy( (*into)->mUDPHistogramStr, from->mUDPHistogramStr );
    }
    if ( from->mSSMMulticastStr != NULL ) {
	(*into)->mSSMMulticastStr = new char[ strlen(from->mSSMMulticastStr) + 1];
        strcpy( (*into)->mSSMMulticastStr, from->mSSMMulticastStr );
    }
    if ( from->mIfrname != NULL ) {
	(*into)->mIfrname = new char[ strlen(from->mIfrname) + 1];
        strcpy( (*into)->mIfrname, from->mIfrname );
    }
#ifdef HAVE_ISOCHRONOUS
    if ( from->mIsochronousStr != NULL ) {
	(*into)->mIsochronousStr = new char[ strlen(from->mIsochronousStr) + 1];
        strcpy( (*into)->mIsochronousStr, from->mIsochronousStr );
    }
#endif
    // Zero out certain entries
    (*into)->mTID = thread_zeroid();
    (*into)->runNext = NULL;
    (*into)->runNow = NULL;
#if defined(HAVE_LINUX_FILTER_H) && defined(HAVE_AF_PACKET)
    (*into)->mSockDrop = INVALID_SOCKET;
#endif
}

/* -------------------------------------------------------------------
 * Delete memory: Does not clean up open file pointers or ptr_parents
 * ------------------------------------------------------------------- */

void Settings_Destroy( thread_Settings *mSettings) {
    DELETE_ARRAY( mSettings->mHost      );
    DELETE_ARRAY( mSettings->mLocalhost );
    DELETE_ARRAY( mSettings->mFileName  );
    DELETE_ARRAY( mSettings->mOutputFileName );
    DELETE_ARRAY( mSettings->mUDPHistogramStr );
    DELETE_ARRAY( mSettings->mSSMMulticastStr);
    FREE_ARRAY( mSettings->mIfrname);
#ifdef HAVE_ISOCHRONOUS
    DELETE_ARRAY( mSettings->mIsochronousStr );
#endif
    DELETE_PTR( mSettings );
} // end ~Settings

/* -------------------------------------------------------------------
 * Parses settings from user's environment variables.
 * ------------------------------------------------------------------- */
void Settings_ParseEnvironment( thread_Settings *mSettings ) {
    char *theVariable;

    int i = 0;
    while ( env_options[i].name != NULL ) {
        theVariable = getenv( env_options[i].name );
        if ( theVariable != NULL ) {
            Settings_Interpret( env_options[i].val, theVariable, mSettings );
        }
        i++;
    }
} // end ParseEnvironment

/* -------------------------------------------------------------------
 * Parse settings from app's command line.
 * ------------------------------------------------------------------- */

void Settings_ParseCommandLine( int argc, char **argv, thread_Settings *mSettings ) {
    int option;
    gnu_opterr = 1; // Fail on an unrecognized command line option
    while ( (option =
             gnu_getopt_long( argc, argv, short_options,
                              long_options, NULL )) != EOF ) {
        Settings_Interpret( option, gnu_optarg, mSettings );
    }

    for ( int i = gnu_optind; i < argc; i++ ) {
        fprintf( stderr, "%s: ignoring extra argument -- %s\n", argv[0], argv[i] );
    }
    // Determine the modal or compound settings now that the full command line has been parsed
    Settings_ModalOptions( mSettings );

} // end ParseCommandLine

/* -------------------------------------------------------------------
 * Interpret individual options, either from the command line
 * or from environment variables.
 * ------------------------------------------------------------------- */

void Settings_Interpret( char option, const char *optarg, thread_Settings *mExtSettings ) {
    char *results;
    switch ( option ) {
        case '1': // Single Client
            setSingleClient( mExtSettings );
            break;

        case 'b': // UDP bandwidth
	    {
		char *tmp= new char [strlen(optarg) + 1];
		strcpy(tmp, optarg);
		// scan for PPS units, just look for 'p' as that's good enough
		if ((((results = strtok(tmp, "p")) != NULL) && strcmp(results,optarg)) \
		    || (((results = strtok(tmp, "P")) != NULL)  && strcmp(results,optarg))) {
		    mExtSettings->mUDPRateUnits = kRate_PPS;
		    mExtSettings->mUDPRate = byte_atoi(results);
		} else {
		    mExtSettings->mUDPRateUnits = kRate_BW;
		    mExtSettings->mUDPRate = byte_atoi(optarg);
		    if (((results = strtok(tmp, ",")) != NULL) && strcmp(results,optarg)) {
			setVaryLoad(mExtSettings);
			mExtSettings->mVariance = byte_atoi(optarg);
		    }
		}
		delete [] tmp;
	    }
	    setBWSet( mExtSettings );
	    break;
        case 'c': // client mode w/ server host to connect to
            mExtSettings->mHost = new char[ strlen( optarg ) + 1 ];
            strcpy( mExtSettings->mHost, optarg );

            if ( mExtSettings->mThreadMode == kMode_Unknown ) {
                mExtSettings->mThreadMode = kMode_Client;
                mExtSettings->mThreads = 1;
            }
            break;

        case 'd': // Dual-test Mode
            if ( mExtSettings->mThreadMode != kMode_Client ) {
                fprintf( stderr, warn_invalid_server_option, option );
                break;
            }
            if ( isCompat( mExtSettings ) ) {
                fprintf( stderr, warn_invalid_compatibility_option, option );
            }
#ifdef HAVE_THREAD
            mExtSettings->mMode = kTest_DualTest;
#else
            fprintf( stderr, warn_invalid_single_threaded, option );
            mExtSettings->mMode = kTest_TradeOff;
#endif
            break;
        case 'e': // Use enhanced reports
            setEnhanced( mExtSettings );
            break;
        case 'f': // format to print in
            mExtSettings->mFormat = (*optarg);
            break;

        case 'h': // print help and exit
	    fprintf(stderr, "%s", usage_long1);
            fprintf(stderr, "%s", usage_long2);
            exit(1);
            break;

        case 'i': // specify interval between periodic bw reports
	    char *end;
	    mExtSettings->mInterval = strtof( optarg, &end );
	    if (*end != '\0') {
		fprintf (stderr, "Invalid value of '%s' for -i interval\n", optarg);
	    } else {
	        if ( mExtSettings->mInterval < SMALLEST_INTERVAL ) {
		    mExtSettings->mInterval = SMALLEST_INTERVAL;
#ifndef HAVE_FASTSAMPLING
		    fprintf (stderr, report_interval_small, mExtSettings->mInterval);
#endif
	        }
		if ( mExtSettings->mInterval < 0.5 ) {
		    setEnhanced( mExtSettings );
		}
	    }
            break;

        case 'l': // length of each buffer
            mExtSettings->mBufLen = byte_atoi( optarg );
            setBuflenSet( mExtSettings );
            break;

        case 'm': // print TCP MSS
            setPrintMSS( mExtSettings );
            break;

        case 'n': // bytes of data
            // amount mode (instead of time mode)
            unsetModeTime( mExtSettings );
            mExtSettings->mAmount = byte_atoi( optarg );
            break;

        case 'o' : // output the report and other messages into the file
            unsetSTDOUT( mExtSettings );
            mExtSettings->mOutputFileName = new char[strlen(optarg)+1];
            strcpy( mExtSettings->mOutputFileName, optarg);
            break;

        case 'p': // server port
            mExtSettings->mPort = atoi( optarg );
            break;

        case 'r': // test mode tradeoff
            if ( mExtSettings->mThreadMode != kMode_Client ) {
                fprintf( stderr, warn_invalid_server_option, option );
                break;
            }
            if ( isCompat( mExtSettings ) ) {
                fprintf( stderr, warn_invalid_compatibility_option, option );
            }

            mExtSettings->mMode = kTest_TradeOff;
            break;

        case 's': // server mode
            if ( mExtSettings->mThreadMode != kMode_Unknown ) {
                fprintf( stderr, warn_invalid_client_option, option );
                break;
            }

            mExtSettings->mThreadMode = kMode_Listener;
            break;

        case 't': // seconds to run the client, server, listener
            // time mode (instead of amount mode), units is 10 ms
            setModeTime( mExtSettings );
            setServerModeTime( mExtSettings );
            mExtSettings->mAmount = (int) (atof( optarg ) * 100.0);
            break;

        case 'u': // UDP instead of TCP
	    setUDP( mExtSettings );
            break;

        case 'v': // print version and exit
	    fprintf( stderr, "%s", version );
            exit(1);
            break;

        case 'w': // TCP window size (socket buffer size)
            mExtSettings->mTCPWin = byte_atoi(optarg);

            if ( mExtSettings->mTCPWin < 2048 ) {
                fprintf( stderr, warn_window_small, mExtSettings->mTCPWin );
            }
            break;

        case 'x': // Limit Reports
            while ( *optarg != '\0' ) {
                switch ( *optarg ) {
                    case 's':
                    case 'S':
                        setNoSettReport( mExtSettings );
                        break;
                    case 'c':
                    case 'C':
                        setNoConnReport( mExtSettings );
                        break;
                    case 'd':
                    case 'D':
                        setNoDataReport( mExtSettings );
                        break;
                    case 'v':
                    case 'V':
                        setNoServReport( mExtSettings );
                        break;
                    case 'm':
                    case 'M':
                        setNoMultReport( mExtSettings );
                        break;
                    default:
                        fprintf(stderr, warn_invalid_report, *optarg);
                }
                optarg++;
            }
            break;
#ifdef HAVE_SCHED_SETSCHEDULER
        case 'z': // Use realtime scheduling
	    setRealtime( mExtSettings );
            break;
#endif

        case 'y': // Reporting Style
            switch ( *optarg ) {
                case 'c':
                case 'C':
                    mExtSettings->mReportMode = kReport_CSV;
                    break;
                default:
                    fprintf( stderr, warn_invalid_report_style, optarg );
            }
            break;


            // more esoteric options
        case 'B': // specify bind address
	    if (mExtSettings->mLocalhost == NULL) {
		mExtSettings->mLocalhost = new char[ strlen( optarg ) + 1 ];
		strcpy( mExtSettings->mLocalhost, optarg );
	    }
            break;

        case 'C': // Run in Compatibility Mode, i.e. no intial nor final header messaging
            setCompat( mExtSettings );
            if ( mExtSettings->mMode != kTest_Normal ) {
                fprintf( stderr, warn_invalid_compatibility_option,
                        ( mExtSettings->mMode == kTest_DualTest ?
                          'd' : 'r' ) );
                mExtSettings->mMode = kTest_Normal;
            }
            break;

        case 'D': // Run as a daemon
            setDaemon( mExtSettings );
            break;

        case 'F' : // Get the input for the data stream from a file
            if ( mExtSettings->mThreadMode != kMode_Client ) {
                fprintf( stderr, warn_invalid_server_option, option );
                break;
            }

            setFileInput( mExtSettings );
            mExtSettings->mFileName = new char[strlen(optarg)+1];
            strcpy( mExtSettings->mFileName, optarg);
            break;

        case 'H' : // Get the SSM host (or Source per the S,G)
            if ( mExtSettings->mThreadMode == kMode_Client ) {
                fprintf( stderr, warn_invalid_client_option, option );
                break;
            }
            mExtSettings->mSSMMulticastStr = new char[strlen(optarg)+1];
            strcpy( mExtSettings->mSSMMulticastStr, optarg);
            setSSMMulticast( mExtSettings );
            break;

        case 'I' : // Set the stdin as the input source
            if ( mExtSettings->mThreadMode != kMode_Client ) {
                fprintf( stderr, warn_invalid_server_option, option );
                break;
            }

            setFileInput( mExtSettings );
            setSTDIN( mExtSettings );
            mExtSettings->mFileName = new char[strlen("<stdin>")+1];
            strcpy( mExtSettings->mFileName,"<stdin>");
            break;

        case 'L': // Listen Port (bidirectional testing client-side)
            if ( mExtSettings->mThreadMode != kMode_Client ) {
                fprintf( stderr, warn_invalid_server_option, option );
                break;
            }

            mExtSettings->mListenPort = atoi( optarg );
            break;

        case 'M': // specify TCP MSS (maximum segment size)
            mExtSettings->mMSS = byte_atoi( optarg );
            break;

        case 'N': // specify TCP nodelay option (disable Jacobson's Algorithm)
            setNoDelay( mExtSettings );
            break;

        case 'P': // number of client threads
#ifdef HAVE_THREAD
            mExtSettings->mThreads = atoi( optarg );
#else
            if ( mExtSettings->mThreadMode != kMode_Server ) {
                fprintf( stderr, warn_invalid_single_threaded, option );
            } else {
                mExtSettings->mThreads = atoi( optarg );
            }
#endif
            break;
#ifdef WIN32
        case 'R':
            setRemoveService( mExtSettings );
            break;
#else
        case 'R':
	    fprintf( stderr, "The --reverse option is currently not supported\n");
	    exit(1);
	    setReverse(mExtSettings);
            break;
#endif

        case 'S': // IP type-of-service
            // TODO use a function that understands base-2
            // the zero base here allows the user to specify
            // "0x#" hex, "0#" octal, and "#" decimal numbers
            mExtSettings->mTOS = strtol( optarg, NULL, 0 );
            break;

        case 'T': // time-to-live for both unicast and multicast
            mExtSettings->mTTL = atoi( optarg );
            break;

        case 'U': // single threaded UDP server
            setSingleUDP( mExtSettings );
            break;

        case 'V': // IPv6 Domain
#ifdef HAVE_IPV6
            setIPV6( mExtSettings );
#else
	    fprintf( stderr, "The --ipv6_domain (-V) option is not enabled in this build.\n");
	    exit(1);
#endif
            break;

        case 'W' :
            setSuggestWin( mExtSettings );
            fprintf( stderr, "The -W option is not available in this release\n");
            break;

        case 'X' :
            setPeerVerDetect( mExtSettings );
            break;

        case 'Z':
#ifdef TCP_CONGESTION
	    setCongestionControl( mExtSettings );
	    mExtSettings->mCongestion = new char[strlen(optarg)+1];
	    strcpy( mExtSettings->mCongestion, optarg);
#else
            fprintf( stderr, "The -Z option is not available on this operating system\n");
#endif
	    break;

        case 0:
	    if (incrdstip) {
		incrdstip = 0;
		setIncrDstIP(mExtSettings);
	    }
	    if (txstarttime) {
#ifdef HAVE_CLOCK_NANOSLEEP
		long seconds;
		int match = 0;
		char f0 = '0';
		char f1 = '0';
		char f2 = '0';
		char f3 = '0';
		char f4 = '0';
		char f5 = '0';
		char f6 = '0';
		char f7 = '0';
		char f8 = '0';
		txstarttime = 0;
		setTxStartTime(mExtSettings);
		match = sscanf(optarg,"%ld.%c%c%c%c%c%c%c%c%c", &seconds, &f0,&f1,&f2,&f3,&f4,&f5,&f6,&f7,&f8);
		if (match > 1) {
		    int i;
		    mExtSettings->txstart.tv_sec = seconds;
		    i = f0 - '0'; mExtSettings->txstart.tv_nsec  = i * 100000000;
		    i = f1 - '0'; mExtSettings->txstart.tv_nsec += i * 10000000;
		    i = f2 - '0'; mExtSettings->txstart.tv_nsec += i * 1000000;
		    i = f3 - '0'; mExtSettings->txstart.tv_nsec += i * 100000;
		    i = f4 - '0'; mExtSettings->txstart.tv_nsec += i * 10000;
		    i = f5 - '0'; mExtSettings->txstart.tv_nsec += i * 1000;
		    i = f6 - '0'; mExtSettings->txstart.tv_nsec += i * 100;
		    i = f7 - '0'; mExtSettings->txstart.tv_nsec += i * 10;
		    i = f8 - '0'; mExtSettings->txstart.tv_nsec += i;
		} else if (match == 1) {
		    mExtSettings->txstart.tv_sec = seconds;
		    mExtSettings->txstart.tv_nsec = 0;
		} else {
		    fprintf(stderr, "WARNING: invalid --txstart-time format\n");
		}
#else
	        fprintf(stderr, "WARNING: --txstart-time not supported\n");
#endif
	    }
	    if (triptime) {
		triptime = 0;
		setTripTime(mExtSettings);
	    }
	    if (udphistogram) {
		udphistogram = 0;
		setUDPHistogram( mExtSettings );
		setEnhanced( mExtSettings );
		// The following are default values which
		mExtSettings->mUDPbins = 1000;
		mExtSettings->mUDPbinsize = 1;
		mExtSettings->mUDPunits = 0;
		mExtSettings->mUDPci_lower = 5;
		mExtSettings->mUDPci_upper = 95;
		if (optarg) {
		    mExtSettings->mUDPHistogramStr = new char[ strlen( optarg ) + 1 ];
		    strcpy(mExtSettings->mUDPHistogramStr, optarg);
		}
	    }
	    if (reversetest) {
		reversetest = 0;
		fprintf( stderr, "WARNING: The --reverse option is currently not supported\n");
		exit(1);
		setReverse(mExtSettings);
	    }
	    if (fqrate) {
#if defined(HAVE_DECL_SO_MAX_PACING_RATE)
	        fqrate=0;
		setFQPacing(mExtSettings);
		mExtSettings->mFQPacingRate = (unsigned int) (bitorbyte_atoi(optarg) / 8);
#else
		fprintf( stderr, "WARNING: The --fq-rate option is not supported\n");
#endif
	    }

#ifdef HAVE_ISOCHRONOUS
	    if (isochronous) {
		isochronous = 0;
		setEnhanced( mExtSettings );
		setIsochronous(mExtSettings);
		// The following are default values which
		// may be overwritten during modal parsing
		mExtSettings->mFPS = 60.0;
		mExtSettings->mMean = 20000000.0;
		mExtSettings->mVariance = 0.0;
		mExtSettings->mBurstIPG = 0.005;
		if (optarg) {
		    mExtSettings->mIsochronousStr = new char[ strlen( optarg ) + 1 ];
		    strcpy( mExtSettings->mIsochronousStr, optarg );
		}
	    }
	    if (burstipg) {
		burstipg = 0;
		burstipg_set = 1;
		char *end;
		mExtSettings->mBurstIPG = strtof(optarg,&end);
		if (*end != '\0') {
		    fprintf (stderr, "Invalid value of '%s' for --ipg\n", optarg);
		}
	    }
#endif
	    break;
        default: // ignore unknown
            break;
    }
} // end Interpret


//  The commmand line options are position independent and hence some settings become "modal"
//  i.e. two passes are required to get all the final settings correct.
//  For example, -V indicates use IPv6 and -u indicates use UDP, and the default socket
//  read/write (UDP payload) size is different for ipv4 and ipv6.
//  So in the Settings_Interpret pass there is no guarantee to know all three of (-u and -V and not -l)
//  while parsing them individually.
//
//  Since Settings_Interpret() will set all the *individual* options and flags
//  then the below code (per the example UDP, v4 or v6, and not -l) can set final
//  values, e.g. a correct default mBufLen.
//
//  Other things that need this are multicast socket or not,
//  -B local bind port parsing, and when to use the default UDP offered load
void Settings_ModalOptions( thread_Settings *mExtSettings ) {
    char *results;
    // Handle default read/write sizes based on v4, v6, UDP or TCP
    if ( !isBuflenSet( mExtSettings ) ) {
	if (isUDP(mExtSettings)) {
	    if (isIPV6(mExtSettings) && mExtSettings->mThreadMode == kMode_Client) {
		mExtSettings->mBufLen = kDefault_UDPBufLenV6;
	    } else {
		mExtSettings->mBufLen = kDefault_UDPBufLen;
	    }
	} else {
	    mExtSettings->mBufLen = kDefault_TCPBufLen;
	}
    }
    // Handle default UDP offered load (TCP will be max, i.e. no read() or write() rate limiting)
    if (!isBWSet(mExtSettings) && isUDP(mExtSettings)) {
	mExtSettings->mUDPRate = kDefault_UDPRate;
    }

    if (mExtSettings->mThreadMode != kMode_Client) {
	if (isVaryLoad(mExtSettings)) {
	    fprintf(stderr, "option of variance ignored as not supported on the server\n");
	}
	if (isTxStartTime(mExtSettings)) {
	    unsetTxStartTime(mExtSettings);
	    fprintf(stderr, "option of --txstart-time ignored as not supported on the server\n");
	}
    }


    // UDP histogram settings
    if (isUDPHistogram(mExtSettings) && isUDP(mExtSettings) && \
	(mExtSettings->mThreadMode != kMode_Client) && mExtSettings->mUDPHistogramStr) {
	if (((results = strtok(mExtSettings->mUDPHistogramStr, ",")) != NULL) && !strcmp(results,mExtSettings->mUDPHistogramStr)) {
	    char *tmp = new char [strlen(results) + 1];
	    strcpy(tmp, results);
	    // scan for microseconds as units
	    if ((strtok(tmp, "u") != NULL) && strcmp(results,tmp)) {
		mExtSettings->mUDPunits = 1;
	    }
	    mExtSettings->mUDPbinsize = atoi(tmp);
	    delete [] tmp;
	    if ((results = strtok(results+strlen(results)+1, ",")) != NULL) {
		mExtSettings->mUDPbins = byte_atoi(results);
		if ((results = strtok(NULL, ",")) != NULL) {
		    mExtSettings->mUDPci_lower = atof(results);
		    if ((results = strtok(NULL, ",")) != NULL) {
			mExtSettings->mUDPci_upper = atof(results);
		    }
		}
	    }
	}
    }
    // L2 settings
    if (l2checks && isUDP(mExtSettings)) {
	l2checks = 0;

	// Client controls hash or not
	if (mExtSettings->mThreadMode == kMode_Client) {
	    setL2LengthCheck(mExtSettings);
	} else {
#if defined(HAVE_LINUX_FILTER_H) && defined(HAVE_AF_PACKET)
	  // Request server to do length checks
	  setL2LengthCheck(mExtSettings);
#else
	  fprintf(stderr, "--l2checks not supported on this platform\n");
#endif
	}
    }


#ifdef HAVE_ISOCHRONOUS
    if (mExtSettings->mBurstIPG > 0.0) {
	if (!isIsochronous(mExtSettings)) {
	    fprintf(stderr, "option --ipg requires the --isochronous option\n");
	    exit(1);
	}
	if (mExtSettings->mThreadMode != kMode_Client) {
	    fprintf(stderr, "option --ipg only supported on clients\n");
	    exit(1);
	}
    }
    if (isIsochronous(mExtSettings) && mExtSettings->mIsochronousStr) {
	// parse client isochronous field,
	// format is --isochronous <int>:<float>,<float> and supports
	// human suffixes, e.g. --isochronous 60:100m,5m
	// which is frames per second, mean and variance
	if (mExtSettings->mThreadMode == kMode_Client) {
	    if (((results = strtok(mExtSettings->mIsochronousStr, ":")) != NULL) && !strcmp(results,mExtSettings->mIsochronousStr)) {
		mExtSettings->mFPS = atof(results);
		if ((results = strtok(NULL, ",")) != NULL) {
		    mExtSettings->mMean = bitorbyte_atof(results);
		    if ((results = strtok(NULL, ",")) != NULL) {
			mExtSettings->mVariance = bitorbyte_atof(results);
		    }
		} else {
		    mExtSettings->mMean = 20000000.0;
		    mExtSettings->mVariance = 0.0;
		}
	    } else {
		fprintf(stderr, "Invalid --isochronous value, format is <fps>:<mean>,<variance> (e.g. 60:18M,1m)\n");
	    }
	}
    }
#endif
    // Check for further mLocalhost (-B) parsing:
    if ( mExtSettings->mLocalhost) {
	// Check for -B device
	if (((results = strtok(mExtSettings->mLocalhost, "%")) != NULL) && ((results = strtok(NULL, "%")) != NULL)) {
	    mExtSettings->mIfrname = new char[ strlen(results) + 1 ];
	    strcpy( mExtSettings->mIfrname, results );
	}
	// Client local host parsing
	if (mExtSettings->mThreadMode == kMode_Client ) {
	    // v4 uses a colon as the delimeter for the local bind port, e.g. 192.168.1.1:6001
	    if (!isIPV6(mExtSettings)) {
		if (((results = strtok(mExtSettings->mLocalhost, ":")) != NULL) && ((results = strtok(NULL, ":")) != NULL)) {
		    mExtSettings->mBindPort = atoi(results);
		}
		// v6 uses bracket notation, e.g. [2001:e30:1401:2:d46e:b891:3082:b939]:6001
	    } else if (mExtSettings->mLocalhost[0] ==  '[') {
		if ((results = strtok(mExtSettings->mLocalhost, "]")) != NULL) {
		    results++;
		    strcpy(mExtSettings->mLocalhost, results);
		    if ((results = strtok(NULL, ":")) != NULL) {
			mExtSettings->mBindPort = atoi(results);
		    }
		}
	    }
	}
    }
    //  Check for a multicast
    if ( mExtSettings->mThreadMode == kMode_Client ) {
	// For client, check the destination host for multicast
	iperf_sockaddr tmp;
	SockAddr_setHostname( mExtSettings->mHost, &tmp,
			      (isIPV6( mExtSettings ) ? 1 : 0 ));
	if ( SockAddr_isMulticast( &tmp ) ) {
	    setMulticast( mExtSettings );
	}
    } else if (mExtSettings->mLocalhost != NULL) {
	// For listener or server, check if a -B bind interface is set and for multicast
	iperf_sockaddr tmp;
	SockAddr_setHostname( mExtSettings->mLocalhost, &tmp,
			      (isIPV6( mExtSettings ) ? 1 : 0 ));
	if ( SockAddr_isMulticast( &tmp ) ) {
	    setMulticast( mExtSettings );
	}
    }
}

void Settings_GetUpperCaseArg(const char *inarg, char *outarg) {

    int len = strlen(inarg);
    strcpy(outarg,inarg);

    if ( (len > 0) && (inarg[len-1] >='a')
         && (inarg[len-1] <= 'z') )
        outarg[len-1]= outarg[len-1]+'A'-'a';
}

void Settings_GetLowerCaseArg(const char *inarg, char *outarg) {

    int len = strlen(inarg);
    strcpy(outarg,inarg);

    if ( (len > 0) && (inarg[len-1] >='A')
         && (inarg[len-1] <= 'Z') )
        outarg[len-1]= outarg[len-1]-'A'+'a';
}

/*
 * Settings_GenerateListenerSettings
 * Called to generate the settings to be passed to the Listener
 * instance that will handle dual testings from the client side
 * this should only return an instance if it was called on
 * the thread_Settings instance generated from the command line
 * for client side execution
 */
void Settings_GenerateListenerSettings( thread_Settings *client, thread_Settings **listener ) {
    if ( !isCompat( client ) && \
         (client->mMode == kTest_DualTest || client->mMode == kTest_TradeOff) ) {
        *listener = new thread_Settings;
        memcpy(*listener, client, sizeof( thread_Settings ));
	setCompat((*listener));
        unsetDaemon( (*listener) );
        if ( client->mListenPort != 0 ) {
            (*listener)->mPort   = client->mListenPort;
        } else {
            (*listener)->mPort   = client->mPort;
        }
	if (client->mMode == kTest_TradeOff)
	    (*listener)->mAmount   = 2 * client->mAmount;
        (*listener)->mFileName   = NULL;
        (*listener)->mHost       = NULL;
        (*listener)->mLocalhost  = NULL;
        (*listener)->mOutputFileName = NULL;
        (*listener)->mMode       = kTest_Normal;
        (*listener)->mThreadMode = kMode_Listener;
        if ( client->mHost != NULL ) {
            (*listener)->mHost = new char[strlen( client->mHost ) + 1];
            strcpy( (*listener)->mHost, client->mHost );
        }
        if ( client->mLocalhost != NULL ) {
            (*listener)->mLocalhost = new char[strlen( client->mLocalhost ) + 1];
            strcpy( (*listener)->mLocalhost, client->mLocalhost );
        }
	(*listener)->mBufLen   = kDefault_UDPBufLen;
    } else {
        *listener = NULL;
    }
}

/*
 * Settings_GenerateClientSettings
 *
 * Called by the Listener to generate the settings to be used by clients
 * per things like dual tests.
 *
 */
void Settings_GenerateClientSettings( thread_Settings *server,
                                      thread_Settings **client,
                                      client_hdr *hdr ) {
    int extendflags = 0;
    int flags = ntohl(hdr->base.flags);
    if ((flags & HEADER_EXTEND) != 0 ) {
	extendflags = ntohl(hdr->extend.flags);
    }
    if ( (flags & HEADER_VERSION1) != 0 ) {
        *client = new thread_Settings;
        memcpy(*client, server, sizeof( thread_Settings ));
        setCompat( (*client) );
        (*client)->mTID = thread_zeroid();
        (*client)->mPort       = (unsigned short) ntohl(hdr->base.mPort);
        (*client)->mThreads    = 1;
        if ( hdr->base.bufferlen != 0 ) {
            (*client)->mBufLen = ntohl(hdr->base.bufferlen);
        }
	(*client)->mAmount     = ntohl(hdr->base.mAmount);
        if ( ((*client)->mAmount & 0x80000000) > 0 ) {
            setModeTime( (*client) );
#ifndef WIN32
            (*client)->mAmount |= 0xFFFFFFFF00000000LL;
#else
            (*client)->mAmount |= 0xFFFFFFFF00000000;
#endif
            (*client)->mAmount = -(*client)->mAmount;
        } else {
	    unsetModeTime( (*client) );
	}
        (*client)->mFileName   = NULL;
        (*client)->mHost       = NULL;
        (*client)->mLocalhost  = NULL;
        (*client)->mOutputFileName = NULL;
        (*client)->mMode       = ((flags & RUN_NOW) == 0 ?
				  kTest_TradeOff : kTest_DualTest);
        (*client)->mThreadMode = kMode_Client;
	if ((flags & HEADER_EXTEND) != 0 ) {
	    if ( !isBWSet(server) ) {
		(*client)->mUDPRate = ntohl(hdr->extend.mRate);
		if ((extendflags & UNITS_PPS) == UNITS_PPS) {
		    (*client)->mUDPRateUnits = kRate_PPS;
		} else {
		    (*client)->mUDPRateUnits = kRate_BW;
		}
	    }
	}
        if ( server->mLocalhost != NULL ) {
            (*client)->mLocalhost = new char[strlen( server->mLocalhost ) + 1];
            strcpy( (*client)->mLocalhost, server->mLocalhost );
        }
        (*client)->mHost = new char[REPORT_ADDRLEN];
        if ( ((sockaddr*)&server->peer)->sa_family == AF_INET ) {
            inet_ntop( AF_INET, &((sockaddr_in*)&server->peer)->sin_addr,
                       (*client)->mHost, REPORT_ADDRLEN);
        }
#ifdef HAVE_IPV6
	else {
            inet_ntop( AF_INET6, &((sockaddr_in6*)&server->peer)->sin6_addr,
                       (*client)->mHost, REPORT_ADDRLEN);
        }
#endif
    } else {
        *client = NULL;
    }
}

/*
 * Settings_GenerateClientHdr
 *
 * Called to generate the client header to be passed to the listener/server
 *
 * This will handle:
 * o) dual testings from the listener/server side
 * o) advanced udp test settings
 *
 * Returns hdr flags set
 */
int Settings_GenerateClientHdr( thread_Settings *client, client_hdr *hdr ) {
    uint32_t flags = 0, extendflags = 0;
    if (isPeerVerDetect(client) || (client->mMode != kTest_Normal && isBWSet(client))) {
	flags |= HEADER_EXTEND;
    }
    flags |= HEADER_SEQNO64B;
    if ( client->mMode != kTest_Normal ) {
	flags |= HEADER_VERSION1;
	if ( isBuflenSet( client ) ) {
	    hdr->base.bufferlen = htonl(client->mBufLen);
	} else {
	    hdr->base.bufferlen = 0;
	}
	if ( client->mListenPort != 0 ) {
	    hdr->base.mPort  = htonl(client->mListenPort);
	} else {
	    hdr->base.mPort  = htonl(client->mPort);
	}
	hdr->base.numThreads = htonl(client->mThreads);
	if ( isModeTime( client ) ) {
	    hdr->base.mAmount = htonl(-(long)client->mAmount);
	} else {
	    hdr->base.mAmount = htonl((long)client->mAmount);
	    hdr->base.mAmount &= htonl( 0x7FFFFFFF );
	}
	if ( client->mMode == kTest_DualTest ) {
	    flags |= RUN_NOW;
	}
    }
    if (isUDP(client)) {
	/*
	 * set the default offset where underlying "inline" subsystems can write into the udp payload
	 */
	hdr->udp.tlvoffset = htons((sizeof(client_hdr_udp_tests) + sizeof(client_hdr_v1) + sizeof(UDP_datagram)));

	if (isL2LengthCheck(client) || isIsochronous(client)) {
	    flags |= HEADER_UDPTESTS;
	    uint16_t testflags = 0;

	    if (isL2LengthCheck(client)) {
		testflags |= HEADER_L2LENCHECK;
		if (isIPV6(client))
		    testflags |= HEADER_L2ETHPIPV6;
	    }
	    if (isIsochronous(client)) {
		hdr->udp.tlvoffset = htons((sizeof(UDP_isoch_payload) + sizeof(client_hdr_udp_tests) + sizeof(client_hdr_v1) + sizeof(UDP_datagram)));
		testflags |= HEADER_UDP_ISOCH;
	    }
	    // Write flags to header so the listener can determine the tests requested
	    hdr->udp.testflags = htons(testflags);
	    hdr->udp.version_u = htonl(IPERF_VERSION_MAJORHEX);
	    hdr->udp.version_l = htonl(IPERF_VERSION_MINORHEX);
	}
    }
    /*
     * Finally, update the header flags (to be passed to the remote server)
     */
    hdr->base.flags = htonl(flags);
    if (flags & HEADER_EXTEND) {
	if (isBWSet(client)) {
	    hdr->extend.mRate = htonl(client->mUDPRate);
	}
	if (client->mUDPRateUnits == kRate_PPS) {
	    extendflags |= UNITS_PPS;
	}
        hdr->extend.typelen.type  = htonl(CLIENTHDR);
	hdr->extend.typelen.length = htonl((sizeof(client_hdrext) - sizeof(hdr_typelen)));
	hdr->extend.reserved = 0;
	hdr->extend.version_u = htonl(IPERF_VERSION_MAJORHEX);
	hdr->extend.version_l = htonl(IPERF_VERSION_MINORHEX);
	hdr->extend.flags  = htonl(extendflags);
    }
    return (flags);
}
