#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 256

void checkAndCorrectHammingCode(char *receivedData) {
    int p1, p2, p3;

    // Calculate parity bits
    p1 = (receivedData[0] ^ receivedData[2] ^ receivedData[4] ^ receivedData[6]) + '0';
    p2 = (receivedData[1] ^ receivedData[2] ^ receivedData[5] ^ receivedData[6]) + '0';
    p3 = (receivedData[3] ^ receivedData[4] ^ receivedData[5] ^ receivedData[6]) + '0';

    // Convert parity bits to integer
    int parityBits = (p3 - '0') * 4 + (p2 - '0') * 2 + (p1 - '0');

    if (parityBits != 0) {
        printf("Error detected and corrected in bit: %d\n", parityBits);
        receivedData[parityBits - 1] = (receivedData[parityBits - 1] == '0') ? '1' : '0';
    } else {
        printf("No error detected.\n");
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char receivedData[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection...\n");

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

    if (client_socket < 0) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Receive data with Hamming Code from the client
    recv(client_socket, receivedData, BUFFER_SIZE, 0);

    // Check and correct Hamming Code
    checkAndCorrectHammingCode(receivedData);

    printf("Received data after error correction: %s\n", receivedData);

    close(server_socket);
    close(client_socket);
    return 0;
}

