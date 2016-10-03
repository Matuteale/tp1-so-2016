gcc -w -g -Wall -pedantic -o client clientSocket.c blackjacklib.c comSOCKs.c marshalling.c
gcc -w -g -Wall -pedantic -o server serverSocket.c blackjacklib.c comSOCKs.c marshalling.c