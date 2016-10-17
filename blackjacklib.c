#include "blackjacklib.h"

Card * newCard(char figure) {
	Card * aux = malloc(sizeof(Card));
	aux->figure = figure;
	aux->value = getCardValue(figure);
	return aux;
}

Seat * newSeat() {
	Seat * aux = calloc(1, sizeof(Seat));
	return aux;
}

Table * newTable() {
	int i;
	Table * aux = malloc(sizeof(Table));
	for(i = 0; i < PLAYERS + 1; i++) {
		aux->seats[i] = newSeat();
	}
	return aux;
}

Deal * newDeal(char card, int playerNumber) {
	Deal * aux = malloc(sizeof(Deal));
	aux->card = card;
	aux->playerNumber = playerNumber;
	return aux;
}

Bet * newBet(int bet, int playerNumber) {
	Bet * aux = malloc(sizeof(Bet));
	aux->bet = bet;
	aux->playerNumber = playerNumber;
	return aux;
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
	for (i = 0; i < PLAYERS+1; i++) {
		deleteSeat(table->seats[i]);
	}
	free(table);
}

void deleteDeal(Deal * deal) {
	free(deal);
}

void deleteBet(Bet * bet) {
	free(bet);
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

void clearSeat(Seat * seat) {
	int i;
	for(i = 0; i < seat->handSize; i++ ) {
		deleteCard(seat->hand[i]);
	}
	seat->handSize = 0;
	seat->score = 0;
	seat->currentBet = 0;
	seat->isActive = 0;
}

void clearTable(Table * table) {
	int i;
	for (i = 0; i < PLAYERS+1; i++) {
		clearSeat(table->seats[i]);
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
	if (hasAce(seat) && aux + 10 <= 21) {
		seat->score = aux + 10;
	}else {
		seat->score = aux;
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

int hasDeckReachedLimit(int deckIndex) {
	return ( (((double)deckIndex) / ((double)(DECK_SIZE)))
		> ((double)(DECK_PENETRATION)) );
}

int hasWon(Seat * seat, int croupierScore) {
    if (seat->score > MAX_SCORE) {
        return LOSE;
    }
    if (seat->score < croupierScore && croupierScore <= MAX_SCORE ) {
        return LOSE;
    }
    if (seat->score == croupierScore) {
    	return DRAW;
    }
    return WIN;
}

void showTable(Table * table) {

	Seat * crSeat = table->seats[CROUPIER_SEAT];
	int i, j;

	printf("----------------------------------------------------------------------\n");
	if (crSeat->isActive == 1)
		printf("PLAYING >>> ");
	printf("Croupier\n");
	printf("Cards:   ");

	for(i = 0; i < crSeat->handSize; i++) {
		printf("%c%c ", crSeat->hand[i]->figure == '0' ? '1' : 0, crSeat->hand[i]->figure);
	}
	printf("SCORE: %d  ", crSeat->score);
	printf("\n\n\n");

	for (j = 0; j < PLAYERS; j++) {
		Seat * seat = table->seats[j];
		if (seat->isActive == 1)
			printf("PLAYING >>> ");
		printf("Player %d\n", j);
		printf("Cards:   ");
		for(i = 0; i < seat->handSize; i++) {
			printf("%c%c ", seat->hand[i]->figure == '0' ? '1' : 0, seat->hand[i]->figure);
		}
		printf("SCORE: %d  ", seat->score);
		printf("BET: %d  ", seat->currentBet);
		printf("\n\n");
	}
	printf("----------------------------------------------------------------------\n");

}

void clearScreen() {
	int i = 30;
	while(i-- > 0) {
		printf("\n");
	}
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

	for (i = 0; i <= size && (c = getchar()) != '\n' ; i++) {
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

char getCharacter() {

	char c;

	char * aux = getStr(1);

	c = aux[0];

	free(aux);

	return c;
}

int getInt(int size) {

	char * string = getStr(size);

	int ret = strToInt(string);

	free(string);

	return ret;
}

int charToInt(char c) {
	if (c < '0' || c > '9') {
		return -1;
	}
	return c - '0';
}

int strToInt(char * str) {
	if (str == NULL) {
		return -1;
	}

	int i = strlen(str)-1;
	int j = 1;
	int aux = 0;

	if (i < 0 || i > MAX_DIGITS) {
		return -1;
	}

	for ( ; i >= 0; i--) {
		int digit = charToInt(str[i]);
		if (digit == -1) {
			return -1;
		}

		aux += (digit * j);
		j*= 10;
	}

	return aux;
}

char toUpper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 'A';
	}
	return c;
}

void clearSTDIN() {
	while(getchar() != '\n');
}

int randInt(int limitInclusive) {

	srand(clock());
	int r = rand() % limitInclusive;
	return r;
}

void shuffle(void ** array, int size) {
	int i;
	int random;
	for (i = 0; i < size; i++) {
		random = randInt(size);
		swap(array, i, random);
	}
}

void swap(void ** array, int i, int j) {
	void * aux = array[i];
	array[i] = array[j];
	array[j] = aux;
}

char * readStrFromFile(char * fileName) {

    FILE * file = fopen(fileName, "r");
    char * str;
    int n = 0;
    int c;

    if (file == NULL) {
    	fprintf(stderr, "ERROR reading file.\n");
    	return NULL;
    }

    str = malloc(MAX_BUF);

    while ((c = fgetc(file)) != EOF)
    {
        str[n++] = (char) c;
    }

    str[n] = '\0';

    realloc(str, strlen(str)+1);

    return str;
}