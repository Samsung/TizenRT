cd ../build

#../main.py sign bl1_sign_hash --debug --privkey_pem_file root_ec256_privkey.pem --hash 7443acad616d263241d5feebe39b7713121a8f0e81384973424c67c051227014 --outfile manifest_signature.json
#../main.py sign bl1_sign --debug --action_type sign --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --bin_file bl2.bin --static_addr 0x12345678 --load_addr 0x28040000 --outfile primary_manifest.bin
#../main.py sign bl1_sign --debug --action_type hash --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --bin_file bl2.bin --static_addr 0x12345678 --load_addr 0x28040000 --outfile primary_manifest.bin
#../main.py sign bl1_sign --debug --action_type sign_from_sig --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --bin_file bl2.bin --static_addr 0x12345678 --load_addr 0x28040000 --outfile primary_manifest.bin

#../main.py sign bl2_sign_hash --debug --privkey_pem_file root_ec256_privkey.pem --hash 7443acad616d263241d5feebe39b7713121a8f0e81384973424c67c051227014 --outfile manifest_signature.json
#../main.py sign bl2_sign --debug --action_type hash --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --bin_file bl2.bin --partition_size 2105344 --version 0.0.1 --security_counter 0 --sign_outfile bl2_sign.bin --hash_outfile bl2_hash.json
#../main.py sign bl2_sign --debug --action_type sign_from_sig --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --signature 30450220797d605a21690cbea6b0587b7820d6e5f1b1b8454e234b7437d46f1b0659c0bb022100eccec973da128fbfd8674a514da549954cedd561eebbe7cf54f2ac0b8ab0c035 --bin_file bl2.bin --partition_size 2105344 --version 0.0.1 --security_counter 0 --sign_outfile bl2_sign.bin --hash_outfile bl2_hash.json
../main.py sign bl2_sign --debug --action_type sign --key_type ec256 --privkey_pem_file root_ec256_privkey.pem --pubkey_pem_file root_ec256_pubkey.pem --bin_file bl2.bin --partition_size 2105344 --version 0.0.1 --security_counter 0 --sign_outfile bl2_sign.bin --hash_outfile bl2_hash.json
