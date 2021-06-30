
//Créer un retourne un socket
short SocketCreate();

// Bind le socket sur le réseau
int BindCreatedSocket(int hSocket);

//Permet de se co,nnecter au socket passé en paramètre
int SocketConnect(int hSocket);

//Permet d'envoyer des données sur le socket au serveur
int SocketSend(int hSocket,char* Rqst,short lenRqst);