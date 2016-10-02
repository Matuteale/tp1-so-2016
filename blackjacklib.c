#include "blackjacklib.h"

Card * newCard(char figure) {
	Card * aux = malloc(sizeof(Card));
	aux->figure = figure;
	aux->value = getCardValue(figure);
	return aux;
}

Seat * newSeat() {
	Seat * aux = calloc(1,sizeof(Seat));
}

Table * newTable() {
	Table * aux = malloc(sizeof(Table));
}

void deleteCard(Card * card) {
	free(card);
}

void deleteSeat(Seat * seat) {
	clearSeat(seat);
	free(seat);
}

void deleteTable(Table * table) {
	int i;
	deleteSeat(table->croupierSeat);
	for (i = 0; i < MAX_PLAYERS; i++) {
		deleteSeat(table->playerSeats[i]);
	}
	free(table);
}

int getCardValue(char figure) {
	switch(figure) {
		case 'A' : {
			return 1;
		}
		case 'J': {
			return 10;
		}
		case 'Q': {
			return 10;
		}
		case 'K': {
			return 10;
		}
		case '0': {
			return 10;
		}
		default: {
			if (figure < '2' || figure > '9') {
				fprintf(stderr, "Invalid card input.");
				exit(-1);
			}
			return figure - '0';
		}
	}
}

int getScore(Card ** hand, int handSize) {
	//TODO
	return 0;
}

void clearSeat(Seat * seat) {
	int i;
	for(i = 0; i < MAX_CARDSINHAND; i++ ) {
		deleteCard(seat->hand[i]);
	}
}

void clearTable(Table * table) {
	int i;
	clearSeat(table->croupierSeat);
	for (i = 0; i < MAX_PLAYERS; i++) {
		clearSeat(table->playerSeats[i]);
	}
}

void addCardToSeat(Card * card, Seat * seat) {
	seat->hand[seat->handSize] = card;
	seat->handSize++;
	calculateScore(seat);
}

void calculateScore(Seat * seat) {
	int aux = 0;
	int i;
	for (i = 0; i < seat->handSize; i++) {
		aux+= seat->hand[i]->value;
	}
	if (aux + 10 > 21) {
		seat->score = aux;
	} else if(hasAce(seat)) {
		seat->score = aux + 10;
	}
}

int hasAce(Seat * seat) {
	int i;
	for (i = 0; i < seat->handSize; i++) {
		if (seat->hand[i]->value == 1) {
			return 1;
		}
	}
	return 0;
}

void setActive(Seat * seat) {
	seat->isActive = 1;
}

void setUnActive(Seat * seat) {
	seat->isActive = 0;
}

//Utilities  -----------------------------------------------------------------------------------

void clearBuffer(char * buffer, int size) {
	int i;
	for (i = 0; i < size; i++) {
		buffer[i] = 0;
	}
}

int indexOf(char * str, char letter) {
	int i = 0;
	while ( str[i] != '\0' && str[i] != letter ) {
		i++;
	}
	if (str[i] == '\0') {
		return -1;
	} else {
		return i;
	}
}

char * getStr(int size) {

	char * aux = malloc(size+2 * sizeof(char));

	char c;
	int i;

	for (i = 0; i < size+1 && (c = getchar()) != '\n' ; i++) {
		aux[i] = c;
	}

	if (i == size+1) {
		if (c != '\n') {
			clearSTDIN();
		}
		return NULL;
	}

	aux[i] = '\0';

	return aux;
}

int getInt(int size) {

	char * string = getStr(size);

	int ret = strToInt(string);

	free(string);

	return ret;
}

int charToInt(char c) {
	if (c < '0' || c > '9') {
		return NULL;
	}
	return c - '0';
}

int strToInt(char * str) {
	if (str == NULL) {
		return NULL;
	}

	int i = strlen(str)-1;
	int j = 1;
	int aux = 0;

	if (i <= 0 || i > MAX_DIGITS) {
		return NULL;
	}

	for ( ; i >= 0; i--) {
		int digit = charToInt(str[i]);
		if (digit == NULL) {
			return NULL;
		}
		aux += (digit * j);
		j*= 10;
	}

	return aux;
}

void clearSTDIN() {
	while(getchar() != '\n');
}