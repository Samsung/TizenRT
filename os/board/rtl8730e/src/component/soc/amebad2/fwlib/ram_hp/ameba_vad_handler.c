#include "ameba_soc.h"
#include "ameba_vad_pc.h"
#include "sysreg_lsys.h"
#include "sysreg_pll.h"
#include "sysreg_pmc.h"
#include "hal_platform.h"

#define VAD_DEV ((VAD_TypeDef *)VAD_REG_BASE)
#define PRE_READ_NUM_BLOCK 15999
#define PRE_READ_NUM_INTERLEAVE 16000

static u32 block_range[4][2] = {{0x20038000, 0x2003FFFF},
	{0x20030000, 0x20037FFF},
	{0x20028000, 0x2002FFFF},
	{0x20020000, 0x20027FFF},
};

/**
  * @brief  obtain the current written address, the value of reg(0ffset 0x5c)
  * @retval return the current written address.
  */
u32 get_current_addr()
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

	if ((addr_ref - PRE_READ_NUM_BLOCK + 1) >= block_range[block_index][0]) {
		return (addr_ref - PRE_READ_NUM_BLOCK + 1);
	} else {
		return (block_range[block_index][1] + 1 - (PRE_READ_NUM_BLOCK - addr_ref + block_range[block_index][0] - 1));
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
		return (addr2 - addr1 + 1);
	} else {
		return (block_range[block_index2][1] - addr1 + 1 + addr2 - block_range[block_index2][0] + 1);
	}
}

/**
  * @brief  obtain the hit address, the value of reg(0ffset 0x54)
  * @retval return the hit address(the written address when a voice activity is detected).
  */
u32 get_hit_addr()
{
	return (VAD_DEV -> VAD_ASSERT_W_SRAM_ADDRSS);
}

/**
  * @brief  obtain the channel index when a VAD activity is detected in block mode
  * @retval return the channel index when a VAD activity is detected in block mode
  */
u32 get_hit_channel()
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

	if ((block_range[block_index][1] - addr_start + 1) >= amount_byte) {
		memcpy(buf, addr_start, amount_byte);
	} else {
		memcpy(buf, addr_start, block_range[block_index][1] - addr_start + 1);
		memcpy(buf + block_range[block_index][1] - addr_start + 1, block_range[block_index][0], amount_byte - (block_range[block_index][1] - addr_start + 1));
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
	if ((block_range[block_index][1] - addr + 1) >= (byte_num + 1)) {
		return (addr + byte_num);
	} else {
		return (byte_num - (block_range[block_index][1] - addr + 1) + block_range[block_index][0]);
	}
}

//we only have to calculate the start address when a voice activity is detected.
//we need a flag to indicate whether this is the first time.
static u8 first_time_flag = 1;


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
void get_vad_data_block(u32 time_period_ms, u8 *buf)
{
	if (first_time_flag) {
		addr_ref = get_hit_addr();
		first_time_flag = 0;
		printf("hit addr is %x\n", addr_ref);
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
	u32 byte_num = get_byte_num(time_period_ms);

	//addr_ref = get_start_addr(addr_ref, 0);
	u32 addr_curr = get_current_addr();
	u32 data_stored = get_data_stored_block(addr_ref, addr_curr);

	printf("\r\naddr_ref %x  addr_curr %x  byte_num %x  data_stored %x\r\n", addr_ref, addr_curr, byte_num, data_stored);

	printf("\r\naddr_ch0  %x  addr_ch1  %x  addr_ch2  %x  addr_ch3  %x\r\n", addr_ch0, addr_ch1, addr_ch2, addr_ch3);

	if (byte_num <= data_stored) {

	} else {
		DelayMs(time_period_ms);
	}

	if (addr_ch0) {
		vad_data_copy_block(addr_ch0, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch0 = get_new_addr_block(addr_ch0, byte_num);
	}

	if (addr_ch1) {
		vad_data_copy(addr_ch1, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch1 = get_new_addr_block(addr_ch1, byte_num);
	}

	if (addr_ch2) {
		vad_data_copy(addr_ch2, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch2 = get_new_addr_block(addr_ch2, byte_num);
	}

	if (addr_ch3) {
		vad_data_copy(addr_ch3, byte_num, buf + used_bytes);
		used_bytes += byte_num;
		addr_ch3 = get_new_addr_block(addr_ch3, byte_num);
	}

	addr_ref = get_new_addr_block(addr_ref, byte_num);
}

/**
  * @brief  This function can only be used in interleave mode. In interleave mode, the block number written is not equal to
            the channel number. So , we need low bound and high bound to calculate the start address
  * @param  hit_addr  the address when voice activity is detected
            byte_num  byte number that needs to be pushed back. This number should be calculted with the channels enabled and sram
                      space enabled. For example, if we enable 96kB VAD sram buffer, which means block A B C will be written, and we
                      let ch0 relates to block B and ch1 related to block A. The data layout is:
                      G0_(ch1_low ch1_high ch0_low ch0_high) G1_(ch1_low ch1_high ch0_low ch0_high) G2_......
                      If the hit address is G1_ch1_low. And we need go back 500ms, we need to go back around 15000*2 = 30000bytes.
                      The specific number is 29997(which means include G1_ch1_low, exclude three other elements in G1 group).
            low_bound:  the lower bound of enabled area
            high_bound  the higher bound of enabled area
  * @retval start address for copy operation
  */
u32 get_start_addr_interleave(u32 hit_addr, u32 byte_num, u32 low_bound, u32 high_bound)
{
	if ((hit_addr - byte_num + 1) >= low_bound) {
		return (hit_addr - byte_num + 1);
	} else {
		printf("##high_bound is %x  low_bound is %x  byte_num is %d  hit_addr is %x\n", high_bound, low_bound, byte_num, hit_addr);
		return (high_bound + 1 - (byte_num - (hit_addr - low_bound + 1)));
	}
}


/**
* @brief  Since memory copy operation is much faster than the audio data collection, we need to make sure we have enough data when get_data is called.
          Suppose addr1 and addr2 are two addresses of reg(0ffset 0x5c, BUF_CTRL_W_ADDRESS) at different time point, we convert them to the same block
          first, and then calculate how many bytes are there between these 2 addresses(addr2 is a latter address, and we assume that the there time difference
          is less then 1s).
* @param  addr1  written address at a former time point
          addr2  written address at a latter time point
          low_bound:  the lower bound of enabled area
          high_bound  the higher bound of enabled area
* @retval bytes number between these 2 addresses
*/
u32 get_data_stored_interleave(u32 addr1, u32 addr2, u32 low_bound, u32 high_bound)
{

	if (addr2 >= addr1) {
		return (addr2 - addr1);
	} else {
		return (high_bound - addr1 + addr2 - low_bound);
	}
}

/**
  * @brief  This function can only be used in interleave mode. After each copy, we need to calculate the next
            start address for latter copy operations. This function will first determine the block index
            according to addr, and then get the lower and upper bound, then calculate the next start addr.
  * @param  addr  the address of a former copy operation
            byte_num  how many bytes should be skipped
            low_bound:  the lower bound of enabled area
            high_bound  the higher bound of enabled area
  * @retval the next address for copying
  */
u32 get_new_addr_interleave(u32 addr, u32 byte_num, u32 low_bound, u32 high_bound)
{
	if ((addr + byte_num) <= high_bound) {
		return (addr + byte_num);
	} else {
		return (low_bound + (byte_num - (high_bound - addr)) - 1);
	}
}

/**
  * @brief  copy data from the VAD buffer to some other places in block mode. Unlike block mode,
            we need the lower bound and upeer bound to accomplish this operation.
  * @param  addr_start  the address to start copying
            amount_byte  how many bytes should be copied
            buf  a bffer malloced by the user
            low_bound:  the lower bound of enabled area
            high_bound  the higher bound of enabled area
  */
void vad_data_copy_interleave(u32 addr_start, u32 byte_num, u8 *buf, u32 low_bound, u32 high_bound)
{
	if ((high_bound - addr_start + 1) >= byte_num) {
		memcpy(buf, addr_start, byte_num);
	} else {
		memcpy(buf, addr_start, high_bound - addr_start + 1);
		memcpy(buf + high_bound - addr_start + 1, low_bound, byte_num - (high_bound - addr_start + 1));
	}
}

//initialized low bound and high bound
static u32 low_bound = 0x2003FFFF;
static u32 high_bound = 0x20020000;
//The block of lowest priority is D(3). We need to set block_low_prio to A(0) to run latter comparisions.
static u32 block_low_prio = 0;
static u32 block_high_prio = 3;
static u32 start_addr = 0;
u32 used_channels = 0;

/**
  * @brief  This function can only be used in interleave mode. This function will fill the buf with audio data of length time_period_ms
  * @param  time_period_ms  the time period of requied audio data
            buf  memory space malloced by the user to store audio data
  */
void get_vad_data_interleave(u32 time_period_ms, u8 *buf)
{
	u32 hit_block = 0;
	u32 sram_sel = 0;
	u32 hit_addr = 0;
	u32 hit_channel = 0;
	u32 hit_addr_new = 0;
	u32 data_stored = 0;

	if (first_time_flag) {
		first_time_flag = 0;

		//find upper bound and lower bound
		sram_sel = VAD_W_SRAM_ADDRESS_SEL(VAD_DEV -> VAD_BUF_CTRL0);

		//in interleave mode, the higher bound is 0x2003FFFF
		low_bound = block_range[sram_sel][0];
		high_bound = block_range[0][1];

		printf("low bound is %x  high_bound is %x\n", low_bound, high_bound);
		hit_addr = get_hit_addr();
		hit_channel = get_hit_channel();
		printf("hit_addr is %x  hit_channel is %x\n", hit_addr, hit_channel);

		//when a voice activity is detected, the hit channel can be ontained from register,
		//hit_block here means the block index related to the hit channel, not the block index
		//in which the hit address resides.
		switch (hit_channel) {
		case 0:
			hit_block = VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			printf("here 0\n");
			break;
		case 1:
			hit_block = VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			printf("here 1\n");
			break;
		case 2:
			hit_block = VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			printf("here 2\n");
			break;
		case 3:
			hit_block = VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			printf("here 3\n");
			break;
		default:
			break;
		}

		//if we enable 3 channels, and relared block are A, B and C, and the enabled sram space is 96k,
		/*
			CH0   CH1   CH2
			A     B     C
		*/

		//So CH2 has the lowest priority, the writtern order is CH0 CH1 CH2 CH0 CH1 CH2 ......
		//if we change the relationship to the following way
		/*
			CH0   CH1   CH2
			B     A     C
		*/
		//the written order now becomes CH1 CH0 CH2 CH1 CH0 CH2

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH0_IN_VADBUF_EN) {
			if (block_low_prio < VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_low_prio = VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			if (block_high_prio > VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_high_prio = VAD_GET_CODEC0_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			used_channels++;
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH1_IN_VADBUF_EN) {
			if (block_low_prio < VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_low_prio = VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}

			if (block_high_prio > VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_high_prio = VAD_GET_CODEC1_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			used_channels++;
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH2_IN_VADBUF_EN) {
			if (block_low_prio < VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_low_prio = VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			if (block_high_prio > VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_high_prio = VAD_GET_CODEC2_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			used_channels++;
		}

		if (VAD_DEV -> VAD_BUF_CTRL0 & VAD_BIT_CH3_IN_VADBUF_EN) {
			if (block_low_prio < VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_low_prio = VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			if (block_high_prio > VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0)) {
				block_high_prio = VAD_GET_CODEC3_IN_BUF_ADD_BLOCK(VAD_DEV -> VAD_BUF_CTRL0);
			}
			used_channels++;
		}

		printf("block_low_prio is %d\n", block_low_prio);
		printf("block_high_prio is %d\n", block_high_prio);


		/*
		Consider this case: when we need to go back 500ms, so the total byte number is
		16000*used_channels. For example, if the lowest priority block is block C(2)，we need to return 16000*(2+1) bytes.
		If we use 3 channels, and the data layout is： (before_0 byte)(before_1 byte)(hit_0 byte)(hit_1 byte)(after_0 byte)(after_1 byte)...
		since the formula we use is hit_0 - start_addr + 1 = total number including hit_0, excluding hit_1, after_0, after_1。
		if block_low_prio = 3, total number = 16000*used_channels，minus the total number of a group，which means
		-used_channels*2，and plus bytes before hit_byte_0，which means (hit_block - block_high_prio)*2, and then plus hit_byte_0
		*/
		u32 reverse_byte = 16000 * used_channels - used_channels * 2 + 1 + (hit_block - block_high_prio) * 2;
		start_addr = get_start_addr_interleave(hit_addr, reverse_byte, low_bound, high_bound);
		printf("fantuishu is %d\n", reverse_byte);
		printf("hit_block is %d\n", hit_block);
	}
	printf("start_addr is %x\n", start_addr);
	//hit_addr_new = hit_addr + block_low_prio - hit_block;
	u32 current_addr = get_current_addr();
	data_stored = get_data_stored_interleave(start_addr, current_addr, low_bound, high_bound);

	if (data_stored >= (time_period_ms * 32 * used_channels)) {

	} else {
		DelayMs(time_period_ms);
	}

	//start copy operation
	vad_data_copy_interleave(start_addr, time_period_ms * 32 * used_channels, buf, low_bound, high_bound);
	//calculate the address of next copy operation
	start_addr = get_new_addr_interleave(start_addr, time_period_ms * 32 * used_channels, low_bound, high_bound);
	printf("next start_addr is %x\n", start_addr);
}

/**
  * @brief  This function will fill the buf with audio data of length time_period_ms
  * @param  time_period_ms  the time period of requied audio data
            buf  memory space malloced by the user to store audio data
  */
void get_vad_data(u32 time_period_ms, u8 *buf)
{
	VAD_TypeDef *VAD;
	if (TrustZone_IsSecure()) {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE_S);
	} else {
		VAD = ((VAD_TypeDef *)VAD_REG_BASE);
	}

	if ((VAD -> VAD_BUF_CTRL0) && VAD_BIT_W_MODE_SEL) {
		get_vad_data_interleave(time_period_ms, buf);
	} else {
		get_vad_data_block(time_period_ms, buf);
	}
}