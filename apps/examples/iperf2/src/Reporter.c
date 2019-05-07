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
 * Reporter.c
 * by Kevin Gibbs <kgibbs@nlanr.net>
 *
 * ________________________________________________________________ */

#include <math.h>
#include "headers.h"
#include "Settings.hpp"
#include "util.h"
#include "Reporter.h"
#include "Thread.h"
#include "Locale.h"
#include "PerfSocket.hpp"
#include "SocketAddr.h"
#include "histogram.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INITIAL_PACKETID
# define INITIAL_PACKETID 0
#endif

/*
  The following 4 functions are provided for Reporting
  styles that do not have all the reporting formats. For
  instance the provided CSV format does not have a settings
  report so it uses settings_notimpl.
*/
		void* connection_notimpl( Connection_Info * nused, int nuse ) {
				return NULL;
		}
		void settings_notimpl( ReporterData * nused ) { }
		void statistics_notimpl( Transfer_Info * nused ) { }
		void serverstatistics_notimpl( Connection_Info *nused1, Transfer_Info *nused2 ) { }

// To add a reporting style include its header here.
#include "report_default.h"
#include "report_CSV.h"

// The following array of report structs contains the
// pointers required for reporting in different reporting
// styles. To add a reporting style add a report struct
// below.
		report_connection connection_reports[kReport_MAXIMUM] = {
				reporter_reportpeer,
				CSV_peer
		};

		report_settings settings_reports[kReport_MAXIMUM] = {
				reporter_reportsettings,
				settings_notimpl
		};

		report_statistics statistics_reports[kReport_MAXIMUM] = {
				reporter_printstats,
				CSV_stats
		};

		report_serverstatistics serverstatistics_reports[kReport_MAXIMUM] = {
				reporter_serverstats,
				CSV_serverstats
		};

		report_statistics multiple_reports[kReport_MAXIMUM] = {
				reporter_multistats,
				CSV_stats
		};

		char buffer[SNBUFFERSIZE]; // Buffer for printing
		ReportHeader *ReportRoot = NULL;
		static int num_multi_slots = 0;
		extern Condition ReportCond;
		extern Condition ReportDoneCond;
		int reporter_process_report ( ReportHeader *report );
		void process_report ( ReportHeader *report );
		int reporter_handle_packet( ReportHeader *report );
		int reporter_condprintstats( ReporterData *stats, MultiHeader *multireport, int force );
		int reporter_print( ReporterData *stats, int type, int end );
		void PrintMSS( ReporterData *stats );

		static void InitDataReport(struct thread_Settings *mSettings);
		static void InitConnectionReport(struct thread_Settings *mSettings);
#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
		static void gettcpistats(ReporterData *stats, int final);
#endif

		MultiHeader* InitMulti( thread_Settings *agent, int inID) {
				MultiHeader *multihdr = NULL;
				if ( agent->mThreads > 1 || agent->mThreadMode == kMode_Server ) {
						if ( isMultipleReport( agent ) ) {
								if (agent->mThreadMode == kMode_Client) {
										num_multi_slots = (agent->mMode == kTest_DualTest) ? ((agent->mThreads * 2) + 1) : (agent->mThreads  + 1);
								} else {
										num_multi_slots = (((agent->mThreads * 2) + 1) > NUM_MULTI_SLOTS) ? ((agent->mThreads * 2) + 1) : NUM_MULTI_SLOTS;
								}
								// printf ("Alloc %d multislots\n", num_multi_slots);
								multihdr = calloc((sizeof(MultiHeader) +  sizeof(ReporterData) +
												   num_multi_slots * sizeof(Transfer_Info)), sizeof(char*));
						} else {
								multihdr = calloc(sizeof(MultiHeader), sizeof(char*));
						}
						if ( multihdr != NULL ) {
								memset( multihdr, 0, sizeof(MultiHeader) );
								Condition_Initialize( &multihdr->barrier );
								multihdr->groupID = inID;
								if (agent->mThreadMode == kMode_Client) {
										multihdr->threads = agent->mThreads;
								}
								if ( isMultipleReport( agent ) ) {
										int i;
										ReporterData *data = NULL;
										multihdr->report = (ReporterData*)(multihdr + 1);
										memset(multihdr->report, 0, sizeof(ReporterData));
										multihdr->data = (Transfer_Info*)(multihdr->report + 1);
										data = multihdr->report;
										for ( i = 0; i < num_multi_slots; i++ ) {
												multihdr->data[i].startTime = -1;
												multihdr->data[i].transferID = inID;
												multihdr->data[i].groupID = -2;
										}
										data->type = TRANSFER_REPORT;
										if ( agent->mInterval != 0.0 ) {
												struct timeval *interval = &data->intervalTime;
												interval->tv_sec = (long) agent->mInterval;
												interval->tv_usec = (long) ((agent->mInterval - interval->tv_sec)
																			* rMillion);
										}
										data->mHost = agent->mHost;
										data->mLocalhost = agent->mLocalhost;
										data->mBufLen = agent->mBufLen;
										data->mMSS = agent->mMSS;
										data->mTCPWin = agent->mTCPWin;
										data->FQPacingRate = agent->mFQPacingRate;
										data->flags = agent->flags;
										data->mThreadMode = agent->mThreadMode;
										data->mode = agent->mReportMode;
										data->info.mFormat = agent->mFormat;
										data->info.mTTL = agent->mTTL;
										if (data->mThreadMode == kMode_Server)
												data->info.sock_callstats.read.binsize = data->mBufLen / 8;
										if ( isEnhanced( agent ) ) {
												data->info.mEnhanced = 1;
										} else {
												data->info.mEnhanced = 0;
										}
										if ( isUDP( agent ) ) {
												multihdr->report->info.mUDP = (char)agent->mThreadMode;
												multihdr->report->info.mUDP = 0;
										} else {
												multihdr->report->info.mTCP = (char)agent->mThreadMode;
										}
										if ( isConnectionReport( agent ) ) {
												data->type |= CONNECTION_REPORT;
												data->connection.peer = agent->peer;
												data->connection.size_peer = agent->size_peer;
												SockAddr_setPortAny( &data->connection.peer );
												data->connection.local = agent->local;
												data->connection.size_local = agent->size_local;
												SockAddr_setPortAny( &data->connection.local );
												data->connection.peerversion = agent->peerversion;
										}
								}
						} else {
								FAIL(1, "Out of Memory!!\n", agent);
						}
				}
				return multihdr;
		}

/*
 * BarrierClient allows for multiple stream clients to be syncronized
 */
		void BarrierClient( ReportHeader *agent ) {
				Condition_Lock(agent->multireport->barrier);
				agent->multireport->threads--;
				if ( agent->multireport->threads == 0 ) {
						// last one set time and wake up everyone
						gettimeofday( &(agent->multireport->startTime), NULL );
						Condition_Broadcast( &agent->multireport->barrier );
				} else {
						Condition_Wait( &agent->multireport->barrier );
				}
				agent->multireport->threads++;
				Condition_Unlock( agent->multireport->barrier );
				agent->report.startTime = agent->multireport->startTime;
				agent->report.nextTime = agent->report.startTime;
				TimeAdd( agent->report.nextTime, agent->report.intervalTime );
		}

/*
 * InitReport is called by a transfer agent (client or
 * server) to setup the needed structures to communicate
 * traffic and connection information.  Also initialize
 * the report start time and next interval report time
 * Finally, in the case of parallel clients, have them all
 * synchronize on compeleting their connect()
 */
		void InitReport(thread_Settings *mSettings) {
				// Note, this must be called in order as
				// the data report structures need to be
				// initialized first
				if (isDataReport(mSettings)) {
						InitDataReport(mSettings);
				}
				if (isConnectionReport(mSettings)) {
						InitConnectionReport(mSettings);
				}
				ReportHeader *reporthdr = mSettings->reporthdr;
				if (reporthdr && (isDataReport(mSettings) || isConnectionReport(mSettings))) {
						//
						// Set the report start times and next report times
						//
#ifdef HAVE_THREAD
						// In the case of parellel clients synchronize them after the connect(),
						// i.e. before their traffic run loops
						if ((reporthdr->report.mThreadMode == kMode_Client) && (reporthdr->multireport != NULL)) {
								// syncronize watches on my mark......
								BarrierClient(reporthdr);
						} else {
								if ( reporthdr->multireport != NULL && isMultipleReport( mSettings )) {
										reporthdr->multireport->threads++;
										if ( reporthdr->multireport->report->startTime.tv_sec == 0 ) {
												gettimeofday( &(reporthdr->multireport->report->startTime), NULL );
										}
										reporthdr->report.startTime = reporthdr->multireport->report->startTime;
								} else {
										// set start time
										gettimeofday( &(reporthdr->report.startTime), NULL );
								}
								reporthdr->report.nextTime = reporthdr->report.startTime;
								TimeAdd( reporthdr->report.nextTime, reporthdr->report.intervalTime );
						}
#else
						// set start time
						gettimeofday( &(reporthdr->report.startTime), NULL );
						// set next report time
						reporthdr->report.nextTime = reporthdr->report.startTime;
						TimeAdd( reporthdr->report.nextTime, reporthdr->report.intervalTime );
#endif
				}
		}

		void InitDataReport(thread_Settings *mSettings) {
				ReportHeader *reporthdr = NULL;
				ReporterData *data = NULL;
				/*
				 * Create in one big chunk
				 */
				reporthdr = calloc( sizeof(ReportHeader) +
									NUM_REPORT_STRUCTS * sizeof(ReportStruct), sizeof(char*));

				if ( reporthdr != NULL ) {
						mSettings->reporthdr = reporthdr;
						reporthdr->data = (ReportStruct*)(reporthdr+1);
						reporthdr->multireport = mSettings->multihdr;
						data = &reporthdr->report;
						reporthdr->reporterindex = NUM_REPORT_STRUCTS - 1;
						data->lastError = INITIAL_PACKETID;
						data->lastDatagrams = INITIAL_PACKETID;
						data->PacketID = INITIAL_PACKETID;
						data->info.transferID = mSettings->mSock;
						data->info.groupID = (mSettings->multihdr != NULL ? mSettings->multihdr->groupID : -1);
						data->type = TRANSFER_REPORT;
						if ( mSettings->mInterval != 0.0 ) {
								struct timeval *interval = &data->intervalTime;
								interval->tv_sec = (long) mSettings->mInterval;
								interval->tv_usec = (long) ((mSettings->mInterval - interval->tv_sec) * rMillion);
						}
						data->mHost = mSettings->mHost;
						data->mLocalhost = mSettings->mLocalhost;
						data->mSSMMulticastStr = mSettings->mSSMMulticastStr;
						data->mIfrname = mSettings->mIfrname;
						data->mBufLen = mSettings->mBufLen;
						data->mMSS = mSettings->mMSS;
						data->mTCPWin = mSettings->mTCPWin;
						data->FQPacingRate = mSettings->mFQPacingRate;
						data->flags = mSettings->flags;
						data->mThreadMode = mSettings->mThreadMode;
						data->mode = mSettings->mReportMode;
						data->info.mFormat = mSettings->mFormat;
						data->info.mTTL = mSettings->mTTL;
						if (data->mThreadMode == kMode_Server)
								data->info.sock_callstats.read.binsize = data->mBufLen / 8;
						if ( isUDP( mSettings ) ) {
								gettimeofday(&data->IPGstart, NULL);
								reporthdr->report.info.mUDP = (char)mSettings->mThreadMode;
						} else {
								reporthdr->report.info.mTCP = (char)mSettings->mThreadMode;
						}
						if ( isEnhanced( mSettings ) ) {
								data->info.mEnhanced = 1;
						} else {
								data->info.mEnhanced = 0;
						}
						if (data->mThreadMode == kMode_Server) {
								if (isUDPHistogram(mSettings)) {
										char name[] = "T8";
										data->info.latency_histogram =  histogram_init(mSettings->mUDPbins,mSettings->mUDPbinsize,0,\
																					   (mSettings->mUDPunits ? 1e6 : 1e3), \
																					   mSettings->mUDPci_lower, mSettings->mUDPci_upper, data->info.transferID, name);
								}
#ifdef HAVE_ISOCHRONOUS
								if (isUDPHistogram(mSettings) && isIsochronous(mSettings)) {
										char name[] = "F8";
										// make sure frame bin size min is 100 microsecond
										if (mSettings->mUDPunits && (mSettings->mUDPbinsize < 100))
												mSettings->mUDPbinsize = 100;
										mSettings->mUDPunits = 1;
										data->info.framelatency_histogram =  histogram_init(mSettings->mUDPbins,mSettings->mUDPbinsize,0, \
																							(mSettings->mUDPunits ? 1e6 : 1e3), mSettings->mUDPci_lower, \
																							mSettings->mUDPci_upper, data->info.transferID, name);
								}
#endif
						}
#ifdef HAVE_ISOCHRONOUS
						if ( isIsochronous( mSettings ) ) {
								data->info.mIsochronous = 1;
						} else {
								data->info.mIsochronous = 0;
						}
#endif
				} else {
						FAIL(1, "Out of Memory!!\n", mSettings);
				}
		}

		void InitConnectionReport (thread_Settings *mSettings) {
				ReportHeader *reporthdr = mSettings->reporthdr;
				ReporterData *data = NULL;

				if (reporthdr == NULL) {
						/*
						 * We don't have a Data Report structure in which to hang
						 * the connection report so allocate a minimal one
						 */
						reporthdr = calloc( sizeof(ReportHeader), sizeof(char*) );
						if (reporthdr == NULL ) {
								FAIL(1, "Out of Memory!!\n", mSettings);
						}
						mSettings->reporthdr = reporthdr;
				}
				// Fill out known fields for the connection report
				data = &reporthdr->report;
				data->info.transferID = mSettings->mSock;
				data->info.groupID = -1;
				data->type |= CONNECTION_REPORT;
				data->connection.peer = mSettings->peer;
				data->connection.size_peer = mSettings->size_peer;
				data->connection.local = mSettings->local;
				data->connection.size_local = mSettings->size_local;
				data->connection.peerversion = mSettings->peerversion;
				// Set the l2mode flags
				data->connection.l2mode = isL2LengthCheck(mSettings);
				if (data->connection.l2mode)
						data->connection.l2mode = ((isIPV6(mSettings) << 1) | data->connection.l2mode);
				if (isEnhanced(mSettings) && isTxStartTime(mSettings)) {
						data->connection.epochStartTime.tv_sec = mSettings->txstart_epoch.tv_sec;
						data->connection.epochStartTime.tv_usec = mSettings->txstart_epoch.tv_usec;
				}
		}

		void PostFirstReport (thread_Settings *mSettings) {
				if (mSettings->reporthdr) {
						ReportHeader *reporthdr = mSettings->reporthdr;
#ifdef HAVE_THREAD
						/*
						 * Update the ReportRoot to include this report.
						 */
						Condition_Lock( ReportCond );
						reporthdr->next = ReportRoot;
						ReportRoot = reporthdr;
						Condition_Signal( &ReportCond );
						Condition_Unlock( ReportCond );
#else
						/*
						 * Process the report in this thread
						 */
						reporthdr->next = NULL;
						process_report ( reporthdr );
#endif
				}
		}

/*
 * ReportPacket is called by a transfer agent to record
 * the arrival or departure of a "packet" (for TCP it
 * will actually represent many packets). This needs to
 * be as simple and fast as possible as it gets called for
 * every "packet".
 */
		void ReportPacket( ReportHeader* agent, ReportStruct *packet ) {
				if ( agent != NULL ) {
						int index = agent->reporterindex;
						/*
						 * First find the appropriate place to put the information
						 */
						if ( agent->agentindex == NUM_REPORT_STRUCTS ) {
								// Just need to make sure that reporter is not on the first
								// item
								while ( index == 0 ) {
										Condition_Signal( &ReportCond );
										Condition_Lock( ReportDoneCond );
										Condition_Wait( &ReportDoneCond );
										Condition_Unlock( ReportDoneCond );
										index = agent->reporterindex;
								}
								agent->agentindex = 0;
						}
						// Need to make sure that reporter is not about to be "lapped"
						while ( index - 1 == agent->agentindex ) {
								Condition_Signal( &ReportCond );
								Condition_Lock( ReportDoneCond );
								Condition_Wait( &ReportDoneCond );
								Condition_Unlock( ReportDoneCond );
								index = agent->reporterindex;
						}

						// Put the information there
						memcpy( agent->data + agent->agentindex, packet, sizeof(ReportStruct) );

						// Updating agentindex MUST be the last thing done
						agent->agentindex++;
#ifndef HAVE_THREAD
						/*
						 * Process the report in this thread
						 */
						process_report ( agent );
#endif
				}
		}

/*
 * CloseReport is called by a transfer agent to finalize
 * the report and signal transfer is over.
 */
		void CloseReport( ReportHeader *agent, ReportStruct *packet ) {
				int currpktid;
				if ( agent != NULL) {

						/*
						 * Using PacketID of -1 ends reporting
						 */
						currpktid = packet->packetID;
						packet->packetID = -1;
						packet->packetLen = 0;
						ReportPacket( agent, packet );
						packet->packetID = currpktid;
				}
		}

/*
 * EndReport signifies the agent no longer is interested
 * in the report. Calls to GetReport will no longer be
 * filled
 */
		void EndReport( ReportHeader *agent ) {
				if ( agent != NULL ) {
						int index = agent->reporterindex;
						while ( index != -1 ) {
								thread_rest();
								index = agent->reporterindex;
						}
						agent->agentindex = -1;
#ifndef HAVE_THREAD
						/*
						 * Process the report in this thread
						 */
						process_report ( agent );
#endif
				}
		}

/*
 * GetReport is called by the agent after a CloseReport
 * but before an EndReport to get the stats generated
 * by the reporter thread.
 */
		Transfer_Info *GetReport( ReportHeader *agent ) {
				int index = agent->reporterindex;
				while ( index != -1 ) {
						thread_rest();
						index = agent->reporterindex;
				}
				return &agent->report.info;
		}

/*
 * ReportSettings will generate a summary report for
 * settings being used with Listeners or Clients
 */
		void ReportSettings( thread_Settings *agent ) {
				if ( isSettingsReport( agent ) ) {
						/*
						 * Create in one big chunk
						 */
						ReportHeader *reporthdr = calloc( sizeof(ReportHeader), sizeof(char*));
						if ( reporthdr != NULL ) {
								ReporterData *data = &reporthdr->report;
								data->info.transferID = agent->mSock;
								data->info.groupID = -1;
								reporthdr->agentindex = -1;
								reporthdr->reporterindex = -1;

								data->mHost = agent->mHost;
								data->mLocalhost = agent->mLocalhost;
								data->mSSMMulticastStr = agent->mSSMMulticastStr;
								data->mIfrname = agent->mIfrname;
								data->mode = agent->mReportMode;
								data->type = SETTINGS_REPORT;
								data->mBufLen = agent->mBufLen;
								data->mMSS = agent->mMSS;
								data->mTCPWin = agent->mTCPWin;
								data->FQPacingRate = agent->mFQPacingRate;
								data->flags = agent->flags;
								data->flags_extend = agent->flags_extend;
								data->mThreadMode = agent->mThreadMode;
								data->mPort = agent->mPort;
								data->info.mFormat = agent->mFormat;
								data->info.mTTL = agent->mTTL;
								data->connection.peer = agent->peer;
								data->connection.size_peer = agent->size_peer;
								data->connection.local = agent->local;
								data->connection.size_local = agent->size_local;
								data->mUDPRate = agent->mUDPRate;
								data->mUDPRateUnits = agent->mUDPRateUnits;
#ifdef HAVE_ISOCHRONOUS
								if (isIsochronous(data)) {
										data->isochstats.mFPS = agent->mFPS;
										data->isochstats.mMean = agent->mMean/8;
										data->isochstats.mVariance = agent->mVariance/8;
										data->isochstats.mBurstIPG = (unsigned int) (agent->mBurstIPG*1000.0);
										data->isochstats.mBurstInterval = (unsigned int) (1 / agent->mFPS * 1000000);
								}
#endif
#ifdef HAVE_THREAD
								/*
								 * Update the ReportRoot to include this report.
								 */
								Condition_Lock( ReportCond );
								reporthdr->next = ReportRoot;
								ReportRoot = reporthdr;
								Condition_Signal( &ReportCond );
								Condition_Unlock( ReportCond );
#else
								/*
								 * Process the report in this thread
								 */
								reporthdr->next = NULL;
								process_report ( reporthdr );
#endif
						} else {
								FAIL(1, "Out of Memory!!\n", agent);
						}
				}
		}

/*
 * ReportServerUDP will generate a report of the UDP
 * statistics as reported by the server on the client
 * side.
 */
		void ReportServerUDP( thread_Settings *agent, server_hdr *server ) {
				unsigned int flags = ntohl(server->base.flags);
				// printf("Server flags = 0x%X\n", flags);
				if (isServerReport(agent) && ((flags & HEADER_VERSION1) != 0)) {
						/*
						 * Create in one big chunk
						 */
						ReportHeader *reporthdr = calloc( sizeof(ReportHeader), sizeof(char*));
						Transfer_Info *stats = &reporthdr->report.info;

						if ( !reporthdr ) {
								FAIL(1, "Out of Memory!!\n", agent);
						}

						stats->transferID = agent->mSock;
						stats->groupID = (agent->multihdr != NULL ? agent->multihdr->groupID \
										  : -1);
						reporthdr->agentindex = -1;
						reporthdr->reporterindex = -1;

						reporthdr->report.type = SERVER_RELAY_REPORT;
						reporthdr->report.mode = agent->mReportMode;
						stats->mFormat = agent->mFormat;
						stats->jitter = ntohl( server->base.jitter1 );
						stats->jitter += ntohl( server->base.jitter2 ) / (double)rMillion;
#ifdef HAVE_INT64_T
						stats->TotalLen = (((intmax_t) ntohl( server->base.total_len1 )) << 32) + \
								ntohl( server->base.total_len2 );
#else
						stats->TotalLen = (intmax_t) ntohl(server->base.total_len2);
#endif
						stats->startTime = 0;
						stats->endTime = ntohl( server->base.stop_sec );
						stats->endTime += ntohl( server->base.stop_usec ) / (double)rMillion;
						if ((flags & HEADER_SEQNO64B)) {
								stats->cntError = (((intmax_t) ntohl( server->extend2.error_cnt2 )) << 32) + \
										ntohl( server->base.error_cnt );
								stats->cntOutofOrder = (((intmax_t) ntohl( server->extend2.outorder_cnt2 )) << 32) + \
										ntohl( server->base.outorder_cnt );
								stats->cntDatagrams = (((intmax_t) ntohl( server->extend2.datagrams2 )) << 32) + \
										ntohl( server->base.datagrams );
						} else {
								stats->cntError  = ntohl( server->base.error_cnt );
								stats->cntOutofOrder = ntohl( server->base.outorder_cnt );
								stats->cntDatagrams = ntohl( server->base.datagrams );
						}

						if ((flags & HEADER_EXTEND) != 0) {
								stats->mEnhanced = 1;
								stats->transit.minTransit = ntohl( server->extend.minTransit1 );
								stats->transit.minTransit += ntohl( server->extend.minTransit2 ) / (double)rMillion;
								stats->transit.maxTransit = ntohl( server->extend.maxTransit1 );
								stats->transit.maxTransit += ntohl( server->extend.maxTransit2 ) / (double)rMillion;
								stats->transit.sumTransit = ntohl( server->extend.sumTransit1 );
								stats->transit.sumTransit += ntohl( server->extend.sumTransit2 ) / (double)rMillion;
								stats->transit.meanTransit = ntohl( server->extend.meanTransit1 );
								stats->transit.meanTransit += ntohl( server->extend.meanTransit2 ) / (double)rMillion;
								stats->transit.m2Transit = ntohl( server->extend.m2Transit1 );
								stats->transit.m2Transit += ntohl( server->extend.m2Transit2 ) / (double)rMillion;
								stats->transit.vdTransit = ntohl( server->extend.vdTransit1 );
								stats->transit.vdTransit += ntohl( server->extend.vdTransit2 ) / (double)rMillion;
								stats->transit.cntTransit = ntohl( server->extend.cntTransit );
								stats->IPGcnt = ntohl( server->extend.IPGcnt );
								stats->IPGsum = ntohl( server->extend.IPGsum );
						}
						stats->mUDP = (char)kMode_Server;
						reporthdr->report.connection.peer = agent->local;
						reporthdr->report.connection.size_peer = agent->size_local;
						reporthdr->report.connection.local = agent->peer;
						reporthdr->report.connection.size_local = agent->size_peer;

#ifdef HAVE_THREAD
						/*
						 * Update the ReportRoot to include this report.
						 */
						Condition_Lock( ReportCond );
						reporthdr->next = ReportRoot;
						ReportRoot = reporthdr;
						Condition_Signal( &ReportCond );
						Condition_Unlock( ReportCond );
#else
						/*
						 * Process the report in this thread
						 */
						reporthdr->next = NULL;
						process_report ( reporthdr );
#endif
				}
		}


/*
 * This function is called only when the reporter thread
 * This function is the loop that the reporter thread processes
 */
		void reporter_spawn( thread_Settings *thread ) {
				do {
						// This section allows for safe exiting with Ctrl-C
						Condition_Lock ( ReportCond );
						if ( ReportRoot == NULL ) {
								// Allow main thread to exit if Ctrl-C is received
								thread_setignore();
								Condition_Wait ( &ReportCond );
								// Stop main thread from exiting until done with all reports
								thread_unsetignore();
						}
						Condition_Unlock ( ReportCond );

				again:
						{
								struct timespec requested;
								requested.tv_sec  = 0;
								requested.tv_nsec = 10000000L;
								nanosleep(&requested, NULL);
						}

						if ( ReportRoot != NULL ) {
								ReportHeader *temp = ReportRoot;
								//Condition_Unlock ( ReportCond );
								if ( reporter_process_report ( temp ) ) {
										// This section allows for more reports to be added while
										// the reporter is processing reports without needing to
										// stop the reporter or immediately notify it
										Condition_Lock ( ReportCond );
										if ( temp == ReportRoot ) {
												// no new reports
												ReportRoot = temp->next;
										} else {
												// new reports added
												ReportHeader *itr = ReportRoot;
												while ( itr->next != temp ) {
														itr = itr->next;
												}
												itr->next = temp->next;
										}
										// finished with report so free it
										if (temp->report.info.latency_histogram) {
												histogram_delete(temp->report.info.latency_histogram);
										}
#ifdef HAVE_ISOCHRONOUS
										if (temp->report.info.framelatency_histogram) {
												histogram_delete(temp->report.info.framelatency_histogram);
										}
#endif
										free( temp );
										Condition_Unlock ( ReportCond );
										Condition_Signal( &ReportDoneCond );
										if (ReportRoot)
												goto again;
								}
								Condition_Signal( &ReportDoneCond );
								/*
								 * Suspend the reporter thread for 10 milliseconds
								 *
								 * This allows the thread to receive client or server threads'
								 * packet events in "aggregates."  This can reduce context
								 * switching allowing for better CPU utilization,
								 * which is very noticble on CPU constrained systems.
								 *
								 * If the realtime flag is set, then don't invoke the
								 * suspend.  This should give better reporter timing on
								 * higher end systems, where a busy-loop thread can be
								 * scheduled without impacting other threads.
								 *
								 * Note that if the reporter thread is signficantly slower
								 * than the Client or Server (traffic) threads this suspend
								 * will still be called even though the traffic threads can be
								 * blocked on the shared memory being full.  Probably should
								 * detect those and avoid the suspend under such conditions.
								 * It's not a big deal though because the traffic threads under
								 * normal conditions are much slower than the reporter thread.
								 * The exception is when there are things like sustained
								 * write errors. Hence, such an optimization is deferred
								 * for a later date.
								 *
								 * Also note, a possible better implementation is for the
								 * reporter thread to block on a traffic thread's signal
								 * instead of a 10 ms suspend.  That implementation
								 * would have to be profiled against this one to make sure
								 * it indeed gave better performance.  Again, deferred.
								 *
								 * Final note, usleep() is being deprecated for nanosleep(),
								 * so use nanosleep if available
								 */
								if ( !isRealtime( thread ) ) {
#ifdef HAVE_NANOSLEEP
										{
												struct timespec requested;
												requested.tv_sec  = 0;
												requested.tv_nsec = 10000000L;
												nanosleep(&requested, NULL);
										}
#else
										usleep(10000);
#endif
								}
						}
				} while ( 1 );
		}

/*
 * Used for single threaded reporting
 */
		void process_report ( ReportHeader *report ) {
				if ( report != NULL ) {
						if ( reporter_process_report( report ) ) {
								if (report->report.info.latency_histogram) {
										histogram_delete(report->report.info.latency_histogram);
								}
#ifdef HAVE_ISOCHRONOUS
								if (report->report.info.framelatency_histogram) {
										histogram_delete(report->report.info.framelatency_histogram);
								}
#endif
								free( report );
						}
				}
		}

/*
 * Process reports starting with "reporthdr"
 */
		int reporter_process_report ( ReportHeader *reporthdr ) {
				int need_free = 0;

				// Recursively process reports
				if ( reporthdr->next != NULL ) {
						if ( reporter_process_report( reporthdr->next ) ) {
								// If we are done with this report then free it
								ReportHeader *temp = reporthdr->next;
								reporthdr->next = reporthdr->next->next;
								if (temp->report.info.latency_histogram) {
										histogram_delete(temp->report.info.latency_histogram);
								}
#ifdef HAVE_ISOCHRONOUS
								if (temp->report.info.framelatency_histogram) {
										histogram_delete(temp->report.info.framelatency_histogram);
								}
#endif
								free( temp );
						}
				}

				if ( (reporthdr->report.type & SETTINGS_REPORT) != 0 ) {
						reporthdr->report.type &= ~SETTINGS_REPORT;
						return reporter_print( &reporthdr->report, SETTINGS_REPORT, 1 );
				} else if ( (reporthdr->report.type & CONNECTION_REPORT) != 0 ) {
						reporthdr->report.type &= ~CONNECTION_REPORT;
						reporter_print( &reporthdr->report, CONNECTION_REPORT,
										(reporthdr->report.type == 0 ? 1 : 0) );
						if ( reporthdr->multireport != NULL && isMultipleReport( (&reporthdr->report) )) {
								if ( (reporthdr->multireport->report->type & CONNECTION_REPORT) != 0 ) {
										reporthdr->multireport->report->type &= ~CONNECTION_REPORT;
										reporter_print( reporthdr->multireport->report, CONNECTION_REPORT,
														(reporthdr->report.type == 0 ? 1 : 0) );
								}
						}
				} else if ( (reporthdr->report.type & SERVER_RELAY_REPORT) != 0 ) {
						reporthdr->report.type &= ~SERVER_RELAY_REPORT;
						return reporter_print( &reporthdr->report, SERVER_RELAY_REPORT, 1 );
				}
				if ( (reporthdr->report.type & TRANSFER_REPORT) != 0 ) {
						// If there are more packets to process then handle them
						if ( reporthdr->reporterindex >= 0 ) {
								// Need to make sure we do not pass the "agent"
								while ( reporthdr->reporterindex != reporthdr->agentindex - 1 ) {
										if ( reporthdr->reporterindex == NUM_REPORT_STRUCTS - 1 ) {
												if ( reporthdr->agentindex == 0 ) {
														break;
												} else {
														reporthdr->reporterindex = 0;
												}
										} else {
												reporthdr->reporterindex++;
										}
										if ( reporter_handle_packet( reporthdr ) ) {
												// No more packets to process
												reporthdr->reporterindex = -1;
												break;
										}
								}
						}
						// If the agent is done with the report then free it
						if ( reporthdr->agentindex == -1 ) {
								need_free = 1;
						}
				}
				return need_free;
		}

/*
 * Updates connection stats
 */
#define L2DROPFILTERCOUNTER 100
		int reporter_handle_packet( ReportHeader *reporthdr ) {
				ReportStruct *packet = &reporthdr->data[reporthdr->reporterindex];
				ReporterData *data = &reporthdr->report;
				Transfer_Info *stats = &reporthdr->report.info;
				int finished = 0;
				double usec_transit;

				data->packetTime = packet->packetTime;
				stats->socket = packet->socket;
				if ( packet->packetID < 0 ) {
						finished = 1;
						if ( reporthdr->report.mThreadMode != kMode_Client ) {
								data->TotalLen += packet->packetLen;
						}
				} else {
						// Process error counters that are mostly
						// unrelated to receiving a valid packet
						//
						// First, are client socket write counters
						if (reporthdr->report.mThreadMode == kMode_Client) {
								if (packet->errwrite) {
										if (packet->errwrite != WriteErrNoAccount) {
												stats->sock_callstats.write.WriteErr++;
												stats->sock_callstats.write.totWriteErr++;
										}
								} else {
										stats->sock_callstats.write.WriteCnt++;
										stats->sock_callstats.write.totWriteCnt++;
								}
								// Next are server l2 errors, filter out first n L2 errors
								// due to BPF AF_PACKET race
						} else if (packet->l2errors && (data->cntDatagrams > L2DROPFILTERCOUNTER)) {
								stats->l2counts.cnt++;
								stats->l2counts.tot_cnt++;
								if (packet->l2errors & L2UNKNOWN) {
										stats->l2counts.unknown++;
										stats->l2counts.tot_unknown++;
								}
								if (packet->l2errors & L2LENERR) {
										stats->l2counts.lengtherr++;
										stats->l2counts.tot_lengtherr++;
								}
								if (packet->l2errors & L2CSUMERR) {
										stats->l2counts.udpcsumerr++;
										stats->l2counts.tot_udpcsumerr++;
								}
						}
						// These are valid packets that need standard iperf accounting
						if (!packet->emptyreport) {
								// update fields common to TCP and UDP, client and server
								data->TotalLen += packet->packetLen;
								// update fields common to TCP and UDP client
								// update fields common to UDP client and server
								if ( isUDP( data ) ) {
										data->cntDatagrams++;
										stats->IPGsum += TimeDifference(data->packetTime, data->IPGstart );
										stats->IPGcnt++;
										data->IPGstart = data->packetTime;
#ifdef HAVE_ISOCHRONOUS
										//printf("fid=%lu bs=%lu remain=%lu\n", packet->frameID, packet->burstsize, packet->remaining);
										if (packet->frameID && packet->burstsize && packet->remaining) {
												int framedelta=0;
												// very first isochronous frame
												if (!data->isochstats.frameID) {
														data->isochstats.framecnt=packet->frameID;
														data->isochstats.framecnt=1;
														stats->isochstats.framecnt=1;
												}
												// perform client and server frame based accounting
												if ((framedelta = (packet->frameID - data->isochstats.frameID))) {
														data->isochstats.framecnt++;
														stats->isochstats.framecnt++;
														if (framedelta > 1) {
																if (stats->mUDP == kMode_Server) {
																		int lost = framedelta - (packet->frameID - packet->prevframeID);
																		stats->isochstats.framelostcnt += lost;
																		data->isochstats.framelostcnt += lost;
																} else {
																		stats->isochstats.framelostcnt += (framedelta-1);
																		data->isochstats.framelostcnt += (framedelta-1);
																		stats->isochstats.slipcnt++;
																		data->isochstats.slipcnt++;
																}
														}
												}
												// peform frame latency checks
												if (stats->framelatency_histogram) {
														static int matchframeid=0;
														// first packet of a burst and not a duplicate
														if ((packet->burstsize == packet->remaining) && (matchframeid!=packet->frameID)) {
																matchframeid=packet->frameID;
														}
														if ((packet->packetLen == packet->remaining) && (packet->frameID == matchframeid)) {
																// last packet of a burst (or first-last in case of a duplicate) and frame id match
																double frametransit = TimeDifference(packet->packetTime, packet->isochStartTime) \
																		- ((packet->burstperiod * (packet->frameID - 1)) / 1000000.0);
																histogram_insert(stats->framelatency_histogram, frametransit);
																matchframeid = 0;  // reset the matchid so any potential duplicate is ignored
														}
												}
												data->isochstats.frameID = packet->frameID;
										}
#endif
										// Finally, update UDP server fields
										if (stats->mUDP == kMode_Server) {
												//subsequent packets
												double transit;
												double deltaTransit;
												transit = TimeDifference( packet->packetTime, packet->sentTime );
												if (stats->latency_histogram) {
														histogram_insert(stats->latency_histogram, transit);
												}

												// packet loss occured if the datagram numbers aren't sequential
												if ( packet->packetID != data->PacketID + 1 ) {
														if (packet->packetID < data->PacketID + 1 ) {
																data->cntOutofOrder++;
														} else {
																data->cntError += packet->packetID - data->PacketID - 1;
														}
												}
												// never decrease datagramID (e.g. if we get an out-of-order packet)
												if ( packet->packetID > data->PacketID ) {
														data->PacketID = packet->packetID;
												}
												if (stats->transit.totcntTransit == 0) {
														// Very first packet
														stats->transit.minTransit = transit;
														stats->transit.maxTransit = transit;
														stats->transit.sumTransit = transit;
														stats->transit.cntTransit = 1;
														stats->transit.totminTransit = transit;
														stats->transit.totmaxTransit = transit;
														stats->transit.totsumTransit = transit;
														stats->transit.totcntTransit = 1;
														// For variance, working units is microseconds
														usec_transit = transit * 1e6;
														stats->transit.vdTransit = usec_transit;
														stats->transit.meanTransit = usec_transit;
														stats->transit.m2Transit = usec_transit * usec_transit;
														stats->transit.totvdTransit = usec_transit;
														stats->transit.totmeanTransit = usec_transit;
														stats->transit.totm2Transit = usec_transit * usec_transit;
												} else {
														// from RFC 1889, Real Time Protocol (RTP)
														// J = J + ( | D(i-1,i) | - J ) /
														// Compute jitter
														deltaTransit = transit - stats->transit.lastTransit;
														if ( deltaTransit < 0.0 ) {
																deltaTransit = -deltaTransit;
														}
														stats->jitter += (deltaTransit - stats->jitter) / (16.0);
														// Compute end/end delay stats
														stats->transit.sumTransit += transit;
														stats->transit.cntTransit++;
														stats->transit.totsumTransit += transit;
														stats->transit.totcntTransit++;
														// mean min max tests
														if (transit < stats->transit.minTransit) {
																stats->transit.minTransit=transit;
														}
														if (transit < stats->transit.totminTransit) {
																stats->transit.totminTransit=transit;
														}
														if (transit > stats->transit.maxTransit) {
																stats->transit.maxTransit=transit;
														}
														if (transit > stats->transit.totmaxTransit) {
																stats->transit.totmaxTransit=transit;
														}
														// For variance, working units is microseconds
														// variance interval
														usec_transit = transit * 1e6;
														stats->transit.vdTransit = usec_transit - stats->transit.meanTransit;
														stats->transit.meanTransit = stats->transit.meanTransit + (stats->transit.vdTransit / stats->transit.cntTransit);
														stats->transit.m2Transit = stats->transit.m2Transit + (stats->transit.vdTransit * (usec_transit - stats->transit.meanTransit));
														// variance total
														stats->transit.totvdTransit = usec_transit - stats->transit.totmeanTransit;
														stats->transit.totmeanTransit = stats->transit.totmeanTransit + (stats->transit.totvdTransit / stats->transit.totcntTransit);
														stats->transit.totm2Transit = stats->transit.totm2Transit + (stats->transit.totvdTransit * (usec_transit - stats->transit.totmeanTransit));
												}
												stats->transit.lastTransit = transit;
										}
								} else if (reporthdr->report.mThreadMode == kMode_Server && (packet->packetLen > 0)) {
										int bin;
										// mean min max tests
										stats->sock_callstats.read.cntRead++;
										stats->sock_callstats.read.totcntRead++;
										bin = (int)floor((packet->packetLen -1)/stats->sock_callstats.read.binsize);
										stats->sock_callstats.read.bins[bin]++;
										stats->sock_callstats.read.totbins[bin]++;
								}
						} else if ((stats->mUDP == kMode_Server) &&	\
								   (stats->transit.cntTransit == 0)) {
								// This is the case when empty reports
								// cross the report interval boundary
								// Hence, set the per interval min to infinity
								// and the per interval max and sum to zero
								stats->transit.minTransit = FLT_MAX;
								stats->transit.maxTransit = FLT_MIN;
								stats->transit.sumTransit = 0;
								stats->transit.vdTransit = 0;
								stats->transit.meanTransit = 0;
								stats->transit.m2Transit = 0;
						}
				}
				// Print a report if appropriate
				return reporter_condprintstats( &reporthdr->report, reporthdr->multireport, finished );
		}

/*
 * Handles summing of threads
 */
		void reporter_handle_multiple_reports( MultiHeader *reporthdr, Transfer_Info *stats, int force ) {
				if ( reporthdr != NULL ) {
						if ( reporthdr->threads > 1 ) {
								int i;
								Transfer_Info *current = NULL;
								// Search for start Time
								for ( i = 0; i < num_multi_slots; i++ ) {
										current = &reporthdr->data[i];
										if ( current->startTime == stats->startTime ) {
												break;
										}
								}
								if ( current->startTime != stats->startTime ) {
										// Find first available
										for ( i = 0; i < num_multi_slots; i++ ) {
												current = &reporthdr->data[i];
												if ( current->startTime < 0 ) {
														break;
												}
										}
										current->cntDatagrams = stats->cntDatagrams;
										current->cntError = stats->cntError;
										current->cntOutofOrder = stats->cntOutofOrder;
										current->TotalLen = stats->TotalLen;
										current->mFormat = stats->mFormat;
										current->mEnhanced = stats->mEnhanced;
										current->endTime = stats->endTime;
										current->jitter = stats->jitter;
										current->startTime = stats->startTime;
										current->IPGcnt = stats->IPGcnt;
										current->startTime = stats->startTime;
										current->IPGsum = stats->IPGsum;
										current->mUDP = stats->mUDP;
										current->mTCP = stats->mTCP;
										if (stats->mTCP == kMode_Server) {
												int ix;
												current->sock_callstats.read.cntRead = stats->sock_callstats.read.cntRead;
												for (ix = 0; ix < 8; ix++) {
														current->sock_callstats.read.bins[ix] = stats->sock_callstats.read.bins[ix];
												}
										} else {
												current->sock_callstats.write.WriteErr = stats->sock_callstats.write.WriteErr;
												current->sock_callstats.write.WriteCnt = stats->sock_callstats.write.WriteCnt;
#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
												if (stats->mTCP == kMode_Client) {
														current->sock_callstats.write.TCPretry = stats->sock_callstats.write.TCPretry;
												}
#endif
										}
										current->free = 1;
								} else {
										current->cntDatagrams += stats->cntDatagrams;
										current->cntError += stats->cntError;
										current->cntOutofOrder += stats->cntOutofOrder;
										current->TotalLen += stats->TotalLen;
										current->IPGcnt += stats->IPGcnt;
										if (stats->mTCP == kMode_Server) {
												int ix;
												current->sock_callstats.read.cntRead += stats->sock_callstats.read.cntRead;
												for (ix = 0; ix < 8; ix++) {
														current->sock_callstats.read.bins[ix] += stats->sock_callstats.read.bins[ix];
												}
										} else {
												current->sock_callstats.write.WriteErr += stats->sock_callstats.write.WriteErr;
												current->sock_callstats.write.WriteCnt += stats->sock_callstats.write.WriteCnt;
#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
												if (stats->mTCP == kMode_Client) {
														current->sock_callstats.write.TCPretry += stats->sock_callstats.write.TCPretry;
												}
#endif
										}
										if ( current->endTime < stats->endTime ) {
												current->endTime = stats->endTime;
										}
										if ( current->jitter < stats->jitter ) {
												current->jitter = stats->jitter;
										}
										current->free++;
										if ( current->free == reporthdr->threads ) {
												void *reserved = reporthdr->report->info.reserved_delay;
												current->free = force;
												memcpy( &reporthdr->report->info, current, sizeof(Transfer_Info) );
												current->startTime = -1;
												reporthdr->report->info.reserved_delay = reserved;
												reporter_print( reporthdr->report, MULTIPLE_REPORT, force );
										}
								}
						}
				}
		}

#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
		static void gettcpistats (ReporterData *stats, int final) {
				static int cnt = 0;
				struct tcp_info tcp_internal;
				socklen_t tcp_info_length = sizeof(struct tcp_info);
				int retry = 0;
				// Read the TCP retry stats for a client.  Do this
				// on  a report interval period.
				int rc = (stats->info.socket==INVALID_SOCKET) ? 0 : 1;
				if (rc) {
						rc = (getsockopt(stats->info.socket, IPPROTO_TCP, TCP_INFO, &tcp_internal, &tcp_info_length) < 0) ? 0 : 1;
						if (!rc)
								stats->info.socket = INVALID_SOCKET;
						else
								// Mark stale now so next call at report interval will update
								stats->info.sock_callstats.write.up_to_date = 1;
				}
				if (!rc) {
						stats->info.sock_callstats.write.TCPretry = 0;
						stats->info.sock_callstats.write.cwnd = -1;
						stats->info.sock_callstats.write.rtt = 0;
				} else {
						retry = tcp_internal.tcpi_total_retrans - stats->info.sock_callstats.write.lastTCPretry;
						stats->info.sock_callstats.write.TCPretry = retry;
						stats->info.sock_callstats.write.totTCPretry += retry;
						stats->info.sock_callstats.write.lastTCPretry = tcp_internal.tcpi_total_retrans;
						stats->info.sock_callstats.write.cwnd = tcp_internal.tcpi_snd_cwnd * tcp_internal.tcpi_snd_mss / 1024;
						stats->info.sock_callstats.write.rtt = tcp_internal.tcpi_rtt;
						// New average = old average * (n-1)/n + new value/n
						cnt++;
						stats->info.sock_callstats.write.meanrtt = (stats->info.sock_callstats.write.meanrtt * ((double) (cnt - 1) / (double) cnt)) + ((double) (tcp_internal.tcpi_rtt) / (double) cnt);
						stats->info.sock_callstats.write.rtt = tcp_internal.tcpi_rtt;
				}
				if (final) {
						stats->info.sock_callstats.write.rtt = stats->info.sock_callstats.write.meanrtt;
				}
		}
#endif
/*
 * Prints reports conditionally
 */
		int reporter_condprintstats( ReporterData *stats, MultiHeader *multireport, int force ) {

#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
				if ((stats->info.mEnhanced && stats->info.mTCP == kMode_Client) && (force || !stats->info.sock_callstats.write.up_to_date))
						gettcpistats(stats, force);
#endif

				if ( force ) {
						stats->info.cntOutofOrder = stats->cntOutofOrder;
						// assume most of the time out-of-order packets are not
						// duplicate packets, so conditionally subtract them from the lost packets.
						stats->info.cntError = stats->cntError;
						stats->info.cntError -= stats->info.cntOutofOrder;
						if ( stats->info.cntError < 0 ) {
								stats->info.cntError = 0;
						}
						stats->info.cntDatagrams = ((stats->info.mUDP == kMode_Server) ? stats->PacketID - INITIAL_PACKETID : stats->cntDatagrams);
						stats->info.TotalLen = stats->TotalLen;
						stats->info.startTime = 0;
						stats->info.endTime = TimeDifference( stats->packetTime, stats->startTime );

						// There is a corner case when the first packet is also the last where the start time (which comes
						// from app level syscall) is greater than the packetTime (which come for kernel level SO_TIMESTAMP)
						// For this case set the start and end time to both zero.
						if (stats->info.endTime < 0) {
								stats->info.endTime = 0;
						}
						if (stats->info.mUDP == kMode_Server) {
								stats->info.l2counts.cnt = stats->info.l2counts.tot_cnt;
								stats->info.l2counts.unknown = stats->info.l2counts.tot_unknown;
								stats->info.l2counts.udpcsumerr = stats->info.l2counts.tot_udpcsumerr;
								stats->info.l2counts.lengtherr = stats->info.l2counts.tot_lengtherr;
								stats->info.transit.minTransit = stats->info.transit.totminTransit;
								stats->info.transit.maxTransit = stats->info.transit.totmaxTransit;
								stats->info.transit.cntTransit = stats->info.transit.totcntTransit;
								stats->info.transit.sumTransit = stats->info.transit.totsumTransit;
								stats->info.transit.meanTransit = stats->info.transit.totmeanTransit;
								stats->info.transit.m2Transit = stats->info.transit.totm2Transit;
								stats->info.transit.vdTransit = stats->info.transit.totvdTransit;
						}
						if ((stats->info.mTCP == kMode_Client) || (stats->info.mUDP == kMode_Client)) {
								stats->info.sock_callstats.write.WriteErr = stats->info.sock_callstats.write.totWriteErr;
								stats->info.sock_callstats.write.WriteCnt = stats->info.sock_callstats.write.totWriteCnt;
								if (stats->info.mTCP == kMode_Client) {
										stats->info.sock_callstats.write.TCPretry = stats->info.sock_callstats.write.totTCPretry;
								}
						}
						if (stats->info.mTCP == kMode_Server) {
								int ix;
								stats->info.sock_callstats.read.cntRead = stats->info.sock_callstats.read.totcntRead;
								for (ix = 0; ix < 8; ix++) {
										stats->info.sock_callstats.read.bins[ix] = stats->info.sock_callstats.read.totbins[ix];
								}
								if (stats->clientStartTime.tv_sec > 0)
										stats->info.tripTime = TimeDifference( stats->packetTime, stats->clientStartTime );
								else
										stats->info.tripTime = 0;
						}
						if (stats->info.endTime > 0) {
								stats->info.IPGcnt = (int) (stats->cntDatagrams / stats->info.endTime);
						} else {
								stats->info.IPGcnt = 0;
						}
						stats->info.IPGsum = 1;
						stats->info.free = 1;
#ifdef HAVE_ISOCHRONOUS
						if (stats->info.mIsochronous) {
								stats->info.isochstats.framecnt = stats->isochstats.framecnt;
								stats->info.isochstats.framelostcnt = stats->isochstats.framelostcnt;
								stats->info.isochstats.slipcnt = stats->isochstats.slipcnt;
						}
#endif
						reporter_print( stats, TRANSFER_REPORT, force );
						if ( isMultipleReport(stats) ) {
								reporter_handle_multiple_reports( multireport, &stats->info, force );
						}
				} else while ((stats->intervalTime.tv_sec != 0 ||
							   stats->intervalTime.tv_usec != 0) &&
							  TimeDifference( stats->nextTime,
											  stats->packetTime ) < 0 ) {
								stats->info.cntOutofOrder = stats->cntOutofOrder - stats->lastOutofOrder;
								stats->lastOutofOrder = stats->cntOutofOrder;
								// assume most of the  time out-of-order packets are not
								// duplicate packets, so conditionally subtract them from the lost packets.
								stats->info.cntError = stats->cntError - stats->lastError;
								stats->info.cntError -= stats->info.cntOutofOrder;
								if ( stats->info.cntError < 0) {
										stats->info.cntError = 0;
								}
								stats->lastError = stats->cntError;
								stats->info.cntDatagrams = ((stats->info.mUDP == kMode_Server) ? stats->PacketID - stats->lastDatagrams :
															stats->cntDatagrams - stats->lastDatagrams);
								stats->lastDatagrams = ((stats->info.mUDP == kMode_Server) ? stats->PacketID : stats->cntDatagrams);
								stats->info.TotalLen = stats->TotalLen - stats->lastTotal;
								stats->lastTotal = stats->TotalLen;
								stats->info.startTime = stats->info.endTime;
								stats->info.endTime = TimeDifference( stats->nextTime, stats->startTime );
								TimeAdd( stats->nextTime, stats->intervalTime );
								stats->info.free = 0;
								reporter_print( stats, TRANSFER_REPORT, force );
								if ( isMultipleReport(stats) ) {
										reporter_handle_multiple_reports( multireport, &stats->info, force );
								}
								/*
								 * Reset transfer stats now that both the individual and SUM reports
								 * have completed
								 */
								if (stats->info.mUDP) {
										stats->info.IPGcnt = 0;
										stats->info.IPGsum = 0;
										if (stats->info.mUDP == kMode_Server) {
												stats->info.l2counts.cnt = 0;
												stats->info.l2counts.unknown = 0;
												stats->info.l2counts.udpcsumerr = 0;
												stats->info.l2counts.lengtherr = 0;
										}
								}
								if (stats->info.mEnhanced) {
										if ((stats->info.mTCP == (char)kMode_Client) || (stats->info.mUDP == (char)kMode_Client)) {
												stats->info.sock_callstats.write.WriteCnt = 0;
												stats->info.sock_callstats.write.WriteErr = 0;
												stats->info.sock_callstats.write.WriteErr = 0;
#ifdef HAVE_STRUCT_TCP_INFO_TCPI_TOTAL_RETRANS
												stats->info.sock_callstats.write.up_to_date = 0;
#endif
										} else if (stats->info.mTCP == (char)kMode_Server) {
												int ix;
												stats->info.sock_callstats.read.cntRead = 0;
												for (ix = 0; ix < 8; ix++) {
														stats->info.sock_callstats.read.bins[ix] = 0;
												}
										}
								}
						}
				return force;
		}

/*
 * This function handles multiple format printing by sending to the
 * appropriate dispatch function
 */
		int reporter_print( ReporterData *stats, int type, int end ) {
				switch ( type ) {
				case TRANSFER_REPORT:
						statistics_reports[stats->mode]( &stats->info );
						if ( end != 0 && isPrintMSS( stats ) && !isUDP( stats ) ) {
								PrintMSS( stats );
						}
						break;
				case SERVER_RELAY_REPORT:
						serverstatistics_reports[stats->mode]( &stats->connection, &stats->info );
						break;
				case SETTINGS_REPORT:
						settings_reports[stats->mode]( stats );
						break;
				case CONNECTION_REPORT:
						stats->info.reserved_delay = connection_reports[stats->mode] \
								(&stats->connection, stats->info.transferID);
						break;
				case MULTIPLE_REPORT:
						multiple_reports[stats->mode]( &stats->info );
						break;
				default:
						fprintf( stderr, "Printing type not implemented! No Output\n" );
				}
				fflush( stdout );
				return end;
		}

/* -------------------------------------------------------------------
 * Report the MSS and MTU, given the MSS (or a guess thereof)
 * ------------------------------------------------------------------- */

// compare the MSS against the (MTU - 40) to (MTU - 80) bytes.
// 40 byte IP header and somewhat arbitrarily, 40 more bytes of IP options.

#define checkMSS_MTU( inMSS, inMTU ) (inMTU-40) >= inMSS  &&  inMSS >= (inMTU-80)

		void PrintMSS( ReporterData *stats ) {
				int inMSS = getsock_tcp_mss( stats->info.transferID );

				if ( inMSS <= 0 ) {
						printf( report_mss_unsupported, stats->info.transferID );
				} else {
						char* net;
						int mtu = 0;

						if ( checkMSS_MTU( inMSS, 1500 ) ) {
								net = "ethernet";
								mtu = 1500;
						} else if ( checkMSS_MTU( inMSS, 4352 ) ) {
								net = "FDDI";
								mtu = 4352;
						} else if ( checkMSS_MTU( inMSS, 9180 ) ) {
								net = "ATM";
								mtu = 9180;
						} else if ( checkMSS_MTU( inMSS, 65280 ) ) {
								net = "HIPPI";
								mtu = 65280;
						} else if ( checkMSS_MTU( inMSS, 576 ) ) {
								net = "minimum";
								mtu = 576;
								printf( "%s", warn_no_pathmtu );
						} else {
								mtu = inMSS + 40;
								net = "unknown interface";
						}

						printf( report_mss,
								stats->info.transferID, inMSS, mtu, net );
				}
		}
// end ReportMSS

#ifdef __cplusplus
} /* end extern "C" */
#endif
