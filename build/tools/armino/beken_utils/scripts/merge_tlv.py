#!/usr/bin/env python3
import os
import json
import logging
from .common import *


class tlv_c:
    def check_field(self):
        pass

    def __init__(self, idx, tlv_dic, base_addr):
        global s_base_addr
        self.idx = idx
        self.tlv_json = tlv_dic

        self.check_field()

        self.fill_en = False
        self.firmware_en = False
        self.value_en = False
        self.crc_en = True
        if "firmware" in self.tlv_json.keys():
            self.firmware_en = True
            self.firmware = tlv_dic['firmware']
            self.firmware_size = os.path.getsize(self.firmware)
            self.crc_firmware_size = crc_size(self.firmware_size, self.crc_en)
            if not os.path.exists(self.firmware):
                logging.error(f'tlv{idx} firmware %s not exists' % (self.firmware))
                exit(0)
        elif "value" in self.tlv_json.keys():
            self.value_en = True
            self.value = tlv_dic['value']
        elif "fill" in self.tlv_json.keys():
            self.fill_en = True
            self.fill = tlv_dic['fill']
        else:
            logging.error(f'Shouls pecify "firmware", "value" or "fill"')
            exit(0)

        self.hex_en = False
        if "hex" in self.tlv_json.keys():
            self.hex_en = True

        self.endian_en = False
        if "endian" in self.tlv_json.keys():
            self.endian_en = True
            self.endian = tlv_dic['endian']

        self.cpu_start_addr = base_addr
        self.cpu_size = size2int(tlv_dic['size'])

        # Must init s_base_addr before checking address!
        if (idx == 0):
            s_base_addr = self.cpu_start_addr
            self.cpu_start_addr = 0
        else:
            if (self.cpu_start_addr <= s_base_addr):
                logging.error(f'tlv{self.idx} start_addr=%x < base_addr=%x' %
                      (self.cpu_start_addr, s_base_addr))
                exit(0)
            self.cpu_start_addr = p2v(self.cpu_start_addr)

        if is_out_of_range(self.cpu_start_addr, self.cpu_size):
            logging.error(f'tlv{self.idx} start=%x size=%x is out of range' %
                  (self.cpu_start_addr, self.cpu_size))
            exit(0)

        # if ((self.cpu_start_addr % 32) != 0):
            # logging.error(f'tlv%x start_addr=%x is not 32 bytes aligned' %(self.cpu_start_addr))
            # exit(0)

        if (self.firmware_en == True) and (self.firmware_size > self.cpu_size):
            logging.error(f'tlv{idx} firmware size %x > %x' %
                  (self.firmware_size, self.cpu_size))
            exit(0)
        self.crc_start_addr = self.cpu_start_addr
        self.crc_size = self.cpu_size
        self.crc_en = False
        self.enc_start_addr = self.cpu_start_addr
        self.enc_size = self.cpu_size
        self.enc_en = False

        if ("crc" in tlv_dic):
            if (tlv_dic['crc'] == 'y') or (tlv_dic['crc'] == 'Y'):
                self.crc_en = True
                self.crc_start_addr = crc_addr(self.cpu_start_addr)
                self.crc_size = crc_size(self.cpu_size, self.crc_en)

        if is_out_of_range(self.crc_start_addr, self.crc_size):
            logging.error(f'tlv{self.idx} crc is out of range')
            exit(0)

        # logging.error(f'tlv%x cpu_start=%x size=%x, crc_start=%x, size=%x, enc_start=%x enc_end=%x'
        #		%(self.idx, self.cpu_start_addr, self.cpu_size, self.crc_start_addr, self.crc_size, self.enc_start_addr, self.enc_size))

    def add_crc(self):
        if (self.firmware_en):
            self.raw_buf = bytearray(self.firmware_size)
            self.crc_buf = bytearray(self.crc_firmware_size)
            with open(self.firmware, 'rb') as f:
                # logging.error(f'TODO add CRC16')
                self.crc_buf = f.read()
                # logging.error(f'=================> len=%u' %(len(self.raw_buf)))
        elif (self.value_en):
            if self.hex_en == True:
                self.crc_buf = bytes.fromhex(self.value)
                if (self.endian_en == True) and (self.endian == "l"):
                    buf_bytes_array = bytearray(self.crc_buf)
                    buf_bytes_array.reverse()
                    self.crc_buf = bytes(buf_bytes_array)
            else:
                self.crc_buf = bytearray(self.value, encoding='utf-8')
            fill_value = 0
            pad_len = self.crc_size - len(self.value)
            for i in range(pad_len):
                self.crc_buf.append(fill_value)
        else:
            fill_value = int(self.fill)
            self.crc_buf = bytearray()
            for i in range(self.crc_size):
                self.crc_buf.append(fill_value)


class tlvs_c:

    def __init__(self, json_file, outfile):
        self.tlvs = []
        self.output_file_name = outfile
        self.json_file_name = json_file
        if not os.path.exists(json_file):
            logging.error(f'JSON configuration file {json_file} not exists')
            exit(0)

        with open(json_file, 'r') as self.json_file:
            try:
                self.json_data = json.load(self.json_file)
            except Exception as e:
                logging.error(f"Could not open : '{self.json_file}': {e}", err=True)

        self.check_json_data()

    def check_json_data(self):
        if ("tlvs" not in self.json_data):
            logging.error('json does not contain field ""!')
            exit(0)

        self.tlvs_cnt = len(self.json_data['tlvs'])
        if (self.tlvs_cnt == 0):
            logging.error(f'tlvsof json does not contain any item!')
            exit(0)

        pre_field_addr = 0
        for idx in range(self.tlvs_cnt):
            tlv = tlv_c(idx, self.json_data['tlvs'][idx], pre_field_addr)
            pre_field_addr = tlv.crc_start_addr + tlv.crc_size
            self.tlvs.append(tlv)

        for idx in range(self.tlvs_cnt):
            if (idx == 0):
                continue

            pre_crc_start_addr = self.tlvs[idx - 1].crc_start_addr
            pre_crc_size = self.tlvs[idx - 1].crc_size
            crc_start_addr = self.tlvs[idx].crc_start_addr
            if ((pre_crc_start_addr + pre_crc_size) > crc_start_addr):
                logging.error(f'tlv%x start=%x size=%x overlapped with tlv%x start=%x'
                      % (idx-1, pre_crc_start_addr, pre_crc_size, idx, crc_start_addr))
                exit(0)
            # check_addr(self.tlvs[idx - 1], self.tlvs[idx])

    def merge_tlvs(self):
        f = open(self.output_file_name, 'wb+')
        for idx in range(self.tlvs_cnt):
            tlv = self.tlvs[idx]
            tlv.add_crc()
            # logging.error(f'merge tlv{idx} start=%x' %(tlv.crc_start_addr))
            f.seek(tlv.crc_start_addr)
            f.write(tlv.crc_buf)

        f.flush()
        f.close()

class merge_tlv:

    def __init__(self, json_file, outfile):
        tlvs = tlvs_c(json_file, outfile)
        tlvs.merge_tlvs()
