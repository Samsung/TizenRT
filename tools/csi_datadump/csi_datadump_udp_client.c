/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define FILENAME "received_data.txt"
#define BYTES 155
FILE* file;
int stop = 0;
int count = 0;
uint16_t length;
int gSockFd;
struct sockaddr_in server_addr;
socklen_t addr_len;

// static void print_buf(const unsigned char* buf, int len)
// {
//     for (int i = 0; i < len; i++){
//         fprintf(file, "%02x", buf[i]);
//     }
//     fprintf(file, "\n");
// }

void* receive_data(void* arg) {
    char buffer[BUFFER_SIZE];
    int n;
    int received;
    while (!stop) {
        received = 0;
        while(received < length){
            n = recvfrom(gSockFd, (char *)buffer + received, length - received, 0, NULL, NULL);
            if (n < 0) {
                if (errno == EBADF) {
                    return NULL;
                }
                printf("Receive Length failed\n");
                break;
            }
            received += n;
        }
        printf("write data success\n");
        // print_buf(buffer,length);
        fwrite(buffer, sizeof(unsigned char), length, file);
        count += 1;
        if(count % 100 == 0){
            fflush(file); //flush every 100 writes
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t recv_thread;
    char message[20];
    if (argc == 3 || argc == 4) {
        if (argc == 4){
            length = atoi(argv[3]);
        }
        else {
            length = BYTES;
        }
    }
    else {
        printf("Input Error\n");
        return -1;
    }

    if ((gSockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        printf("Invalid IP address\n");
        close(gSockFd);
        return -1;
    }
    addr_len = sizeof(server_addr);

    // Send START command
    strncpy(message, "START", 6);
    sendto(gSockFd, message, strlen(message), 0, (const struct sockaddr *)&server_addr, addr_len);
    
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return -1;
    }
    pthread_create(&recv_thread, NULL, receive_data, NULL);

    // Wait for the user to type "stop"
    while (1) {
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';  // Remove newline character

        if (strncmp(message, "STOP", 5) == 0) {
            stop = 1;
            sendto(gSockFd, (const char *)message, strlen(message), 0, (const struct sockaddr *) &server_addr, addr_len);
            printf("Stop command sent to server\n");
            shutdown(gSockFd,SHUT_RDWR);
            close(gSockFd);
            sleep(2);
            break;
        }
    }
    pthread_join(recv_thread, NULL);
    fclose(file);
    return 0;
}
