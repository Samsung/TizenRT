#include "command_ate.h"

#if defined(CONFIG_ATE_TEST)
#include "sys_driver.h"
#include <stdlib.h>
#include <driver/adc.h>
#include <driver/sdmadc.h>
#include <driver/otp.h>
#include <driver/efuse.h>
#include <driver/hal/hal_gpio_types.h>

typedef UINT16 heap_t;
size_t MinHeapInsert(heap_t *heap, size_t heap_size, heap_t x);
heap_t MinHeapReplace(heap_t *heap, size_t heap_size, heap_t x);

extern void test_adc_for_ate(adc_chan_t channel, adc_mode_t mode,\
					  uint32_t pre_div, uint32_t samp_rate,\
					  uint32_t filter, uint32_t sta_ctrl,\
					  uint32_t usCount, uint16_t *pDataBuffer);
extern void test_sdmadc_for_ate(sdmadc_chan_t channel, sdmadc_mode_t mode,\
					  uint32_t cic2_bpss, uint32_t cic2_gain,\
					  uint32_t int_enable, uint32_t cali_gains,\
					  uint32_t usCount, uint16_t *pDataBuffer);

void sctrl_cal_adc(const unsigned char *content, UINT8 *tx_buffer)
{
    UINT32 index;
    UINT32 sum = 0;
    static UINT16 values[2];
    UINT16 usData[100];
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
#else
    UINT8 addr, data;
#endif
    int ret = -1;
    /* try to load vdddiv from efuse before calibrate adc */
    if (!values[0] && !values[1])
    {
        ret = sctrl_load_vdddig_from_efuse();
        if (ret < 0)
        {
            /* vdddig not valid in efuse, return 55 BB */
            tx_buffer[0] = 0x55;
            tx_buffer[1] = 0xBB;
            uart_send_bytes_for_ate(tx_buffer, 2);
            return;
        }
    }

    test_adc_for_ate(3, 3, 0x20, 0x20, 0, 0x07, 100, usData);
    for (index = 36; index < 100; index++)
    {
        sum += usData[index];
    }
    sum = sum >> 6;

    if ((content[2] < 1) ||(content[2] > 3))
    {
        /* for legacy command, just return calibrated value */
        goto legacy_exit;
    }

    if (content[2] == 1)
    {
        /* save value of 1V */
        values[0] = (UINT16)sum;
    }
    else if (content[2] == 2)
    {
        /* save value of 2V */
        values[1] = (UINT16)sum;
    }
#if 0
    else if (device_id != DEVICE_ID_BK7256)
    {
        /* cali adc not valid on BK7231N_B/C/D/E/F, return 55 33 */
        tx_buffer[0] = 0x55;
        tx_buffer[1] = 0xAA;
        uart_print(tx_buffer, 2);
        return;
    }
#endif
    else
    {
        /* verify with 1.5V, success */
        index  = values[0];
        index += values[1];
        index  = index >> 1;


        tx_buffer[0] = 0x55;
        if (abs(index - sum) <= (sum /200)){
             tx_buffer[1] = 0x33;
        } else {
             tx_buffer[1] = 0xCC;
        }

        if ((values[0] >= 0x2000) || (values[1] >= 0x2000))
        {
            tx_buffer[1] = 0xCC;
        }
        if (tx_buffer[1] == 0x33)
        {
            /* BIT 07 06 05 04 03 02 01 00 */
            /* EFUSE_18 A7 A6 A5 A4 A3 A2 A1 A0 - 1V<7:0>*/
            /* EFUSE_19 B7 B6 B5 B4 B3 B2 B1 B0 - 1V<15:8> */
            /* EFUSE_20 B7 B6 B5 B4 B3 B2 B1 B0 - 2V<7:0> */
            /* EFUSE_21 Bb Ba B9 B8 Ab Aa A9 A8 - 2V<15:8> */
            /* note: Ax for 1Volt Bx for 2Volt */
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
            ret = bk_otp_apb_update(OTP_GADC_CALIBRATION, (uint8_t *)values, sizeof(values));
            if (ret != BK_OK)
            {
	            tx_buffer[1] = 0xCC;
	            goto exit;
            }
#else
            addr = 18;
            data = (UINT8)values[0];
            if (bk_efuse_write_byte(addr, data) != 0)
            {
                tx_buffer[1] = 0xCC;
                goto exit;
            }
            addr = 19;
            data = (UINT8)(values[0] >> 8);
            if (bk_efuse_write_byte(addr, data) != 0)
            {
                tx_buffer[1] = 0xCC;
                goto exit;
            }

            addr = 20;
            data = (UINT8)values[1];
            if (bk_efuse_write_byte(addr, data) != 0)
            {
                tx_buffer[1] = 0xCC;
                goto exit;
            }
            addr = 21;
            data = (UINT8)(values[1] >> 8);
            if (bk_efuse_write_byte(addr, data) != 0)
            {
                tx_buffer[1] = 0xCC;
                goto exit;
            }
#endif
        }
        goto exit;
    }

legacy_exit:
    tx_buffer[0] = 0x0E;
    tx_buffer[1] = (UINT8)(sum >> 0) & 0xFF;
    tx_buffer[2] = (UINT8)(sum >> 8) & 0xFF;
    uart_send_bytes_for_ate(tx_buffer,3);
    return;

exit:
    uart_send_bytes_for_ate(tx_buffer, 2);
    return;
}

void swap1(UINT16 a[], int low, int high)
{
    UINT16 t = a[low];
    a[low] = a[high];
    a[high] = t;
}

int partition(UINT16 a[], int low, int high)
{
    UINT16 point = a[low];

    while(low<high)
    {
        while(low<high && a[high]>=point)
        {
            high--;
        }
        swap1(a,low,high);

        while(low<high && a[low]<=point)
        {
            low++;
        }
        swap1(a,low,high);
    }
    return low;
}

void quicksort(UINT16 a[], int low, int high)
{
    if(low<high){
        UINT16 point = partition(a,low,high);
        quicksort(a,low,point-1);
        quicksort(a,point+1,high);
    }
}

void ate_cali_adc(const unsigned char *content, UINT8 *tx_buffer)
{
    UINT32 index;
    UINT32 sum = 0;
    static UINT16 values[2];
    UINT16 usData[150];
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
#else
    UINT8 addr, data;
    UINT8 i = 0;
#endif
    int ret = -1;

    sys_drv_set_ana_pwd_gadc_buf(0x1);
    /* try to load vdddiv from efuse before calibrate adc */
    if (!values[0] && !values[1])
    {
        ret = sctrl_load_vdddig_from_efuse();
        if (ret < 0)
        {
            /* vdddig not valid in efuse, return 55 BB */
            tx_buffer[0] = 0x55;
            tx_buffer[1] = 0xBB;
            goto exit;
        }
    }

    if (content[2] == 1)
    {
        tx_buffer[0] = 0x0E;
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
        ret = bk_otp_apb_read(OTP_GADC_CALIBRATION, tx_buffer + 1, 4);
        if (ret != BK_OK)
        {
            tx_buffer[0] = 0x55;
            tx_buffer[1] = 0xAA;
            goto exit;
        }
#else
        for(i = 0;i < 4;i++)
        {
            if(bk_efuse_read_byte(i+18, &data) != 0)
            {
                tx_buffer[0] = 0x55;
                tx_buffer[1] = 0xAA;
                goto exit;
            }
            tx_buffer[i+1] = data;
        }
#endif
        uart_send_bytes_for_ate(tx_buffer,5);
        return;
    }
    else if (content[2] == 2)
    {
        tx_buffer[0] = 0x55;
        if ((values[0] >= 0x2000) || (values[1] >= 0x2000))
        {
            tx_buffer[1] = 0xDD;
            goto exit;
        }
        /* EFUSE_18 A7 A6 A5 A4 A3 A2 A1 A0 - 1V<7:0>*/
        /* EFUSE_19 B7 B6 B5 B4 B3 B2 B1 B0 - 1V<15:8> */
        /* EFUSE_20 B7 B6 B5 B4 B3 B2 B1 B0 - 2V<7:0> */
        /* EFUSE_21 Bb Ba B9 B8 Ab Aa A9 A8 - 2V<15:8> */
        /* note: Ax for 1Volt Bx for 2Volt */
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
        ret = bk_otp_apb_update(OTP_GADC_CALIBRATION, (uint8_t *)values, sizeof(values));
        if (ret != BK_OK)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }
#else
        addr = 18;
        data = (UINT8)values[0];
        if (bk_efuse_write_byte(addr, data) != 0)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }
        addr = 19;
        data = (UINT8)(values[0] >> 8);
        if (bk_efuse_write_byte(addr, data) != 0)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }

        addr = 20;
        data = (UINT8)values[1];
        if (bk_efuse_write_byte(addr, data) != 0)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }
        addr = 21;
        data = (UINT8)(values[1] >> 8);
        if (bk_efuse_write_byte(addr, data) != 0)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }
#endif
        tx_buffer[1] = 0x33;
        sys_drv_set_ana_pwd_gadc_buf(0x0);
        goto exit;
    }
    else if ((content[2] == 0x10) || (content[2] == 0x15) || (content[2] == 0x20))
    {
        test_adc_for_ate(4, 3, 0x20, 0x20, 0, 0x07, 150, usData);
        quicksort(usData,50,149);

        for (index = 80; index < 120; index++)
        {
            sum += usData[index];
        }
        sum = sum / 40;
        /* save value of Volt */
        if(content[2] == 0x10)
            values[0] = (UINT16)sum;
        else if(content[2] == 0x20)
            values[1] = (UINT16)sum;
        else
            ;
        tx_buffer[0] = 0x0E;
        tx_buffer[1] = (UINT8)(sum >> 0) & 0xFF;
        tx_buffer[2] = (UINT8)(sum >> 8) & 0xFF;
        uart_send_bytes_for_ate(tx_buffer,3);
        return;
    }

exit:
    uart_send_bytes_for_ate(tx_buffer, 2);
    return;
}

#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
void ate_cali_sdmadc(const unsigned char *content, UINT8 *tx_buffer)
{
    UINT32 index;
    UINT32 sum = 0;
    static UINT16 values[2];
    UINT16 usData[150];
    int ret = -1;

    /* try to load vdddiv from efuse before calibrate adc */
    if (!values[0] && !values[1])
    {
        ret = sctrl_load_vdddig_from_efuse();
        if (ret < 0)
        {
            /* vdddig not valid in efuse, return 55 BB */
            tx_buffer[0] = 0x55;
            tx_buffer[1] = 0xBB;
            goto exit;
        }
    }

    if (content[2] == 1)
    {
        tx_buffer[0] = 0x0E;
        ret = bk_otp_apb_read(OTP_SDMADC_CALIBRATION, tx_buffer + 1, 4);
        if (ret != BK_OK)
        {
            tx_buffer[0] = 0x55;
            tx_buffer[1] = 0xAA;
            goto exit;
        }
        uart_send_bytes_for_ate(tx_buffer,5);
        return;
    }
    else if (content[2] == 2)
    {
        tx_buffer[0] = 0x55;
        /* EFUSE_18 A7 A6 A5 A4 A3 A2 A1 A0 - 1V<7:0>*/
        /* EFUSE_19 B7 B6 B5 B4 B3 B2 B1 B0 - 1V<15:8> */
        /* EFUSE_20 B7 B6 B5 B4 B3 B2 B1 B0 - 2V<7:0> */
        /* EFUSE_21 Bb Ba B9 B8 Ab Aa A9 A8 - 2V<15:8> */
        /* note: Ax for 1Volt Bx for 2Volt */
        ret = bk_otp_apb_update(OTP_SDMADC_CALIBRATION, (uint8_t *)values, sizeof(values));
        if (ret != BK_OK)
        {
            tx_buffer[1] = 0xCC;
            goto exit;
        }
        tx_buffer[1] = 0x33;
        goto exit;
    }
    else if ((content[2] == 0x10) || (content[2] == 0x15) || (content[2] == 0x20))
    {
        test_sdmadc_for_ate(SDMADC_4, SDMADC_CONTINUOUS_MODE, 0x1, 0x2d, 0x8, 0x1000, 150, usData);
        quicksort(usData,50,149);

        for (index = 80; index < 120; index++)
        {
            sum += usData[index];
        }
        sum = sum / 40 + CFG_SDMADC_OFFSET;
        /* save value of Volt */
        if(content[2] == 0x10)
            values[0] = (UINT16)sum;
        else if(content[2] == 0x20)
            values[1] = (UINT16)sum;
        else
            ;
        tx_buffer[0] = 0x0E;
        tx_buffer[1] = (UINT8)(sum >> 0) & 0xFF;
        tx_buffer[2] = (UINT8)(sum >> 8) & 0xFF;
        uart_send_bytes_for_ate(tx_buffer,3);
        return;
    }

exit:
    uart_send_bytes_for_ate(tx_buffer, 2);
    return;
}
#endif

#endif // CFG_ATE_TEST
