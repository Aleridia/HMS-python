#include "../includes/server.h"
#include "../includes/socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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


char** str_split(char* pData, const char pDelim)
{
    static char** lReturn = 0;


    int i;
    i = 0;

    char* tmp = pData;
    size_t count     = 0;
    char* last_comma = 0;
    char lDelim[2];
    lDelim[0] = pDelim;
    lDelim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (pDelim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (pData + strlen(pData) - 1);
    count++;

    //allocation du tableau (String)
    lReturn = malloc(sizeof(char*) * count);
    if(lReturn)
    {

        char *ptr = strtok(pData, lDelim);
        while(ptr){

            //assignation du string à une case du tableau
            lReturn[i] = (char*) malloc(sizeof(ptr));

            strcpy(lReturn[i], ptr);

            printf("%s\n", lReturn[i]);
            i++;

            ptr = strtok(NULL, lDelim);
        }
    }

    return lReturn;
}

void processData(char** pData){

    printf("**** New message ****\n");

    //enum pour les capteurs

    if(strcmp(pData[0], "temperature") == 0)
    {
        printf("Température : %s, le %s\n", pData[2], pData[1]);
    }
    else if (strcmp(pData[0], "OTHER") == 0)
    {
        printf("Autre capteur : %s, le %s\n", pData[2], pData[1]);
    }
    else
    {
        printf("Capteur inconnu\n");
    }


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
    listen(socket_desc, 3);

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
        processData(str_split(client_message, '|'));

        close(sock);
        sleep(1);
    }

    return 0;
}
