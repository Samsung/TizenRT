#ifndef _FREERTOS_SKBUFF_H_
#define _FREERTOS_SKBUFF_H_

#if (RTL8735B_SUPPORT == 1)
// For Lextra(PCI-E like interface), RX buffer along with its skb is required to be
// 	pre-allocation and set into rx buffer descriptor ring during initialization.
#if (SKB_PRE_ALLOCATE_RX==1)
#define MAX_SKB_BUF_NUM			(8 + 4)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#define MAX_LOCAL_SKB_NUM		(10 + 18)	//tx+rx
#else
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_NUM		10             //tx+rx, ping 10k test
#else
#define MAX_SKB_BUF_NUM		24	//8              //tx+rx
#endif
#define MAX_LOCAL_SKB_NUM		(MAX_SKB_BUF_NUM + 2)	//tx+rx, +2: AP mode broadcast
#endif
#elif (RTL8721D_SUPPORT == 1)
#if (SKB_PRE_ALLOCATE_RX==1)
#ifdef CONFIG_HIGH_TP_TEST
#define MAX_SKB_BUF_NUM			(28)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#else
#define MAX_SKB_BUF_NUM			(8 + 4)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#endif
#define MAX_LOCAL_SKB_NUM		(10 + 18)	//tx+rx
#else
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_NUM		10             //tx+rx, ping 10k test
#else
#define MAX_SKB_BUF_NUM		8              //tx+rx
#endif
#define MAX_LOCAL_SKB_NUM		(MAX_SKB_BUF_NUM + 2)	//tx+rx, +2: AP mode broadcast
#endif

#elif (RTL8721F_SUPPORT == 1)
#if (SKB_PRE_ALLOCATE_RX==1)
#if defined(CONFIG_HIGH_TP_TEST)
#define MAX_SKB_BUF_NUM			(28)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#else
#define MAX_SKB_BUF_NUM			(8 + 4)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#endif
#define MAX_LOCAL_SKB_NUM		(10 + 18)	//tx+rx
#else
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_NUM		10             //tx+rx, ping 10k test
#elif defined(CONFIG_AS_INIC_NP) //For iNIC throughput request
#define MAX_SKB_BUF_NUM			10
#else
#define MAX_SKB_BUF_NUM		8              //tx+rx
#endif
#define MAX_LOCAL_SKB_NUM		(MAX_SKB_BUF_NUM + 2)	//tx+rx, +2: AP mode broadcast
#endif

#elif (RTL8730E_SUPPORT == 1)
#if (SKB_PRE_ALLOCATE_RX==1)
#if defined(CONFIG_HIGH_TP_TEST)
#define MAX_SKB_BUF_NUM			(28)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#else
#define MAX_SKB_BUF_NUM			(8 + 4)		//tx+rx (8 + RX_Q_DESC_NUM) Reduce rx skb number due to memory limitation
#endif
#define MAX_LOCAL_SKB_NUM		(10 + 18)	//tx+rx
#else
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_NUM		10             //tx+rx, ping 10k test
#elif defined(CONFIG_HIGH_TP_TEST)
#define MAX_SKB_BUF_NUM			10
#else
#define MAX_SKB_BUF_NUM		4              //tx+rx
#endif
#define MAX_LOCAL_SKB_NUM		(MAX_SKB_BUF_NUM + 2)	//tx+rx, +2: AP mode broadcast
#endif

#elif (RTL8720E_SUPPORT == 1)
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_NUM		10             //tx+rx, ping 10k test
#elif defined(CONFIG_HIGH_TP_TEST)
#define MAX_SKB_BUF_NUM		10
#else
#define MAX_SKB_BUF_NUM		4              //tx+rx for single core, rx for inic NP
#endif
#define MAX_LOCAL_SKB_NUM		(MAX_SKB_BUF_NUM + 2)	//tx+rx, +2: AP mode broadcast

#else
#ifndef CONFIG_HIGH_TP
#define MAX_LOCAL_SKB_NUM		10
#define MAX_SKB_BUF_NUM			7
#else
#define MAX_LOCAL_SKB_NUM		100
#define MAX_SKB_BUF_NUM			100
#endif
#endif

extern int max_local_skb_num;
extern int max_skb_buf_num;

#endif