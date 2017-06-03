#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

void printExit(char *msg);

void handleRequest(char *request);

void setupUDPSocket(char *address, uint16_t port);

static int socketDesc;
uint16_t port;
static unsigned int maxRequestCount = 0;
static unsigned int currentRequestCount = 0;
static struct sockaddr_in serverSock, client_sock;
static socklen_t socklen = sizeof(client_sock);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Invalid arg number.\nUsage: ./server ADDRESS PORT MAX_REQUEST_COUNT\n");
        exit(EXIT_FAILURE);
    }

    port = (uint16_t) atoi(argv[2]);
    maxRequestCount = (unsigned int) atoi(argv[3]);
    printf("%d, %d, %s\n", port, maxRequestCount, argv[1]);

    char buffer[BUFFER_SIZE];
    size_t receivedSize;
    setupUDPSocket(argv[1], port);

    int shutdown = 0;
    while (!shutdown) {
        fflush(stdout);

        if ((receivedSize = (size_t) recvfrom(socketDesc, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock, &socklen)) == -1) {
            printExit("recvfrom() error");
        }

        if (strncmp(buffer, "HELLO", 5) == 0) {
            printf("Hello there! Connection [%s][%d]\n", inet_ntoa(client_sock.sin_addr), ntohs(client_sock.sin_port));
            sendto(socketDesc, buffer, receivedSize, 0, (struct sockaddr *) &client_sock, socklen);
        } else if (strncmp(buffer, "BYE", 3) == 0) {
            printf("Farewell m8! Connection [%s][%d]\n", inet_ntoa(client_sock.sin_addr), ntohs(client_sock.sin_port));
            sendto(socketDesc, buffer, receivedSize, 0, (struct sockaddr *) &client_sock, socklen);
        } else {
            if (currentRequestCount < maxRequestCount)
                handleRequest(buffer);
            else {
                // sleep, waiting for REQUEST_HANDLED signal
                handleRequest(buffer);
            }
        }
        memset((char *) buffer, 0, BUFFER_SIZE);

        if (sendto(socketDesc, buffer, receivedSize, 0, (struct sockaddr *) &client_sock, socklen) == -1) {
            printExit("sendto()");
        }
    }

    close(socket);
    return 0;
}


void printExit(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void handleRequest(char *request) {
    currentRequestCount++;

    char response[BUFFER_SIZE];
    strcpy(response, request);
    response[strlen(response) - 1] = '\0';
    if (fork() == 0) {
        printf("Processing request [%s] in %d\n", response, getpid());
        exit(0);
    } else {
        wait(NULL);
    }
}

void setupUDPSocket(char *address, uint16_t port) {
    if ((socketDesc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printExit("socket() error");
    }

    memset((char *) &serverSock, 0, sizeof(serverSock));

    serverSock.sin_family = AF_INET;
    serverSock.sin_port = htons(port);
    serverSock.sin_addr.s_addr = inet_addr(address);
    if (bind(socketDesc, (struct sockaddr *) &serverSock, sizeof(serverSock)) == -1) {
        printExit("bind() error");
    }
}