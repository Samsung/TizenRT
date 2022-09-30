#ifndef __WIFIFW_DMA_RAM_H__
#define __WIFIFW_DMA_RAM_H__

#ifndef __ASSEMBLY__
/*--------------------Define -------------------------------------------*/
#define DDMACH0_MSK					BIT0
#define DDMACH1_MSK					BIT1
#define DDMACH0_ISR_CLEAR			BIT0
#define DDMACH1_ISR_CLEAR			BIT1
#define DDMACH0_ISR					BIT0//8 
#define DDMACH1_ISR					BIT1//9 

#define BIT_MASK_DDMA_DLEN		0x3FFFF
#define BIT_DDMA_OWN				BIT31
#define BIT_DDMA_CHKSUM_EN		BIT29
#define BIT_MASK_DDMA_ADDR		0xFFFFFFFF
#define BIT_DDMA_CHKSUM_STS		BIT3
#define DDMACH_DEST_OFSET			0x4
#define DDMACH_CTRL_OFSET			0x8

/*--------------------Function declaration------------------------------*/
extern BOOLEAN InitDDMA_8720E(u8  ch_num,  u32 saddr,  u32 daddr,  u32 dma_size, BOOLEAN chksum_en);
extern BOOLEAN WaitDDMARDY_8720E(u8 ch_num);
extern BOOLEAN ChkDDMAChksum_8720E(u8 ch_num);
extern BOOLEAN CheckDDMADone_8720E(u8 ch_num, u8 chk_chksum);
#endif  /* #ifndef __ASSEMBLY__ */
#endif /* __WIFIFW_DMA_H__ */
