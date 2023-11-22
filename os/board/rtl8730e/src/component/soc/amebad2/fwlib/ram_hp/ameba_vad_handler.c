#include "ameba_soc.h"
#include "ameba_vad_pc.h"
#include "sysreg_lsys.h"
#include "sysreg_pll.h"
#include "sysreg_pmc.h"
#include "hal_platform.h"

#define VAD_DEV ((VAD_TypeDef *)VAD_REG_BASE)
#define PRE_READ_NUM_BLOCK 12800
#define LAST_SAMPLE_BYTES  2

static u32 block_range[4][2] = {{0x20038000, 0x2003FFFF},
	{0x20030000, 0x20037FFF},
	{0x20028000, 0x2002FFFF},
	{0x20020000, 0x20027FFF},
};

/**
  * @brief  obtain the current written address, the value of reg(0ffset 0x5c)
  * @retval return the current written address.
  */
u32 get_current_addr(void)
{
	return VAD_DEV -> VAD_BUF_CTRL_W_ADDRESS;
}

/**
  * @brief  the SR of VAD is 16k, this function will calculate the byte number in in period whose length is time_period_ms
  * @param  time_period_ms
  * @retval the number of bytes produced during in a period measured in ms in one channel.
  */
u32 get_byte_num(u32 time_period_ms)
{
	return (time_period_ms * 32);
}

/**
  * @brief  In block mode, we can know the block index according to an address
  * @param  addr
            This parameter should ranging between 0x20020000 and 0x20037FFF
  * @retval the block index of addr. 3-D 2-C 1-B 0-A.
  */
u8 get_block_index(u32 addr)
{
	if ((addr >= 0x20020000) && (addr <= 0x20027FFF)) {
		return 3;
	} else if ((addr >= 0x20028000) && (addr <= 0x2002FFFF)) {
		return 2;
	} else if ((addr >= 0x20030000) && (addr <= 0x20037FFF)) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  In VAD application, we often need audio data before a voice activity is detected. You can tune the period with
            macro PRE_READ_NUM_BLOCK. For example, in block mode, if we need 500ms data before the hit place, we need to go back
            to around 15000 bytes. Since we calculate the address with "(addr_ref - PRE_READ_NUM_BLOCK + 1)". addr_ref is an even number
            which points to the lower byte of a sample, so here PRE_READ_NUM_BLOCK should be 15999 to make sure we have 15000bytes(500ms)
            in total.
  * @param  addr_ref  the hit address when a voice activity is detected, reg(0ffset 0x5c).
            channel_index  this parameter can be 0, 1, 2, 3.
                           there are 4 channels in total. We need this parameter because different channel can match to different block
                           the start address is  calculated according to the block index.
  * @retval the converted start address in the block relted to the channel_index
  */
u32 get_start_addr_block(u32 addr_ref, u8 channel_index)
{
	u8 block_index = 0;

	if (channel_index == 0) {
		block_index = VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
	}

	if (channel_index == 1) {
		block_index = VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
	}

	if (channel_index == 2) {
		block_index = VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
	}

	if (channel_index == 3) {
		block_index = VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
	}

	u8 block_ref = get_block_index(addr_ref);

	addr_ref = addr_ref + (block_ref - block_index) * 0x8000;

	if ((addr_ref - PRE_READ_NUM_BLOCK) >= block_range[block_index][0]) {
		return (addr_ref - PRE_READ_NUM_BLOCK);
	} else {
		return (block_range[block_index][1] + LAST_SAMPLE_BYTES - (PRE_READ_NUM_BLOCK - addr_ref + block_range[block_index][0]));
	}
}

/**
  * @brief  Since memory copy operation is much faster than the audio data collection, we need to make sure we have enough data when get_data is called.
            Suppose addr1 and addr2 are two addresses of reg(0ffset 0x5c, BUF_CTRL_W_ADDRESS) at different time point, we convert them to the same block
            first, and then calculate how many bytes are there between these 2 addresses(addr2 is a latter address, and we assume that the there time difference
            is less then 1s).
  * @param  addr1  written address at a former time point
            addr2  written address at a latter time point
  * @retval bytes number between these 2 addresses
  */
u32 get_data_stored_block(u32 addr1, u32 addr2)
{
	u8 block_index1 = get_block_index(addr1);
	u8 block_index2 = get_block_index(addr2);

	addr1 = addr1 + (block_index1 - block_index2) * 0x8000;

	if (addr2 >= addr1) {
		return (addr2 - addr1);
	} else {
		return (block_range[block_index2][1] + LAST_SAMPLE_BYTES - addr1 + addr2 - block_range[block_index2][0]);
	}
}

/**
  * @brief  obtain the hit address, the value of reg(0ffset 0x54)
  * @retval return the hit address(the written address when a voice activity is detected).
  */
u32 get_hit_addr(void)
{
	return (VAD_DEV -> VAD_ASSERT_W_SRAM_ADDRSS);
}

/**
  * @brief  obtain the channel index when a VAD activity is detected in block mode
  * @retval return the channel index when a VAD activity is detected in block mode
  */
u32 get_hit_channel(void)
{
	return (VAD_GET_CH_W_VAD_ASSERT(VAD_DEV -> VAD_BUF_CTRL0));
}

/**
  * @brief  copy data from the VAD buffer to some other places in block mode. With addr_start,
            we can know its block index, thus we get the lower bound and upeer bound of this block
            and we need to copy data in this range.
  * @param  addr_start  the address to start copying
            amount_byte  how many bytes should be copied
            buf  a bffer malloced by the user
  */
void vad_data_copy_block(u32 addr_start, u32 amount_byte, u8 *buf)
{
	u8 block_index = get_block_index(addr_start);

	if ((block_range[block_index][1] - addr_start + LAST_SAMPLE_BYTES) >= amount_byte) {
		_memcpy((void *)buf, (void *)addr_start, amount_byte);
		DCache_CleanInvalidate((u32)buf, (u32)amount_byte);

	} else {
		memcpy((void *)buf, (void *)addr_start, block_range[block_index][1]  + LAST_SAMPLE_BYTES - addr_start);
		memcpy((void *)(buf + block_range[block_index][1]  + LAST_SAMPLE_BYTES - addr_start), (void *)(block_range[block_index][0]),
			   amount_byte - (block_range[block_index][1]  + LAST_SAMPLE_BYTES - addr_start));
	}
}

/**
  * @brief  This function can only be used in block mode. After each copy, we need to calculate the next
            start address for latter copy operations. This function will first determine the block index
            according to addr, and then get the lower and upper bound, then calculate the next start addr.
  * @param  addr  the address of a former copy operation
            byte_num  how many bytes should be skipped
  * @retval the next address for copying
  */
u32 get_new_addr_block(u32 addr, u32 byte_num)
{
	u8 block_index = get_block_index(addr);
	if ((block_range[block_index][1] - addr + LAST_SAMPLE_BYTES) >= (byte_num)) {
		return (addr + byte_num);
	} else {
		return (byte_num - (block_range[block_index][1] - addr + LAST_SAMPLE_BYTES) + block_range[block_index][0]);
	}
}

//we only have to calculate the start address when a voice activity is detected.
//we need a flag to indicate whether this is the first time.
u8 first_time_flag = 1;


static u32 addr_ref = 0;
static u32 addr_ch0 = 0;
static u32 addr_ch1 = 0;
static u32 addr_ch2 = 0;
static u32 addr_ch3 = 0;

/**
  * @brief  This function can only be used in block mode. This function will fill the buf with audio data of length time_period_ms
            If you have n channels enabled , we will fill the buffer in the order ch0(if enabled) ch1(if enabled) ch2(if enabled)
            ch3((if enabled))
  * @param  time_period_ms  the time period of requied audio data
            buf  memory space malloced by the user to store audio data
  */
void get_vad_data(u32 time_period_ms, u8 *buf)
{
	if (first_time_flag) {
		addr_ref = get_hit_addr();
		first_time_flag = 0;
		//printf("hit addr is %x\n", addr_ref);
		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH0_IN_VADBUF_EN) {
			addr_ch0 = get_start_addr_block(addr_ref, 0);
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH1_IN_VADBUF_EN) {
			addr_ch1 = get_start_addr_block(addr_ref, 1);
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH2_IN_VADBUF_EN) {
			addr_ch2 = get_start_addr_block(addr_ref, 2);
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH3_IN_VADBUF_EN) {
			addr_ch3 = get_start_addr_block(addr_ref, 3);
		}
		addr_ref = get_start_addr_block(addr_ref, 0);
	} else {

	}

	u32 used_bytes = 0;
	u32 block_number = 0;
	u32 byte_num = get_byte_num(time_period_ms);
	u32 addr_curr = get_current_addr();
	u32 data_stored = get_data_stored_block(addr_ref, addr_curr);

	//printf("\r\naddr_ref %x  addr_curr %x  byte_num %x  data_stored %x\r\n", addr_ref, addr_curr, byte_num, data_stored);
	//printf("\r\naddr_ch0  %x  addr_ch1  %x  addr_ch2  %x  addr_ch3  %x\r\n", addr_ch0, addr_ch1, addr_ch2, addr_ch3);

	while (byte_num > data_stored) {
		addr_curr = get_current_addr();
		data_stored = get_data_stored_block(addr_ref, addr_curr);
	}


	if (addr_ch0) {
		vad_data_copy_block(addr_ch0, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch0 = get_new_addr_block(addr_ch0, byte_num);
		block_number = block_number + 1;
	}

	if (addr_ch1) {
		vad_data_copy_block(addr_ch1, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch1 = get_new_addr_block(addr_ch1, byte_num);
		block_number = block_number + 1;
	}

	if (addr_ch2) {
		vad_data_copy_block(addr_ch2, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch2 = get_new_addr_block(addr_ch2, byte_num);
		block_number = block_number + 1;
	}

	if (addr_ch3) {
		vad_data_copy_block(addr_ch3, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch3 = get_new_addr_block(addr_ch3, byte_num);
		block_number = block_number + 1;
	}

	u8 *temp_buf = malloc(used_bytes);
	/* example:turn block0  block1  block2 into 0 1 2 0 1 2...(interleave)*/
	u32 length = byte_num / 2;
	u32 m = 0;
	for (u32 j = 0; j < length ; j++) {
		for (u32 i = 0 ; i < block_number; i++) {
			((u16 *)temp_buf)[m] = ((u16 *)buf)[i * length + j];
			m++;
		}
	}

	memcpy(buf, temp_buf, used_bytes);
	DCache_CleanInvalidate((u32)buf, (u32)used_bytes);
	free(temp_buf);

	addr_ref = get_new_addr_block(addr_ref, byte_num);
}

