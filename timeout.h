#ifndef TIMEOUT_H
#define TIMEOUT_H
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#endif

void * expensiveCall(void * data);
int doOrTimeout(struct timespec * max_wait);
void timeout(int maxTimeToWait);
