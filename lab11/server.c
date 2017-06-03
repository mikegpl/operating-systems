#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 512

void printExit(char *msg);

void handleRequest(char *request);

void setupUDPSocket(char *address, uint16_t port);

void setupSignalHandling();

void signalHandler(int signo);

static int socketDesc, run = 1;
static uint16_t port;
static unsigned int maxRequestCount = 0, currentRequestCount = 0;
static struct sockaddr_in serverSock, client_sock;
static socklen_t socklen = sizeof(client_sock);
static sigset_t eventMask;


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Invalid arg number.\nUsage: ./server ADDRESS PORT MAX_REQUEST_COUNT\n");
        exit(EXIT_FAILURE);
    }

    port = (uint16_t) atoi(argv[2]);
    maxRequestCount = (unsigned int) atoi(argv[3]);

    setupUDPSocket(argv[1], port);
    setupSignalHandling();


    char buffer[BUFFER_SIZE];
    size_t receivedSize;
    while (run) {
        fflush(stdout);

        printf("Hanging on receive\n");
        if ((receivedSize = (size_t) recvfrom(socketDesc, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock, &socklen)) == -1) {
            printExit("recvfrom() error");
        }
        printf("After receive\n");

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
                sigsuspend(&eventMask);
                handleRequest(buffer);
            }
        }
        memset((char *) buffer, 0, BUFFER_SIZE);

        if (sendto(socketDesc, buffer, receivedSize, 0, (struct sockaddr *) &client_sock, socklen) == -1) {
            printExit("sendto()");
        }
    }

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
        pid_t parentPid = getppid();
        kill(parentPid, SIGRTMIN);
        printf("Processing request [%s] in %d\n", response, getpid());
        sleep(2);
        kill(parentPid, SIGRTMIN + 1);
        exit(0);
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

void setupSignalHandling() {
    signal(SIGRTMIN, signalHandler);
    signal(SIGRTMIN + 1, signalHandler);
    sigfillset(&eventMask);
    sigdelset(&eventMask, SIGRTMIN + 1);
}

void signalHandler(int signo) {
    if (signo == SIGINT) {
        close(socketDesc);
        exit(EXIT_SUCCESS);
    } else if (signo == SIGRTMIN) {
        printf("Server: processing request started\n");
    } else if (signo == SIGRTMIN + 1) {
        printf("Server: processing request finished\n");
        currentRequestCount--;
    }
}