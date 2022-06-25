/*
	C socket server example
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "keyValStore.h"


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
