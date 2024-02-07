// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 12345

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Client setup
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to the server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Receive and display messages from the server
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);

        if (recv_size <= 0) {
            printf("Connection closed by the server.\n");
            break;
        }

        printf("%s", buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}


