# HMS 

## Version pré-requis
Python: 3.7.3
GCC: 8.3.0

## Installation

Commencer par clone le repo : ```git clone https://github.com/Aleridia/HMS-python```

### Python
Merci de bien mettre à jour pip! Nécessaire pour l'installation des modules (peut poser des problèmes d'incompatibilité si mauvaise version de pip)

```sh
python -m pip install -U pip
pip install numpy scikit-learn matplotlib pandas
```

### C

### Compilation

```sh
cd C/src
gcc -pthread server.c -lpthread -o server
gcc client.c -o client
```

## Utilisation

Ordre d'exécution : ```script python => serveur C => client C/src```
Utilisez 3 shell différents

```sh
cd Python/
python3 TestIsolationForest_socket.py
```

Spécifiez l'IP du serveur où le script python tourne :

```sh
cd C/src
./server 127.0.1
```

Usage : ```./client <vms> <name> <source```
```sh
cd C/src
./client 127.0.0.1 temp0 ../../data/temperature.tsv
```