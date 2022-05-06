#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int server() {

    char *ip= "127.0.0.1";
    int port = 5678;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[512];
    char server_msg[512] = "You have successfully connected to the server.\n";
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0){
        perror("Socket error");
        exit(1);
    }
    printf("TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //binding socket to ip adress and port
    n = bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(n<0){
        perror("Error binding");
        exit(1);
    }
    printf("Bound to port: %d\n", port);

    listen(server_sock, 5);
    printf("Listening\n");

    while(1){
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &addr_size);
        printf("Client connected.\n");
    }

    send(client_sock, server_msg, sizeof(server_msg), 0);


    close(server_sock);

    return 0;
}