from bkutils.protocol.bootrom.v2_0.v1_4 import FlashRead4kProtocol
import unittest


class TestFlashRead4kProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = FlashRead4kProtocol()

    def test_cmd(self):
        ADDR = 0x12345678
        cmd = self.protocol.cmd(ADDR)
        Scmd = 0x09
        payload = bytearray([ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, 0xff, 0xf4, (1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff, Scmd])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 11 + 4 + 4096    # 11 + 4 + 4096
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        operate = bytes([0x09])
        status = 0x0
        ADDR = 0x12345678
        DATA = [0x12, 0x34, 0x56]
        payload = bytearray([ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        payload.extend(DATA)
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content, ADDR), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

    def test_get_read_content(self):
        operate = bytes([0x09])
        status = 0x0
        ADDR = 0x12345678
        DATA = [0x12, 0x34, 0x56]
        payload = bytearray([ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        payload.extend(DATA)
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertEqual(self.protocol.get_read_content(response_content), bytes(DATA), "get_read_content failed")

if __name__ == '__main__':
    unittest.main()