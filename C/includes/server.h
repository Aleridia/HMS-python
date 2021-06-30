#ifndef SERVER
#define SERVER

// Retourne une connexion vers le serveur
//Prend un tableau de String
void processData(char** data);

// Récupère le message d'un fils
char* getMessage();

// Split un string selon un délimiteur
char** str_split(char* pData, const char pDelim);

#endif