#include "client.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int client(){

    char *ip= "127.0.0.1";
    int port = 4567;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[512];
    int n;

    sock = sock(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Socket error");
        exit(1);
    }
    printf("TCP client socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(sock, (struct sockaddr*) &addr, sizeoff(addr));
    if(connection_status < 0){
        printf("Error connecting to the remote socket.\n");
    }
    printf("Connected.\n");

    //Revieve data from server
    recv(sock, &buffer, sizeof(buffer), 0);

    //Print out server response
    printf(buffer);

    close(sock);


    return 0;
}