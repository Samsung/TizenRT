set -e

cd ../build

../main.py steps get_app_bin_hash --debug
../main.py steps sign_app_bin_hash --debug
../main.py steps sign_from_app_sig --debug
../main.py steps get_ota_bin_hash --debug
../main.py steps sign_ota_bin_hash --debug
../main.py steps sign_from_ota_sig --debug
../main.py steps pack --debug
