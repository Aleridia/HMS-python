#include "../includes/client.h"
#include "../includes/socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAX_LENGTH 150

int hSocket;

//Create a Socket for server communication
short SocketCreate(void)
{
    printf("Create the socket\n");
    return socket(AF_INET, SOCK_STREAM, 0);
}

//try to connect with server
int SocketConnect(int hSocket)
{
    int ServerPort = 90190;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
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

void sendMessage()
{
    // On suppose qu'on est sur le capteur temperature
    FILE *file = NULL;

    file = fopen("../../data/temperature.tsv", "r+");

    //check if we opened the file
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier d entree.\n");
        return;
    }

    char *buffer = (char *)malloc(MAX_LENGTH);

    while (!feof(file))
    {
        fgets(buffer, MAX_LENGTH, file);

        if (ferror(file))
        {
            fprintf(stderr, "Reading error with code \n");
            break;
        }

        printf("%s\n", buffer);

        //Send data to the server
        SocketSend(hSocket, buffer, MAX_LENGTH);
    }


    fclose(file);
}

int main()
{

    //Create socket
    hSocket = SocketCreate();

    if (hSocket == -1)
    {
        printf("Could not create socket\n");
        return 1;
    }

    printf("Socket is created\n");

    //Connect to remote server
    if (SocketConnect(hSocket) < 0)
    {
        perror("connect failed.\n");
        return 1;
    }
    printf("Sucessfully conected with server\n");

    sendMessage();

    close(hSocket);
    shutdown(hSocket, 0);

    return 0;
}
