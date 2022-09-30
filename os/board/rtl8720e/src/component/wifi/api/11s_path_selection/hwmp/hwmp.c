#include "hwmp.h"

#ifdef CONFIG_RTK_MESH

#pragma pack(1)

struct _rann_retx_entry_ rann_retx_entry;
struct _rann_root_entry_ rann_root_entry;
pann_retx_t pann_retx_pool[MAX_MPP_NUM];
pann_mpp_t pann_mpp_pool[MAX_MPP_NUM];

unsigned int My_SeqNum = 1;
int My_RreqId = 1;
int g_rawsocket_fd = -1;
int g_ioctrl_socket_fd = -1;

unsigned char BroadCast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
int SeqGap = 0;


char interface_ioctl[IFNAMSIZ] = {0};
char interface_rawsocket[IFNAMSIZ] = {0};
//unsigned char my_address[ETH_ALEN];
unsigned char *const my_address = LwIP_GetMAC(0);

#ifdef CONFIG_RTK_MESH_METRIC_REFINE

//char *wlan0_ifName = "wlan0";
//char *wlan1_ifName = "wlan1";
#define wlan0_ifName    NODE_CFG_IFNAME

unsigned char wlan0_hwAddress[ETH_ALEN];
unsigned char wlan1_hwAddress[ETH_ALEN];
unsigned char wlan0_crossbandEnable = 0;
unsigned char wlan1_crossbandEnable = 0;

unsigned char wlan0_is5G = 0;
unsigned char wlan1_is5G = 0;

struct mibValue mibValueOf5G = {
	.rssi_threshold = 15,
	.cu_threshold = 160,
	.noise_threshold = 50,
	.rssi_weight = 2,
	.cu_weight = 3,
	.noise_weight = 2
};

struct mibValue mibValueOf2G = {
	.rssi_threshold = 5,
	.cu_threshold = 100,
	.noise_threshold = 30,
	.rssi_weight = 4,
	.cu_weight = 12,
	.noise_weight = 5
};

struct mibValue *mibValuePointer = &mibValueOf5G;
#endif

//static struct pathsel_context pathsel_cnt;
//static struct pathsel_context *hapd = &pathsel_cnt;

int pathSwitchThreshold = 20;

//===================================================================================================
//following added by shlu_2007_01_16
unsigned char PreMAC[ETH_ALEN] = {0};
//unsigned short Hello_random=0;
//#define  PATH_OnDemand_Debug 1                   //Path selection debug used

int My_interFace = 0;


int isPortal = 0;
int toBeRoot = 0;
int isRoot = 0;

static int rootDecision = 0;               // have made decision to root

ti_t i_timer[MAX_TIMER_INSTANCE] = INIT_TIMER_INSTANCE();

u_int64_t macAddr_atoi(unsigned char *a)
{
	u_int64_t i = 0;
	int k;
	for (k = 0; k < ETH_ALEN; k++) {
		i |= (a[k] << ((ETH_ALEN - k - 1) * 8));
	}
	return i;
}

//for the solution of Sequence Number overflow
/*
int CheckSeq(int StoredSeq,int RecvSeq)
{
	SeqGap=0;
	SeqGap = (int)(RecvSeq - StoredSeq);
	return SeqGap;
}
*/
//-----------------------------------------------------------------------


//int dump_table();   mark by chuangch 2007.09.14

static int is_my_stat(unsigned char addr[ETH_ALEN])
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 0;
}

static int notify_path_found(unsigned char dest_addr[ETH_ALEN])
{
	if (rtw_wx_mesh_path_found(NODE_CFG_IFACE_IDX, dest_addr) < 0) {
		// If no wireless name : no wireless extensions
		// printf("Update_table  Error:%s\n\a",strerror(errno));
		return (-1);
	}

	return 0;
}


// Recv Data frame but has no path information for nexthop,so generate a unicast rerr to inform the Datafrom-Sorce
// SourceAddr is my address and DesAddr is the data frame destination address
//===============================================================================================================

int gen_rerr(unsigned char *DesAddr, unsigned char *DataSrcAddr, unsigned char TTL, unsigned short SeqNum, unsigned char RERR_flag)
{

	struct rerr ptr;
	// if(RERR_flag==2) { // don't have path to dest

	ptr.Category    = _MESH_CATEGORY_ID_;
	ptr.Action      = _HWMP_MESH_PATHSEL_ACTION_ID_;
	ptr.Id          = _MESH_PERR_IE_;
	ptr.Length      = (sizeof(struct rerr) - 2);
	ptr.ModeFlags   = 0;
	ptr.NumOfDes    = htonl(1);
	memcpy(ptr.DesAddress, DesAddr, ETH_ALEN);
	ptr.DesSeqNum   = htonl(0); // htonl(Entry.dsn);
	send_packet((unsigned char *)&ptr, my_address, DataSrcAddr, my_address, 0, TTL, SeqNum);
	return 1;
	/*
	}
	else { //  neighbor node dispear
	    RERR_Check(DesAddr);
	    return 1;
	}
	*/
}


int recv_rerr(struct rerr *input_rerr, unsigned char *PreHopAdd, unsigned char TTL, unsigned short SeqNum, unsigned char *DesAdd)
{
	struct path_sel_entry Entry;                    //clean the table entry

	/*
	{
	    printf("@@@ recv_RERR : (can't find %X %X %X %X %X %X) from prehop : %x %x %x %x %x %x\n",
	    input_rerr->DesAddress[0], input_rerr->DesAddress[1], input_rerr->DesAddress[2], input_rerr->DesAddress[3], input_rerr->DesAddress[4], input_rerr->DesAddress[5],
	    PreHopAdd[0], PreHopAdd[1], PreHopAdd[2], PreHopAdd[3], PreHopAdd[4], PreHopAdd[5]);
	}
	*/

	if (query_table(input_rerr->DesAddress, &Entry) > 0) {
		if ((memcmp(Entry.nexthopMAC, PreHopAdd, ETH_ALEN)) == 0) {
			// clear path selection table
			remove_table(input_rerr->DesAddress);



			//relay RERR ,NO really Destinaiton and Source
			// not sending the packet if I am the dest
			if (memcmp(my_address, DesAdd, ETH_ALEN)) {
				send_packet((unsigned char *)input_rerr, my_address, DesAdd, my_address, 0, TTL, SeqNum);
			}
		} else {
			return 1;
		}
	}

	return 1;
}


//gen_rrep(My_Address,My_SeqNum,Entry1.destMAC,Entry1.dsn,0, 0,My_Address);
//DesAdd is who product RREP, Source is Who product RREQ
//=========================================================================
//modify by chuangch 2007/04/10
int gen_rrep(unsigned char *DesAdd, unsigned int DesSeq, unsigned char *Source, unsigned int SourceSeq, unsigned int NewMetric, unsigned char hopcount,
			 unsigned char TTL, unsigned short SeqNum, unsigned char Is6AddrFormat, unsigned char *Addr5, unsigned char *Addr6)
{

	struct rrep  ptr;

	ptr.Category    = _MESH_CATEGORY_ID_;
	ptr.Action      = _HWMP_MESH_PATHSEL_ACTION_ID_;
	ptr.Id          = _MESH_PREP_IE_;
	ptr.Length      = (sizeof(struct rrep) - 2);
	ptr.ModeFlags   = 0;
	ptr.HopCount    = hopcount;
	ptr.ttl         = TTL;
	//ptr.SrcCount			=1;
	memcpy(ptr.DesAddress, DesAdd, ETH_ALEN);

	ptr.DesSeqNum   = htonl(DesSeq);
	ptr.LifeTime    = htonl(255);

	ptr.Metric      = htonl(NewMetric);

	memcpy(ptr.first.SourceAddress, Source, ETH_ALEN);
	ptr.first.SourceSeqNum	= htonl(SourceSeq);
	ptr.first.DepDesSeqNum	= htonl(0); //?

	if (Is6AddrFormat) {
		send_packet_with_6addr((unsigned char *)&ptr, my_address, Source, my_address, 0, ptr.ttl, SeqNum, Addr5, Addr6);
	} else {
		send_packet((unsigned char *)&ptr, my_address, Source, my_address, 0, ptr.ttl, SeqNum);
	}
	return 1;

}
//=========================================================================

static void forward_rrep(struct rrep *ptr,
						 unsigned char TTL,
						 unsigned short SeqNum,
						 unsigned char Is6AddrFormat,
						 unsigned char *addr5,
						 unsigned char *addr6
						)
{

	if (ptr->ttl <= 0) {
		return;
	}
	ptr->DesSeqNum = htonl(ptr->DesSeqNum);
	ptr->LifeTime = htonl(ptr->LifeTime);
	ptr->Metric = htonl(ptr->Metric);
	ptr->first.SourceSeqNum = htonl(ptr->first.SourceSeqNum);
	ptr->first.DepDesSeqNum = htonl(ptr->first.DepDesSeqNum);

	if (Is6AddrFormat) {
		send_packet_with_6addr((unsigned char *)ptr, my_address, ptr->first.SourceAddress, ptr->DesAddress, 0, TTL, SeqNum, addr5, addr6);
	} else {
		send_packet((unsigned char *)ptr, my_address, ptr->first.SourceAddress, ptr->DesAddress, 0, TTL, SeqNum);
	}

	return;
}


static void printRREP(char *msg, struct rrep *pRREP)
{
	printf("\n%s\n------------RREP-------------\n", msg);
	printf("Category:				%d\n", pRREP->Category);
	printf("Action:					%d\n", pRREP->Action);
	printf("Id:						%d\n", pRREP->Id);
	printf("Length:					%d\n", pRREP->Length);
	printf("ModeFlags:				%d\n", pRREP->ModeFlags);
	printf("HopCount:				%d\n", pRREP->HopCount);
	printf("ttl:					%d\n", pRREP->ttl);
	printf("DesAddress:				%x:%x:%x:%x:%x:%x\n",
		   0xff & pRREP->DesAddress[0],
		   0xff & pRREP->DesAddress[1],
		   0xff & pRREP->DesAddress[2],
		   0xff & pRREP->DesAddress[3],
		   0xff & pRREP->DesAddress[4],
		   0xff & pRREP->DesAddress[5]);
	printf("DesSeqNum:				%d\n", pRREP->DesSeqNum);
	printf("LifeTime:				%d\n", pRREP->LifeTime);
	printf("Metric:					%d\n", pRREP->Metric);
	printf("first.SourceAddress:	%x:%x:%x:%x:%x:%x\n",
		   0xff & pRREP->first.SourceAddress[0],
		   0xff & pRREP->first.SourceAddress[1],
		   0xff & pRREP->first.SourceAddress[2],
		   0xff & pRREP->first.SourceAddress[3],
		   0xff & pRREP->first.SourceAddress[4],
		   0xff & pRREP->first.SourceAddress[5]);
	printf("first.SourceSeqNum:		%d\n", pRREP->first.SourceSeqNum);
	printf("first.DepCount:			%d\n", pRREP->first.DepCount);
	printf("first.DepMacAddress:	%x:%x:%x:%x:%x:%x\n",
		   0xff & pRREP->first.DepMacAddress[0],
		   0xff & pRREP->first.DepMacAddress[1],
		   0xff & pRREP->first.DepMacAddress[2],
		   0xff & pRREP->first.DepMacAddress[3],
		   0xff & pRREP->first.DepMacAddress[4],
		   0xff & pRREP->first.DepMacAddress[5]);
	printf("first.DepDesSeqNum:		%d\n", pRREP->first.DepDesSeqNum);
	printf("------------RREP-------------\n");
}


//DesAdd is who product RREP, Source is Who product RREQ
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int recv_rrep(struct rrep *input_rrep,
			  unsigned char *My_Prehop,
			  unsigned char *My_Add,
			  unsigned int My_Prehop_metric,
			  unsigned int rssiMetric,
			  unsigned int cuMetric,
			  unsigned int noiseMetric,
			  int My_interFace,
			  unsigned char TTL,
			  unsigned short SeqNum,
			  unsigned char Is6AddrFormat,
			  unsigned char *addr5,
			  unsigned char *addr6)
#else
int recv_rrep(struct rrep *input_rrep,
			  unsigned char *My_Prehop,
			  unsigned int My_Prehop_metric,
			  int My_interFace,
			  unsigned char TTL,
			  unsigned short SeqNum,
			  unsigned char Is6AddrFormat,
			  unsigned char *addr5,
			  unsigned char *addr6)

#endif
{
	if (memcmp(input_rrep->DesAddress, my_address, ETH_ALEN) == 0) { // for e.g., the situation is caused by rann's delay
		return 0;
	}

	input_rrep->LifeTime = htonl(input_rrep->LifeTime);
	input_rrep->DesSeqNum = htonl(input_rrep->DesSeqNum);
	input_rrep->first.SourceSeqNum = htonl(input_rrep->first.SourceSeqNum);
	input_rrep->first.DepDesSeqNum = htonl(input_rrep->first.DepDesSeqNum);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned int preHop_metric = calculateMetric(My_Add, My_Prehop_metric, rssiMetric, cuMetric, noiseMetric);
	updateMetric(My_Add, My_Prehop, &(preHop_metric));
	input_rrep->Metric = htonl(input_rrep->Metric) + preHop_metric;
#else
	input_rrep->Metric = htonl(input_rrep->Metric) + My_Prehop_metric;
#endif
	--input_rrep->ttl;
	++input_rrep->HopCount;

//2017.7.27 Jinzhou more tests needed
	/*
	    updatePathByFreshness(input_rrep->DesAddress, input_rrep->DesSeqNum,
	                input_rrep->Metric, input_rrep->HopCount, My_Prehop,
	                My_interFace);
	*/

//2017.7.28 Jinzhou modify explain:
	/*
	Assume this node is A, and the node send RREP is B:
	B will call
	gen_rrep(my_address=Bm, My_SeqNum, input->SourceAddress=Am, input->SourceSeqNum, 0, 0, _MESH_HEADER_TTL_, SeqNum, 1,input->SourceAddress=Am,input->first.DesAddress=Be);
	to generate RREP and send it to A.
	If only 3 addresses are used, should update Mesh Source Address (input_rrep->DesAddress) which is Bm,
	if 6 addresses are used, should update the Ultimate Source Address (Addr6), which is Be.
	*/

	if (1 == Is6AddrFormat) {
		updatePathByFreshness(addr6, input_rrep->DesSeqNum, input_rrep->Metric, input_rrep->HopCount, My_Prehop, My_interFace);
	} else {
		updatePathByFreshness(input_rrep->DesAddress, input_rrep->DesSeqNum, input_rrep->Metric, input_rrep->HopCount, My_Prehop, My_interFace);
	}

	/*
	//Check the entry for the RREP Destination who product the RREP
	temp_result=query_table(input_rrep->DesAddress,&Entry1);
	if(temp_result<=0) {       //need to create table entry for rrep.desnation
		create_table_entryA(input_rrep->DesAddress,input_rrep->DesSeqNum,My_Prehop,My_interFace,(input_rrep->HopCount),(input_rrep->Metric));
		temp_result=query_table(input_rrep->DesAddress,&Entry1);
	}
	else {


		// updatePendingPath_by_src(input_rrep->DesAddress, input_rrep->Metric, input_rrep->DesSeqNum, input_rrep->HopCount, My_Prehop);

		//check seq#
		// *
		if(CheckSeq(Entry1.dsn,input_rrep->DesSeqNum)< 0) {
			;
		} * //
		// *
		if( CheckSeq(Entry1.dsn,input_rrep->DesSeqNum) > 0
			||(Entry1.metric > input_rrep->Metric && !memcmp(Entry1.nexthopMAC,My_Prehop,6))
			|| Entry1.metric > input_rrep->Metric + pathSwitchThreshold )
		{
			Entry1.dsn=input_rrep->DesSeqNum;
			memcpy(Entry1.nexthopMAC,My_Prehop,6);
			Entry1.hopcount=(input_rrep->HopCount);
			Entry1.metric=(input_rrep->Metric);
			modify_table(&Entry1);

		}
		else {return 0;}
		* ///

	}
	*/

	if ((memcmp(input_rrep->first.SourceAddress, my_address, ETH_ALEN)) == 0) {
		// close by popen printf("***** REEP is for me,and I will send back RREP_ACK (daemon msg)\n");

		// fix:Initial ping several timeout (bug num: Productlization Phase 2 29, 2007/10/16)
		if (1 == Is6AddrFormat) {
			notify_path_found(addr6);
		} else {
			notify_path_found(input_rrep->DesAddress);
		}
		// end of fix:Initial ping several timeout.


		return 1;
	}

	//	input_rrep->Metric = input_rrep->Metric + My_Prehop_metric;
	// close by popen printf("***** REEP is not for me,relay the RREP (daemon msg)\n");

	forward_rrep(input_rrep, TTL, SeqNum, Is6AddrFormat, addr5, addr6);
	return 1;
}


//following modify by chuangch 2007/04/10
//=========================================================================
int gen_rreq(unsigned char *DesAdd, unsigned char TTL, unsigned short SeqNum)
{
	struct rreq ptr;

	ptr.Category    = _MESH_CATEGORY_ID_;
	ptr.Action      = _HWMP_MESH_PATHSEL_ACTION_ID_;
	ptr.Id          = _MESH_PREQ_IE_;
	ptr.Length      = sizeof(struct rreq) - 2;
	ptr.ModeFlags   = 1;
	ptr.HopCount    = 0;
	ptr.ttl         = TTL;
	//ptr->DesCount		=1;
	ptr.RreqId      = htonl(My_RreqId);
	memcpy(ptr.SourceAddress, my_address, ETH_ALEN);
	ptr.SourceSeqNum	= htonl(My_SeqNum);
	ptr.Lifetime    = htonl(255);
	ptr.Metric      = htonl(0);
	ptr.first.DO    = 1;          //initial DO=1; RF=1;
	ptr.first.RF    = 1;
	ptr.first.Reserved	= 0;
	memcpy(ptr.first.DesAddress, DesAdd, ETH_ALEN);
	ptr.first.DesSeqNum	= htonl(0);     //if maintain ,this is not 0

	My_SeqNum++;
	My_RreqId++;
	send_packet((unsigned char *)&ptr, my_address, DesAdd, my_address, 1, TTL, SeqNum); //1 for broadcast
	return 1;

}
//=========================================================================

static void forward_rreq(struct rreq *for_out, unsigned char TTL, unsigned short SeqNum)
{
	unsigned char DesAdd[6]; // just an unused parameter prepreared for send_packet
	if (for_out->ttl <= 0) {
		return;
	}

	for_out->RreqId = htonl(for_out->RreqId);
	for_out->Metric = htonl(for_out->Metric);
	for_out->Lifetime = htonl(for_out->Lifetime);
	for_out->first.DesSeqNum = htonl(for_out->first.DesSeqNum);
	for_out->SourceSeqNum = htonl(for_out->SourceSeqNum);

	send_packet((unsigned char *)for_out, my_address, DesAdd, for_out->SourceAddress, 1, TTL, SeqNum);
}



// Main idea: The decision of whether forwarding or droping a rreq shall depends on
//            the "freshness" of the DSN and the "goodness" of the metric. The freshness
//            is to compare with previous rreq's {issuer, dest} pair. Thus, one SRC could
//            request two hosts, says A and B, and their SEQ NOs are 1 and 2 respectively.
//            If current MP have received rreq for B, it should NOT drop the rreq for A,
//            because rreq for A is still a "fresh" RREQ for current MP.
// Design logic:
// Step 1: reject rreq from myself
// Step 2: always update path to pre-hop ('cos it is almost a near real-time metric)
// Step 3: check the freshness of tuple {src, dest, DSN} and the goodness of metric
//         if not fresh enough and not good enough, drop it
// Step 4: forward & reply
//         4a: reply rrep if current node is destination or destination is my station
//         4b: forward rreq with DO=1
//         4c: when DO=0 and having a path to desition which go through src, drop it
//         4d: reply rrep and forward rreq with DO=1, when DO=0 and having a path to destination which does not go throuth src
//         4e: forward rreq for 3b~3e
//
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int recv_rreq(struct rreq *input, unsigned char *My_Prehop, unsigned char *My_Add, unsigned int My_Prehop_metric, unsigned int rssiMetric,
			  unsigned int cuMetric, unsigned int noiseMetric, int My_interFace, unsigned char TTL, unsigned short SeqNum)
#else
int recv_rreq(struct rreq *input, unsigned char *My_Prehop, unsigned int My_Prehop_metric, int My_interFace, unsigned char TTL, unsigned short SeqNum)
#endif
{

	///// Step 1: reject rreq from myself
	if (memcmp(input->SourceAddress, my_address, ETH_ALEN) == 0) {
		return 0;            //get the RREQ from my_sent RREQ
	}

	input->Lifetime = ntohl(input->Lifetime);
	input->RreqId = ntohl(input->RreqId);
	input->SourceSeqNum = ntohl(input->SourceSeqNum);
	input->first.DesSeqNum = ntohl(input->first.DesSeqNum);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned int preHop_metric = calculateMetric(My_Add, My_Prehop_metric, rssiMetric, cuMetric, noiseMetric);
	updateMetric(My_Add, My_Prehop, &(preHop_metric));
	input->Metric = ntohl(input->Metric) + preHop_metric;
#else
	input->Metric = ntohl(input->Metric) + My_Prehop_metric;
#endif
	++input->HopCount;
	--input->ttl;

	//// Step 3: check the freshness of tuple {src, dest, DSN} and the goodness of metric
	if (updatePathByFreshness(input->SourceAddress, input->SourceSeqNum,
							  input->Metric, input->HopCount, My_Prehop,
							  My_interFace) == 0) {
		return 0;
	}

	///// Step 4: forward & reply

	// case 4a
	if (isPortal && (memcmp(input->first.DesAddress, my_address, ETH_ALEN)) == 0) {
		gen_rrep(my_address, My_SeqNum, input->SourceAddress, input->SourceSeqNum, 0, 0, _MESH_HEADER_TTL_, SeqNum, 0, 0, 0);
		My_SeqNum++;
		return 1;
	}

	if (isPortal && is_my_stat(input->first.DesAddress) > 0) {
		gen_rrep(my_address, My_SeqNum, input->SourceAddress, input->SourceSeqNum, 0, 0, _MESH_HEADER_TTL_, SeqNum, 1, input->SourceAddress, input->first.DesAddress);
		My_SeqNum++;
		return 1;
	}

	// case 4b
	if (input->first.DO == 1) {      //DO=1
		forward_rreq(input, TTL, SeqNum);
		return 1;
	}

#if 0 //never enter this section, mark out
	result = query_table(input->first.DesAddress, &Entry2);
	//DO=0
	if (result >= 0) {   //I have the information to destination

		// case 4c
		if (memcmp(Entry2.nexthopMAC, Entry1.nexthopMAC, ETH_ALEN) == 0) {
			/// Stanley
			// case 1:  nexthop = prehop, abort!
			// case 2: Happens when DO=0
			//    C: rreq issuer, X: rreq target
			//    A (current node): rrep issuer (DO=0, auto reply)
			//       A-----B (best)----C-----X
			//       |-------(worse)---|
			//    A has to check whether Path_to_X->next == nexthop of reponse, if true, abort
			return 0;
		}
		if (idx_pending >= 0) {
			// in the future (when pending update), case 4c will happen, so abort it.
			if (memcmp(Entry2.nexthopMAC, pending_path[idx_pending].nexthopMAC, ETH_ALEN) == 0) {
				return 0;
			}
		}

		// case 4d
		gen_rrep(Entry2.destMAC, Entry2.dsn, Entry1.destMAC, input->SourceSeqNum, (Entry2.metric), (Entry2.hopcount), My_Address, TTL, SeqNum, 0, 0, 0);

		if (input->first.RF == 1) { //RF=1
			// close by popen printf("***** send back RREP and generatie a RREQ with DO=1,RF=1,and broadcast it (daemon msg)\n");
			input->first.DO = 1;
			forward_rreq(input, My_Address, TTL, SeqNum);
		}
	} else {                  //I have no information to destination
		// case 4e
		forward_rreq(input, My_Address, TTL, SeqNum);
	}
#endif

	return 1;

}

#define BRCTL_SET_MESH_PATHSELPID 111
#define BRCTL_GET_MESH_PORTSTAT 112
#define CNT_REFRESH_STATION 3

int update_root_info(unsigned char root_addr[ETH_ALEN])
{
	return rtw_wx_mesh_update_routing_info(NODE_CFG_IFACE_IDX, root_addr);
}

/*
#ifndef IPC_BASED_PATHSEL
static int pathsel_init_fifo()
{

	// Here is an assumption that the fifo is created already by iwcontrol

	int flags;
	struct stat status;

	if(stat(PATHSEL_FIFO, &status) == 0)
		unlink(PATHSEL_FIFO);
	if((mkfifo(PATHSEL_FIFO, FILE_MODE) < 0)){
		// printf("mkfifo %s fifo error: %s!\n", DAEMON_FIFO, strerror(errno));
		return -1;
	}
	if((hapd->readfifo = open(PATHSEL_FIFO, O_RDONLY | O_NONBLOCK, 0)) < 0)
	{
		// printf("pathsel_init_fifo readfifo error\n");
	}

	HOSTAPD_DEBUG(HOSTAPD_DEBUG_MSGDUMPS, "hapd->readfifo = %d\n", hapd->readfifo);

	if ((flags = fcntl(hapd->readfifo, F_GETFL, 0)) < 0) {
		return -1;
	}
	else {
		flags |= O_NONBLOCK;
		if ((flags = fcntl(hapd->readfifo, F_SETFL, flags)) < 0) {
			// printf("F_SETFL: error\n");
			return -1;
		}
	}

	return 0;
}
#endif
*/

/*
#ifndef IPC_BASED_PATHSEL
static int pathsel_init_fifo()
{

	//Here is an assumption that the fifo is created already by iwcontrol

	FIFO_BUF = malloc(FIFO_SIZE);

	if(FIFO_BUF == NULL){
		return -1;
	}

	memset(FIFO_BUF,0,FIFO_SIZE);
	hapd->readfifo = FIFO_BUF;

	HOSTAPD_DEBUG(HOSTAPD_DEBUG_MSGDUMPS, "hapd->readfifo = %d\n", hapd->readfifo);

	return 0;
}
#endif
*/

//#ifdef IPC_BASED_PATHSEL

int gHasData = 0;

/*
int iw_sockets_open(void)
{
	static const int families[] = {
		AF_INET, AF_IPX, AF_AX25, AF_APPLETALK
	};
	unsigned int  i;
	int           sock;


	// Try all families we support
	for(i = 0; i < sizeof(families)/sizeof(int); ++i)
	{
		// Try to open the socket, if success returns it
		sock = socket(families[i], SOCK_DGRAM, 0);
		if(sock >= 0)
			return sock;
	}

	return -1;
}*/

/*
int iw_sockets_open(void)
{
  return sockets_open();
}
*/

#ifdef IPC_BASED_PATHSEL
/*
void callback_usr1()
{
	gHasData = 1;
	signal(SIGUSR1, SIG_IGN);
}
*/

void callback_usr1()
{
	gHasData = 1;
	//signal(SIGUSR1, SIG_IGN);
}

int SendPid()
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 0;
}

void dequeue_from_driver(struct rtwreq wrq, char *buf)
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
}

struct rtwreq datawrq;
unsigned char databuf[256];

#endif // IPC_BASE_PATHSEL

/*
static void do_daemon()
{
#ifndef IPC_BASED_PATHSEL
	int  nRead;
#endif
	unsigned char DesAdd[ETH_ALEN]={0};
	unsigned char SrcAdd[ETH_ALEN]={0};

	char *RecvQueueBuf;

	while (1)
	{

#ifdef IPC_BASED_PATHSEL
		pause();
		RecvQueueBuf = (char *)databuf;
		while(gHasData == 1)
		{
			datawrq.u.data.length = sizeof(databuf);
			datawrq.u.data.pointer = (caddr_t) databuf;
			dequeue_from_driver(datawrq, (char *)databuf);
#else
			RecvQueueBuf = (char *)&hapd->RecvBuf[5];
			nRead = read(hapd->readfifo, hapd->RecvBuf, MAX_MSG_SIZE);
			if (nRead > 0)
			{
#endif

			if(RecvQueueBuf[0]> 53) {

				struct rreq *ptr1;
				struct rrep *ptr2;
				struct rerr *ptr3;
				struct pann *pann_ptr;
				struct rann *rann_ptr;

				unsigned char PreHopAdd[ETH_ALEN]={0},TTL=0,RERR_flag=0;
				unsigned char Addr5[ETH_ALEN]={0}, Addr6[ETH_ALEN]={0};
				unsigned short SeqNum=0;
				unsigned int PreMetric=0;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
				unsigned char MyAdd[ETH_ALEN]={0};
				unsigned int RssiMetric=0;
				unsigned int CuMetric=0;
				unsigned int NoiseMetric=0;
#endif
				DOT11s_GEN_RREQ_PACKET *ptrA = NULL;
				DOT11s_GEN_RERR_PACKET *ptrB = NULL;
				DOT11s_RECV_PACKET *ptrC = NULL;

				ptrA = (DOT11s_GEN_RREQ_PACKET *)(RecvQueueBuf);
				ptrB = (DOT11s_GEN_RERR_PACKET *)(RecvQueueBuf);
				ptrC = (DOT11s_RECV_PACKET *)(RecvQueueBuf);

				memcpy(PreHopAdd, ptrC->PreHopMACAddr,ETH_ALEN);
				PreMetric=ptrC->Pre_Hop_Metric;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
				memcpy(MyAdd, ptrC->InterfaceMACAddr,ETH_ALEN);
				RssiMetric=ptrC->RSSI_Metric;
				CuMetric=ptrC->Cu_Metric;
				NoiseMetric=ptrC->Noise_Metric;
#endif
				TTL = ptrC->TTL;
				SeqNum = ptrC->Seq_num;
				switch(RecvQueueBuf[0])
				{
					case WIFI_EVENT_11S_PATHSEL_GEN_RREQ:								//gen rreq   54
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("gen RREQ\n%d  %d\n",ptrA->TTL,ptrA->Seq_num);
#endif
						memcpy(DesAdd,ptrA->destMACAddr,ETH_ALEN);
						TTL = ptrA->TTL;
						SeqNum = ptrA->Seq_num;
						gen_rreq(DesAdd,TTL,SeqNum);
						break;

					case WIFI_EVENT_11S_PATHSEL_GEN_RERR:								//gen rerr   55
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("gen RERR\n");
#endif
						memcpy(DesAdd,ptrB->SorNMACAddr,ETH_ALEN);
						memcpy(SrcAdd,ptrB->DataDestMAC,ETH_ALEN);
						memcpy(PreMAC,ptrB->PrehopMAC,ETH_ALEN);
						RERR_flag = ptrB->Flag;
						TTL = ptrB->TTL;
						SeqNum = ptrB->Seq_num;
						gen_rerr(DesAdd,SrcAdd,TTL,SeqNum,RERR_flag);
						break;
					case WIFI_EVENT_11S_PATHSEL_RECV_RREQ:                  //Recv RREQ  56
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RREQ\n");
#endif
						ptr1=(struct rreq*) (ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_rreq(ptr1, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace,TTL,SeqNum);
#else
						recv_rreq(ptr1, PreHopAdd, PreMetric, My_interFace,TTL,SeqNum);
#endif
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RREP:									//Recv RREP  57
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RREP\n");
#endif
						ptr2=(struct rrep*)(ptrC->ReceiveData);
						memcpy(Addr5,ptrC->MACAddr5,ETH_ALEN);
						memcpy(Addr6,ptrC->MACAddr6,ETH_ALEN);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_rrep(ptr2, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace,TTL,SeqNum,ptrC->Is6AddrFormat,Addr5,Addr6);
#else
						recv_rrep(ptr2, PreHopAdd, PreMetric, My_interFace,TTL,SeqNum,ptrC->Is6AddrFormat,Addr5,Addr6);
#endif

						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RERR:									//Recv RERR  58
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RERR\n");
#endif
						ptr3=(struct rerr*)(ptrC->ReceiveData);
						memcpy(DesAdd,ptrC->DesMACAddr,ETH_ALEN);
						recv_rerr(ptr3, PreHopAdd, TTL, SeqNum, DesAdd);
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_PANN:		//add by mcinnis 20070415
						pann_ptr = (struct pann *)(ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_pann(pann_ptr, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace, TTL, SeqNum);
#else
						recv_pann(pann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
#endif
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RANN:		//add by chuangch_ 20070507
						rann_ptr = (struct rann *)(ptrC->ReceiveData);
						recv_rann(rann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
						break;

					default:
#ifdef PATHSEL_DAEMON_DEBUG
						// printf("Receive undefine frame in pathslection.c\n");
#endif
						break;
				}
#ifdef PATH_OnDemand_Debug

				//dump_table();
 				//chuangch 2007.09.14
#endif
			} //end of switch
		} //end of if
	}   //end of while

}
*/

/*
static void do_daemon()
{
#ifndef IPC_BASED_PATHSEL
	int  nRead = 0;
#endif
	unsigned char DesAdd[ETH_ALEN]={0};
	unsigned char SrcAdd[ETH_ALEN]={0};

	char *RecvQueueBuf;

	while (1)
	{

#ifdef IPC_BASED_PATHSEL
		callback_usr1();//pause();
		RecvQueueBuf = (char *)databuf;
		while(gHasData == 1)
		{
			datawrq.u.data.length = sizeof(databuf);
			datawrq.u.data.pointer = (caddr_t) databuf;
			dequeue_from_driver(datawrq, (char *)databuf);
#else
			RecvQueueBuf = (char *)&hapd->RecvBuf[5];
			//nRead = read(hapd->readfifo, hapd->RecvBuf, MAX_MSG_SIZE);
            //hunter: wrong implimentation !
            _FIXIT_
            for(int i = 0 ; i < MAX_MSG_SIZE; i++){
              nRead = i;
              if (hapd->readfifo[i] == '\n'){
                break;
              }
              hapd->RecvBuf[i] = hapd->readfifo[i];
            }

			if (nRead > 0)
			{
#endif

			if(RecvQueueBuf[0]> 53) {

				struct rreq *ptr1;
				struct rrep *ptr2;
				struct rerr *ptr3;
				struct pann *pann_ptr;
				struct rann *rann_ptr;

				unsigned char PreHopAdd[ETH_ALEN]={0},TTL=0,RERR_flag=0;
				unsigned char Addr5[ETH_ALEN]={0}, Addr6[ETH_ALEN]={0};
				unsigned short SeqNum=0;
				unsigned int PreMetric=0;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
				unsigned char MyAdd[ETH_ALEN]={0};
				unsigned int RssiMetric=0;
				unsigned int CuMetric=0;
				unsigned int NoiseMetric=0;
#endif
				DOT11s_GEN_RREQ_PACKET *ptrA = NULL;
				DOT11s_GEN_RERR_PACKET *ptrB = NULL;
				DOT11s_RECV_PACKET *ptrC = NULL;

				ptrA = (DOT11s_GEN_RREQ_PACKET *)(RecvQueueBuf);
				ptrB = (DOT11s_GEN_RERR_PACKET *)(RecvQueueBuf);
				ptrC = (DOT11s_RECV_PACKET *)(RecvQueueBuf);

				memcpy(PreHopAdd, ptrC->PreHopMACAddr,ETH_ALEN);
				PreMetric=ptrC->Pre_Hop_Metric;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
				memcpy(MyAdd, ptrC->InterfaceMACAddr,ETH_ALEN);
				RssiMetric=ptrC->RSSI_Metric;
				CuMetric=ptrC->Cu_Metric;
				NoiseMetric=ptrC->Noise_Metric;
#endif
				TTL = ptrC->TTL;
				SeqNum = ptrC->Seq_num;
				switch(RecvQueueBuf[0])
				{
					case WIFI_EVENT_11S_PATHSEL_GEN_RREQ:								//gen rreq   54
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("gen RREQ\n%d  %d\n",ptrA->TTL,ptrA->Seq_num);
#endif
						memcpy(DesAdd,ptrA->destMACAddr,ETH_ALEN);
						TTL = ptrA->TTL;
						SeqNum = ptrA->Seq_num;
						gen_rreq(DesAdd,TTL,SeqNum);
						break;

					case WIFI_EVENT_11S_PATHSEL_GEN_RERR:								//gen rerr   55
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("gen RERR\n");
#endif
						memcpy(DesAdd,ptrB->SorNMACAddr,ETH_ALEN);
						memcpy(SrcAdd,ptrB->DataDestMAC,ETH_ALEN);
						memcpy(PreMAC,ptrB->PrehopMAC,ETH_ALEN);
						RERR_flag = ptrB->Flag;
						TTL = ptrB->TTL;
						SeqNum = ptrB->Seq_num;
						gen_rerr(DesAdd,SrcAdd,TTL,SeqNum,RERR_flag);
						break;
					case WIFI_EVENT_11S_PATHSEL_RECV_RREQ:                  //Recv RREQ  56
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RREQ\n");
#endif
						ptr1=(struct rreq*) (ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_rreq(ptr1, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace,TTL,SeqNum);
#else
						recv_rreq(ptr1, PreHopAdd, PreMetric, My_interFace,TTL,SeqNum);
#endif
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RREP:									//Recv RREP  57
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RREP\n");
#endif
						ptr2=(struct rrep*)(ptrC->ReceiveData);
						memcpy(Addr5,ptrC->MACAddr5,ETH_ALEN);
						memcpy(Addr6,ptrC->MACAddr6,ETH_ALEN);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_rrep(ptr2, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace,TTL,SeqNum,ptrC->Is6AddrFormat,Addr5,Addr6);
#else
						recv_rrep(ptr2, PreHopAdd, PreMetric, My_interFace,TTL,SeqNum,ptrC->Is6AddrFormat,Addr5,Addr6);
#endif

						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RERR:									//Recv RERR  58
#ifdef PATHSEL_DAEMON_DEBUG
						//printf("Receive RERR\n");
#endif
						ptr3=(struct rerr*)(ptrC->ReceiveData);
						memcpy(DesAdd,ptrC->DesMACAddr,ETH_ALEN);
						recv_rerr(ptr3, PreHopAdd, TTL, SeqNum, DesAdd);
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_PANN:		//add by mcinnis 20070415
						pann_ptr = (struct pann *)(ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
						recv_pann(pann_ptr, PreHopAdd, MyAdd,PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace, TTL, SeqNum);
#else
						recv_pann(pann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
#endif
						break;

					case WIFI_EVENT_11S_PATHSEL_RECV_RANN:		//add by chuangch_ 20070507
						rann_ptr = (struct rann *)(ptrC->ReceiveData);
						recv_rann(rann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
						break;

					default:
#ifdef PATHSEL_DAEMON_DEBUG
						// printf("Receive undefine frame in pathslection.c\n");
#endif
						break;
				}
#ifdef PATH_OnDemand_Debug

				//dump_table();
 				//chuangch 2007.09.14
#endif
			} //end of switch
		} //end of if
	}   //end of while

}
*/

int hwmp_init(void)
{

	/*
	if ((g_rawsocket_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
	    printf("Error: init g_rawsocket_fd failed\n");
	    return -1;
	}*/

	//try: IPPROTO_IP 0, IPPROTO_UDP 17

	if ((g_rawsocket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == -1) {
		printf("Error: init g_rawsocket_fd failed\n");
		return -1;
	}

	if (init_root() < 0) {
		printf("Error: init_root() failed\n");
		return -1;
	}

	return 0;
}

//static void do_daemon()
void hwmp_hdl(unsigned char *buf, int buf_len, int flags, void *userdata)
{
	static char initialized = 0;

	(initialized == 0) ? ((hwmp_init() < 0) ? (NULL /*root init failed handler*/) : (initialized = 1)) : (NULL /*root already initialized*/);

	unsigned char *RecvQueueBuf = buf;

	unsigned char DesAdd[ETH_ALEN] = {0};
	unsigned char SrcAdd[ETH_ALEN] = {0};
	struct rreq *ptr1;
	struct rrep *ptr2;
	struct rerr *ptr3;

	struct preq *preq_ptr;
	struct prep *prep_ptr;

	struct pann *pann_ptr;
	struct rann *rann_ptr;

	unsigned char PreHopAdd[ETH_ALEN] = {0}, TTL = 0, RERR_flag = 0;
	unsigned char Addr5[ETH_ALEN] = {0}, Addr6[ETH_ALEN] = {0};
	unsigned short SeqNum = 0;
	unsigned int PreMetric = 0;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned char MyAdd[ETH_ALEN] = {0};
	unsigned int RssiMetric = 0;
	unsigned int CuMetric = 0;
	unsigned int NoiseMetric = 0;
#endif

	DOT11s_GEN_RREQ_PACKET *ptrA = NULL;
	DOT11s_GEN_RERR_PACKET *ptrB = NULL;
	DOT11s_RECV_PACKET *ptrC = NULL;
	DOT11s_GEN_PREQ_PACKET *ptrD = NULL;
	DOT11s_HWMP_PACKET *ptrE = NULL;

	ptrA = (DOT11s_GEN_RREQ_PACKET *)(RecvQueueBuf);
	ptrB = (DOT11s_GEN_RERR_PACKET *)(RecvQueueBuf);
	ptrC = (DOT11s_RECV_PACKET *)(RecvQueueBuf);
	ptrD = (DOT11s_GEN_PREQ_PACKET *)(RecvQueueBuf);
	ptrE = (DOT11s_HWMP_PACKET *)(RecvQueueBuf);

	memcpy(PreHopAdd, ptrC->PreHopMACAddr, ETH_ALEN);
	PreMetric = ptrC->Pre_Hop_Metric;

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	memcpy(MyAdd, ptrC->InterfaceMACAddr, ETH_ALEN);
	RssiMetric = ptrC->RSSI_Metric;
	CuMetric = ptrC->Cu_Metric;
	NoiseMetric = ptrC->Noise_Metric;
#endif

	TTL = ptrC->TTL;
	SeqNum = ptrC->Seq_num;

	//#JZDBG
	if (WIFI_EVENT_11S_PATHSEL_RECV_RANN != RecvQueueBuf[0]) { //too many log, don't print WIFI_EVENT_PATHSEL_RECV_RANN
		printf("RecvQueueBuf[0]:%d\n", RecvQueueBuf[0]);
	}
	switch (RecvQueueBuf[0]) {

	case WIFI_EVENT_11S_PATHSEL_GEN_PREQ:
		memcpy(DesAdd, ptrD->destMACAddr, ETH_ALEN);
		//TTL = ptrD->TTL;
		// SeqNum = ptrD->Seq_num;
		gen_preq(ptrE);
		//gen_preq(DesAdd, TTL, SeqNum);
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_PREQ:
		// ptr1=(struct Preq*) (ptrC->ReceiveData);
		//recv_preq(ptr1, PreHopAdd, PreMetric, My_interFace,TTL,SeqNum);
		recv_preq(ptrE, My_interFace);
		break;

	case WIFI_EVENT_11S_PATHSEL_GEN_RREQ:
		memcpy(DesAdd, ptrA->destMACAddr, ETH_ALEN);
		TTL = ptrA->TTL;
		SeqNum = ptrA->Seq_num;
		gen_rreq(DesAdd, TTL, SeqNum);
		break;

	case WIFI_EVENT_11S_PATHSEL_GEN_RERR:
		memcpy(DesAdd, ptrB->SorNMACAddr, ETH_ALEN);
		memcpy(SrcAdd, ptrB->DataDestMAC, ETH_ALEN);
		memcpy(PreMAC, ptrB->PrehopMAC, ETH_ALEN);
		RERR_flag = ptrB->Flag;
		TTL = ptrB->TTL;
		SeqNum = ptrB->Seq_num;
		gen_rerr(DesAdd, SrcAdd, TTL, SeqNum, RERR_flag);
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_RREQ:
		ptr1 = (struct rreq *)(ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
		recv_rreq(ptr1, PreHopAdd, MyAdd, PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace, TTL, SeqNum);
#else
		recv_rreq(ptr1, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
#endif
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_RREP:
		ptr2 = (struct rrep *)(ptrC->ReceiveData);
		memcpy(Addr5, ptrC->MACAddr5, ETH_ALEN);
		memcpy(Addr6, ptrC->MACAddr6, ETH_ALEN);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
		recv_rrep(ptr2, PreHopAdd, MyAdd, PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace, TTL, SeqNum, ptrC->Is6AddrFormat, Addr5, Addr6);
#else
		recv_rrep(ptr2, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum, ptrC->Is6AddrFormat, Addr5, Addr6);
#endif
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_RERR:
		ptr3 = (struct rerr *)(ptrC->ReceiveData);
		memcpy(DesAdd, ptrC->DesMACAddr, ETH_ALEN);
		recv_rerr(ptr3, PreHopAdd, TTL, SeqNum, DesAdd);
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_PANN:
		pann_ptr = (struct pann *)(ptrC->ReceiveData);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
		recv_pann(pann_ptr, PreHopAdd, MyAdd, PreMetric, RssiMetric, CuMetric, NoiseMetric, My_interFace, TTL, SeqNum);
#else
		recv_pann(pann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
#endif
		break;

	case WIFI_EVENT_11S_PATHSEL_RECV_RANN:
		rann_ptr = (struct rann *)(ptrC->ReceiveData);
		recv_rann(rann_ptr, PreHopAdd, PreMetric, My_interFace, TTL, SeqNum);
		break;

	default: //should not happen
		break;
	}
}

/*
void test_rawsocket_send(int signo)
{

	//printf("Catch a signal -- SIGALRM1 \n");


//------------!!! filled in correct number !!!-----------------//
//======= the destination is means destination for RREQ want to find============//
// change by bc5678 2007.1.16
	unsigned char dest_mac_addr[6] = {0x00, 0xE0, 0x4C, 0x86, 0x01, 0x97};
	unsigned char nexthop_mac_addr[6] = {0x00, 0xE0, 0x4C, 0x86, 0x01, 0x97};
	unsigned char src_mac_addr[6] = {0x00, 0xE0, 0x4C, 0x86, 0x90, 0x5D};
	unsigned char *ra, *ta, *da, *sa;
	int i;
	// unsigned char broadcast_mac_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	// create socket
	/ *
	if ((s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		// fprintf(stderr, "Error: can't socket()\n");
	}
	* /
/ *
	if ((g_rawsocket_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		// fprintf(stderr, "Error: can't socket()\n");
	}
	* /
	struct sockaddr_ll sll; // target address
	int send_result = 0;

	memset(&sll, 0, sizeof(sll));

	// prepare sockaddr_ll

	// index of the network device
	//sll.sll_ifindex	= get_iface_index(g_rawsocket_fd, interface_ioctl);
	sll.sll_ifindex	= get_iface_index(g_rawsocket_fd, interface_rawsocket);
	// address length
	sll.sll_halen = ETH_ALEN; // 6

	sll.sll_family = PF_PACKET;

//	unsigned char meshframe[1024]; // buffer for mesh frame
//	memset(meshframe, 0, sizeof(meshframe));

	// MAC - begin
	memset(sll.sll_addr, 0, 8);
	memset(sll.sll_addr, 0xff, 6);
	memcpy(sll.sll_addr, src_mac_addr, 6);
	// MAC - end

//	sll.sll_protocol = htons(ETH_P_IP);
//	sll.sll_protocol = ETH_P_ALL;
	// sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_protocol = 0;


	// ARP hardware identifier is ethernet
	// sll.sll_hatype = ARPHRD_ETHER;
	sll.sll_hatype = 0;

	// target is another host
	//sll.sll_pkttype = PACKET_OTHERHOST;
	sll.sll_pkttype = 0;

// ====================== change by bc5678
	int zero_offset = 14;
	unsigned char meshframe[256]; // buffer for mesh frame
	memset(meshframe, 0x0, sizeof(meshframe));

	// Type & Subtype
    SetFrameSubType(meshframe+zero_offset, WIFI_11S_MESH_ACTION);
    SetToDs(meshframe+zero_offset);
    SetFrDs(meshframe+zero_offset);

    //      next hop
    ra = GetAddr1Ptr(meshframe+zero_offset);
    memcpy(ra, nexthop_mac_addr, 6);

    // transmitter = source = self
    ta = GetAddr2Ptr(meshframe+zero_offset);
    sa = GetAddr4Ptr(meshframe+zero_offset);
    for (i = 0; i < 6;i++) {
            ta[i] = sa[i] = src_mac_addr[i];
    }

    // destination addr
    da = GetAddr3Ptr(meshframe+zero_offset);
    memcpy(da, dest_mac_addr, 6);


	// rreq



	struct rreq * ptr = (struct rreq*)(meshframe+zero_offset+34);

	//modify by chuangch 2007/04/03

  ptr->Category     =5;
	ptr->Action     =2;
	ptr->Id         =0;    //T.B.D
	ptr->Length         =32;
	ptr->ModeFlags  =1;
	ptr->HopCount       =0;
	ptr->ttl        =255;
	//ptr->DesCount       =1;
	ptr->RreqId     = 0;
	//ptr->SourceAddress = macAddr_atoi(src_mac_addr);
	ptr->SourceSeqNum   = 0;
	ptr->Lifetime = 255;
	ptr->Metric     =0;


//	memset(meshframe, 0xD1, 6);
//	memcpy(meshframe+6, src_mac_addr, 6);
//	memset(meshframe+12, 0xEA, 2);
//	memset(meshframe+14, 0xFD, 50);

	// send the packet
	if ((send_result = sendto(g_rawsocket_fd, meshframe, 14+34+sizeof(struct rreq), 0, (struct sockaddr*)&sll, sizeof(sll))) == -1) {
		// fprintf(stderr, "Error: can't sendto()\n");
	} else {
		// printf(".");
	}


  //  signal(SIGALRM, test_rawsocket_send);
	return;
}

// The following function is written by Jason
void test_rawsocket_unicast(int signo)
{

	//printf("Catch a signal -- SIGALRM1 \n");


//------------!!! filled in correct number !!!-----------------//
//======= the destination is means destination for RREQ want to find============//
// change by bc5678 2007.1.16
	unsigned char dest_mac_addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char nexthop_mac_addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char src_mac_addr[6] = {0x00, 0xE0, 0x4C, 0x88, 0x88, 0x88};
	unsigned char *ra, *ta, *da, *sa;
	int i;
	// unsigned char broadcast_mac_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	// create socket
	/ *
	if ((s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		// fprintf(stderr, "Error: can't socket()\n");
	}
	* /
/ *
	if ((g_rawsocket_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		// fprintf(stderr, "Error: can't socket()\n");
	}
	* /
	struct sockaddr_ll sll; // target address
	int send_result = 0;

	memset(&sll, 0, sizeof(sll));

	// prepare sockaddr_ll

	// index of the network device

	sll.sll_ifindex	= get_iface_index(g_rawsocket_fd, interface_rawsocket);
	//sll.sll_ifindex	= get_iface_index(g_rawsocket_fd, interface_ioctl);
	// address length
	sll.sll_halen = ETH_ALEN; // 6

	sll.sll_family = PF_PACKET;

//	unsigned char meshframe[1024]; // buffer for mesh frame
//	memset(meshframe, 0, sizeof(meshframe));

	// MAC - begin
	memset(sll.sll_addr, 0, 8);
	memset(sll.sll_addr, 0xff, 6);
	memcpy(sll.sll_addr, src_mac_addr, 6);
	// MAC - end

//	sll.sll_protocol = htons(ETH_P_IP);
//	sll.sll_protocol = ETH_P_ALL;
	// sll.sll_protocol = htons(ETH_P_ALL);
	sll.sll_protocol = 0;


	// ARP hardware identifier is ethernet
	// sll.sll_hatype = ARPHRD_ETHER;
	sll.sll_hatype = 0;

	// target is another host
	//sll.sll_pkttype = PACKET_OTHERHOST;
	sll.sll_pkttype = 0;

// ====================== change by bc5678
	int zero_offset = 14;
	unsigned char meshframe[256]; // buffer for mesh frame
	memset(meshframe, 0x0, sizeof(meshframe));

	// Type & Subtype
    SetFrameSubType(meshframe+zero_offset, WIFI_11S_MESH_ACTION);
    SetToDs(meshframe+zero_offset);
    SetFrDs(meshframe+zero_offset);

    //      next hop
    ra = GetAddr1Ptr(meshframe+zero_offset);
    memcpy(ra, nexthop_mac_addr, 6);

    // transmitter = source = self
    ta = GetAddr2Ptr(meshframe+zero_offset);
    sa = GetAddr4Ptr(meshframe+zero_offset);
    for (i = 0; i < 6;i++) {
            ta[i] = sa[i] = src_mac_addr[i];
    }

    // destination addr
    da = GetAddr3Ptr(meshframe+zero_offset);
    memcpy(da, dest_mac_addr, 6);


	// rreq



	struct rreq * ptr = (struct rreq*)(meshframe+zero_offset+34);

  ptr->Category     =5;
	ptr->Action     =2;
	ptr->Id         =0;    //T.B.D
	ptr->Length         =32;
	ptr->ModeFlags  =1;
	ptr->ttl        =255;
	//ptr->DesCount       =1;
	ptr->HopCount       =0;
	ptr->RreqId     = 0;
	//ptr->SourceAddress = macAddr_atoi(src_mac_addr);
	ptr->SourceSeqNum   = 0;
	ptr->Metric     =0;


//	memset(meshframe, 0xD1, 6);
//	memcpy(meshframe+6, src_mac_addr, 6);
//	memset(meshframe+12, 0xEA, 2);
//	memset(meshframe+14, 0xFD, 50);

	// send the packet
	if ((send_result = sendto(g_rawsocket_fd, meshframe, 14+34+sizeof(struct rreq), 0, (struct sockaddr*)&sll, sizeof(sll))) == -1) {
		// fprintf(stderr, "Error: can't sendto()\n");
	} else {
		// printf(".");
	}


  //  signal(SIGALRM, test_rawsocket_send);
	return;
}

*/

int init_root(void)
{
	unsigned char buf[2] = {0};
	unsigned char zeromac[ETH_ALEN] = {0};
	int ret = -1;

	if (isPortal == 1 && toBeRoot == 1) {
		//system("brctl stp br0 on");

		if (memcmp(rann_root_entry.mac, my_address, ETH_ALEN) > 0 || !memcmp(rann_root_entry.mac, zeromac, ETH_ALEN)) {
			isRoot = 1;
			memcpy(rann_root_entry.mac, my_address, ETH_ALEN);
			rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
			rann_root_entry.flag = 1;
			update_root_info(my_address);
		}
	} else {
		//system("brctl stp br0 off");

		if (memcmp(rann_root_entry.mac, my_address, ETH_ALEN) == 0) {
			isRoot = 0;
			memset(rann_root_entry.mac, 0, ETH_ALEN);
			rann_root_entry.flag = 0;
			update_root_info(zeromac);
		}
	}
	reset_timer();

	// Get wireless name
	//memset(wrq.ifr_name, 0, sizeof wrq.ifr_name);
	//strncpy(wrq.ifr_name, interface_ioctl, IFNAMSIZ);

	buf[0] = toBeRoot;
	buf[1] = isPortal;

	if (rtw_wx_mesh_set_routing_info(NODE_CFG_IFACE_IDX, buf) < 0) {
		// printf("SIOC_SET_ROUTING_INFO  Error:%s\n\a",strerror(errno));
		goto init_root_err;
	}

	ret = 0;

init_root_err:
	return ret;
}

void callback_usr2()
{
	int state = GetMeshPortStat();
	if (state >= 0) {
		if (state == BR_STATE_BLOCKING && isPortal == 1) {
			printf("[pathsel]  Mesh Portal Disable\n");
			isPortal = 0;
			init_root();
		} else if (state == BR_STATE_FORWARDING && isPortal == 0) {
			printf("[pathsel]  Mesh Portal Enalbe\n");
			isPortal = 1;
			init_root();
		}
	}
}

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
static int get_iface_mib(int fd, const char *interface_name, char *mib_name, unsigned char *mib_variable, int size)
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 1;
}


static int set_iface_mib(int fd, const char *interface_name, char *mib_name, unsigned char value)
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 1;
}

int init_metricMibValue(char *interfaceName, char interfaceIs5G)
{
	//initialization based on 5G or 2G
	if (interfaceIs5G == IS_5G_BAND) {
		mibValuePointer = &mibValueOf5G;
	} else {
		mibValuePointer = &mibValueOf2G;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_rssiThreshold", mibValuePointer->rssi_threshold) < 0) {
		goto exit;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_cuThreshold", mibValuePointer->cu_threshold) < 0) {
		goto exit;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_noiseThreshold", mibValuePointer->noise_threshold) < 0) {
		goto exit;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_rssiWeight", mibValuePointer->rssi_weight) < 0) {
		goto exit;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_cuWeight", mibValuePointer->cu_weight) < 0) {
		goto exit;
	}

	if (set_iface_mib(g_rawsocket_fd, interfaceName, "mesh_noiseWeight", mibValuePointer->noise_weight) < 0) {
		goto exit;
	}

	return 1;
exit:
	printf("Set wlan mib failed\n");
	return -1;
}
#endif

/*
int main(int argc, char *argv[])
{
    //================================================
    // struct ifreq ifr;
    int opt;

    signal(SIGUSR1, SIG_IGN);

    //Generate Hello Random Number
    // srand( time(NULL) );
    // Hello_random = (unsigned short) (rand()%10000);


#ifdef _RTL8187B_
    if(geteuid() != 0)
    {
        printf("%s ERROR: Please run this program with 'root'\n", argv[0]);
        exit(1);
    }
#endif
    // parse arguments
    memset(hapd->wlan_iface, 0, sizeof(hapd->wlan_iface));

    hapd->debug = HOSTAPD_DEBUG_NO;
    hapd->is_daemon = 0;

    while ((opt = getopt(argc, argv, "n:i:PRd")) > 0) {
        switch (opt) {
            case 'P':
                isPortal = 1;
                break;
            case 'R':
                toBeRoot = 1;
                break;
            case 'd':
                hapd->is_daemon = 1;
                break;
            case 'n':
                hapd->debug = atoi(optarg);
                break;
            case 'i':
                strcpy(interface_ioctl, optarg);
                break;
            default:
                printf("Usage: pathsel interface [-P] [-R] [-d] [-i interface] [-n 0/1/2/3]\n");
                return -1;
        } // end of switch
    } // end of while

    if(strlen(interface_ioctl) == 0) {
        printf("pathsel error! : no mesh interface");
        return -1;
    }

    strcpy(interface_rawsocket, interface_ioctl);

    // destroy old process and create a PID file
    {
        int pid_fd;
        FILE *fp;
        char line[20], pid_file_name[100];
        pid_t pid;

        snprintf(pid_file_name, sizeof(pid_file_name),"/var/run/pathsel-%s.pid", interface_ioctl);

        if ((fp = fopen(pid_file_name, "r")) != NULL) {
            fgets(line, sizeof(line), fp);
            if (sscanf(line, "%d", &pid)) {
                if (pid > 1)
                    kill(pid, SIGTERM);
            }
            fclose(fp);
        }
        pid_fd = pidfile_acquire(pid_file_name);
        if (pid_fd < 0)
            return 0;

        if(hapd->is_daemon == 1) {
            if (daemon(0,1) == -1) {
                printf("fork pathselection error!\n");
                exit(1);
            }
        }
        pidfile_write_release(pid_fd);
    }

	// fifo initialization
#ifndef IPC_BASED_PATHSEL
    if (pathsel_init_fifo() < 0) {
        printf("Init fifo fail.\n");
        return -1;
    }
#endif

    if ((g_rawsocket_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        fprintf(stderr, "Error: can't socket()\n");
        return -1;
    }

    // Create a channel to the NET kernel
    if((g_ioctrl_socket_fd = sockets_open()) < 0)
    {
        printf("Init g_ioctrl_socket_fd fail.\n");
        close(g_rawsocket_fd);
        return -1;
    }

	if(get_iface_name(g_rawsocket_fd, interface_ioctl, my_address) <0) {
		printf("Get rann address failed!\n");
		return -1;
	}

#ifdef CONFIG_RTK_MESH_METRIC_REFINE

	if(get_iface_name(g_rawsocket_fd, wlan0_ifName, wlan0_hwAddress) <0) {
		printf("Get wlan0 address failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan0_ifName,"phyBandSelect", &wlan0_is5G, 1 ) <0) {
		printf("Get wlan0 phyBandSelect mib failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan0_ifName,"mesh_crossbandEnable", &wlan0_crossbandEnable, 1 ) <0) {
		printf("Get wlan0 crossbandEnable mib failed!\n");
		return -1;
	}

	init_metricMibValue(wlan0_ifName, wlan0_is5G);

#ifdef FOR_DUAL_BAND
	if(get_iface_name(g_rawsocket_fd, wlan1_ifName, wlan1_hwAddress) <0) {
		printf("Get wlan1 address failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan1_ifName,"phyBandSelect", &wlan1_is5G, 1 ) <0) {
		printf("Get wlan1 phyBandSelect mib failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan1_ifName,"mesh_crossbandEnable", &wlan1_crossbandEnable, 1 ) <0) {
		printf("Get wlan1 crossbandEnable mib failed!\n");
		return -1;
	}

	init_metricMibValue(wlan1_ifName, wlan1_is5G);
#endif

#endif

#ifdef IPC_BASED_PATHSEL
	signal(SIGUSR1, callback_usr1);
	if(SendPid() < 0) {
        printf("pathsel error! : send pid fail\n");
        return -1;
    }
#endif

    if( init_root() < 0 ) {
        printf("Init root failed\n");
        return -1;
    }

    signal(SIGUSR2, callback_usr2);
	if(SendPidToBridge() < 0) {
        printf("pathsel error! : send pid to bridge fail\n");
        return -1;
    }

    signal(SIGALRM, TimerFunc);
    alarm(1);
    do_daemon();

    close(g_rawsocket_fd);
    close(g_ioctrl_socket_fd);
    g_rawsocket_fd = -1;
    g_ioctrl_socket_fd = -1;

    return 0;
}*/

/*
int _main_(int argc, char *argv[])
{
    //================================================
    // struct ifreq ifr;

    //signal(SIGUSR1, SIG_IGN);

    //Generate Hello Random Number
    // srand( time(NULL) );
    // Hello_random = (unsigned short) (rand()%10000);


    // parse arguments
    memset(hapd->wlan_iface, 0, sizeof(hapd->wlan_iface));

    hapd->debug = HOSTAPD_DEBUG_NO;
    hapd->is_daemon = NODE_CFG_DAEMON;
	isPortal = NODE_CFG_PORTOAL;
	toBeRoot = NODE_CFG_BE_ROOT;
	strcpy(interface_ioctl, NODE_CFG_IFNAME);

    strcpy(interface_rawsocket, interface_ioctl);

    // destroy old process and create a PID file

	// fifo initialization
#ifndef IPC_BASED_PATHSEL
    if (pathsel_init_fifo() < 0) {
        printf("Init fifo fail.\n");
        return -1;
    }
#endif

    if ((g_rawsocket_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        //fprintf(stderr, "Error: can't socket()\n");
        printf("Error: can't socket()\n");
        return -1;
    }

    // Create a channel to the NET kernel
    if((g_ioctrl_socket_fd = sockets_open()) < 0)
    {
        printf("Init g_ioctrl_socket_fd fail.\n");
        close(g_rawsocket_fd);
        return -1;
    }

	if(get_iface_name(g_rawsocket_fd, interface_ioctl, my_address) <0) {
		printf("Get rann address failed!\n");
		return -1;
	}

#ifdef CONFIG_RTK_MESH_METRIC_REFINE

	if(get_iface_name(g_rawsocket_fd, wlan0_ifName, wlan0_hwAddress) <0) {
		printf("Get wlan0 address failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan0_ifName,"phyBandSelect", &wlan0_is5G, 1 ) <0) {
		printf("Get wlan0 phyBandSelect mib failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan0_ifName,"mesh_crossbandEnable", &wlan0_crossbandEnable, 1 ) <0) {
		printf("Get wlan0 crossbandEnable mib failed!\n");
		return -1;
	}

	init_metricMibValue(wlan0_ifName, wlan0_is5G);

#ifdef FOR_DUAL_BAND
	if(get_iface_name(g_rawsocket_fd, wlan1_ifName, wlan1_hwAddress) <0) {
		printf("Get wlan1 address failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan1_ifName,"phyBandSelect", &wlan1_is5G, 1 ) <0) {
		printf("Get wlan1 phyBandSelect mib failed!\n");
		return -1;
	}

	if(get_iface_mib(g_rawsocket_fd, wlan1_ifName,"mesh_crossbandEnable", &wlan1_crossbandEnable, 1 ) <0) {
		printf("Get wlan1 crossbandEnable mib failed!\n");
		return -1;
	}

	init_metricMibValue(wlan1_ifName, wlan1_is5G);
#endif

#endif

#ifdef IPC_BASED_PATHSEL
	signal(SIGUSR1, callback_usr1);
	if(SendPid() < 0) {
        printf("pathsel error! : send pid fail\n");
        return -1;
    }
#endif

    if( init_root() < 0 ) {
        printf("Init root failed\n");
        return -1;
    }

    //signal(SIGUSR2, callback_usr2);
    callback_usr2();

	if(SendPidToBridge() < 0) {
        printf("pathsel error! : send pid to bridge fail\n");
        return -1;
    }

    //signal(SIGALRM, TimerFunc);
    //alarm(1);

    //do_daemon();

    close(g_rawsocket_fd);
    close(g_ioctrl_socket_fd);
    g_rawsocket_fd = -1;
    g_ioctrl_socket_fd = -1;

    return 0;
}
*/

/*
void HelloMessage()
{
	unsigned char MyAdd[ETH_ALEN]={0};

	if(get_iface_name(g_rawsocket_fd, interface_rawsocket, MyAdd) <0) {
		// printf("Get my address errror!\n");
	}
	else {
		Hello_random++;
		unsigned char Hello[4] ={5,9,8,2};
		send_packet(Hello,MyAdd,broadcast_mac_addr,MyAdd,1,1,Hello_random);
		Hello_random  = Hello_random % 10000;
	}

	return;
}
*/

int SendPidToBridge()
{
	int ret = 0;
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return ret;
}

int GetMeshPortStat()
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 1;
}

void TimerFunc()
{
	static int i = 0;

	i = (i + 1) % 16;

	if (i == 0x08) {	// every 16 secs
#ifdef IPC_BASED_PATHSEL
		SendPid();
#endif
	}


	pann_handler();		// every 1 sec
	//alarm(1);
	vTaskDelay(1000);
}

/****************************************************************************
 *
 * NAME: gen_rreq_ttl_1
 *
 * DESCRIPTION: generate RREQ with "ttl = 1" and broadcast it.
 *
 * PARAMETERS:      Name           Description
 *                  My_Address     my mac address
 *                  DesAdd         the destination's MAC address of RREQ
 *                  TTL            time to live
 *                  SeqNum         sequence number
 * RETURNS:         1
 *
 ****************************************************************************/
/*
int gen_rreq_ttl_1(unsigned char *My_Address,unsigned char *DesAdd,unsigned char TTL,unsigned short SeqNum)
{
	struct rreq  ptr;

	ptr.Category = 5;
	ptr.Action = 2;
	ptr.Id = 30;    //T.B.D
	ptr.Length = sizeof(struct rreq) - 2;
	ptr.ModeFlags = 1;
	ptr.HopCount = 0;
	ptr.ttl = 1;
	//ptr.DesCount     =1;
	ptr.RreqId = My_RreqId;
	memcpy(ptr.SourceAddress,My_Address,6);
	ptr.SourceSeqNum = htonl(My_SeqNum);
	ptr.Lifetime = htonl(255);
	ptr.Metric = htonl(0);
	ptr.first.DO = 0;           //initial DO=0; RF=1;
	ptr.first.RF = 1;
	ptr.first.Reserved = 0;
	memcpy(ptr.first.DesAddress,DesAdd,6);
	ptr.first.DesSeqNum = htonl(0);     //if maintain ,this is not 0

	My_SeqNum++;
	My_RreqId++;

	// @@@ Be careful, SeqNum is a two bytes number
	send_packet((unsigned char*)&ptr, My_Address, DesAdd, My_Address, 1, TTL, SeqNum);   //1 for broadcast

	return 1;
}
*/

void reset_timer()
{
	int i = 0;
	for (i = 0; i < MAX_TIMER_INSTANCE ; i++)
		if (i == 0 || i == 3 || i == 6) {
			i_timer[i].c_count = i_timer[i].p_count;
		}
}


/****************************************************************************
 *
 * NAME: pann_handler
 *
 * DESCRIPTION: Per second execute func¡ABasis device (mpp, root, ap) execute other func
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:         1
 * None.
 ****************************************************************************/
void pann_handler()
{

	int i = 0;
	if (isRoot == 0 && isPortal == 0) {
		for (i = 0; i < MAX_TIMER_INSTANCE - 1; i++) {
			if (i == 0 || i == 3) {
				continue;
			}
			i_timer[i].c_count--;
			if (i_timer[i].c_count == 0) {
				i_timer[i].func();
				i_timer[i].c_count = i_timer[i].p_count;
			}
		}
	}
	if (isRoot == 0 && isPortal == 1) {         // portal
		for (i = 0; i < MAX_TIMER_INSTANCE; i++) {
			if (i == 3) {
				continue;    // portal don't gen RANN
			}
			if (i == 6 && rootDecision == 1) {
				continue;    // have made decision to root
			}
			i_timer[i].c_count--;
			if (i_timer[i].c_count == 0) {
				i_timer[i].func();
				i_timer[i].c_count = i_timer[i].p_count;
			}
		}
	} else if (isRoot == 1 && isPortal == 1) {			 // root
		for (i = 0; i < 4; i++) {       // root don't retx RANN, tbl RANN, root decision
			i_timer[i].c_count--;
			if (i_timer[i].c_count == 0) {
				i_timer[i].func();
				i_timer[i].c_count = i_timer[i].p_count;
			}
		}
	}
}

/****************************************************************************
 *
 * NAME: root_decision
 *
 * DESCRIPTION: portals decide a root base on mac address in PANNs when mesh has no root.
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void root_decision()
{

#ifdef Tree_debug
	printf("root_decision!!!!!!!!!!><!!!!!!!\n");
	fflush(stdout);
#endif

	int i;
	int n = 0;                  // check if any portal wanna be root
	unsigned char root[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  // record who is the root

	if (rootDecision == 1) {
		return;
	}

	/*for(i = 0; i < portal_n; ){
		count++;
		if(pann_mpp_pool[count].flag == 0){
			continue;
		}
		if(pann_mpp_pool[count].beRoot == 1) {
			n++;
		}
		i++;
	    }*/
	for (i = 0; i < MAX_MPP_NUM; i++) {
		if (pann_mpp_pool[i].flag == 0) {
			continue;
		}
		if (pann_mpp_pool[i].beRoot == 1) {
			n++;
		}
	}
	if (toBeRoot == 0) {

		/*if(n == 0){             // no portal wanna be root, enter AODV mode.
		}
		else */
		if (n > 0) {        // root is not me, keep route to root
			// Chris: unnecessary decision. removed
			/*if(n == 1){
			        //for(i = 0; i < portal_n; ){
			        for(i = 0; i <MAX_MPP_NUM ; i++){
			            if(pann_mpp_pool[i].flag == 0){
			                continue;
			            }
			            if(pann_mpp_pool[i].beRoot == 1){
			                memcpy(root, pann_mpp_pool[i].mac , ETH_ALEN);
			            }
			            }
			        }
			        else{*/
			//for(i = 0; i < portal_n; ){
			for (i = 0; i < MAX_MPP_NUM ; i++) {
				if (pann_mpp_pool[i].flag == 0) {
					continue;
				}
				if (pann_mpp_pool[i].beRoot != 1) {
					continue;
				}
				if (memcmp(pann_mpp_pool[i].mac, root, ETH_ALEN) < 0) {
					memcpy(root, pann_mpp_pool[i].mac, ETH_ALEN);
				}
			}
			//}
		}
	} else if (toBeRoot == 1) {
		if (n == 0) {           //I am the root
			isRoot = 1;
#ifdef Tree_debug
			printf("2. I am ROOT!!!!!!!!!!!!!!!\n");
#endif
		} else if (n > 0) {
			if (memcmp(my_address, root, ETH_ALEN) < 0) {
				memcpy(root, my_address, ETH_ALEN);
			}
			for (i = 0; i < MAX_MPP_NUM; i++) {
				if (pann_mpp_pool[i].flag == 0) {
					continue;
				}
				if (pann_mpp_pool[i].beRoot != 1) {
					continue;
				}
				if (memcmp(pann_mpp_pool[i].mac, root, ETH_ALEN) < 0) {
					memcpy(root, pann_mpp_pool[i].mac, ETH_ALEN);
				}
			}
			if (memcmp(root, my_address, ETH_ALEN) == 0) {
				isRoot = 1;
#ifdef Tree_debug
				printf("3. I am ROOT!!!!!!!!!!!!\n");
#endif
			}
		}
	}
	rootDecision = 1;
	if (isRoot == 1) {
		memcpy(rann_root_entry.mac, my_address, ETH_ALEN);
		rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
		rann_root_entry.flag = 1;

		update_root_info(my_address);
	}
}

/****************************************************************************
 *
 * NAME: gen_pann
 *
 * DESCRIPTION: generate PANN (portal or root) and broadcast it.
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void gen_pann()
{
#ifdef Tree_debug
	printf("GEN_PANN!!!!!!!!!!!!!!!!!!!!!!!!!\n");
#endif

	struct pann ptr;
	unsigned char nexthop_mac_addr[ETH_ALEN] = {0};

	ptr.category       = _MESH_CATEGORY_ID_;
	ptr.action         = _GANN_ACTION_ID_;
	ptr.eid            = _MESH_GANN_IE_;
	ptr.length         = (sizeof(struct pann) - 2);
	if (toBeRoot == 1) {
		ptr.flag = 1;
	} else {
		ptr.flag = 0;
	}
	ptr.hopCount = 0;
	ptr.ttl = _MESH_HEADER_TTL_;
	memcpy(ptr.origAddr, my_address, ETH_ALEN);

	ptr.seqNum = htonl(My_SeqNum);

	ptr.metric = htonl(0);

	send_packet((unsigned char *)&ptr, my_address, nexthop_mac_addr, my_address, 1, ptr.ttl, htons(My_SeqNum)); // broadcast -> flag = 1

	My_SeqNum++;

	return;
}

static void updateMppPool(struct pann *pann)
{
	int i;
	int first = -1;

	// check whether the mac of the pann (parameter) exist , and reset the timeout time if it really does.
	for (i = 0; i < MAX_MPP_NUM; i++) {
		if (pann_mpp_pool[i].flag == 1) {
			if (memcmp(pann_mpp_pool[i].mac, pann->origAddr, ETH_ALEN) == 0) {
				first = -1;
				pann_mpp_pool[i].timeout = PORTAL_ANNOUNCEMENT_TIMEOUT;
				pann_mpp_pool[i].seqNum = pann->seqNum;
				pann_mpp_pool[i].flag = 1;
				pann_mpp_pool[i].beRoot = pann->flag;
				break;
			}
		} else if (first == -1) {
			first = i;
		}
	}

	// record the the mac of the pann(parameter), if it doesn't exist
	if (first >= 0) {
		memcpy(pann_mpp_pool[first].mac, pann->origAddr, ETH_ALEN);
		pann_mpp_pool[first].timeout = PORTAL_ANNOUNCEMENT_TIMEOUT;
		pann_mpp_pool[first].seqNum = pann->seqNum;
		pann_mpp_pool[first].flag = 1;
		pann_mpp_pool[first].beRoot = pann->flag;
	}
}

static void updatePannRetx(struct pann *pann)
{
	int i;
	int first = -1;

	for (i = 0; i < MAX_MPP_NUM; i++) {
		if (pann_retx_pool[i].flag == 1) {
			if (memcmp(pann_retx_pool[i].pann.origAddr, pann->origAddr, ETH_ALEN) == 0) {  //existence
				first = -1;
				if (ntohl(pann_retx_pool[i].pann.seqNum) < pann->seqNum) {
					memcpy((void *)&pann_retx_pool[i].pann, pann, sizeof(pann_retx_pool[i].pann));
					pann_retx_pool[i].pann.seqNum = htonl(pann->seqNum);
					pann_retx_pool[i].pann.metric = htonl(pann->metric);
					pann_retx_pool[i].timeout = PORTAL_PROPAGATION_DELAY;
					pann_retx_pool[i].flag = 1;
				}

				break;
			}
		} else if (first == -1) {
			first = i;
		}
	}

	if (first >= 0) {
		memcpy((void *)&pann_retx_pool[first].pann, pann, sizeof(pann_retx_pool[first].pann));
		pann_retx_pool[first].pann.seqNum = htonl(pann->seqNum);
		pann_retx_pool[first].pann.metric = htonl(pann->metric);
		pann_retx_pool[first].timeout = PORTAL_PROPAGATION_DELAY;
		pann_retx_pool[first].flag = 1;
#ifdef Tree_debug
		printf("put into retx[%d] = %02X-%02X-%02X-%02X-%02X-%02X\n", first,
			   pann->origAddr[0], pann->origAddr[1], pann->origAddr[2],
			   pann->origAddr[3], pann->origAddr[4], pann->origAddr[5]);
#endif
	}

}

/****************************************************************************
 *
 * NAME: recv_pann
 *
 * DESCRIPTION: update portal info, retransmit table and keep route to portal when receive PANN.
 *
 * PARAMETERS:      Name           Description
 *                  pann           the pann you received
 *                  My_Prehop      recore who relayed pann to you
 *                  metric
 *                  My_interFace
 *                  TTL
 *                  SeqNum
 *
 * RETURNS:       0
 *
 ****************************************************************************/
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int recv_pann(struct pann *pann, unsigned char *My_Prehop, unsigned char *My_Add, unsigned int metric, unsigned int rssiMetric, unsigned int cuMetric,
			  unsigned int noiseMetric, int My_interFace, int TTL, int SeqNum)
{
#else
int recv_pann(struct pann *pann, unsigned char *My_Prehop, unsigned int metric, int My_interFace, int TTL, int SeqNum)
{
#endif
	if ((memcmp(pann->origAddr, my_address, ETH_ALEN)) == 0) {
		return 0;            // get the packet from myself
	}

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned int preHop_metric = calculateMetric(My_Add, metric, rssiMetric, cuMetric, noiseMetric);
	updateMetric(My_Add, My_Prehop, &(preHop_metric));
	pann->metric = ntohl(pann->metric) + preHop_metric;
#else
	pann->metric = ntohl(pann->metric) + metric;
#endif

	pann->seqNum = ntohl(pann->seqNum);
	++pann->hopCount;
	--pann->ttl;

	/* check the freshness and the goodness of metric*/
	if (updatePathByFreshness(pann->origAddr, pann->seqNum,
							  pann->metric, pann->hopCount, My_Prehop,
							  My_interFace) == 0) {
		return 0;
	}

	updateMppPool(pann);
	updatePannRetx(pann);

	return 3;
}
/****************************************************************************
 *
 * NAME: retx_pann
 *
 * DESCRIPTION: rebroadcast the pann produced by the portals
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void retx_pann()
{
	int i;
	unsigned char nexthop_mac_addr[ETH_ALEN] = {0};

#ifdef Tree_debug
	printf("\n    --retransmit package---\n");
#endif
	for (i = 0; i < MAX_MPP_NUM; i++) {

		if (pann_retx_pool[i].flag == 1) {
			pann_retx_pool[i].timeout--;
#ifdef Tree_debug
			printf("        %d %02X-%02X-%02X-%02X-%02X-%02X\n", pann_retx_pool[i].timeout,
				   pann_retx_pool[i].pann.origAddr[0], pann_retx_pool[i].pann.origAddr[1], pann_retx_pool[i].pann.origAddr[2],
				   pann_retx_pool[i].pann.origAddr[3], pann_retx_pool[i].pann.origAddr[4], pann_retx_pool[i].pann.origAddr[5]);
#endif
			if (pann_retx_pool[i].timeout == 0) {
				pann_retx_pool[i].flag = 0;

				if (pann_retx_pool[i].pann.ttl <= 0) { // ttl <= 0, discard PANN
					return;
				}

#ifdef Tree_debug
				printf("PANN_RETX\n");
#endif
				send_packet((unsigned char *)&pann_retx_pool[i].pann, my_address, nexthop_mac_addr, my_address, 1, pann_retx_pool[i].pann.ttl,
							htons(ntohl(pann_retx_pool[i].pann.seqNum)));
			}
		}
	}
}

void tbl_pann()
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
}

/****************************************************************************
 *
 * NAME: gen_rann
 *
 * DESCRIPTION: generate rann (root) and broadcast it.
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void gen_rann()
{
#ifdef Tree_debug
	printf("GEN_RANN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
#endif

	unsigned char nexthop_mac_addr[ETH_ALEN] = {0};
	struct rann ptr;

	ptr.category     = _MESH_CATEGORY_ID_;
	ptr.action       = _HWMP_MESH_PATHSEL_ACTION_ID_;
	ptr.Id           = _MESH_RANN_IE_;
	ptr.length       = (sizeof(struct rann) - 2);
	ptr.flag         = 0;
	ptr.hopCount     = 0;
	ptr.ttl          = _MESH_HEADER_TTL_;
	memcpy(ptr.origAddr, my_address, ETH_ALEN);
	ptr.seqNum = htonl(My_SeqNum);
	ptr.metric = htonl(0);

	send_packet((unsigned char *)&ptr, my_address, nexthop_mac_addr, my_address, 1, ptr.ttl, htons(My_SeqNum));	// flag = 1, for broadcast

	rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
	rann_root_entry.seqNum = My_SeqNum;

	My_SeqNum++;

	return;
}
/****************************************************************************
 *
 * NAME: retx_rann
 *
 * DESCRIPTION: rebroadcast the rann produced by the portals or root
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void retx_rann()
{
	unsigned char nexthop_mac_addr[ETH_ALEN] = {0};

#ifdef Tree_debug
	printf("\n    --retransmit RANN package---\n");
#endif
	if (rann_retx_entry.flag == 1) {
		rann_retx_entry.timeout--;
#ifdef Tree_debug
		printf("        %d %02X-%02X-%02X-%02X-%02X-%02X\n", rann_retx_entry.timeout,
			   rann_retx_entry.rann.origAddr[0], rann_retx_entry.rann.origAddr[1], rann_retx_entry.rann.origAddr[2],
			   rann_retx_entry.rann.origAddr[3], rann_retx_entry.rann.origAddr[4], rann_retx_entry.rann.origAddr[5]);
#endif
		if (rann_retx_entry.timeout == 0) {
			rann_retx_entry.flag = 0;
#ifdef Tree_debug
			printf("RANN RETX\n");
#endif
			if (rann_retx_entry.rann.ttl <= 0) { // ttl <= 0, discard RANN
				return;
			}
			send_packet((unsigned char *)&rann_retx_entry.rann, my_address, nexthop_mac_addr, my_address, 1, rann_retx_entry.rann.ttl,
						htons(ntohl(rann_retx_entry.rann.seqNum)));
		}
	}
}
/****************************************************************************
 *
 * NAME: tbl_rann
 *
 * DESCRIPTION: manage the root info.
 *
 * PARAMETERS:      Name           Description
 * None.
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
void tbl_rann()
{
#ifdef Tree_debug
	printf("\n    --print RANN mac table---\n");
#endif
	// int query_result = 0;
	if (rann_root_entry.flag == 1) {
		rann_root_entry.timeout--;
		if (rann_root_entry.timeout == 0) {

			remove_table(rann_root_entry.mac);

			rann_root_entry.flag = 0;
			//if(isPortal == 1){
			rootDecision = 0;
			//}
			memset(rann_root_entry.mac, 0, ETH_ALEN);
			update_root_info(rann_root_entry.mac);   // clear root mac info to driver chuangch 2007/07/10
#ifdef Tree_debug
			printf("tbl_rann reset root mac,rootDecision,portal_n done\n");
#endif
		}
#ifdef Tree_debug
		printf(">>>        %d %02X-%02X-%02X-%02X-%02X-%02X\n", rann_root_entry.timeout,
			   rann_root_entry.mac[0], rann_root_entry.mac[1], rann_root_entry.mac[2],
			   rann_root_entry.mac[3], rann_root_entry.mac[4], rann_root_entry.mac[5]);
#endif
	}
}
/****************************************************************************
 *
 * NAME: recv_pann
 *
 * DESCRIPTION: update root info, retransmit table and keep route to root when receive RANN.
 *
 * PARAMETERS:      Name           Description
 *                  pann           the pann you received
 *                  My_Prehop      recore who relayed rann to you
 *                  metric
 *                  My_interFace
 *                  TTL
 *                  SeqNum
 *
 * RETURNS:       0
 *
 ****************************************************************************/
int recv_rann(struct rann *rann, unsigned char *My_Prehop, unsigned int metric, int My_interFace, int TTL, int SeqNum)
{
	int update_retx = 0;
	rann->metric = ntohl(rann->metric) + metric;
	rann->seqNum = ntohl(rann->seqNum);
	rann->lifetime = ntohl(rann->lifetime);
	++rann->hopCount;
	--rann->ttl;

	if ((memcmp(rann->origAddr, my_address, ETH_ALEN)) == 0) {
		//  printf("get self RANN packet!!!");
		return 0;            // get the packet from myself
	}

	if (isRoot == 1) {
		if (memcmp(rann->origAddr, my_address, ETH_ALEN) < 0) { // has more apropriate ROOT in mesh
			printf("WARNING - There are more than one root in mesh, I quit.");
			isRoot = 0;
		} else {
			printf("WARNING - There are more than one root in mesh, I'm an apropriate root.\n");
			return 0;
		}
	}

	// check whether the mac of the rann (parameter) exist , and reset the timeout time if it really does.
	if (rann_root_entry.flag == 1) {            // already has root info
		if (memcmp(rann->origAddr, rann_root_entry.mac, ETH_ALEN) < 0) {   // sender of RANN is different from my root info
			memcpy(rann_root_entry.mac, rann->origAddr, ETH_ALEN);
			rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
			rann_root_entry.seqNum = rann->seqNum;
			rann_root_entry.flag = 1;
			rootDecision = 1;
			update_retx = 1;
			update_root_info(rann_root_entry.mac); // update root mac into driver

		} // memcmp < 0
		else if (memcmp(rann_root_entry.mac, rann->origAddr, ETH_ALEN) == 0) {
			update_retx = 1;
			if (rann_root_entry.seqNum < rann->seqNum) {
				rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
				rann_root_entry.seqNum = rann->seqNum;
				rann_root_entry.flag = 1;
			}
		} // memcmp = 0
	} // flag = 1
	else if (rann_root_entry.flag == 0) { // doesn't has root info
		memcpy(rann_root_entry.mac, rann->origAddr, ETH_ALEN);
		rann_root_entry.timeout = ROOT_ANNOUNCEMENT_TIMEOUT;
		rann_root_entry.seqNum = rann->seqNum;
		rann_root_entry.flag = 1;
		rootDecision = 1;
		update_retx = 1;
		update_root_info(rann_root_entry.mac); // update root mac into driver
	} // flag = 0


	if (update_retx > 0) {
		if (updatePathByFreshness(rann->origAddr, rann->seqNum,
								  rann->metric, rann->hopCount, My_Prehop,
								  My_interFace) == 0) {
			return 0;
		}

		memcpy((void *)&rann_retx_entry.rann, rann, sizeof(rann_retx_entry.rann));
		rann_retx_entry.rann.seqNum = htonl(rann->seqNum);
		rann_retx_entry.rann.metric = htonl(rann->metric);
		rann_retx_entry.rann.lifetime = htonl(rann->lifetime);
		rann_retx_entry.timeout = ROOT_PROPAGATION_DELAY;
		rann_retx_entry.flag = 1;
	}
	return 0;
}


int gen_preq(DOT11s_HWMP_PACKET *hwmp_packet)
{
	struct preq preq;// = malloc(sizeof(struct Preq));
	//0d01822c0100000a00000000e04c8396c10b00000011bd18000000000000ff0000000005ffffffffffff0000
	preq.category = _MESH_CATEGORY_ID_;
	preq.action = _HWMP_MESH_PATHSEL_ACTION_ID_;
	preq.element_id = _MESH_PREQ_IE_;
	preq.length = sizeof(struct preq) - 2; //37
	preq.flags = hwmp_packet->flags; //TODO
	preq.hop_count = hwmp_packet->hop_count; //TODO
	preq.ttl = hwmp_packet->ttl;//TODO
	preq.preq_id = htonl(hwmp_packet->preq_id); //TODO
	// memcpy(preq.orig_addr,my_address, MACADDRLEN);
	preq.orig_sn = htonl(hwmp_packet->orig_sn); //OPRIG SN
	preq.lifetime = htonl(255); //TODO
	preq.metric = htonl(0); //TODO
	preq.target_flags = hwmp_packet->target_flags;
	memcpy(preq.target, hwmp_packet->target, ETH_ALEN);
	preq.target_sn = hwmp_packet->target_sn;

	My_SeqNum++;
	My_RreqId++;
	//send_packet((unsigned char*)&preq, my_address, preq.target, my_address, 1, preq.ttl, preq.orig_sn);
	send_mesh_packet((u8 *)&preq, my_address, preq.target, my_address, 1, preq.ttl, preq.orig_sn);  //for broadcast
	return 1;
}


int recv_preq(DOT11s_HWMP_PACKET *hwmp_packet, int My_interFace)

{
	struct preq preq;

	///// Step 1: reject preq from myself
	//  if(memcmp(hwmp_packet->orig_addr, my_address, MACADDRLEN)==0) {
	//    return 0;            //get the RREQ from my_sent RREQ
	//  }

	printf("Get the PREQ from %X %X %X %X %X %X, will target %X %X %X %X %X %X\n",
		   hwmp_packet->orig_addr[0], hwmp_packet->orig_addr[1], hwmp_packet->orig_addr[2], hwmp_packet->orig_addr[3], hwmp_packet->orig_addr[4],
		   hwmp_packet->orig_addr[5],
		   hwmp_packet->target[0], hwmp_packet->target[1], hwmp_packet->target[2], hwmp_packet->target[3], hwmp_packet->target[4], hwmp_packet->target[5]);

	preq.lifetime = ntohl(hwmp_packet->lifetime);
	preq.preq_id = ntohl(hwmp_packet->preq_id);
	preq.orig_sn = ntohl(hwmp_packet->orig_sn);
	preq.target_sn = ntohl(hwmp_packet->target_sn);
	preq.metric = ntohl(hwmp_packet->metric) + 9; //9My_Prehop_metric;

	++preq.hop_count;
	--preq.ttl;

	//// Step 3: check the freshness of tuple {src, dest, DSN} and the goodness of metric
	if (updatePathByFreshness(hwmp_packet->orig_addr, preq.orig_sn,
							  preq.metric, hwmp_packet->hop_count, hwmp_packet->target,
							  My_interFace) == 0) {
		return 0;
	}

	///// Step 4: forward & reply

	// case 4a
	//if(isPortal && (memcmp(preq->t,my_address, MACADDRLEN))==0)
	{
		//    gen_prep(my_address,My_SeqNum, preq.orig_addr, preq.orig_sn, 0, 0, _MESH_HEADER_TTL_, hwmp_packet->target_sn,0,0,0);
		My_SeqNum++;
		return 1;
	}

	//  if (isPortal && is_my_stat(input->first.DesAddress)>0)
	/* {
	       gen_prep(my_address, My_SeqNum, preq->orig_addr, preq->orig_sn, 0, 0, _MESH_HEADER_TTL_, hwmp_packet->target_sn, 1, preq->orig_addr, preq->da);
	       My_SeqNum++;
	       return 1;
	   }*/
	return 1;

}



/*
static int get_iface_index(int fd, const char* interface_name)
{
    return NODE_CFG_IFACE_IDX; //"wlan0", index = 1
}
*/

/*
static void send_rawsocket(char *meshframe, int len, char* src_mac_addr)
{
    struct sockaddr_ll sll; // target address
    int send_result = 0;
    memset(&sll, 0, sizeof(sll));
    sll.sll_ifindex = get_iface_index(g_rawsocket_fd, interface_rawsocket);
    // address length
    sll.sll_halen = ETH_ALEN; // 6
    sll.sll_family = PF_PACKET;

    // MAC - begin
    memset(sll.sll_addr, 0, 8);
    memcpy(sll.sll_addr, src_mac_addr, 6);
    // MAC - end

    sll.sll_protocol = 0;
    sll.sll_hatype = 0;
    sll.sll_pkttype = 0;

    // send the packet
    if ((send_result = sendto(g_rawsocket_fd, meshframe, len, 0, (struct sockaddr*)&sll, sizeof(sll))) == -1) {
        // fprintf(stderr, "Error: can't sendto()\n");
    } else {
        //printf(".");
    }
    //  signal(SIGALRM, test_rawsocket_send);
    return;
}*/

//purpose: bypass lwip layer, without calling WLAN functions directly
static int send_rawsocket(char *meshframe, int len, char *src_mac_addr)
{
	if (rtw_wx_mesh_tx_raw_data(meshframe, len) < 0) {
		// printf("send raw socket failed");
		return -1;
	}

	return 0;
}

static int modify_table(struct path_sel_entry *pEntry_in)
{
#ifndef _RTL865X_
	char buf[ETH_ALEN + sizeof(struct path_sel_entry)];
	struct path_sel_entry Entry;
	memcpy(Entry.destMAC, pEntry_in->destMAC, ETH_ALEN);
	memcpy(Entry.nexthopMAC, pEntry_in->nexthopMAC, ETH_ALEN);
	Entry.dsn = pEntry_in->dsn;
	Entry.metric = pEntry_in->metric;
	Entry.hopcount = pEntry_in->hopcount;
#endif

	/* Get wireless name */
	//memset(wrq.ifr_name, 0, sizeof wrq.ifr_name);
	//strncpy(wrq.ifr_name, interface_ioctl, IFNAMSIZ);

#ifdef _RTL865X_
	if (rtw_wx_mesh_table_u(NODE_CFG_IFACE_IDX, pEntry_in) < 0) {
		// If no wireless name : no wireless extensions
		return (-1);
	}
#else
	memcpy(buf, Entry.destMAC, ETH_ALEN);
	memcpy(buf + ETH_ALEN, &Entry, sizeof(struct path_sel_entry));
	if (rtw_wx_mesh_table_u(NODE_CFG_IFACE_IDX, buf) < 0) {
		// If no wireless name : no wireless extensions
		return (-1);
	}
#endif
	return 1;
}

static int create_table_entry(struct path_sel_entry *pEntry_in)
{
	if (rtw_wx_mesh_table_a(NODE_CFG_IFACE_IDX, pEntry_in) < 0) {
		return (-1);
	}

	return 1;
}


//write a new destination entry in the path selection table
static void create_table_entryA(unsigned char *desAddress, unsigned int desSeq, unsigned char *nexhop, int interfaceNum, unsigned char hopcount,
								unsigned int metric)
{
	struct path_sel_entry Entry;
	memset((void *)&Entry, 0, sizeof(struct path_sel_entry));
	memcpy(Entry.destMAC, desAddress, ETH_ALEN);

//	Entry.isvalid=1;
	Entry.dsn = desSeq;
	memcpy(Entry.nexthopMAC, nexhop, ETH_ALEN);
	Entry.metric = metric;
	Entry.hopcount = hopcount;
//	memset(Entry.modify_time,0,8);

	if (create_table_entry(&Entry) < 0) {
		// printf("at path_selec.c create table error\n");
	}

	return;
}

int query_table(unsigned char *destaddr, struct path_sel_entry *pEntry_out)
{
	memset((void *)pEntry_out, 0, sizeof(struct path_sel_entry));
	memcpy(pEntry_out, destaddr, ETH_ALEN);

	if (rtw_wx_mesh_table_q(NODE_CFG_IFACE_IDX, pEntry_out) < 0) {
		// If no wireless name : no wireless extensions
		return (-2);
	}
	return 1;
}

int query_whole_table(int *ptable_no_out)
{
	if (rtw_wx_mesh_table_qa(NODE_CFG_IFACE_IDX, ptable_no_out) < 0) {
		// If no wireless name : no wireless extensions
		return (-2);
	}
	return 1;
}


FIXIT
//API name should be "remove_entry"
int remove_table(unsigned char *invalid_addr)
{
	unsigned char buf[ETH_ALEN];
	memcpy(buf, invalid_addr, ETH_ALEN);

	//clear conresponding path table entry
	if (rtw_wx_mesh_table_r(NODE_CFG_IFACE_IDX, buf) < 0) {
		// If no wireless name : no wireless extensions
		// printf("dump_path_table  Error:%s\n\a",strerror(errno));
		return (1);
	}

	return 0;
}

#ifdef CONFIG_RTK_MESH_METRIC_REFINE

int retrieveMetricMibValues(unsigned char wlanX, struct mibValue *pointer)
{
	printf("%s %d not implemented yet\r\n", __func__, __LINE__);
	return 1;
}

int calculateMetric(unsigned char *My_Add, unsigned int metric, unsigned int rssiMetric, unsigned int cuMetric, unsigned int noiseMetric)
{
	unsigned char wlanX = 0;
	unsigned int cur_m = 0;
	unsigned char crossbandEnable = 0;
	unsigned int prev_m = (metric >> 1);

	if (memcmp(wlan0_hwAddress, My_Add, ETH_ALEN) == 0) {
		wlanX = 0;
		crossbandEnable = wlan0_crossbandEnable;
	}
#ifdef FOR_DUAL_BAND
	else if (memcmp(wlan1_hwAddress, My_Add, ETH_ALEN) == 0) {
		wlanX = 1;
		crossbandEnable = wlan1_crossbandEnable;
	}
#endif
	else {
		return 0;
	}

	if ((wlanX == 0 && wlan0_is5G == IS_5G_BAND) //wlan0 == 5G
#ifdef FOR_DUAL_BAND
		|| (wlanX == 1 && wlan1_is5G == IS_5G_BAND) //wlan1 == 5G
#endif
	   ) {
		retrieveMetricMibValues(wlanX, &mibValueOf5G);
		mibValuePointer = &mibValueOf5G;
		//printf("5G\n");
	} else {
		retrieveMetricMibValues(wlanX, &mibValueOf2G);
		mibValuePointer = &mibValueOf2G;
		//printf("2G\n");
	}

	if (crossbandEnable) {
		unsigned int rssiScore = (rssiMetric < mibValuePointer->rssi_threshold) ? (100 - rssiMetric) << 2 : (100 - rssiMetric);
		unsigned int cuScore = (cuMetric > mibValuePointer->cu_threshold) ? (cuMetric << 1) : cuMetric;
		unsigned int noiseScore = (cuMetric > mibValuePointer->cu_threshold
								   && noiseMetric > mibValuePointer->noise_threshold) ? noiseMetric : 0;
		//unsigned int noiseScore = (noiseMetric > mibValuePointer->noise_threshold)?(noiseMetric<<1):noiseMetric;

		cur_m = (rssiScore * mibValuePointer->rssi_weight) + (cuScore * mibValuePointer->cu_weight) +
				(noiseScore * mibValuePointer->noise_weight);
#if 0
		printf("RssiT:%u CuT:%u NoiseT:%u \n", mibValuePointer->rssi_threshold, mibValuePointer->cu_threshold, mibValuePointer->noise_threshold);
		printf("RssiW:%u CuW:%u NoiseW:%u \n", mibValuePointer->rssi_weight, mibValuePointer->cu_weight, mibValuePointer->noise_weight);
		printf("RssiM:%u CuM:%u NoiseM:%u \n", rssiMetric, cuMetric, noiseMetric);
		printf("RssiS:%u CuS:%u NoiseS:%u \n", rssiScore, cuScore, noiseScore);
#endif
	} else {
		cur_m = (rssiMetric < 28) ? (499 + ((28 - rssiMetric) << 6)) : ((100 - rssiMetric) << 1);
	}

	//printf("Previous metric: %u     Calculated metric:%u (%u) \n",prev_m, cur_m, (cur_m>>1));

	if (prev_m == 0) {
		return cur_m;
	} else {
		return (prev_m + (cur_m >> 1));
	}
}

int updateMetric(unsigned char *My_Add, unsigned char *My_Prehop, unsigned int *metric)
{
	if (rtw_wx_mesh_metric_u(NODE_CFG_IFACE_IDX, My_Prehop, metric) < 0) {
		return (-1);
	}

	return 1;

}
#endif

// Design logic:
//    Check the freshness of tuple {src, dest, DSN} and the goodness of metric
//         3a: if not fresh or equal freshness with worse metric, drop
//         3b: if equal freshness with better metric, (pending) update path to src
//         3c: if fresher record and better metric, (pending) update path to src
//         3d: if fresher record but worse metric with the same prehop, (pending) update path to src
//         3e: if fresher record but worse metric with different prehop, (pending) update path to src
//         3f: if no entry existed in the driver, immediately update path to src
//
//     Exception 1: an MP has rebooted and driver's table was cleaned, so the dsn is smaller than current stored one
//     Exception 2: an mpp/root has just rebooted and driver's table contains an entry inserted by PANN/RANN
//     Exception 3: (not handled now) dsn is larger than max value of unsigned int
//
// Parameters:
//   src: for RREQ, src is rreq's issuer;  for RREP, src is the rrep's issuer.
//   dest always the other end opposed to src
//   dsn: related to src  (so, it can be shared by rreq, rrep, ...)
//   My_Prehop: pre-hop to src
//
int updatePathByFreshness(unsigned char *destMAC, unsigned int dsn,
						  unsigned int metric, unsigned char hopcount, unsigned char *My_Prehop,
						  int My_interFace)
{
	int qResult;
	struct path_sel_entry pathEntry;

#if 0
	/*always update path to pre-hop ('cos it is almost a near real-time metric) */
	if (memcmp(destMAC, My_Prehop, ETH_ALEN) != 0) {
		qResult = query_table(My_Prehop, &pathEntry);
		if (qResult == -2) {           //need to create table entry for pre-hop
			create_table_entryA(My_Prehop, 0, My_Prehop, My_interFace, 1, metric);
		}
	}
#endif

	qResult = query_table(destMAC, &pathEntry);
	if (qResult > 0 && pathEntry.dsn <= dsn) {
		if (memcmp(My_Prehop, pathEntry.nexthopMAC, ETH_ALEN) == 0 ||
			betterPath(metric, hopcount, pathEntry.metric, pathEntry.hopcount)) {

			memcpy(pathEntry.destMAC, destMAC, ETH_ALEN);
			memcpy(pathEntry.nexthopMAC, My_Prehop, ETH_ALEN);
			pathEntry.dsn        = dsn;
			pathEntry.metric       = metric;
			pathEntry.hopcount = hopcount;
			modify_table(&pathEntry);
			return 1;
		}
	} else if (qResult == -2) { /*query table command succeed, but the pathEntry is empty*/
		create_table_entryA(destMAC, dsn, My_Prehop, My_interFace, hopcount, metric);
		return 1;
	}

	return 0;
}

static void send_packet_with_6addr(unsigned char *RBody,
								   unsigned char *self_mac_addr, unsigned char *dest_mac_addr,
								   unsigned char *src_mac_addr,
								   int flag, unsigned char TTL,
								   unsigned short SeqNum,
								   unsigned char *addr5,
								   unsigned char *addr6
								  )
{

	unsigned char *ra, *ta, *da, *sa;
	int zero_offset = 14;
	unsigned char meshframe[256]; // buffer for mesh frame

	if (flag == 1) {
		memcpy(dest_mac_addr, broadcast_mac_addr, ETH_ALEN);
	}

	memset(meshframe, 0, sizeof(meshframe));
	meshframe[zero_offset + 30] = 0x01;
	meshframe[zero_offset + 31] = TTL;
	memcpy(meshframe + 32 + zero_offset, &SeqNum, 2);

	// Type & Subtype
	SetFrameSubType(meshframe + zero_offset, WIFI_11S_MESH_ACTION);
	SetToDs(meshframe + zero_offset);
	SetFrDs(meshframe + zero_offset);

	// next hop
	ra = GetAddr1Ptr(meshframe + zero_offset);
	memcpy(ra, dest_mac_addr, ETH_ALEN);  /*driver would check next hop later*/

	// transmitter = self
	ta = GetAddr2Ptr(meshframe + zero_offset);
	memcpy(ta, self_mac_addr, ETH_ALEN);

	// destination addr
	da = GetAddr3Ptr(meshframe + zero_offset);
	memcpy(da, dest_mac_addr, ETH_ALEN);

	// source addr
	sa = GetAddr4Ptr(meshframe + zero_offset);
	memcpy(sa, src_mac_addr, ETH_ALEN);

	// add by Jason 2007.04.26
	memcpy(meshframe + zero_offset + WLAN_HDR_A4_MESH_MGT_LEN, addr5, ETH_ALEN);
	memcpy(meshframe + zero_offset + WLAN_HDR_A4_MESH_MGT_LEN + ETH_ALEN, addr6, ETH_ALEN);
	// body[3] is the length field of [RREQ|RREP|RERR|RREP-ACK] IE, but not include the length of Category field and Action field
	memcpy(meshframe + zero_offset + WLAN_HDR_A6_MESH_MGT_LEN, RBody, 2 + RBody[3]);

	int frm_len = zero_offset + WLAN_HDR_A6_MESH_MGT_LEN + 2 + RBody[3];
	send_rawsocket((char *)meshframe, frm_len, (char *)src_mac_addr);
}

static void send_packet(unsigned char *RBody, unsigned char *self_mac_addr, unsigned char *dest_mac_addr, unsigned char *src_mac_addr, int flag,
						unsigned char TTL, unsigned short SeqNum)
{
	unsigned char *ra, *ta, *da, *sa;
	int zero_offset = 14;
	unsigned char meshframe[256]; // buffer for mesh frame

	if (flag == 1) {
		memcpy(dest_mac_addr, broadcast_mac_addr, ETH_ALEN);
	}


	memset(meshframe, 0, sizeof(meshframe));

	meshframe[zero_offset + 31] = TTL;
	memcpy(meshframe + 32 + zero_offset, &SeqNum, 2);

	// Type & Subtype
	SetFrameSubType(meshframe + zero_offset, WIFI_11S_MESH_ACTION);
	SetToDs(meshframe + zero_offset);
	SetFrDs(meshframe + zero_offset);

	// next hop
	ra = GetAddr1Ptr(meshframe + zero_offset);
	memcpy(ra, dest_mac_addr, ETH_ALEN); /*driver would check next hop later*/

	// transmitter = self
	ta = GetAddr2Ptr(meshframe + zero_offset);
	memcpy(ta, self_mac_addr, ETH_ALEN);

	// destination addr
	da = GetAddr3Ptr(meshframe + zero_offset);
	memcpy(da, dest_mac_addr, ETH_ALEN);

	// source addr
	sa = GetAddr4Ptr(meshframe + zero_offset);
	memcpy(sa, src_mac_addr, ETH_ALEN);

	// body[3] is the length field of [RREQ|RREP|RERR|RREP-ACK] IE, but not include the length of Category field and Action field
	memcpy(meshframe + zero_offset + WLAN_HDR_A4_MESH_MGT_LEN, RBody, 2 + RBody[3]);

	int frm_len = zero_offset + WLAN_HDR_A4_MESH_MGT_LEN + 2 + RBody[3];

	send_rawsocket((char *)meshframe, frm_len, (char *)src_mac_addr);
}


void send_mesh_packet(u8 *RBody, u8 *self_mac_addr, u8 *dest_mac_addr, u8 *src_mac_addr, int flag, u8 TTL, u32 SeqNum)
{
	unsigned char *ra, *ta, *da, *sa;
	int zero_offset = 14;
	u8 meshframe[256]; // buffer for mesh frame

	if (flag == 1) {
		memcpy(dest_mac_addr, broadcast_mac_addr, ETH_ALEN);
	}

	memset(meshframe, 0, sizeof(meshframe));

	meshframe[zero_offset + 31] = TTL;
	memcpy(meshframe + 32 + zero_offset, &SeqNum, 4);

	// Type & Subtype
	SetFrameSubType(meshframe + zero_offset, WIFI_11S_MESH_ACTION);
	SetToDs(meshframe + zero_offset);
	SetFrDs(meshframe + zero_offset);

	// next hop
	ra = GetAddr1Ptr(meshframe + zero_offset);
	memcpy(ra, dest_mac_addr, ETH_ALEN); //driver would check next hop later

	// transmitter = self
	ta = GetAddr2Ptr(meshframe + zero_offset);
	memcpy(ta, self_mac_addr, ETH_ALEN);

	// destination addr
	da = GetAddr3Ptr(meshframe + zero_offset);
	memcpy(da, dest_mac_addr, ETH_ALEN);

	// source addr
	sa = GetAddr4Ptr(meshframe + zero_offset);
	memcpy(sa, src_mac_addr, ETH_ALEN);

	// body[3] is the length field of [RREQ|RREP|RERR|RREP-ACK] IE, but not include the length of Category field and Action field
	memcpy(meshframe + zero_offset + WLAN_HDR_A4_MESH_MGT_LEN + 2, RBody, 2 + RBody[3]);

	int frm_len = zero_offset + WLAN_HDR_A4_MESH_MGT_LEN + 4 + RBody[3];
	send_rawsocket((char *)meshframe, frm_len, (char *)src_mac_addr);

}

#endif

