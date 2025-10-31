import os
import shutil
import json
import csv
import logging


class Key_Content_post_processing:
    def __init__(self, sour, dest, debug=False):
        self.sour = sour
        self.dest = dest
        self.path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        self.debug = debug

    def cp_json_data_to_csv(self):
        sourfile = os.path.join(self.path, self.sour, 'key_content.json')
        destfile = os.path.join(self.path, self.dest, 'security.csv')

        logging.debug(f'sourfile :{sourfile}')
        logging.debug(f'destfile :{destfile}')

        with open(sourfile, 'r', encoding='utf-8') as json_file:
            json_data = json.load(json_file)
            aes_key_data = json_data["key_content"]["Flash Aes key"]['content']
            print(f'Aes_key_data :{aes_key_data}')

        with open(destfile, 'r', newline='', encoding='utf-8') as infile:
            reader = csv.reader(infile)
            csv_data = [row for row in reader]
            key_to_modify = 'flash_aes_key'
            new_value = aes_key_data
            for row in csv_data:
                print(f'row :{row}')
                if row[0] == key_to_modify:
                    row[1] = new_value
                print(f'row[1] :{row[1]}')

        with open(destfile, 'w', newline='', encoding='utf-8') as outfile:
            writer = csv.writer(outfile)
            writer.writerows(csv_data)

    def cp_ecdsa_keypairs_to_destfodler(self):
        s_pubkey_pem  = os.path.join(self.path, self.sour, 'root_ec256_pubkey.pem')
        s_privkey_pem = os.path.join(self.path, self.sour, 'root_ec256_privkey.pem')
        d_pubkey_pem  = os.path.join(self.path, self.dest)
        d_privkey_pem = os.path.join(self.path, self.dest)

        logging.debug(f's_pubkey_pem :{s_pubkey_pem}')
        logging.debug(f'd_pubkey_pem :{d_pubkey_pem}')

        logging.debug(f's_privkey_pem :{s_privkey_pem}')
        logging.debug(f'd_privkey_pem :{d_privkey_pem}')

        if os.path.exists(s_pubkey_pem) and os.path.exists(d_pubkey_pem):
            shutil.copy(s_pubkey_pem, d_pubkey_pem) 
        else:
            raise Exception(f'copy s_pubkey_pem fail')

        if os.path.exists(s_privkey_pem) and os.path.exists(d_privkey_pem):
            shutil.copy(s_privkey_pem, d_privkey_pem) 
        else:
            raise Exception(f'copy s_privkey_pem fail')

def keys_data_post_processing(sour,dest):
    cyg = Key_Content_post_processing(sour, dest, False)
    cyg.cp_json_data_to_csv()
    cyg.cp_ecdsa_keypairs_to_destfodler()