// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUCKET_SIZE 10
#define OUTGOING_RATE 1
#define PORT 12345

typedef struct {
    int arrival_time;
    int packet_size;
} Packet;

typedef struct {
    int bucket_size;
    int current_size;
} Bucket;

void leakyBucket(Packet *packets, int num_packets, Bucket *bucket, int client_socket) {
    int i;
    char buffer[1024];

    for (i = 0; i < num_packets; i++) {
        sleep(packets[i].arrival_time); // Simulating arrival time

        sprintf(buffer, "Packet %d (Size: %d bytes) arrived at time %d seconds.\n", i + 1, packets[i].packet_size, packets[i].arrival_time);
        send(client_socket, buffer, strlen(buffer), 0);

        if (packets[i].packet_size <= bucket->current_size) {
            sprintf(buffer, "Packet %d is conforming. Sending to the network.\n", i + 1);
            send(client_socket, buffer, strlen(buffer), 0);

            bucket->current_size -= packets[i].packet_size;
        } else {
            sprintf(buffer, "Packet %d is nonconforming. Dropping the packet.\n", i + 1);
            send(client_socket, buffer, strlen(buffer), 0);
        }

        // Refill the bucket based on outgoing rate
        if (bucket->current_size < BUCKET_SIZE) {
            int refill_amount = (BUCKET_SIZE - bucket->current_size) > OUTGOING_RATE ? OUTGOING_RATE : (BUCKET_SIZE - bucket->current_size);
            bucket->current_size += refill_amount;
        }
    }
}

int main() {
    int num_packets;
    printf("Enter the number of packets: ");
    scanf("%d", &num_packets);

    Packet packets[num_packets];

    for (int i = 0; i < num_packets; i++) {
        printf("Enter arrival time for Packet %d (in seconds): ", i + 1);
        scanf("%d", &packets[i].arrival_time);

        printf("Enter size for Packet %d (in bytes): ", i + 1);
        scanf("%d", &packets[i].packet_size);
    }

    Bucket bucket = {
        .bucket_size = BUCKET_SIZE,
        .current_size = BUCKET_SIZE
    };

    // Server setup
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Leaky Bucket Server is waiting for connections...\n");

    // Accept client connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Perform leaky bucket simulation
    leakyBucket(packets, num_packets, &bucket, client_socket);

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}


