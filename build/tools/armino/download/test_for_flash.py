# modify the comport and file path before using 'python .\test_for_bk7236_xnf.py' to run test
import sys
import os
import time
import pandas
sys.path.append(os.path.dirname(__file__))

from bkutils.bk_log.log_utils import *
from bkutils.action_service.download import Download
from bkutils.action_service.dump import Dump
from bkutils.common.common import RESET_TYPE, LINKCHECK, DownloadInfo,ReadInfo,EraseInfo, BinInfo, parse_binfile_info, parse_writeinfo_otp, parse_device_mac_info, parse_erase_info, parse_dump_info, available_serial_ports, crc32_ver2
from bkutils.chip_controller.serial_helper import SerialHelper
from bkutils.chip_controller.base_controller import BaseController
from bkutils.chip_controller.flash_controller import FlashController
from bkutils.chip_controller.reg_controller import RegController
from bkutils.chip_controller import get_chip_set_with_chip_id


def write_cmd_and_power_down(serial_ctrl:SerialHelper, cmd):
    serial_ctrl.drain()  
    serial_ctrl.write_cmd(cmd)
    #reset
    serial_ctrl.ser.dtr = 0
    serial_ctrl.ser.rts = 1
    time.sleep(0.2) 
    serial_ctrl.ser.rts = 0
    

def write_flash_status_reg_val_power_down(flash_ctrl:FlashController,write_val,retry=5):
    #flash_ctrl = FlashController(serial_instance=serial_instance,flash_cfg=chip_set._flash_cfg)
    fwsp = flash_ctrl.BOOTROM_PROTOCOL.FlashWriteSRProtocol()
    write_reg_code = flash_ctrl._flash_cfg.flash_status_reg_write_code
    if len(write_reg_code) == 1:
        write_cmd_and_power_down(flash_ctrl.serial_instance,fwsp.cmd(write_reg_code[0], write_val))
        
    else:
        for idx in range(len(write_reg_code)):
            write_cmd_and_power_down(flash_ctrl.serial_instance,fwsp.cmd(write_reg_code[idx], [write_val[idx]]))
            
            time.sleep(0.01)


def test_flash_sr(port,work_baudrate=2000000,max_retry_cnt=5):
    
    bk_dl = Download(port,115200,LINKCHECK.BOOTROM,RESET_TYPE.DTR_RTS,50,True,max_retry_cnt,None)
    base_controller = BaseController(serial_instance=bk_dl.ser)
    
    result = False
    start = time.perf_counter()
    try:

        # get bus            
        tmp_res,linktype = base_controller.get_bus(reset_type=RESET_TYPE.DTR_RTS, linkcheck_obj=LINKCHECK.BOOTROM)
        if not tmp_res:
            raise Exception("get bus fail.")
        # get chip id
        time.sleep(0.1) #fix read chip id fail sometimes                 
        chip_id = base_controller.get_chip_id_retry()
        if chip_id is None:
            raise Exception("get chip id fail.")               
        BKLog.w("chip id is 0x{:x}".format(chip_id))     
        # init chip_set class
        chip_set = get_chip_set_with_chip_id(chip_id)
        if chip_set is None:
            raise Exception("get chip set fail.")
        # update serial
        chip_set.update_serial_instance(bk_dl.ser)
        # parse flash
        BKLog.i("parse flash info...") 
        if not chip_set.parse_flash_info():
            raise Exception("parse flash info fail.")
        BKLog.i("set baudrate to {0}...".format(work_baudrate)) 
        if not chip_set.set_baudrate(baudrate=work_baudrate, delay_ms=20):
            raise Exception("set baudrate {0} fail.".format(work_baudrate))
        
        BKLog.i("unprotect and protect flash before drop current.") 
        if not chip_set.un_protect_flash(protect=False):
            raise Exception("unprotect flash fail.")
        if not chip_set.un_protect_flash(protect=True):
            raise Exception("protect flash fail.")
        
        flash_ctrl = FlashController(serial_instance=bk_dl.ser,flash_cfg=chip_set._flash_cfg)
        unprotect_reg_val, mask = flash_ctrl._flash_cfg.unprotect_register_value
        reg_val = flash_ctrl._read_flash_status_reg_val()           
        write_val = unprotect_reg_val
        for _ in range(len(write_val)):
            write_val[_] = write_val[_] | (reg_val[_] & (mask[_] ^ 0xff))
        write_flash_status_reg_val_power_down(flash_ctrl,write_val)

        #reset
        # BKLog.i("do hard reset.") 
        # bk_dl.ser.ser.dtr = 0
        # bk_dl.ser.ser.rts = 1
        
        # for _ in range(500):
        #     BKLog.i(f"unprotect times:{_+1}.") 
        #     if not chip_set.un_protect_flash(protect=False):
        #         break
        #     BKLog.i(f"protect times:{_+1}.")
        #     if not chip_set.un_protect_flash(protect=True):
        #         break
        #     time.sleep(0.001)
        # time.sleep(0.2)
        # bk_dl.ser.ser.rts = 0
        #get bus
        bk_dl.ser.reset(115200)            
        tmp_res,linktype = base_controller.get_bus(reset_type=RESET_TYPE.DTR_RTS, linkcheck_obj=LINKCHECK.BOOTROM)
        if not tmp_res:
            raise Exception("get bus fail.")
        # get chip id
        time.sleep(0.1) #fix read chip id fail sometimes 
        BKLog.i("unprotect and protect flash after drop current.") 
        if not chip_set.un_protect_flash(protect=False):
            raise Exception("unprotect flash fail.")
        if not chip_set.un_protect_flash(protect=True):
            raise Exception("protect flash fail.")
        result = True
    except Exception as e:
        print(str(e))
        result = False
    finally:
        if result:
            print(f'test complete, all pass')
        costtime = time.perf_counter()-start
        print(f'Elapse time {costtime:.6f} seconds')


def test_flash_erase_and_write(port,bin_path,work_baudrate=2000000,max_retry_cnt=5):
    bk_dl = Download(port,115200,LINKCHECK.BOOTROM,RESET_TYPE.DTR_RTS,115200,50,True,max_retry_cnt,None)
    base_controller = BaseController(serial_instance=bk_dl.ser)
    
    result = False
    start = time.perf_counter()
    try:
        bintype,binlist,pathlist = parse_binfile_info(bin_path,0)
        if len(binlist)==0:    
            raise Exception(f"Bin file error: {bin_path}, please check.")                  
        # get bus            
        tmp_res,linktype = base_controller.get_bus(reset_type=RESET_TYPE.DTR_RTS, linkcheck_obj=LINKCHECK.BOOTROM)
        if not tmp_res:
            raise Exception("get bus fail.")
        # get chip id
        time.sleep(0.1) #fix read chip id fail sometimes                 
        chip_id = base_controller.get_chip_id_retry()
        if chip_id is None:
            raise Exception("get chip id fail.")               
        BKLog.w("chip id is 0x{:x}".format(chip_id))     
        # init chip_set class
        chip_set = get_chip_set_with_chip_id(chip_id)
        if chip_set is None:
            raise Exception("get chip set fail.")
        # update serial
        chip_set.update_serial_instance(bk_dl.ser)
               
        flash_ret, msg = bk_dl.download_flash(work_baudrate,binlist,False,chip_set,115200)
        if not flash_ret:
            raise Exception(msg)
        result = True
    except Exception as e:
        print(str(e))
        result = False
    finally:
        if result:
            print(f'test complete, all pass')
        costtime = time.perf_counter()-start
        print(f'Elapse time {costtime:.6f} seconds')

def flash_ctrl_wait_for_flash_busy_bit(reg_ctrl:RegController,reg_addr:int):   
    while True:
        read_ret = reg_ctrl.read_reg(reg_addr)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_addr:x}")
        if not (read_ret & 0x80000000):
            break
def flash_ctrl_read_flash_data(reg_ctrl:RegController, flash_addr:int,oplen:int):
    reg_flash_0x15 = 0x44030000 + 0x15*4
    reg_flash_0x04 = 0x44030000 + 0x04*4
    reg_flash_0x06 = 0x44030000 + 0x06*4
    read_content = bytearray()
    flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)
    
    while oplen>0:
        # 1. config read addr of flash & op=5
        read_ret = reg_ctrl.read_reg(reg_flash_0x15)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x15:x}")
        read_ret &= ~(0xffffff)
        read_ret |= flash_addr&0xffffff
        read_ret &= ~(0x1f<<24)
        read_ret |= (0x5<<24)
        if not reg_ctrl.write_reg(reg_flash_0x15,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x15:x}")

        # 2. trig OP=5
        read_ret = reg_ctrl.read_reg(reg_flash_0x04)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x04:x}")
        read_ret |= (0x1<<29)
        if not reg_ctrl.write_reg(reg_flash_0x04,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x04:x}")
        flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)

        # 3. read datas
        for _ in range(8):
            read_ret = int(reg_ctrl.read_reg(reg_flash_0x06))
            if read_ret is None:
                raise Exception(f"read reg fail @0x{reg_flash_0x06:x}")      
            read_content+=read_ret.to_bytes(4,"little")
        flash_addr+=0x20
        oplen-=0x20
    return read_content

def flash_monitor_debug(reg_ctrl:RegController):
    reg_sys_base = 0x44010000
    reg_sys_0x38 = reg_sys_base + 0x38 * 4
    write_reg_v = 0
    read_ret = reg_ctrl.read_reg(reg_sys_0x38)
    if read_ret is None:
        raise Exception(f"read reg fail @0x{reg_sys_0x38:x}")
    read_ret &= ~(0x0f << 16)
    read_ret |= (0x09 << 16)
    if not reg_ctrl.write_reg(reg_sys_0x38,read_ret):
        raise Exception(f"write reg fail @0x{reg_sys_0x38:x}")
    BKLog.i(f"sys_38: 0x{read_ret&0xffffffff:x}")
    reg_iomx_base = 0x44830000

    reg_gpio_00 = reg_iomx_base + 0x00 * 4
    write_reg_v = 13 << 24
    reg_ctrl.write_reg(reg_gpio_00,write_reg_v)

    reg_gpio_01 = reg_iomx_base + 0x01 * 4
    write_reg_v = 14 << 24
    reg_ctrl.write_reg(reg_gpio_01,write_reg_v)

    reg_gpio_06 = reg_iomx_base + 0x06 * 4
    write_reg_v = 15 << 24
    reg_ctrl.write_reg(reg_gpio_06,write_reg_v)

    reg_gpio_07 = reg_iomx_base + 0x07 * 4
    write_reg_v = 16 << 24
    reg_ctrl.write_reg(reg_gpio_07,write_reg_v)
    return 0

def test_flash_by_flash_controller(port,bin_path:str,flash_addr:int,max_retry_cnt=5):
    '''
    1,write flash by flash spi(bootrom download operation)
    2,switch flash to 4 lines by reg write/read(flash controller)
    3,read flash by reg write/read(flash controller)
    4,compare read content with write in step 1
    '''
    bk_dl = Download(port,115200,LINKCHECK.BOOTROM,RESET_TYPE.CMD_HEX,115200,50,True,max_retry_cnt,None)
    base_controller = BaseController(serial_instance=bk_dl.ser)
    
    result = False
    start = time.perf_counter()
    try:                           
        bintype,binlist,pathlist = parse_binfile_info(bin_path,0)
        if len(binlist)==0:    
            raise Exception(f"Bin file error: {bin_path}, please check.")                  
        # get bus            
        tmp_res,linktype = base_controller.get_bus(reset_type=RESET_TYPE.DTR_RTS, linkcheck_obj=LINKCHECK.BOOTROM)
        if not tmp_res:
            raise Exception("get bus fail.")
        # get chip id
        time.sleep(0.1) #fix read chip id fail sometimes                 
        chip_id = 0x7236
        if chip_id is None:
            raise Exception("get chip id fail.")               
        BKLog.w("chip id is 0x{:x}".format(chip_id))     
        # init chip_set class
        chip_set = get_chip_set_with_chip_id(chip_id)
        if chip_set is None:
            raise Exception("get chip set fail.")
        # update serial
        chip_set.update_serial_instance(bk_dl.ser)
        reg_ctrl = RegController(serial_instance=bk_dl.ser)  
        flash_monitor_debug(reg_ctrl)

        flash_ret, msg = bk_dl.download_flash(1000000,binlist,False,chip_set,115200)
        if not flash_ret:
            raise Exception(msg)
        time.sleep(0.1)       
        res_0x0_0x1000 = chip_set.read_and_check_sector(0,max_retry_cnt)
        BKLog.i(res_0x0_0x1000[:0x100].hex())

        # flash_monitor_debug(reg_ctrl)

        SOC = 'BK7236'
        if SOC == 'BK7236' :
            #sys 0x02<9> = 0,select flash controller
            reg_sys_base = 0x44010000
            reg_sys_0x02 = reg_sys_base + 0x02 * 4
            read_ret = reg_ctrl.read_reg(reg_sys_0x02)
            if read_ret is None:
                raise Exception(f"read reg fail @0x{reg_sys_0x02:x}")
            read_ret &= ~(1<<9)
            if not reg_ctrl.write_reg(reg_sys_0x02,read_ret):
                raise Exception(f"write reg fail @0x{reg_sys_0x02:x}")
        if SOC == 'BK7236' :
            reg_flash_base = 0x44030000
            reg_flash_0x04 = reg_flash_base + 0x04 * 4
            reg_flash_0x05 = reg_flash_base + 0x05 * 4
            reg_flash_0x06 = reg_flash_base + 0x06 * 4
            reg_flash_0x07 = reg_flash_base + 0x07 * 4
            reg_flash_0x08 = reg_flash_base + 0x08 * 4
            reg_flash_0x09 = reg_flash_base + 0x09 * 4
            reg_flash_0x0a = reg_flash_base + 0x0a * 4
            reg_flash_0x0b = reg_flash_base + 0x0b * 4
            reg_flash_0x0c = reg_flash_base + 0x0c * 4
            reg_flash_0x15 = reg_flash_base + 0x15 * 4

        #read mid to make sure flash ctrl ok        
        flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)
        # 1.1. read flash mid OP = 20
        read_ret = reg_ctrl.read_reg(reg_flash_0x15)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x15:x}")
        read_ret = (0x14<<24)
        if not reg_ctrl.write_reg(reg_flash_0x15,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x15:x}")

        # 1.2 trig flash OP
        read_ret = reg_ctrl.read_reg(reg_flash_0x04)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x04:x}")
        read_ret |= (0x1<<29)
        if not reg_ctrl.write_reg(reg_flash_0x04,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x04:x}")
        flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)

        # 1.3 read mid reg
        read_ret = reg_ctrl.read_reg(reg_flash_0x08)
        if read_ret==0:
            raise Exception("read flash mid fail.")
        BKLog.i(f"get flash mid: 0x{read_ret&0xffffff:x}")

        # 2.1 read SR2 OP = 6
        read_ret = reg_ctrl.read_reg(reg_flash_0x15)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x15:x}")
        read_ret = (0x6<<24)
        if not reg_ctrl.write_reg(reg_flash_0x15,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x15:x}")
  
        # 2.2 trig flash OP = 06
        read_ret = reg_ctrl.read_reg(reg_flash_0x04)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x04:x}")
        read_ret |= (0x1<<29)
        if not reg_ctrl.write_reg(reg_flash_0x04,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x04:x}")
        flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)

        # 2.3 read sr2 reg
        read_ret = reg_ctrl.read_reg(reg_flash_0x09)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x09:x}")
        BKLog.i(f"get flash SR: 0x{read_ret&0xffffffff:x}")

        #3. set line_4,setting reg
        time.sleep(0.1)

        read_ret = reg_ctrl.read_reg(reg_flash_0x09)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x09:x}")
        read_ret |= (0xA0<<22)
        if not reg_ctrl.write_reg(reg_flash_0x09,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x09:x}")

        # 3.1 read SR2 OP = 6
        read_ret = reg_ctrl.read_reg(reg_flash_0x15)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x15:x}")
        read_ret = (0x6<<24)
        if not reg_ctrl.write_reg(reg_flash_0x15,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x15:x}")
  
        # 3.2 trig flash OP = 06
        read_ret = reg_ctrl.read_reg(reg_flash_0x04)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x04:x}")
        read_ret |= (0x1<<29)
        if not reg_ctrl.write_reg(reg_flash_0x04,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x04:x}")
        flash_ctrl_wait_for_flash_busy_bit(reg_ctrl,reg_flash_0x04)

        # 3.3 read sr2 reg
        read_ret = reg_ctrl.read_reg(reg_flash_0x09)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x09:x}")
        BKLog.i(f"get flash SR: 0x{read_ret&0xffffffff:x}")
        time.sleep(0.1)

        read_ret = reg_ctrl.read_reg(reg_flash_0x0a)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x0a:x}")
        read_ret &= ~(0x0f << 4)
        read_ret |= (0x2 << 4)
        if not reg_ctrl.write_reg(reg_flash_0x0a,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x0a:x}")

        time.sleep(0.1)

        # 3.4 enable qwfr OP = 22
        read_ret = reg_ctrl.read_reg(reg_flash_0x15)
        if read_ret is None:
            raise Exception(f"read reg fail @0x{reg_flash_0x15:x}")
        read_ret = (0x16<<24)
        if not reg_ctrl.write_reg(reg_flash_0x15,read_ret):
            raise Exception(f"write reg fail @0x{reg_flash_0x15:x}")
        time.sleep(0.1)

        #read flash
        read_content = flash_ctrl_read_flash_data(reg_ctrl,flash_addr,0x100)
        if read_content is None:
            raise Exception(f"read flash by flash ctrl fail.")
        BKLog.i(read_content.hex())
        if res_0x0_0x1000[:0x100]!=read_content:
            raise Exception(f"compare read content fail.")
        result = True
    except Exception as e:
        BKLog.e(str(e))
        result = False
    finally:
        base_controller.do_reboot()
        if result:
            BKLog.w(f'test complete, all pass')
        costtime = time.perf_counter()-start
        BKLog.w(f'Elapse time {costtime:.6f} seconds')

# def test_loop(port,bin_path,work_baudrate,max_retry_cnt,loop_count):
#     for _ in range(loop_count):
#         print(f'Start loop {_+1} ...')
#         test_flash_erase_and_write(port,bin_path,work_baudrate,max_retry_cnt)
#         print(f'End loop {_+1}.')
#         time.sleep(0.5)

def test_loop(port,bin_path,flash_addr,max_retry_cnt,loop_count):
    for _ in range(loop_count):
        print(f'Start loop {_+1} ...')        
        test_flash_by_flash_controller(port,bin_path,flash_addr,max_retry_cnt)
        print(f'End loop {_+1}.')
        time.sleep(0.5)

if __name__ == '__main__':
    # with open(".\\test_0xaa_0x1000.bin",'wb') as of:
    #     data = bytearray()
    #     data+=(b'\xaa'*0x1000)        
    #     of.write(data)
    #     of.close()
    
    # BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    # test_loop(port = '4',
    #         bin_path="e:/BK_File/BK7236/beken7231_crc.bin",
    #         flash_addr=0x0,
    #         max_retry_cnt=5,
    #         loop_count=1)