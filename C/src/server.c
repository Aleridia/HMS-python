//#include "../includes/server.h"
#include "../includes/socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LENGTH 150
#include <pthread.h>

short SocketCreate(void)
{
    printf("Create the socket\n");
    return socket(AF_INET, SOCK_STREAM, 0);
}

//try to connect with server
int SocketConnect(int hSocket, char* ip)
{
    int ServerPort = 2525;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr(ip);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);
    return connect(hSocket, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
}

// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int hSocket, char *Rqst, short lenRqst)
{
    struct timeval tv;
    tv.tv_sec = 20; /* 20 Secs Timeout */
    tv.tv_usec = 0;
    if (setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    return send(hSocket, Rqst, lenRqst, 0);
}

int BindCreatedSocket(int hSocket)
{
    int ClientPort = 90190;
    struct sockaddr_in remote = {0};

    /* Internet address family */
    remote.sin_family = AF_INET;

    /* Any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(ClientPort); /* Local port */

    return bind(hSocket, (struct sockaddr *)&remote, sizeof(remote));
}

int ia_desc;
void* handle_client(void *sock) {
	char client_message[128] = {0};
	ssize_t read;
		
	int socket = (int)sock;
	while((read = recv(socket, client_message, 128, 0)) > 0) {
		int i;
		for(i = 0; i < 128; i++) {
			if(client_message[i] == '|') {
				client_message[i+1] = '\0';
				break;
			}
		}
		printf("%s\n", client_message);
		
		SocketSend(ia_desc, client_message, i+1);
			
	}
    close(socket);
}
void processData(char *sensor)
{

    if (strcmp(sensor, "temperature") == 0)
    {
    }
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Usage : ./server <ip_python>\n");
		return -1;
	}
	
	char* ip = argv[1];
	
	ia_desc = SocketCreate();
	if (SocketConnect(ia_desc, ip) < 0) {
		perror("connect failed.\n");
    }
	
    int socket_desc, sock, clientLen;
    struct sockaddr_in client;

    //Create socket
    socket_desc = SocketCreate();
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }

    printf("Socket created\n");

    //Bind
    if (BindCreatedSocket(socket_desc) < 0)
    {
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("bind done\n");

    //Listen
    listen(socket_desc, 5);

    //Accept and incoming connection
    while (1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);

        if (sock < 0)
        {
            perror("accept failed");
            return 1;
        }
		
        printf("Connection accepted\n");
		pthread_t thread;
		pthread_create(&thread, NULL, &handle_client, (void*)sock);
    }

    return 0;
}