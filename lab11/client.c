#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid arg number.\nUsage: ./client ADDRESS PORT\n");
        exit(EXIT_FAILURE);
    }

    printf("This is client with pid: [%d]\n", getpid());
    int sock;
    socklen_t socklen = sizeof(struct sockaddr_in);
    uint16_t port = (uint16_t) atoi(argv[2]);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        perror("Connecting error");
        exit(EXIT_FAILURE);
    }


    // HELLO
    char *buffer = "HELLO";
    sendto(sock, buffer, 5, 0, (struct sockaddr *) &addr, socklen);
    recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &socklen);
    if (strncmp(buffer, "HELLO", 5) != 0) {
        fprintf(stderr, "Yo m8, you didn't greet properly\n");
        exit(EXIT_FAILURE);
    }

    // REQUEST
    size_t lineSize;
    ssize_t charsRead = getline(&buffer, &lineSize, stdin);
    sendto(sock, buffer, (size_t) charsRead, 0, (struct sockaddr *) &addr, socklen);

    // BYE
    buffer = "BYE";
    sendto(sock, buffer, 3, 0, (struct sockaddr *) &addr, socklen);
    recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &socklen);
    if (strncmp(buffer, "BYE", 3) != 0) {
        fprintf(stderr, "Yo m8, you didn't say goodbye properly\n");
        exit(EXIT_FAILURE);
    }


    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}
