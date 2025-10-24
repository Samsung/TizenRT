cd ../build
#../main.py gen ppc --debug --ppc_csv ppc.csv --gpio_dev_csv gpio_dev.csv --outfile _ppc.h
#../main.py gen mpc --debug --mpc_csv mpc.csv --outfile _mpc.h
#../main.py gen security --debug --security_csv security.csv --outfile _security.h
#../main.py gen ota --debug --ota_csv ota.csv --outfile _ota.h
#../main.py gen otp_efuse --debug --flash_aes_type FIXED --flash_aes_key 73c7bf397f2ad6bf4e7403a7b965dc5ce0645df039c2d69c814ffb403183fb18 --pubkey_pem_file root_ec256_pubkey.pem --secure_boot --outfile otp_efuse_config.json
#../main.py gen partition --debug --partition_csv partitions.csv --ota_type XIP
../main.py gen all --debug
