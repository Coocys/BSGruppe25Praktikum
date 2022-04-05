/*
	C socket server example
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "keyValStore.h"


_Noreturn void launch(struct Server *server){
    char buffer[30000];
    char *hello = "HTTP/1.1 200 OK\nGMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Test Webserver</h1></body></html>";
    int address_length = sizeof (server->address);
    int new_socket;

    while (1){
        printf("===== WAITING FOR CONNECTION =====\n");
        new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);
        read(new_socket, buffer, 30000);
        printf("%s/n", buffer);
        write(new_socket, hello, strlen(hello));
        close(new_socket);
    }
}

int main()
{
    // struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    // server.launch(&server);

    char key[300] = "TesteKey";
    char* keyPointer = key;
    char value[300] = "Teste";
    char* valuePointer = value;
    char newValue[300] = "AHAHAHAHA";
    char* newValuePointer = newValue;

    put(keyPointer, valuePointer);
    put("ABC", "Alphabet");
    put("ABC", "Alpha Kevin");

    get(keyPointer, newValuePointer);
    printf("Key: %s, Value: %s\n", keyPointer, newValuePointer); // Key: TesteKey, Value: Teste


    get("ABC", newValuePointer);
    printf("Key: ABC, Value: %s\n", newValuePointer); // Key: TesteKey, Value: Alpha Kevin

    del(keyPointer);

    if (get(keyPointer, newValuePointer) == -1)
        printf("Teste del!\n");

    put("Second", "2nd");
    get("Second", newValuePointer);

    return 0;
}
