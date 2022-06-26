//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "keyValStore.h"
#include <regex.h>

#define TRUE 1
#define FALSE 0
#define PORT 5678

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025]; //data buffer of 1K

    //set of socket descriptors
    fd_set readfds;

    //a message
    char *message = "ECHO Daemon v1.0 \r\n";

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 5678
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));

            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }

                    //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    buffer[valread] = '\0';
//                    send(sd , buffer , strlen(buffer) , 0 );
//                    puts(buffer);

                    char bufferSplit[1024];
                    strcpy(bufferSplit, buffer);
                    int f = 0;
                    int e = 0;
                    char *p = strtok (bufferSplit, " ");
                    char *array[3];

                    while (p != NULL)
                    {
                        e++;
                        array[f++] = p;
                        p = strtok (NULL, " ");
                    }

                    if(e == 1){
                        char * ret = strstr(array[0], "QUIT");
                        if (ret){
                            char* byeMessage = "See you again!\n";
                            send(sd,byeMessage, strlen(byeMessage), 0);
                            // Somebody disconnected , get his details and print
                            getpeername(sd , (struct sockaddr*)&address , \
                            (socklen_t*)&addrlen);
                            printf("Host disconnected , ip %s , port %d \n" ,
                                   inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                            //Close the socket and mark as 0 in list for reuse
                            close( sd );
                            removeSub(sd);
                            client_socket[i] = 0;
                        }
                        else
                            puts("No Command");
                    }
                    else if(e == 2){
                        if(strcmp(array[0], "GET") == 0){
                            char res[300] = "key_nonexistent\n";
                            char* resPointer = res;

                            get(array[1], resPointer);

                            char getMessage[200] = "> ";
                            strcat(getMessage, array[0]);
                            strcat(getMessage, ":");
                            strcat(getMessage, array[1]);
                            strcat(getMessage, ":");
                            strcat(getMessage, res);

                            send(sd, getMessage, strlen(getMessage), 0);
                        }
                        else if(strcmp(array[0], "DEL") == 0){

                            char res[300] = "key_nonexistent\n";
                            char* resPointer = res;
                            int subscribers[10];
                            int pubCode = -1;

                            int getCode = get(array[1], resPointer);

                            if(getCode == 0){
                                pubCode = pub(array[1], subscribers);
                            }


                            int delCode = del(array[1]);


                            char getMessage[200] = "> ";
                            strcat(getMessage, array[0]);
                            strcat(getMessage, ":");
                            strcat(getMessage, array[1]);

                            if(delCode == -1)
                                strcat(getMessage, ":key_nonexistent\n");
                            else
                                strcat(getMessage, ":key_deleted\n");

                            send(sd, getMessage, strlen(getMessage), 0);

                            if(delCode == 0 && pubCode == 0) {

                                for(int j = 0; j < 10; ++j) {

                                    if (subscribers[j] == 0)
                                        break;

                                    if(subscribers[j] == sd)
                                        continue;

                                    send(subscribers[j], getMessage, strlen(getMessage), 0);
                                }

                            }
                        }
                        else if(strcmp(array[0], "SUB") == 0){
                            char res[300] = "key_nonexistent\n";
                            char* resPointer = res;
                            int getCode = get(array[1], resPointer);

                            puts("Is SUB");
                            puts(array[0]);
                            puts(array[1]);

                            char subMessage[200] = "> ";
                            strcat(subMessage, array[0]);
                            strcat(subMessage, ":");
                            strcat(subMessage, array[1]);
                            strcat(subMessage, "\n");


                            if(getCode == 0)
                                sub(array[1], sd);

                            if(getCode == -1){
                                strcat(subMessage, ":");
                                strcat(subMessage, res);
                            }



                            send(sd, subMessage, strlen(subMessage), 0);
                        }
                        else
                            puts("No Command");
                    }
                    else if(e == 3){
                        if(strcmp(array[0], "PUT") == 0){
                            int putCode = put(array[1], array[2]);
                            if(putCode == -1)
                                puts("is Full");
                            else if(putCode == 1)
                                puts("Changes value");
                            else
                                puts("get gut");
                            char putMessage[200] = "> ";
                            strcat(putMessage, array[0]);
                            strcat(putMessage, ":");
                            strcat(putMessage, array[1]);
                            strcat(putMessage, ":");
                            strcat(putMessage, array[2]);

                            send(sd, putMessage, strlen(putMessage), 0);
                            if(putCode == 1) {
                                int subscribers[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                int pubCode = pub(array[1], subscribers);
                                for(int k = 0; k < 10; ++k)
                                    printf("Index: %i, clientNo: %i\n", k, subscribers[k]);
                                if(pubCode == 0){
                                    for(int j = 0; i < 10; ++j) {

                                        if (subscribers[j] == 0)
                                            break;

                                        if(subscribers[j] == sd)
                                            continue;

                                        send(subscribers[j], putMessage, strlen(putMessage), 0);
                                    }
                                }
                            }
                        }
                        else
                            puts("No Command");
                    }
                    else
                        puts("No Command");
                }
            }
        }
    }

    return 0;
}