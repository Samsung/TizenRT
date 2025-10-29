from bkutils.protocol.bootrom.v2_0 import FlashCustomReadExtProtocol
import unittest


class TestFlashCustomReadExtProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = FlashCustomReadExtProtocol()

    def test_cmd(self):
        ADDR = 0x12345678
        size = 4
        cmd = self.protocol.cmd(ADDR, size)
        Scmd = 0xE8
        payload = bytes([ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff, size&0xff])
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, 0xff, 0xf4, (1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff, Scmd])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        size = 4
        expected_length = 11 + 4 + size    # 11 + 4 + 4096
        self.assertEqual(self.protocol.expect_length(), expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        operate = bytes([0xE8])
        DATA = [0x12, 0x34, 0x56, 0x78]
        status = 0x0
        ADDR = 0x12345678
        payload = bytearray([ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        payload.extend(DATA)
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")
        
if __name__ == '__main__':
    unittest.main()