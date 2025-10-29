from bkutils.action_service.action_base import ActionBase
from bkutils.action_service.tizen.serial_port_config import TizenSerialPortConfig
from bkutils.chip_controller.chip_set import get_chip_set_with_chip_id


class TizenActionBase(ActionBase):
    def __init__(self, port="/dev/ttyUSB0"):
        if port is None:
            raise Exception("port is None.")
        self.config = TizenSerialPortConfig(port)
        if not self.config.check_config():
            raise Exception("port config not found")
        super().__init__(port, self.config.work_baudrate)

        self.init_chip_set()

    def init_chip_set(self):
        # init chip_set class
        self.chip_set = get_chip_set_with_chip_id(self.config.chip_id)

        if self.chip_set is None:
            raise Exception("get chip set fail.")
        self.chip_set.update_serial_instance(self.ser)
        self.chip_set.update_flash_mid(self.config.flash_id)
