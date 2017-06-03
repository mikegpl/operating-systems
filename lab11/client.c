#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFLEN 512
#define PORT 33000

int main(int argc, char *argv[]) {
    printf("This is client with pid: [%d]\n", getpid());
    int sock;
    socklen_t slen = sizeof(struct sockaddr_in);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        perror("Connecting error");
        exit(EXIT_FAILURE);
    }

    char *buffer;
    size_t size;
    size_t recv_len;
    // while(1){
    getline(&buffer, &size, stdin);
    sendto(sock, buffer, BUFLEN, 0, (struct sockaddr *) &addr, slen);
    recvfrom(sock, buffer, BUFLEN, 0, (struct sockaddr *) &addr, &slen);
    // }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
