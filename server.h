//
// Created by julia on 03/04/2022.
//

#ifndef BSGRUPPE25PRAKTIKUM_SERVER_H
#define BSGRUPPE25PRAKTIKUM_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>


struct Server{
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    void (*launch)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void (*launch)(struct Server *server));

#endif //BSGRUPPE25PRAKTIKUM_SERVER_H
