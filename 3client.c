#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 256

void addHammingCode(char *data, char *hammingCode) {
    // Copy original data to hammingCode buffer
    strcpy(hammingCode, data);

    // Calculate parity bits
    hammingCode[3] = (data[0] ^ data[1] ^ data[3]) + '0';
    hammingCode[5] = (data[0] ^ data[2] ^ data[3]) + '0';
    hammingCode[6] = (data[1] ^ data[2] ^ data[3]) + '0';
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char hammingCode[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Enter 4 bits of data to transmit: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

    if (strlen(buffer) != 4) {
        printf("Error: Enter exactly 4 bits of data.\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Add Hamming Code to the data
    addHammingCode(buffer, hammingCode);

    // Send data with Hamming Code to the server
    send(client_socket, hammingCode, strlen(hammingCode), 0);

    close(client_socket);
    return 0;
}

