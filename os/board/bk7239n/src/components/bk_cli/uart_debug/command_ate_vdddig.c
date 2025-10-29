#include "command_ate.h"

#if defined(CONFIG_ATE_TEST)
#include "sys_hal.h"
#include "aon_pmu_hal.h"
#include <driver/otp.h>
#include <driver/efuse.h>

extern UINT32 device_id;

extern void delay_us(UINT32 us);
extern void ate_time_delay(volatile uint32_t times);
void rwnx_cal_dia_start();
void rwnx_cal_dia_stop();
void rwnx_cal_set_reg_mod_pa(UINT16 reg_mod, UINT16 reg_pa);

static int check_efuse_error_range(UINT8 addr, UINT8 new_byte, UINT8 error_range)
{
    UINT8 addr_efuse, data;
    UINT16 old_value;
    UINT16 new_value;
    int ret;

    addr_efuse = addr;
    data = 0;

    ret = bk_efuse_read_byte(addr_efuse, &data);
    if (ret != 0)
    {
        return -1;
    }

    new_value = new_byte;
    old_value = data;

    if ((new_value > old_value + error_range) || (old_value > new_value + error_range))
    {
        return -1;
    }

    return 0;
}

/**
 * user  format: [0x80, 0x7F]=>[-128, 127], base on vdddig 4, should adjust 
 * reg   format: vdddig [0,7], bandgap [0x00, 0x3F]
 * efuse format: xxyy yyyy, xx {00 01 10 11}=>{4 5 6 3}
 */
int sctrl_convert_vdddig_from_efuse_to_user(INT8 in_value, INT8 *out_value)
{
    if (0xC0 == (in_value & 0xC0))
    {
        /* VDDDIG=3 */
        *out_value = (in_value & 0x3F) - 0x20;
    }
    else if (0x40 == (in_value & 0xC0))
    {
        /* VDDDIG=5 */
        *out_value = (in_value & 0x3F) + 0x20;
    }
    else if (0x80 == (in_value & 0xC0))
    {
        /* VDDDIG=6 */
        *out_value = (in_value & 0x3F) + 0x40;
    }
    else
    {
        /* VDDDIG=4 */
        *out_value = in_value;
    }

    return 0;
}

int sctrl_convert_vdddig_from_user_to_efuse(INT8 in_value, INT8 *out_value)
{
    UINT32 vdddig;
    INT32 bandgap;

    /* should call sctrl_convert_vdddig_from_user_to_reg to convert bandgap to [0x10, 0x2F] */
    /* but legacy code already use vdddig=4 and bandgap=[0,63] */
#if 0
    sctrl_convert_vdddig_from_user_to_reg(in_value, &vdddig, &bandgap);
#else
    bandgap = (INT32)in_value;
    vdddig  = 4;

    while ((bandgap < 0x00) && (vdddig > 3))
    {
        vdddig -= 1;
        bandgap += 0x20;
    }
    while ((bandgap >= 0x40) && (vdddig < 6))
    {
        vdddig += 1;
        bandgap -= 0x20;
    }
#endif
    if ((vdddig < 3) || (6 < vdddig))
    {
        return -1;
    }
    else if (vdddig == 3)
    {
        *out_value = bandgap | (0x3 << 6);
    }
    else
    {
        *out_value = bandgap | ((vdddig - 4) << 6);
    }

    return 0;
}

int sctrl_convert_vdddig_from_user_to_reg(INT8 in_value, UINT32 *out_vdddig, INT32 *out_bandgap)
{
    *out_bandgap = (INT32)in_value;
    *out_vdddig  = 4;

    while ((*out_bandgap < 0x10) && (*out_vdddig > 3))
    {
        *out_vdddig -= 1;
        *out_bandgap += 0x20;
    }
    while ((*out_bandgap >= 0x30) && (*out_vdddig < 6))
    {
        *out_vdddig += 1;
        *out_bandgap -= 0x20;
    }

    return 0;
}

#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
#define BANDGAP_BGCAL_MANUAL_MASK    (0x3FU)
#define SYS_DIG_VDD_ACTIVE_MASK      (0xF)

int sctrl_load_vdddig_from_efuse(void)
{
    bk_err_t result;
    uint8_t new_bandgap = 0xFF;

    result = bk_otp_apb_read(OTP_VDDDIG_BANDGAP, (uint8_t *)&new_bandgap, sizeof(new_bandgap));
    if ((BK_OK != result) || (0x00 == new_bandgap) || (0xFF == new_bandgap))
    {
        return -1;
    }

    sys_hal_set_bgcalm(new_bandgap);

    return 0;
}

int sctrl_set_vdddig_1voltage(void)
{
    //not support
    return 0;
}

int sctrl_set_vdddig_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    tx_buffer[0] = 0x55;

    if (cnt < 3)
    {
        /* 55 54 xx - xx: vdddig */
        tx_buffer[1] = 0xCC;
    }
    else
    {
        UINT32 vdddig = content[2];

        if (vdddig > SYS_DIG_VDD_ACTIVE_MASK)
        {
            /* failed */
            tx_buffer[1] = 0xCC;
        }
        else
        {
            /* success */
            sys_hal_set_vdd_value(vdddig);
            tx_buffer[1] = 0x33;
        }
    }
    uart_send_bytes_for_ate(tx_buffer, 2);

	return 0;
}

int sctrl_set_bandgap_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    UINT8 bandgap = 0;

	tx_buffer[0] = 0x55;
    if (cnt < 3)
    {
        if (bk_otp_apb_read(OTP_VDDDIG_BANDGAP, &bandgap, sizeof(bandgap)) == 0)
        {
            /* success */
            tx_buffer[0] = 0x0E;
            tx_buffer[1] = bandgap;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return 0;
        }
        else
        {
            tx_buffer[1] = 0xAA;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
    }
    bandgap = content[2];
    if (bandgap > BANDGAP_BGCAL_MANUAL_MASK)
    {
        tx_buffer[1] = 0xAA;
        uart_send_bytes_for_ate(tx_buffer, 2);
        return -1;
    }

    if (bk_otp_apb_update(OTP_VDDDIG_BANDGAP, &bandgap, sizeof(bandgap)) == 0)
    {
        /* success */
        sys_hal_set_bgcalm(bandgap);
        tx_buffer[1] = 0x33;
    }
    else
    {
        tx_buffer[1] = 0xCC;
    }

    uart_send_bytes_for_ate(tx_buffer, 2);

    return 0;
}

int sctrl_set_bandgap_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    UINT8 bandgap = 0;

    if (cnt > 2) {
		//set bandgap
		bandgap = content[2];
    } else {
		bandgap = sys_hal_cali_bgcalm();
    }

    sys_hal_set_bgcalm(bandgap);

	tx_buffer[0] = 0xE;
	tx_buffer[1] = bandgap;
    uart_send_bytes_for_ate(tx_buffer, 2);

    return 0;
}

int sctrl_check_dpll_unlock(UINT8 bandgap, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_dia_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    UINT8 dia = 0;

	tx_buffer[0] = 0x55;
    if (cnt < 3)
    {
        if (bk_otp_apb_read(OTP_DIA, &dia, sizeof(dia)) == 0)
        {
            /* success */
            rwnx_cal_set_reg_mod_pa(0, dia);
            tx_buffer[0] = 0x0E;
            tx_buffer[1] = dia;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return 0;
        }
        else
        {
            tx_buffer[1] = 0xAA;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
    }
    dia = content[2];
    if (dia > 0x1F)
    {
        tx_buffer[1] = 0xDD;
        uart_send_bytes_for_ate(tx_buffer, 2);
        return -1;
    }

    if (bk_otp_apb_update(OTP_DIA, &dia, sizeof(dia)) == 0)
    {
        /* success */
        tx_buffer[1] = 0x33;
    }
    else
    {
        tx_buffer[1] = 0xCC;
    }

	rwnx_cal_dia_stop();
    uart_send_bytes_for_ate(tx_buffer, 2);

    return 0;
}

int sctrl_set_dia_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    UINT8 dia = 0;

	rwnx_cal_dia_start();
    if (cnt > 2) {
        //set dia
		dia = content[2];
        tx_buffer[0] = 0xE;
        tx_buffer[1] = dia;
        rwnx_cal_set_reg_mod_pa(0, dia);
    } else {
        tx_buffer[0] = 0x55;
        tx_buffer[1] = 0xCC;
    }

    uart_send_bytes_for_ate(tx_buffer, 2);

    return 0;
}

#elif ((!defined(CONFIG_SOC_BK7251)) && (!defined(CONFIG_SOC_BK7271)))
#define BANDGAP_BGCAL_MANUAL_MASK    (0x3FU)
#define SYS_DIG_VDD_ACTIVE_MASK      (0x7)

#define DEVICE_ID_BK7256            (0x22041021)
#define DEVICE_ID_BK7256_C          (0x22091022)

int sctrl_load_vdddig_from_efuse(void)
{
    UINT8 addr,data;
#define VDDIG_MAX 7
#define VDDIG_MIN 4
    int32_t old_vddig;
    int32_t new_vddig;
    uint32_t old_bandgap_calm;
    uint32_t new_bandgap_calm;

    addr = (UINT8)17;
    data = 0;
    bk_efuse_read_byte(addr, &data);
    if ((0x00 == data) || (0xFF == data))
    {
        return -1;
    }

    old_bandgap_calm = sys_hal_get_bgcalm();
    old_vddig = (int32_t)sys_hal_get_vdd_value();
    new_bandgap_calm = (uint32_t)(data & BANDGAP_BGCAL_MANUAL_MASK);
    new_vddig = (int32_t)((data >> 6) & 0x3);

    new_vddig += 4;

    if ((old_bandgap_calm != new_bandgap_calm) || (old_vddig != new_vddig))
    {
    #if 0
        /* keep vdddig and bandgap as calibrated by factory in new version */
        if (device_id != DEVICE_ID_BK7256 && device_id != DEVICE_ID_BK7256_C)
        {
            /* keep bandgap_calm in [0x10, 0x30) */
            if ((new_bandgap_calm >= 0x30) && (new_vddig < VDDIG_MAX))
            {
                new_vddig = new_vddig + 1;
                new_bandgap_calm = new_bandgap_calm - 0x20;
            }
            else if ((new_bandgap_calm < 0x10) && (new_vddig > VDDIG_MIN))
            {
                new_vddig = new_vddig - 1;
                new_bandgap_calm = new_bandgap_calm + 0x20;
            }
        }
    #endif
        if ((new_vddig < VDDIG_MIN) || (new_vddig > VDDIG_MAX))
        {
            new_vddig = old_vddig;
            new_bandgap_calm = (uint32_t)old_bandgap_calm;
        }

        sys_hal_set_bgcalm(new_bandgap_calm);
        if (old_vddig != new_vddig)
        {
            sys_hal_set_vdd_value(new_vddig);
        }
    }

    return 0;
}

int sctrl_set_vdddig_1voltage(void)
{
    /* set vdddig to 1.00V, accordding efuse[12] as 1.17V */
    uint8_t addr,data;
    UINT32 vdddig;
    UINT32 bandgap;

    /* 1. read bandgap at 11th byte in efuse */
    addr = 11;
    data = 0;
    bk_efuse_read_byte(addr, &data);
    if ((0 == data) || (0xFF == data))
    {
        return -1;
    }

    /* 2. read vdddig in  */
    vdddig = (UINT32)sys_hal_get_vdd_value();
    if (vdddig < 2)
    {
        return -1;
    }

    /* 3. assume base + (vdddig * 0.1 + data * 0.0035)=1.17V */
    data = data & BANDGAP_BGCAL_MANUAL_MASK;
    /* base + (vdddig - 1) * 0.1 + (efuse.data - 17) * 0.0035=1.17-0.1-0.0595=1.0105V */
    /* base + (vdddig - 2) * 0.1 + (efuse.data + 12) * 0.0035=1.17-0.2+0.042=1.012V */
    if (data >= 17)
    {
        data -= 17;
        vdddig--;
    }
    else
    {
        data += 12;
        vdddig -= 2;
    }

    /* 4. update bandgap and vdddig */
    sys_hal_set_vdd_value(vdddig);
    bandgap = (UINT32)(BANDGAP_BGCAL_MANUAL_MASK & data);
    sys_hal_set_bgcalm(bandgap);

    /* success */
    return 0;
}

int sctrl_set_vdddig_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    tx_buffer[0] = 0x55;

    if (device_id != DEVICE_ID_BK7256 && device_id != DEVICE_ID_BK7256_C)
    {
        /* not valid on BK7231N_B/C/D/E/F */
        tx_buffer[1] = 0xAA;
    }
    else if (cnt < 4)
    {
        /* 55 54 xx yy - xx: vdddig yy: bandgap */
        tx_buffer[1] = 0xCC;
    }
    else
    {
        UINT32 vdddig = content[2];
        INT32 bandgap = content[3];

        if (vdddig > SYS_DIG_VDD_ACTIVE_MASK)
        {
            /* failed */
            tx_buffer[1] = 0xCC;
        }
        else if ((bandgap < 0) || (bandgap > BANDGAP_BGCAL_MANUAL_MASK))
        {
            /* failed */
            tx_buffer[1] = 0xCC;
        }
        else
        {
            /* success */
            sys_hal_set_bgcalm(bandgap);
            sys_hal_set_vdd_value(vdddig);
            tx_buffer[1] = 0x33;
        }
    }
    uart_send_bytes_for_ate(tx_buffer, 2);

	return 0;
}

int sctrl_set_bandgap_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    uint8 addr, data;

    tx_buffer[0] = 0x55;
    #if 0
    if(device_id != DEVICE_ID_BK7256 && device_id != DEVICE_ID_BK7256_C)
    {
        /* 55 43 xx */
        if (cnt < 3)
        {
            tx_buffer[1] = 0xCC;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
        addr = 17;
        data = ((UINT8)content[2] & BANDGAP_BGCAL_MANUAL_MASK);
        /* map vdddig[3,4,5,6] to [3,0,1,2] at bit[7:6] */
        if (sctrl_convert_vdddig_from_user_to_efuse(content[2], (INT8 *)&data) < 0)
        {
            tx_buffer[1] = 0xAA;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
        if (bk_efuse_write_byte(addr, data) == 0)
        {
            /* success */
            tx_buffer[1] = 0x33;
        }
        else if(check_efuse_error_range(addr, data, 3) == 0)
        {
            /* error in [-3, 3] treat exist value as right */
            tx_buffer[1] = 0x33;
        }
        else
        {
            tx_buffer[1] = 0xCC;
        }
        uart_send_bytes_for_ate(tx_buffer, 2);
    }
    else
    #endif
    {
        /* 55 43 xx yy - xx: vdddig yy: bandgap */
        UINT32 vdddig;

        addr = 17;

        if (cnt < 4)
        {
            tx_buffer[1] = 0xAA;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
        vdddig = content[2];
        if (content[3] < 0)
        {
            data = 0;
        }
        else if (content[3] > BANDGAP_BGCAL_MANUAL_MASK)
        {
            data = BANDGAP_BGCAL_MANUAL_MASK;
        }
        else
        {
            data = ((UINT8)content[3] & BANDGAP_BGCAL_MANUAL_MASK);
        }

        /* map vdddig[4,5,6,7] to [0,1,2,3] at bit[7:6] */
        if ((vdddig < 4) || (7 < vdddig))
        {
            tx_buffer[1] = 0xAA;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return -1;
        }
        else
        {
            data = data | ((vdddig - 4) << 6);
        }
        if (bk_efuse_write_byte(addr, data) == 0)
        {
            /* success */
            tx_buffer[1] = 0x33;
        }
        #if 0
        else if(check_efuse_error_range(addr, data, 3) == 0)
        {
            /* error in [-3, 3] treat exist value as right */
            tx_buffer[1] = 0x33;
        }
        #endif
        else
        {
            tx_buffer[1] = 0xCC;
        }
        uart_send_bytes_for_ate(tx_buffer, 2);
    }

    return 0;
}

int sctrl_set_bandgap_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    if (cnt < 3)
    {
        /* ignore */
        return -1;
    }
    else if (device_id != DEVICE_ID_BK7256 && device_id != DEVICE_ID_BK7256_C)
    {
        INT32 bandgap;
        UINT32 vdddig;

        sctrl_convert_vdddig_from_user_to_reg(content[2], &vdddig, &bandgap);

        sys_hal_set_bgcalm(bandgap);
        sys_hal_set_vdd_value(vdddig);
    }
    else
    {
        INT32 bandgap;

        if (content[2] < 0)
        {
            bandgap = 0;
        }
        else if (content[2] > BANDGAP_BGCAL_MANUAL_MASK)
        {
            bandgap = BANDGAP_BGCAL_MANUAL_MASK;
        }
        else
        {
            bandgap = (content[2] & BANDGAP_BGCAL_MANUAL_MASK);
        }

        sys_hal_set_bgcalm(bandgap);
    }

    return 0;
}

int sctrl_check_dpll_unlock(UINT8 bandgap, UINT8 *tx_buffer)
{
    UINT32 analog0;
    UINT32 analog0_bak;
    UINT32 analog5_bak;
    //UINT32 analog_state;
    //INT32 index;

    tx_buffer[0] = 0x55;

    if (device_id != DEVICE_ID_BK7256 && device_id != DEVICE_ID_BK7256_C)
    {
        /* not valid on BK7231N_B/C/D/E/F */
#if 1
        tx_buffer[1] = 0xAA;
        return -1;
#else
        tx_buffer[1] = 0x33;
        return 0;
#endif
    }

    /* 1. update dpll unlock threshold */
    analog0 = sys_hal_analog_get(ANALOG_REG0);
    analog0_bak = analog0;
    analog0 &= ~(0xF << 5);
    analog0 |= (0x3 << 5);
    sys_hal_analog_set(ANALOG_REG0, analog0);

    /* 2. update bandgap */
    analog5_bak = sys_hal_analog_get(ANALOG_REG5);
    sys_hal_set_bgcalm(bandgap);

    /* 3. check dpll unlock */
    ate_time_delay(1050); //100us in 120MHZ

    #if 0// not find SCTRL_ANALOG_STATE
    for (index = 3; index > 0; index--)
    {
		analog_state = REG_READ(SCTRL_ANALOG_STATE);
		if (analog_state & ANALOG_STATE_DPLL_UNLOCK)
		{
			break;
		}
		ate_time_delay(210); //20us in 120MHZ
    }
    #endif
    /* 4. recover bandgap */
    sys_hal_analog_set(ANALOG_REG5, analog5_bak);

    /* 5. recover dpll unlock threshold */
    sys_hal_analog_set(ANALOG_REG0, analog0_bak);

#if 0
	/* 6. cali dpll if needed */
	if (analog_state & ANALOG_STATE_DPLL_UNLOCK)
	{
		/* failed: dpll unlock */
		tx_buffer[1] = 0xCC;
		bk7011_cal_dpll();
	}
	else
	{
		/* success: dpll locked */
		tx_buffer[1] = 0x33;
	}
#endif
    return 0;
}

int sctrl_set_dia_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_dia_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}
#else

int sctrl_load_vdddig_from_efuse(void)
{
    //not support
    return 0;
}

int sctrl_set_vdddig_1voltage(void)
{
    //not support
    return 0;
}

int sctrl_set_vdddig_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_bandgap_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_bandgap_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_check_dpll_unlock(UINT8 bandgap, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_dia_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}

int sctrl_set_dia_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    //not support
    return 0;
}
#endif
#endif // CFG_ATE_TEST
