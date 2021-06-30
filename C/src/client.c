#include "../includes/client.h"
#include "../includes/socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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
    struct sockaddr_in remote= {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);
    return connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
}

// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int hSocket,char* Rqst,short lenRqst)
{
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    return send(hSocket, Rqst, lenRqst, 0);
}


void sendMessage()
{
    // On suppose qu'on est sur le capteur temperature
    char name[256];
    sprintf(name, "temperature");

    FILE * file = NULL;

    file = fopen("../data/data_modifie.tsv", "r");

    if(file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier d entree.\n");
        return;
    }

    char * line = NULL;
    size_t  len = 0, nb = 0;
    int indexCapteur = 0;

    while (getline(&line, &len, file) != -1)
    {
        if(nb == 0)
        {
            char *index = strtok(line, "\t");
            int j = 0;
            while(index != NULL)
            {
                if(strcmp(index, name) == 0)
                {
                    indexCapteur = j;
                }

                j++;
                index = strtok(NULL, "\t");
            }
           

            nb++;
            continue;
        }

        char *ptr = strtok(line, "\t");

        char s[1024];
        s[0] = '\0';

        //ajout du capteur
        strcat(s, name);
        strcat(s,"|");

        int i = 0;

        while(ptr != NULL)
        {
            if(i != indexCapteur && i != 0)
            {
                ptr = strtok(NULL, "\t");
                i++;
                continue;
            }

            char * newline = strchr( ptr, '\n' );
            if (newline)
                *newline = 0;
            
            if(i > 0)
                strcat(s, "|");
            strcat(s, ptr);

            ptr = strtok(NULL, "\t");

            i++;
        }

        //Send data to the server
        SocketSend(hSocket, s, strlen(s));

        nb++;
    }

    fclose(file);
}

int main()
{

    //Create socket
    hSocket = SocketCreate();
    if(hSocket == -1)
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
    shutdown(hSocket,0);
    shutdown(hSocket,1);
    shutdown(hSocket,2);

    return 0;
}
