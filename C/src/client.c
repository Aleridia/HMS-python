#include "../includes/client.h"
#include "../includes/socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LENGTH 150
#define TYPE "temperature"
#define IP "127.0.0.1"
#define FILEPATH "../../data/temperature.tsv"

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
    remote.sin_addr.s_addr = inet_addr(IP); //Local Host
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

    /*************** FILE *****************/
    // On suppose qu'on est sur le capteur temperature
    FILE *file = NULL;

    file = fopen(FILEPATH, "r+");

    //check if we opened the file
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier d entree.\n");
        return;
    }

    char *buffer = (char *)malloc(MAX_LENGTH);

    while (!feof(file))
    {

        /**************** SOCKET **********************/
        //Create socket
        hSocket = SocketCreate();

        if (hSocket == -1)
        {
            printf("Could not create socket\n");
        }

        printf("Socket is created\n");

        //Connect to remote server
        if (SocketConnect(hSocket) < 0)
        {
            perror("connect failed.\n");
        }
        printf("Sucessfully conected with server\n");

        fgets(buffer, MAX_LENGTH, file);

        if (ferror(file))
        {
            fprintf(stderr, "Reading error with code \n");
            break;
        }

        char *token = strtok(buffer, ";");

        //get the timecode
        char *date = strtok(NULL, ";");

        //get the value
        char *tmp = strtok(NULL, ";");
        char value[strlen(tmp)];
        strcpy(value, tmp);

        //delete the \n
        value[strlen(value) - 2] = '\0';

        //put the sensor type
        strcat(value, ";");
        strcat(value, TYPE);

        //Send data to the server
        SocketSend(hSocket, value, MAX_LENGTH);

        close(hSocket);
        shutdown(hSocket, 0);
        sleep(0.1);
    }

    fclose(file);
}

int main()
{

    sendMessage();

    return 0;
}
