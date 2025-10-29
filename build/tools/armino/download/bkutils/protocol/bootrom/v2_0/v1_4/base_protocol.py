from abc import ABC, abstractmethod


class BaseBootRomProtocol(ABC):

    def __init__(self):
        pass

    def cmd_generate(self, cmd, payload:list = []):
        cmd = bytearray()
        cmd.extend(self.base_tx_type_and_opcode)
        cmd.append(1 + len(payload))
        cmd.append(cmd)
        cmd.extend(payload)
        return cmd
    
    @abstractmethod
    def cmd(self):
        pass

    @abstractmethod
    def expect_length(self):
        pass
