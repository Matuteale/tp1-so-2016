#ifndef BLACKJACKLIB_H
#define BLACKJACKLIB_H

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>

#define CLOCK 100000

#define CARDS_PER_DECK 52
#define PLAYING_DECKS 6
#define DECK_PENETRATION 0.8
#define SUITS 4
#define MAX_SCORE 21
#define STARTING_MONEY 100

#define SRV_PATH "/tmp/srv\0"
#define MAX_BUF 1024
#define MAX_PATH 64
#define MAX_PID_LENGTH 20
#define MAX_PLAYERS 4
#define MAX_CARDSINHAND 22
#define MAX_DIGITS floor(log10(abs(INT_MAX)))
#define CROUPIER_SEAT MAX_PLAYERS

// MSGS CODES
#define SUCCESS "1"

// ACTIONS
#define BET 'A'
#define PLAY 'B'
#define DEAL 'C'
#define CLEARSEAT 'D'
#define CLEARTABLE 'E'
#define SHUFFLE 'F'
#define SETACTIVE 'G'
#define SETUNACTIVE 'H'
#define UPDATEBALANCE 'I'

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
	Seat * seats[MAX_PLAYERS+1];
}Table;

typedef struct Deal{
	char card;
	int playerNumber;
}Deal;

typedef struct Bet{
	int bet;
	int playerNumber;
}Bet;

Card * newCard(char figure);
Seat * newSeat();
Table * newTable();
Deal * newDeal(char card, int playerNumber);

void deleteCard(Card * card);
void deleteSeat(Seat * seat);
void deleteTable(Table * table);
void deleteDeal(Deal * deal);
int getCardValue(char figure);
void clearSeat(Seat * seat);
void clearTable(Table * table);
void addCardToSeat(Card * card, Seat * seat);
void calculateScore(Seat * seat);
int hasAce(Seat * seat);
void setActive(Seat * seat);
void setUnActive(Seat * seat);
int hasDeckReachedLimit(int deckIndex);
void showTable(Table * table);
void clearScreen();

// Utilities -----------------------------------------------------------------------------------
void clearBuffer(char * buffer, int size);
int indexOf(char * str, char letter);
char * getStr(int size);
int getInt(int size);
int charToInt(char c);
int strToInt(char * str);
void clearSTDIN();


#endif