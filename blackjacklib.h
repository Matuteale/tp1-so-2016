#ifndef BLACKJACKLIB_H
#define BLACKJACKLIB_H
#endif

#define SRV_PATH "/tmp/srv"
#define MAX_BUF 1024
#define MAX_PATH 64
#define MAX_PID_LENGTH 20
#define MAX_PLAYERS 8

// MSGS CODES
#define SUCCESS "1"

struct ClientInfo{
	char * clientin;
	char * clientout;
	int clientinfd;
	int clientoutfd;
};

void clearBuffer(char * buffer, int size);
int indexOf(char * str, char letter);