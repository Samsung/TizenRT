from dataclasses import dataclass, field
from typing import List
import time
from bkutils.action_service.tizen.tizen_action_base import TizenActionBase
from bkutils.bk_log.log_utils import BKLog
from bkutils.chip_controller.base_controller import BaseController
from bkutils.common.common import FLASH_SIZE_DEF, RESET_TYPE, BinInfo


@dataclass
class TizenDownloadInfo:
    vecbinInfo: List[BinInfo] = field(default_factory=list)
    pre_erase: int = 1


class TizenDownload(TizenActionBase):

    def __init__(
        self,
        port="/dev/ttyUSB0",
        link_check_baudrate=115200,
        reset_baudrate=115200,
        link_check_type=None,
        reset_type=None,
        recnt=5,
    ):
        super().__init__(port)
        self.retry = recnt
        self.reset_type = reset_type
        self.link_check_type = link_check_type

    def download(self, download_info: TizenDownloadInfo):
        BKLog.i("====================download start...====================")
        result = False
        start_time = time.time()
        try:
            if download_info.pre_erase == 1:
                erase_idx = 0
                for binfile in download_info.vecbinInfo:
                    BKLog.w(f"[{erase_idx}] erase flash.")
                    BKLog.i(f"[{erase_idx}] start: 0x{binfile.iStartAddress:x}.")
                    BKLog.i(f"[{erase_idx}] length: 0x{binfile.iFileLength:x}.")
                    if not self.chip_set.erase_flash(
                        False,
                        binfile.iStartAddress,
                        binfile.iStartAddress + binfile.iFileLength,
                        False,
                        self.retry,
                    ):
                        raise Exception("erase flash fail.")
                    erase_idx += 1
            # write each bin to flash
            time.sleep(0.001)
            write_idx = 0
            for binfile in download_info.vecbinInfo:
                if binfile.iStartAddress & 0xFFF:
                    binfile.iStartAddress = (
                        int(
                            (binfile.iStartAddress + FLASH_SIZE_DEF.SIZE_4K)
                            / FLASH_SIZE_DEF.SIZE_4K
                        )
                        * FLASH_SIZE_DEF.SIZE_4K
                    )
                BKLog.w(f"[{write_idx}] write flash.")
                BKLog.i(f"[{write_idx}] start: 0x{binfile.iStartAddress:x}.")
                BKLog.i(f"[{write_idx}] length: 0x{binfile.iFileLength:x}.")
                # write flash by bin start size and size
                if not self.chip_set.download_flash(
                    binfile.pfile,
                    binfile.iStartAddress,
                    self.retry,
                    self.reset_type == RESET_TYPE.DTR_RTS,
                    self.reset_type,
                    self.link_check_type,
                ):
                    raise Exception("Write flash fail.")
                write_idx += 1
            BKLog.w("Write flash done.")
            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("download complete, all pass.")
                BKLog.w("done.")
            else:
                BKLog.e("download fail.")
            BKLog.i(f"Elapse time: {elapsed_time:.6f} seconds.\n")
            return result

    def bootloader_ota(self, download_info: TizenDownloadInfo):
        BKLog.i(
            "====================download bootloader ota start...===================="
        )
        result = False
        start_time = time.time()
        try:
            if self.config.ota_done:
                raise Exception("ota done, no need to download bootloader ota.")

            base_controller = BaseController(self.ser)
            if self.config.is_bl1_link():
                operate_area = "A"
            else:
                partition_flag = base_controller.read_bootloader_partition_flag()
                if partition_flag is None:
                    raise Exception("read bootloader partition flag fail.")

                # 如果为2，下发A，如果为其他值，下发B
                operate_area = "A" if partition_flag == 2 else "B"
                running_area = "A" if operate_area == "B" else "A"
                BKLog.i(f"bootloader is running in {running_area},load {operate_area}.")

            indices = [0,2] if operate_area == "A" else [1,3]
            selected_bins = [download_info.vecbinInfo[i] for i in indices]
            download_info.vecbinInfo = selected_bins

            if download_info.pre_erase == 1:
                erase_idx = 0
                for binfile in download_info.vecbinInfo:
                    BKLog.w(f"[{erase_idx}] erase flash.")
                    BKLog.i(f"[{erase_idx}] start: 0x{binfile.iStartAddress:x}.")
                    BKLog.i(f"[{erase_idx}] length: 0x{binfile.iFileLength:x}.")
                    if not self.chip_set.erase_flash(
                        False,
                        binfile.iStartAddress,
                        binfile.iStartAddress + binfile.iFileLength,
                        False,
                        self.retry,
                    ):
                        raise Exception("erase flash fail.")
                    erase_idx += 1
            # write each bin to flash
            time.sleep(0.001)
            write_idx = 0
            for binfile in download_info.vecbinInfo:
                if binfile.iStartAddress & 0xFFF:
                    binfile.iStartAddress = (
                        int(
                            (binfile.iStartAddress + FLASH_SIZE_DEF.SIZE_4K)
                            / FLASH_SIZE_DEF.SIZE_4K
                        )
                        * FLASH_SIZE_DEF.SIZE_4K
                    )
                BKLog.w(f"[{write_idx}] write flash.")
                BKLog.i(f"[{write_idx}] start: 0x{binfile.iStartAddress:x}.")
                BKLog.i(f"[{write_idx}] length: 0x{binfile.iFileLength:x}.")
                # write flash by bin start size and size
                if not self.chip_set.download_flash(
                    binfile.pfile,
                    binfile.iStartAddress,
                    self.retry,
                    self.reset_type == RESET_TYPE.DTR_RTS,
                    self.reset_type,
                    self.link_check_type,
                ):
                    raise Exception("Write flash fail.")
                write_idx += 1
            BKLog.w("Write flash done.")
            if not self.config.is_bl1_link() and not base_controller.write_bootloader_ota_success_flag():
                raise Exception("Write bootloader ota success flag fail.")
            self.config.ota_done = True
            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("download complete, all pass.")
                BKLog.w("done.")
            else:
                if not self.config.ota_done:
                    BKLog.e("download fail.")
            BKLog.i(f"Elapse time: {elapsed_time:.6f} seconds.\n")
            return result
