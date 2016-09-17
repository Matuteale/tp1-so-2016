#include "blackjacklib.h"


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