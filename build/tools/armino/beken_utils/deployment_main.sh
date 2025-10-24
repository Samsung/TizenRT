#! /bin/sh

sign_src_folder=$1
final_output_folder=$2
echo "sign_src_folder is $sign_src_folder"
echo "final_output_folder is  $final_output_folder"

mkdir -p $final_output_folder

#for debug test (auto gengerate flash_aes_key,rsa keypairs)
#./main.py  pipeline mock
./main.py  pipeline server_keys
./main.py  pipeline process_keys_data --dstfolder $sign_src_folder
./main.py pack all --config_dir  $sign_src_folder

cp -rf ./key_output/aes_encrypt_key_content_gen.bin  $final_output_folder
cp -rf ./key_output/rsa_privkey_gen.bin  $final_output_folder
cp -rf ./key_output/*.json  $final_output_folder
cp -rf ./$sign_src_folder/install/*.bin   $final_output_folder

rm -rf ./bl1_rotpk_digest.bin ./bl1_rotpk_digest.txt  ./rotpk_digest.json
