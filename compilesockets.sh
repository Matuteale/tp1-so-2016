gcc -w -g -Wall -pedantic -o client client.c blackjacklib.c comSOCKs.c marshalling.c -lrt -lpthread -ldl
gcc -w -g -Wall -pedantic -o server server.c blackjacklib.c comSOCKs.c marshalling.c DBfnc.c database.c sqlite3.c logging.c -lrt -lpthread -ldl
gcc -w -g -Wall -pedantic -o loggingServer loggingServer.c