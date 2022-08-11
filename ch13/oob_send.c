#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handing(char *m) {
    fputs(m, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in serv_addr;

    if (argc != 3) {
        printf("usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handing("socket() error! ");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 请求建立连接
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handing("connect() error! ");     
    }

    write(sock, "123", 3);
    send(sock, "4", 1, MSG_OOB);  // send紧急消息
    write(sock, "567", 3);
    send(sock, "890", 3, MSG_OOB);

    close(sock);
    return 0;
}