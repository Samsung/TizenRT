import logging
import threading
import time
from bkutils.action_service.action_base import ActionBase
from bkutils.bk_log.log_utils import BKLog
from bkutils.chip_controller.flash_controller_tsingteng import FlashControllerTsingteng
from bkutils.chip_controller import get_chip_set_with_chip_id
from bkutils.chip_controller.base_controller import BaseController
from bkutils.common.common import LINKCHECK, RESET_TYPE
from bkutils.chip_controller.chip_set import BK7239N


class PreWorker(ActionBase):

    def __init__(
        self,
        port="/dev/ttyUSB0",
        link_check_baudrate=115200,
        reset_baudrate=115200,
        worker_baudrate=2000000,
        link_check_type=None,
        reset_type=None,
        getbus_times=100,
        recnt=5,
        chipid=None,
        debug=3,
    ) -> None:
        super().__init__(port, link_check_baudrate)
        self.link_check_type = link_check_type
        self.reset_type = reset_type
        self.reset_baudrate = reset_baudrate
        self.worker_baudrate = worker_baudrate
        self.getbus_times = getbus_times
        self.retry = recnt
        self.chip_id = chipid
        self.debug = debug

    def pre_work(self):
        BKLog.i("====================pre work start...====================")
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        try:
            # get bus
            tmp_res, linktype = base_controller.get_bus(
                reset_type=RESET_TYPE.MULTI,
                linkcheck_obj=self.link_check_type,
                reset_baudrate=self.reset_baudrate,
                max_try_count=self.getbus_times,
            )
            if not tmp_res:
                raise Exception("get bus fail.")
            # get chip id
            time.sleep(0.1)  # fix read chip id fail sometimes
            # if self.chip_id is None:
            #     self.chip_id = base_controller.get_chip_id_retry(self.retry)
            #     if self.chip_id is None:
            #         raise Exception("get chip id fail.")
            #     if self.chip_id==0x7236 or self.chip_id >> 16==0x7236:
            #         sec_id = base_controller.get_second_chip_id()
            #         if sec_id is None:
            #             raise Exception("get sec chip id fail.") 
            # # BKLog.w("chip id is 0x{:x}".format(self.chip_id))
            # # init chip_set class
            # chip_set = get_chip_set_with_chip_id(self.chip_id,sec_id)
            chip_set = BK7239N()
            if chip_set is None:
                raise Exception("get chip set fail.")
            # check image file whether with crc32 per 32 bytes, BK7259 need remove it
            # chip_set.pre_check_image_file(downloadinfo.vecbinInfo)
            # update serial
            chip_set.update_serial_instance(self.ser)

            if not chip_set.pre_set_efuse_nocrc():
                raise Exception("pre set efuse fail.")
            # parse flash
            BKLog.i("parse flash info...")
            if not chip_set.parse_flash_info():
                raise Exception("parse flash info fail.")
            th_flash_ctrl = FlashControllerTsingteng(
                serial_instance=self.ser, flash_cfg=chip_set._flash_cfg
            )

            BKLog.i("set baudrate to {0}...".format(self.worker_baudrate))
            if not chip_set.set_baudrate(baudrate=self.worker_baudrate, delay_ms=20,isBl1= linktype == LINKCHECK.BOOTROM):
                raise Exception("set baudrate {0} fail.".format(self.worker_baudrate))

            if chip_set._flash_mid == 0x1370CD:
                if not th_flash_ctrl.tsingteng_flash_fix_mid_incorrect():
                    raise Exception("execute fix flash mid patch fail.")
                chip_set._flash_mid = 0x1360CD
            # if chip_set._flash_mid == 0x1560EB:
            #     if not th_flash_ctrl.tsingteng_flash_detect_bad_product(
            #         downloadinfo.vecbinInfo
            #     ):
            #         raise Exception("execute detect bad flash patch fail.")
            #     if not th_flash_ctrl.tsingteng_flash_high_temperature_patch():
            #         raise Exception("execute high temperature patch fail.")
            # unprotect flash
            BKLog.i("unprotect flash...")
            if not chip_set.un_protect_flash(protect=False):
                raise Exception("unprotect flash fail.")
            BKLog.i("unprotect flash OK.")

            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("pre work complete, all pass.")
                BKLog.w("done.")
            else:
                BKLog.e("pre work fail.")
            BKLog.i(f"Elapse time: {elapsed_time:.6f} seconds.\n\n")
            return result, chip_set.CHIP_ID if result else None, chip_set._flash_mid if result else None, self.worker_baudrate, linktype
