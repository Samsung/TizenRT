from bkutils.protocol.bootrom.v2_0 import FlashEraseAllExtProtocol
import unittest


class TestFlashEraseAllExtProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = FlashEraseAllExtProtocol()

    def test_cmd(self):
        timeout_sec = 10
        cmd = self.protocol.cmd(timeout_sec)
        Scmd_byte = 0xEA
        payload = bytearray([timeout_sec])
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, 0xff, 0xf4, (1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff, Scmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 12    # 11 + 1
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        operate = bytes([0xEA])
        status = 0x0
        T_OUT = 10
        payload = bytes([T_OUT])
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

if __name__ == '__main__':
    unittest.main()