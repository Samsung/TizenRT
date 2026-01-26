//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <common/bk_include.h>
#include <driver/psram_types.h>
#include "psram_hal.h"
#include "psram_ll_macro_def.h"
#include "bk_pm_internal_api.h"
#include "sys_driver.h"
#include "aon_pmu_ll.h"
#include <driver/psram.h>
#include <modules/chip_support.h>

static void psram_delay(volatile uint32_t times)
{
	while(times--);
}

__FLASH_BOOT_CODE void psram_hal_set_sf_reset(uint32_t value)
{
	psram_ll_set_sf_reset_value(value);
}

__FLASH_BOOT_CODE void psram_hal_set_cmd_reset(void)
{
	psram_ll_set_reg8_value(0x4);
}

__FLASH_BOOT_CODE void psram_hal_cmd_write(uint32_t addr, uint32_t value)
{
	psram_ll_set_write_address(addr);
	psram_ll_set_write_data(value);
	psram_ll_set_reg8_value(0x1);
	while(psram_ll_get_reg8_value() & 0x1);
}

__FLASH_BOOT_CODE uint32_t psram_hal_cmd_read(uint32_t addr)
{
	uint8_t m = 10, i = 5;

	for (i = 5; i > 0; i--)
	{
		psram_ll_set_write_address(addr);
		psram_ll_set_reg8_value(0x2);

		m = 10;

		while(psram_ll_get_reg8_value() & 0x2)
		{
			for (int j = 0; j < 5000; j++) {}

			m--;

			if (m == 0)
				break;
		};

		if (m != 0)
		{
			return psram_ll_get_regb_value();
		}

		psram_hal_set_sf_reset(0);
		psram_hal_set_sf_reset(1);
		psram_hal_set_cmd_reset();
	}

	return 0;
}

__FLASH_BOOT_CODE void psram_hal_set_clk(psram_clk_t clk)
{
	switch (clk)
	{
#if ((defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7236N)))
		case PSRAM_160M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-240
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_120M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-240
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_80M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-240
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
#else
		case PSRAM_240M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_160M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_120M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_80M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
#endif
		default:
			break;
	}
}

__FLASH_BOOT_CODE void psram_hal_set_voltage(psram_voltage_t voltage)
{
#if (COINFG_SOC_BK7256XX)
	switch (voltage)
	{
		case PSRAM_OUT_3_20V:
			sys_drv_psram_psldo_vset(0, 1);
			break;
		case PSRAM_OUT_3_0V:
			sys_drv_psram_psldo_vset(0, 0);
			break;
		case PSRAM_OUT_2_0V:
			sys_drv_psram_psldo_vset(1, 1);
			break;
		case PSRAM_OUT_1_80V:
			sys_drv_psram_psldo_vset(1, 0);
			break;
		default:
			break;
	}
#elif ((defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7236N)))
	switch (voltage)
	{
		case PSRAM_OUT_3_0V:
			sys_drv_psram_psldo_vset(0, 0);
			break;
		case PSRAM_OUT_1_95V:
			sys_drv_psram_psldo_vset(1, 3);
			break;
		case PSRAM_OUT_1_90V:
			sys_drv_psram_psldo_vset(1, 2);
			break;
		case PSRAM_OUT_1_85V:
			sys_drv_psram_psldo_vset(1, 1);
			break;
		case PSRAM_OUT_1_80V:
			sys_drv_psram_psldo_vset(1, 0);
			break;
		default:
			break;
	}
#else
	switch (voltage)
	{
		case PSRAM_OUT_3_0V:
			sys_drv_psram_psldo_vset(0, 0);
			break;
		case PSRAM_OUT_1_95V:
			sys_drv_psram_psldo_vset(1, 0);
			break;
		case PSRAM_OUT_1_90V:
			sys_drv_psram_psldo_vset(1, 1);
			break;
		case PSRAM_OUT_1_85V:
			sys_drv_psram_psldo_vset(1, 2);
			break;
		case PSRAM_OUT_1_80V:
			sys_drv_psram_psldo_vset(1, 3);
			break;
		default:
			break;
	}
#endif
}

static inline bool is_psram_addr_out_of_range(uint32_t addr)
{
#if (defined(CONFIG_PSRAM_WRITE_THROUGH))
	return ((addr < SOC_PSRAM_DATA_BASE) || (addr > (SOC_PSRAM_DATA_BASE + SOC_PSRAM_DATA_SIZE)));
#else
	return false;
#endif
}

static inline bool is_32bytes_aligned(uint32_t addr)
{
	return ((addr & 0x1f) == 0);
}

int psram_hal_set_write_through(psram_write_through_area_t area, uint32_t enable, uint32_t start, uint32_t end)
{
#if (defined(CONFIG_PSRAM_WRITE_THROUGH))

	if (area > PSRAM_WRITE_THROUGH_AREA_COUNT) {
		return BK_ERR_PSRAM_AREA;
	}

	if (enable) {
		if (start >= end) {
			return BK_ERR_PSRAM_ADDR_RELATION;
		}

		if (is_psram_addr_out_of_range(start) || is_psram_addr_out_of_range(end)) {
			return BK_ERR_PSRAM_ADDR_OUT_OF_RANGE;
		}

		if ((!is_32bytes_aligned(start)) || (!is_32bytes_aligned(end))) {
			return BK_ERR_PSRAM_ADDR_ALIGN;
		}
		psram_ll_set_cover_start(area, start >> 5);
		psram_ll_set_cover_stop_enable(area, BIT(31) | (end >> 5));
	} else {
		psram_ll_set_cover_stop_enable(area, 0);
	}

	return BK_OK;
#else
	return BK_FAIL;
#endif
}

void psram_hal_set_transfer_mode(uint32_t value)
{
	psram_ll_set_reg4_wrap_config(value);
}

__FLASH_BOOT_CODE static int psram_hal_APS6408L_init(uint32_t *id)
{
	uint32_t val = 0;

#if (defined(CONFIG_SOC_BK7256XX))
	uint32_t chip_id = bk_get_hardware_chip_id_version();

	if (chip_id == CHIP_VERSION_C)
		psram_hal_set_mode_value(PSRAM_MODE1);
	else
		psram_hal_set_mode_value(PSRAM_MODE3);
#elif (defined(CONFIG_SOC_BK7239XX))
	psram_hal_set_mode_value(PSRAM_MODE2);
	//psram_hal_set_reg5_value(0x1704);   //PSRAM 120M config
	uint32_t chip_id = bk_get_hardware_chip_id_version();

	if (chip_id == CHIP_VERSION_D)
		psram_hal_set_reg5_value(0x14c5); //PSRAM 120M config for D version MP chip, psram calibration value, sck_delay=1, clk_drv=3(max), dat_drv=2 ,dqs_delay=5
	else
		psram_hal_set_reg5_value(0x6C3);  //PSRAM 160M config, psram calibration value, sck_delay=0, clk_drv=3(max), dat_drv=3(max),dqs_delay=3
#else
	psram_hal_set_mode_value(PSRAM_MODE6);//PSRAM_MODE2
	//psram_hal_set_reg5_value(0x282);
	psram_hal_set_reg5_value(0x380);
#endif

	psram_hal_set_cmd_reset();

	psram_delay(500);

	val = psram_hal_cmd_read(0x00000000);//1 0001 10001101
#if ((defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7239XX)))
	// 8 line
	val = ((val >> 8) & 0xFF) | ((val & 0xFF) << 8);
#endif

	if (val == 0 || val != *id)
	{
		return -1;
	}
	else
	{
		*id = val;
	}

	val = psram_hal_get_regb_value();

#if (defined(CONFIG_SOC_BK7256XX))
	val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x3 << 8);
#elif (defined(CONFIG_SOC_BK7239XX))
	//20250925: asic psram data to decrease strength for bk7239nv4
	//val = (val & ~(0x1F << 8)) | (0x2 << 10) | (0x3 << 8);    //PSRAM 120M config
	//20260115: asic psram data to increase strength for bk7239nv4  set data strength = 1
	if (chip_id == CHIP_VERSION_D)
		val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x1 << 8);      //PSRAM 120M config, data strength = 1 for D version MP chip
	else
	    val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x0 << 8);      //PSRAM 160M config, data strength = 3 (0 is max driver strength, psram driver strength may affect RF)
#else
	val = (val & ~(0x1F)) | (0x4 << 2) | 0x3;
#endif
	psram_hal_cmd_write(0x00000000, val);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101
	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
#if ((defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7239XX)))
	//val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz   //PSRAM 120M config
	val = (val & ~(0x7 << 13)) | (0x1 << 13);//write latency 110 166Mhz     //PSRAM 160M config
#else
	val = (val & ~(0x7 << 5)) | (0x6 << 5);
#endif
	psram_hal_cmd_write(0x00000004, val);

	return 0;
}

__FLASH_BOOT_CODE static int psram_hal_W955D8MKY_5J_init(uint32_t *id)
{
	uint32_t val = 0;
	uint32_t io_drv = 0; /*range [0, 3]*/

	psram_hal_set_mode_value(PSRAM_MODE8);// mode 8
	psram_hal_set_reg5_value(0x6C3);

	psram_hal_set_cmd_reset();
	psram_delay(500);

	val = psram_hal_cmd_read(0x01000000);
	// (VOID *)val; // Statement with no effect
#if (defined(CONFIG_SOC_BK7256XX))
	// 8 line
	val = ((val >> 8) & 0xFF) | ((val & 0xFF) << 8);
#endif


#if (defined(CONFIG_SOC_BK7256XX))
	val = 0x8F1C | (io_drv << 12);
#else
	val = 0x1C8F | (io_drv << 4);
#endif

	psram_hal_cmd_write(0x01000000, 0x8f14);
	psram_hal_cmd_read(0x01000000);

	return 0;
}

__FLASH_BOOT_CODE static int psram_hal_APS128XXO_OB9_init(uint32_t *id)
{
	uint32_t val = 0;
	psram_hal_set_mode_value(PSRAM_MODE7);
#if (!defined(CONFIG_SOC_BK7256XX))
	//psram_hal_set_reg5_value(0x292);
	psram_hal_set_reg5_value(0x380);
#endif
	psram_hal_set_cmd_reset();

	psram_delay(500);

	val = psram_hal_cmd_read(0x00000000);//1 0001 10001101

#if (defined(CONFIG_SOC_BK7256XX))
	// 8 line
	val = ((val >> 8) & 0xFF) | ((val & 0xFF) << 8);
#endif
	if (val == 0 || val != *id)
	{
		return -1;
	}
	else
	{
		*id = val;
	}

	val = psram_hal_get_regb_value();
#if (defined(CONFIG_SOC_BK7256XX))
	val = (val & ~(0x7 << 10)) | (0x4 << 10);
#else
	val = (val & ~(0x7 << 2)) | (0x4 << 2);
#endif
	psram_hal_cmd_write(0x00000000, val);

	psram_hal_cmd_read(0x00000004);
	val = psram_hal_get_regb_value();
#if (defined(CONFIG_SOC_BK7256XX))
	val = (val & ~(0x7 << 13)) | (0x6 << 13);
#else
	val = (val & ~(0x7 << 5)) | (0x6 << 5);
#endif
	psram_hal_cmd_write(0x00000004, val);

	psram_hal_cmd_read(0x00000008);//1 0001 10001101
	val = psram_hal_get_regb_value();
	val |= 0x40;
	psram_hal_cmd_write(0x00000008,val);

	return 0;
}

__FLASH_BOOT_CODE uint32_t psram_hal_config_init(uint32_t id)
{
	int ret = 0;
	uint32_t val = 0;
	uint32_t type = id;

	psram_hal_set_sf_reset(1);

	/* set psram bypass */
	val = psram_hal_get_reg2_value();
	val |= (0x1 << 1);
	psram_hal_set_reg2_value(val);

	if (id != 0)
	{
		if (id == PSRAM_APS6408L_ID)
		{
			psram_hal_APS6408L_init(&type);
			return type;
		}
		else if (id == PSRAM_APS128XXO_OB9_ID)
		{
			psram_hal_APS128XXO_OB9_init(&type);
			return type;
		}
		else //id == PSRAM_W955D8MKY_5J_ID
		{
			psram_hal_W955D8MKY_5J_init(&type);
			return type;
		}
	}
	else
	{
		type = PSRAM_APS6408L_ID;
		ret = psram_hal_APS6408L_init(&type);
		if (ret == 0)
		{
			return type;
		}

		type = PSRAM_APS128XXO_OB9_ID;
		ret = psram_hal_APS128XXO_OB9_init(&type);
		if (ret == 0)
		{
			return type;
		}

		type = PSRAM_W955D8MKY_5J_ID;
		ret = psram_hal_W955D8MKY_5J_init(&type);

		return type;
	}

}

// config 1: psram power and clk config, need wait clk stable
__FLASH_BOOT_CODE void psram_hal_power_clk_enable(uint8_t enable)
{
	if (enable)
	{
		psram_delay(500);

		sys_drv_psram_ldo_enable(1);
		//psram_delay(500);
		extern void delay_us(UINT32 us);
		delay_us(1000);
		bk_pm_module_vote_power_ctrl(POWER_SUB_MODULE_NAME_BAKP_PSRAM, PM_POWER_MODULE_STATE_ON);

		// psram bus clk always open
		sys_drv_psram_psram_disckg(1);

		//psram 80M
		psram_hal_set_clk(PSRAM_80M);

		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);//psram_clk_enable bit19=1
	}
	else
	{
		bk_psram_heap_init_flag_set(false);

		bk_pm_module_vote_power_ctrl(POWER_SUB_MODULE_NAME_BAKP_PSRAM, PM_POWER_MODULE_STATE_OFF);
		psram_hal_set_sf_reset(0);

		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_DOWN);//psram_clk_disable

		// power down
		sys_drv_psram_ldo_enable(0);
	}

	psram_delay(3000);
}

// config 2: reset psram and wait psram ready
void psram_hal_reset(void)
{
#if (defined(CONFIG_SOC_BK7256XX))
	psram_hal_set_sf_reset(1);

#if (defined(CONFIG_PSRAM_APS6408L_O))

	int chip_id = bk_get_hardware_chip_id_version();

	if (chip_id == CHIP_VERSION_C)
		psram_hal_set_mode_value(PSRAM_MODE1);
	else
		psram_hal_set_mode_value(PSRAM_MODE3);
#else
	//psram_hal_set_mode_value(PSRAM_MODE5);// mode 5
	psram_hal_set_mode_value(PSRAM_MODE4);// mode 4
#endif
	psram_hal_set_cmd_reset();
#endif //CONFIG_SOC_BK7256XX
}

// config 3: psram config
void psram_hal_config(void)
{
#if (defined(CONFIG_SOC_BK7256XX))
	uint32_t val = 0;

#if (defined(CONFIG_PSRAM_APS6408L_O))

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	val = psram_hal_get_regb_value();

	val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x3 << 8);

	psram_hal_cmd_write(0x00000000, val);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz

	psram_hal_cmd_write(0x00000004, val);
#else
	uint32_t io_drv = 0; /*range [0, 3]*/

	val = psram_hal_cmd_read(0x01000000);

	//val = 0x8F1F | (io_drv << 12);// mode 5
	val = 0x8F1C | (io_drv << 12);// mode 4

	psram_hal_cmd_write(0x01000000, val);

	psram_hal_cmd_read(0x01000000);
#endif

#endif //CONFIG_SOC_BK7256XX
}


