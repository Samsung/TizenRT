from bkutils.action_service.action_base import ActionBase
from bkutils.action_service.tizen.tizen_action_base import TizenActionBase
from bkutils.bk_log.log_utils import BKLog
from bkutils.chip_controller.base_controller import BaseController
from bkutils.chip_controller.flash_controller_tsingteng import FlashControllerTsingteng


class TizenEndWorker(TizenActionBase):

    def __init__(self, port="/dev/ttyUSB0"):
        super().__init__(port)

    def end_worker(self, reboot_after_donwload=True):
        BKLog.i("====================end worker start...====================")
        try:
            BKLog.i("protect flash...")
            if not self.chip_set.un_protect_flash(protect=True):
                raise Exception("protect flash fail.")
            BKLog.i("protect flash OK.")

            if (
                self.chip_set._flash_mid == 0x1360CD
                or self.chip_set._flash_mid == 0x1460CD
                or self.chip_set._flash_mid == 0x1560EB
            ):
                th_flash_ctrl = FlashControllerTsingteng(
                    serial_instance=self.ser, flash_cfg=self.chip_set._flash_cfg
                )
                if not th_flash_ctrl.tsingteng_flash_scratch_solution(
                    self.chip_set._flash_mid
                ):
                    raise Exception("Execute flash scratch solution fail.")
        except Exception as e:
            BKLog.e(str(e))

        finally:
            if reboot_after_donwload:
                base_controller = BaseController(serial_instance=self.ser)
                base_controller.do_reboot()
            self.ser.disconnect()
            BKLog.i("done.\n")
