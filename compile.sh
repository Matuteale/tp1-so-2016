gcc -w -g -Wall -pedantic -o client client.c blackjacklib.c comFIFOs.c comSOCKs.c
gcc -w -g -Wall -pedantic -o server server.c blackjacklib.c comFIFOs.c comSOCKs.c