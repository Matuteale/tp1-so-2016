gcc -w -g -Wall -pedantic -o client client.c blackjacklib.c comFIFOs.c marshalling.c
gcc -w -g -Wall -pedantic -o server server.c blackjacklib.c comFIFOs.c marshalling.c
gcc -w -g -Wall -pedantic -o logging logging.c