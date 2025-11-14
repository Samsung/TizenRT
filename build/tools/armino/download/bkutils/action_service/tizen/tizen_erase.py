from dataclasses import dataclass
import time
from bkutils.action_service.action_base import ActionBase
from bkutils.action_service.tizen.tizen_action_base import TizenActionBase
from bkutils.bk_log.log_utils import BKLog
from bkutils.chip_controller.chip_set import get_chip_set_with_chip_id


@dataclass
class TizenEraseInfo:
    start_addr: int = 0
    length: int = 0
    retry: int = 10


class TizenErase(TizenActionBase):

    def __init__(self, port="/dev/ttyUSB0"):
        super().__init__(port)

    def erase(self, erase_info: TizenEraseInfo):
        BKLog.i("====================erase start...====================")
        result = False
        start_time = time.time()
        try:
            BKLog.w("erase flash.")
            BKLog.i(f"start: 0x{erase_info.start_addr:x}.")
            BKLog.i(f"length: 0x{erase_info.length:x}.")

            if not self.chip_set.erase_flash(
                False,
                erase_info.start_addr,
                erase_info.start_addr + erase_info.length,
                False,
                erase_info.retry,
            ):
                raise Exception("erase flash fail.")
            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("erase complete, all pass.")
                BKLog.w("done.")
            else:
                BKLog.e("erase fail.")
            BKLog.i(f"Elapse time: {elapsed_time:.6f} seconds.\n")
            return result
