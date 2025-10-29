from .base_boot_rom_protocol import BaseBootRomProtocol
from ....common.common import crc32_ver2


class ChipEnterCbusModeExtProtocol(BaseBootRomProtocol):

    def cmd(self, part_addr:list, part_size:list, flash_addr:list):
        data = []
        data = part_addr+part_size+flash_addr
        crc = crc32_ver2(0xffffffff,data)
        data.append(crc&0xff)
        data.append((crc>>8)&0xff)
        data.append((crc>>16)&0xff)
        data.append((crc>>24)&0xff)       
        
        payload = []
        # payload = [len(data)]
        payload.extend(data)
        return self.command_generate(0x15, payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content)==bytes([0x15]) and self.get_response_payload(response_content)==bytes([0x00])
