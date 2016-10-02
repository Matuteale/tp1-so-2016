#include "data.h"

char * newData(int i) {
	char* buffer = malloc(MAX_BUF);
    memset(buffer, 0, MAX_BUF);
    snprintf(buffer, MAX_BUF, "%c%d\n", 'i',i);	
}

char * newData(char * c) {
	char* buffer = malloc(MAX_BUF);
    memset(buffer, 0, MAX_BUF);
    snprintf(buffer, MAX_BUF, "%c%s\n", 's',c);	
}

int decypherData(DataPackage data) {
	char c = data->data;
	switch(c) {
		case 's':
			char * str = &c + 1;
			break;
		case 'i':
			int num = &c + 1;
			break;
	}
}