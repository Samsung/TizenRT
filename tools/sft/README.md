# Simple File transfer

(I) Connecting board to network of PC:-
Use below commands to connect  board to router to which PC is connected.

1. wm_test start

2. wm_test join <SSID> <security> <password> (ex. - wm_test join TPWIFI wpa2_aes 20202020)



 (II) Running the sft application to transfer audio from the RTL8730E board to PC:-

For transfering each file use following command on RTL8730E board.
sft
On the PC terminal use minimal_file_transfer.py application present in attachment. Command to be used is below:

python minimal_file_transfer.py -a <IP of RTL8730E board> -s <location at PC> -t <location at board>
python minimal_file_transfer.py -a "192.168.1.248" -s D:\\check_audio\record2.pcm -t /mnt/record2.pcm
