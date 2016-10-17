#ifndef BLACKJACKLIB_H
#define BLACKJACKLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include "database.h"
#include "config.h"

#define CLOCK 200000	//Determina el clock del cliente al que se realizan todas las jugadas.(microseg)
#define SCREEN_TIME 3	//Tiempo que queda en sleep el cliente al terminar la ronda (seg).

#define CARDS_PER_DECK 52
#define DECK_SIZE (CARDS_PER_DECK * PLAYING_DECKS)
#define SUITS 4
#define MAX_SCORE 21
#define CROUPIER_MINSCORE 17

#define MAX_BUF 1024
#define MAX_PATH 64
#define MAX_CARDSINHAND 22
#define MAX_DIGITS floor(log10(abs(INT_MAX)))
#define CROUPIER_SEAT PLAYERS

#define LOSE -1
#define DRAW 0
#define WIN 1

// MSGS CODES
#define SUCCESS "1"
#define FAIL "0"

// ACTIONS
#define BET 10
#define PLAY 11
#define DEAL 12
#define CLEARSEAT 13
#define CLEARTABLE 14
#define SHUFFLE 15
#define SETACTIVE 16
#define SETUNACTIVE 17
#define UPDATEBALANCE 18
#define UPDATEBET 19
#define DISCONNECT 20

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
	Seat * seats[PLAYERS+1];
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
Bet * newBet(int bet, int playerNumber);

void deleteCard(Card * card);
void deleteSeat(Seat * seat);
void deleteTable(Table * table);
void deleteDeal(Deal * deal);
void deleteBet(Bet * bet);
int getCardValue(char figure);
void clearSeat(Seat * seat);
void clearTable(Table * table);
void addCardToSeat(Card * card, Seat * seat);
void calculateScore(Seat * seat);
int hasAce(Seat * seat);
void setActive(Seat * seat);
void setUnActive(Seat * seat);
int hasDeckReachedLimit(int deckIndex);
int hasWon(Seat * seat, int croupierScore);
void showTable(Table * table);
void clearScreen();

// Utilities -----------------------------------------------------------------------------------
void clearBuffer(char * buffer, int size);
int indexOf(char * str, char letter);
char getCharacter();
char * getStr(int size);
int getInt(int size);
int charToInt(char c);
int strToInt(char * str);
char toUpper(char c);
void clearSTDIN();
int randInt(int limitInclusive);
void shuffle(void ** array, int size);
void swap(void ** array, int i, int j);
char * readStrFromFile(char * fileName);


#endif