# How To Run csi_datadump_udp_client.c File

## Steps:
### 1. First compile the file:
```bash
gcc -o program csi_datadump_udp_client.c -lpthread
```

### 2. Run the program 
    Command Format: ./program <IP_ADDRESS> <PORT> <BYTES>

Here "BYTES" is the number of bytes you want to receive from server (Optional Parameter)
Default Value: 155

```bash
./program 192.168.1.120 5000
```
OR

```bash
./program 192.168.1.120 5000 155
```

## How to STOP the datadump
#### Type *STOP* in terminal and press enter.

You will get the file "received_data.txt" in your current directory, In which data will be saved in the binary format.
