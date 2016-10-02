#ifndef BLACKJACKLIB_H
#define BLACKJACKLIB_H

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>

#define SRV_PATH "/tmp/srv\0"
#define MAX_BUF 1024
#define MAX_PATH 64
#define MAX_PID_LENGTH 20
#define MAX_PLAYERS 8
#define MAX_CARDSINHAND 22
#define MAX_DIGITS floor(log10(abs(INT_MAX)))

// MSGS CODES
#define SUCCESS "1"

typedef struct Card{
	char figure;
	int value;
}Card;

typedef struct Seat{
	Card * hand[MAX_CARDSINHAND];
	int handSize;
	int score;
	int currentBet;
	int isActive;
}Seat;

typedef struct Table{
	Seat * croupierSeat;
	Seat * playerSeats[MAX_PLAYERS];
}Table;

typedef struct Deal{
	char card;
	int playerNumber;
}Deal;

Card * newCard(char figure);
Seat * newSeat();
Table * newTable();
void deleteCard(Card * card);
void deleteSeat(Seat * seat);
void deleteTable(Table * table);
int getCardValue(char figure);
int getScore(Card ** hand, int handSize);
void clearSeat(Seat * seat);
void clearTable(Table * table);
void addCardToSeat(Card * card, Seat * seat);
void calculateScore(Seat * seat);
int hasAce(Seat * seat);
void setActive(Seat * seat);
void setUnActive(Seat * seat);

// Utilities -----------------------------------------------------------------------------------
void clearBuffer(char * buffer, int size);
int indexOf(char * str, char letter);
char * getStr(int size);
int getInt(int size);
int charToInt(char c);
int strToInt(char * str);
void clearSTDIN();


#endif