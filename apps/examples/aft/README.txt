This is guided to use AFT (Audio File Transfer) sample app to transfer file from RTL8730E board to local PC

1. We need to connect the local pc and RTL8730E board on same network.
2. We need to install python on local pc and save the minimal_file_transfer.py file in the local pc (or destination).
3. Run the Aft app on RTL8730E board.
4. Run the below command to transfer the file from RTL8730E board to local PC:
       python minimal_file_transfer.py -a <IP of RTL8730E board> -s <location at PC> -t <location at board>
       
i.e. : python minimal_file_transfer.py -a "192.168.1.248" -s D:\check_audio\record2.pcm -t /mnt/record2.pcm
