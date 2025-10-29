import time
from serial import Timeout
from ..reg_controller import RegController


class BK7236_HighBaudrate(RegController):
    def __init__(self, serial_instance):
        super().__init__(serial_instance)
        self.ser = serial_instance
        self.sys_reg_base = 0x44010000
        self.sys_ana_reg0_addr = self.sys_reg_base+(0x40<<2)
        self.sys_ana_reg_max = 28
        self.sys_ana_reg_spi_state = self.sys_reg_base+(0x3a<<2)
        self.aon_pmu_reg_base = 0x44000000
        self.pmu_id_mask = 0xFFFF0000
        self.pmu_id_mpw_v2_3 = 0x22710010
        self.pmu_id_mpw_v4 = 0x22C20010
        self.pmu_id_mpw_a = 0x23640810
        self.pm_clksel_flash_480m = 0x1
        self.pm_clksel_core_320m = 0x2

        self.uart_0__reg_base = 0x44820000
        self.spi_reg_base = 0x44870000
        self.spi_reg_ctrl_addr = self.spi_reg_base+0x4*4
        self.spi_2_reg_base = 0x45880000
        self.spi_2_reg_ctrl_addr = self.spi_2_reg_base+0x4*4
        self.uart_config_clk_divid = 8
    

    def _sys_analog_get(self, ana_reg):
        if ana_reg<0 or ana_reg>=self.sys_ana_reg_max:
            return 0
        reg_addr = self.sys_ana_reg0_addr+ana_reg*4
        return self.read_reg(reg_addr)
    
    def _sys_analog_set(self, ana_reg, val:int):
        if ana_reg<0 or ana_reg>=self.sys_ana_reg_max:
            return False
        reg_addr = self.sys_ana_reg0_addr+ana_reg*4
        self.write_reg(reg_addr,val)
        while True:
            if not (self.read_reg(self.sys_ana_reg_spi_state)&(1<<ana_reg)):
                break
    def _sys_analog_set_bit_val(self,ana_reg,pos,mask,value):
        res = self.read_reg(self.sys_ana_reg0_addr+ana_reg*4)
        if res is None:
            return False
        res &= ~(mask<<pos)
        res |= ((value&mask)<<pos)
        return self._sys_analog_set(ana_reg,res)

    def _get_aon_pmu_id(self):
        val = self.read_reg(self.aon_pmu_reg_base+(0x7c<<2))
        if val is None:
            return 0
        return val

    def _sys_set_ana_reg5_adc_div(self, val:int):
        return self._sys_analog_set_bit_val(5,10,0x3,val)
    
    def _sys_set_ana_reg0_dsptrig(self, val:int):
        return self._sys_analog_set_bit_val(0,26,0x1,val)
    
    def _sys_set_ana_reg9_spi_latch1v(self, val:int):
        return self._sys_analog_set_bit_val(9,9,0x1,val)
    
    def _sys_set_ana_reg3_inbufen0v9(self, val:int):
        return self._sys_analog_set_bit_val(3,6,0x1,val)

    def _sys_set_ana_reg5_pwdaudpll(self, val:int):
        return self._sys_analog_set_bit_val(5,13,0x1,val)
    
    def _sys_set_ana_reg25_spi_trigger(self, val:int):
        return self._sys_analog_set_bit_val(0x19,18,0x1,val)
    
    def _sys_get_cpu_clk_div_mode1_cksel_core(self):
        '''
        struct {
        uint32_t clkdiv_core : 4; /**<bit[0 : 3] */
        uint32_t cksel_core : 2; /**<bit[4 : 5] */
        uint32_t clkdiv_bus : 1; /**<bit[6 : 6] */
        uint32_t reserved_bit_7_7 : 1; /**<bit[7 : 7] */
        uint32_t clkdiv_uart0 : 2; /**<bit[8 : 9] */
        uint32_t clksel_uart0 : 1; /**<bit[10 : 10] */
        uint32_t clkdiv_uart1 : 2; /**<bit[11 : 12] */
        uint32_t cksel_uart1 : 1; /**<bit[13 : 13] */
        uint32_t clkdiv_uart2 : 2; /**<bit[14 : 15] */
        uint32_t cksel_uart2 : 1; /**<bit[16 : 16] */
        uint32_t cksel_sadc : 1; /**<bit[17 : 17] */
        uint32_t cksel_pwm0 : 1; /**<bit[18 : 18] */
        uint32_t cksel_pwm1 : 1; /**<bit[19 : 19] */
        uint32_t cksel_tim0 : 1; /**<bit[20 : 20] */
        uint32_t cksel_tim1 : 1; /**<bit[21 : 21] */
        uint32_t cksel_tim2 : 1; /**<bit[22 : 22] */
        uint32_t reserved_bit_23_23 : 1; /**<bit[23 : 23] */
        uint32_t cksel_i2s : 1; /**<bit[24 : 24] */
        uint32_t cksel_aud : 1; /**<bit[25 : 25] */
        uint32_t clkdiv_jpeg : 4; /**<bit[26 : 29] */
        uint32_t cksel_jpeg : 1; /**<bit[30 : 30] */
        uint32_t clkdiv_disp_l : 1; /**<bit[31 : 31] */
        };
        '''
        raw_v = self.read_reg(self.sys_reg_base+0x8*4)
        return (raw_v>>4)&0x3
    
    def _sys_set_cpu_clk_div_mode1_cksel_core(self, val:int):
        return self._set_reg_bit_val(self.sys_reg_base+0x8*4,4,0x3,val)

    def _sys_get_cpu_clk_div_model_clkdiv_core(self):
        raw_v = self.read_reg(self.sys_reg_base+0x8*4)
        return raw_v&0xf
    
    def _sys_set_cpu_clk_div_mode1_clkdiv_core(self, val:int):
        return self._set_reg_bit_val(self.sys_reg_base+0x8*4,0,0xf,val)

    def _sys_set_cpu_clk_div_mode2_cksel_flash(self, val:int):
        '''
        uint32_t clkdiv_disp_h : 3; /**<bit[0 : 2] */
        uint32_t cksel_disp : 1; /**<bit[3 : 3] */
        uint32_t ckdiv_psram : 1; /**<bit[4 : 4] */
        uint32_t cksel_psram : 1; /**<bit[5 : 5] */
        uint32_t ckdiv_qspi0 : 4; /**<bit[6 : 9] */
        uint32_t cksel_qspi0 : 1; /**<bit[10 : 10] */
        uint32_t reserved_11_13 : 3; /**<bit[11 : 13] */
        uint32_t ckdiv_sdio : 3; /**<bit[14 : 16] */
        uint32_t cksel_sdio : 1; /**<bit[17 : 17] */
        uint32_t ckdiv_auxs : 4; /**<bit[18 : 21] */
        uint32_t cksel_auxs : 2; /**<bit[22 : 23] */
        uint32_t cksel_flash : 2; /**<bit[24 : 25] */
        uint32_t ckdiv_flash : 2; /**<bit[26 : 27] */
        uint32_t ckdiv_i2s0 : 3; /**<bit[28 : 30] */
        uint32_t reserved_bit_31_31 : 1; /**<bit[31 : 31] */
        '''
        return self._set_reg_bit_val(self.sys_reg_base+0x9*4,24,0x3,val)
        

    def _sys_get_cpu_clk_div_mode2_cksel_flash(self):        
        raw_v = self.read_reg(self.sys_reg_base+0x9*4)
        return (raw_v>>24)&0x3
    
    def _sys_get_cpu_clk_div_mode2_ckdiv_flash(self):
        raw_v = self.read_reg(self.sys_reg_base+0x9*4)
        return (raw_v>>26)&0x3
    
    def _sys_set_cpu_clk_div_mode2_ckdiv_flash(self, val:int):
        return self._set_reg_bit_val(self.sys_reg_base+0x9*4,26,0x3,val)
    
    def _sys_set_cpu_anaspi_freq(self, val:int):
        '''
        uint32_t anaspi_freq : 6; /**<bit[0 : 5] */
        uint32_t reserved_bit_6_31 : 1; /**<bit[6 : 31] */
        '''
        return self._set_reg_bit_val(self.sys_reg_base+0xb*4,0,0x3f,val)

    def sys_hal_dpll_cpu_flash_time_early_init(self, pmu_id):
        core_sel = self._sys_get_cpu_clk_div_mode1_cksel_core()
        core_div = self._sys_get_cpu_clk_div_model_clkdiv_core()
        if pmu_id&self.pmu_id_mask == self.pmu_id_mpw_v2_3&self.pmu_id_mask \
         or pmu_id&self.pmu_id_mask == self.pmu_id_mpw_v4&self.pmu_id_mask:
            self._sys_set_cpu_clk_div_mode2_cksel_flash(0x2)
        else:
            if self._sys_get_cpu_clk_div_mode2_cksel_flash() != self.pm_clksel_flash_480m:
                if self._sys_get_cpu_clk_div_mode2_ckdiv_flash()!=0x1:
                    self._sys_set_cpu_clk_div_mode2_ckdiv_flash(0x1)
                self._sys_set_cpu_clk_div_mode2_cksel_flash(self.pm_clksel_flash_480m)
        if core_sel != self.pm_clksel_core_320m and core_div!=0x1:
            self._sys_set_cpu_clk_div_mode1_clkdiv_core(int(320000000/4700000)-1)
            time.sleep(0.01)
            self._sys_set_cpu_clk_div_mode1_cksel_core(0x2)
            time.sleep(0.01)

    def sys_hal_early_init(self):
        pmu_id = self._get_aon_pmu_id()
        val = self._sys_analog_get(5)
        val |= (0x1<<5)|(0x1<<3)|(0x1<<2)
        self._sys_analog_set(5,val)
        self._sys_set_ana_reg5_adc_div(1)
        val = self._sys_analog_get(0)
        val |= (0x13<<20)
        self._sys_analog_set(0,val)
        self._sys_analog_set(0,0xF1305B56)
        self._sys_set_ana_reg0_dsptrig(1)
        self._sys_set_ana_reg0_dsptrig(0)
        self._sys_analog_set(2,0x7E003450)
        self._sys_analog_set(3,0xC5F00B88)

        self._sys_set_ana_reg9_spi_latch1v(1)
        if pmu_id&self.pmu_id_mask == self.pmu_id_mpw_v2_3&self.pmu_id_mask:
            self._sys_analog_set(8,0x57E62FFE)
        else:
            self._sys_analog_set(8,0x57E62F26)

        self._sys_analog_set(9,0x787BC8A4)
        if pmu_id&self.pmu_id_mask == self.pmu_id_mpw_v2_3&self.pmu_id_mask:
            self._sys_analog_set(10,0xC35543C7)
            self._sys_analog_set(11,0x9FEF31F7)
            self._sys_analog_set(12,0x9F03EF6F)
            self._sys_analog_set(13,0x1F6FB3FF)

        elif pmu_id&self.pmu_id_mask == self.pmu_id_mpw_v4&self.pmu_id_mask:
            self._sys_analog_set(10,0xC35543C7)
            self._sys_analog_set(11,0xD77EB9FF)
            self._sys_analog_set(12,0xD77ECA4A)
            self._sys_analog_set(13,0x547AB0F5)
        elif pmu_id&self.pmu_id_mask == self.pmu_id_mpw_a&self.pmu_id_mask:
            self._sys_analog_set(10,0xC35543C7)
            self._sys_analog_set(11,0xD07EB878)
            self._sys_analog_set(12,0xD07ECA40)
            self._sys_analog_set(13,0x727070EE)
            self._sys_analog_set(25,0x961FAA4)
            self._sys_set_ana_reg3_inbufen0v9(1)
        else:
            self._sys_analog_set(10,0xC3D543A7)
            self._sys_analog_set(11,0xF47E9878)
            self._sys_analog_set(12,0xF47ECA20)
            self._sys_analog_set(13,0x727070EE)
            self._sys_analog_set(25,0x961FAA4)
            self._sys_set_ana_reg3_inbufen0v9(1)
        self._sys_set_ana_reg9_spi_latch1v(0)
        self.sys_hal_dpll_cpu_flash_time_early_init(pmu_id)

    def spi_set_divider_armino(self, div:int):
        val_ori = self.read_reg(self.spi_reg_ctrl_addr)
        div &= 0xff
        val_r = val_ori
        val_r &=~0xff00
        val_r |= (div<<8)
        if val_r!=val_ori:
            self.write_reg(self.spi_reg_ctrl_addr,val_r)

        val_ori = self.read_reg(self.spi_2_reg_ctrl_addr)
        div &= 0xff
        val_r = val_ori
        val_r &=~0xff00
        val_r |= (div<<8)
        if val_r!=val_ori:
            self.write_reg(self.spi_2_reg_ctrl_addr,val_r)  
            
    def uart_select_clock(self, dco_freq:int, baudrate:int):
        val_r = self.read_reg(self.uart_0__reg_base+4*4)
        if val_r is None:
            return False
        val_r &= ~(0xFFFF00)
        baud_div = int(dco_freq/baudrate) - 1
        val_r |= (baud_div << self.uart_config_clk_divid)
        self.write_reg(self.uart_0__reg_base+4*4,val_r)
            #return False
        actual_baud = int(26000000/(baud_div+1))
        self.ser.reset(actual_baud)
        time.sleep(0.02)
        val_actual_r = self.read_reg(self.uart_0__reg_base+4*4)
        if val_actual_r is None or val_actual_r!= val_r:
            return False
        #uart change to apll
        val_r = self.read_reg(self.sys_reg_base+8*4)
        if val_r is None:
            return False
        val_r |= (1<<10)
        self.write_reg(self.sys_reg_base+8*4,val_r)
            #return False
        self.ser.reset(baudrate)
        time.sleep(0.02)
        val_actual_r = self.read_reg(self.sys_reg_base+8*4)
        if val_actual_r is None or val_actual_r!= val_r:
            return False

        return True

    def use_apll_set_baudrate(self, baudrate:int):
        self.sys_hal_early_init()
        #sys_drv_apll_en val=1
        self._sys_set_ana_reg5_pwdaudpll(0)
        #sys_drv_apll_cal_val_set
        self._sys_analog_set(26,0x88AF2EC9)
        #sys_drv_apll_config_set
        self._sys_analog_set(25,0xC2A0AE86)
        #sys_drv_apll_spi_trigger_set
        self._sys_set_ana_reg25_spi_trigger(1)
        time.sleep(0.001)
        self._sys_set_ana_reg25_spi_trigger(0)

        capacity_p44 = self.read_reg(0x44000400+0x2C*4)
        capacity_p44|=(2<<8)
        self.write_reg(0x44000400+0x2C*4,capacity_p44)

        self.spi_set_divider_armino(0)
        return self.uart_select_clock(90000000,baudrate)



if __name__ == "__main__":
    pass
 