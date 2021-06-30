#include "../includes/server.h"
#include "../includes/socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LENGTH 150

short SocketCreate(void)
{
    printf("Create the socket\n");
    return socket(AF_INET, SOCK_STREAM, 0);

}

int BindCreatedSocket(int hSocket)
{
    int ClientPort = 90190;
    struct sockaddr_in  remote= {0};

    /* Internet address family */
    remote.sin_family = AF_INET;

    /* Any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(ClientPort); /* Local port */

    return bind(hSocket,(struct sockaddr *)&remote,sizeof(remote));
}

int main() {

    int socket_desc, sock, clientLen;
    struct sockaddr_in client;
    char client_message[200]= {0};

    //Create socket
    socket_desc = SocketCreate();
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }

    printf("Socket created\n");

    //Bind
    if( BindCreatedSocket(socket_desc) < 0)
    {
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("bind done\n");

    //Listen
    listen(socket_desc, 5);

    //Accept and incoming connection
    while(1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        sock = accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&clientLen);

        if (sock < 0)
        {
            perror("accept failed");
            return 1;
        }

        printf("Connection accepted\n");
        memset(client_message, '\0', sizeof client_message);

        //Receive a reply from the client
        if( recv(sock, client_message, 200, 0) < 0)
        {
            printf("recv failed");
            break;
        }

        printf("%s\n", client_message);
        close(sock);
    }

    return 0;
}