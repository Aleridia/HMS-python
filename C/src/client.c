#include "../includes/client.h"
#include "../includes/socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

//Create a Socket for server communication
short SocketCreate(void)
{
    //printf("Create the socket\n");
    return socket(AF_INET, SOCK_STREAM, 0);
}

//try to connect with server
int SocketConnect(int hSocket, char* ip)
{
    int ServerPort = 90190;
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

int main(int argc, char *argv[])
{
	if(argc < 4) {
		printf("Usage : ./client <vms> <name> <source>\n");
		return -1;
	}
	
	char* ip = argv[1];
	char* name = argv[2];
	char* source = argv[3];
	
	printf("Initializing client with name %s and %s as source file.\n", name, source);
	
	FILE *file = NULL;

    file = fopen(source, "r");

    //check if we opened the file
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier d entree.\n");
        return -1;
    }

    float temps;
    clock_t t1, t2;
    int sequence = 0;

    t1 = clock();
	
	size_t len = 0;
    ssize_t read;
	char * line = NULL;
	int hSocket = SocketCreate();
	
	if (SocketConnect(hSocket, ip) < 0) {
		perror("connect failed.\n");
    }
	
	while ((read = getline(&line, &len, file)) != -1) {
		char *token = strtok(line, ";");
		char *date = strtok(NULL, ";");
		char *value = strtok(NULL, ";");
		float f = strtod(value, NULL);
		
		//printf("%s, %f\n", date, f);
		//if it makes 5s, change the sequence
        if((float)(clock() - t1 )/CLOCKS_PER_SEC > 5.0)
        {
            //update the sequence
            sequence ++;
            t1 = clock();
        }
		
		char toString[256];
		sprintf(toString, "%f;%s|", f, name); //, name); //| is used a common end character, instead of \0, ; is used as separator!
		printf("%s\n", toString);
		int size = strlen(toString);
		SocketSend(hSocket, toString, size);
    }
	
	close(hSocket);
	shutdown(hSocket, 0);

    fclose(file);
	if (line)
        free(line);
    return 0;
}
