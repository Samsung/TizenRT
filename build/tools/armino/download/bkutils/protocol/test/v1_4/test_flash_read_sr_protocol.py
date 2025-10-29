from bkutils.protocol.bootrom.v2_0.v1_4 import FlashReadSRProtocol
import unittest


class TestFlashReadSRProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = FlashReadSRProtocol()

    def test_cmd(self):
        reg_addr = 0x12
        cmd = self.protocol.cmd(reg_addr)
        Scmd = 0x0c
        payload = bytearray([reg_addr&0xff])
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, 0xff, 0xf4, (1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff, Scmd])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 13   # 11 + 2
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        operate = bytes([0x0c])
        status = 0x0
        REG_ADDR = 0x12
        REG_VALUE = [0x34]
        payload = bytearray([REG_ADDR&0xff])
        payload.extend(REG_VALUE)
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content, REG_ADDR), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

    def test_get_status_regist_val(self):
        operate = bytes([0x0c])
        status = 0x0
        REG_ADDR = 0x12
        REG_VALUE = [0x34]
        payload = bytearray([REG_ADDR&0xff])
        payload.extend(REG_VALUE)
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertEqual(self.protocol.get_status_regist_val(response_content), REG_VALUE[0], "get_status_regist_val failed")

if __name__ == '__main__':
    unittest.main()