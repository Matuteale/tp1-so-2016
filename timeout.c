#include "timeout.h"

pthread_mutex_t calculating = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;

void * expensiveCall(void * data) {
	int oldtype;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
	for(;;) {}
	pthread_cond_signal(&done);
	return NULL;
}

int doOrTimeout(struct timespec * max_wait) {
	struct timespec abs_time;
	pthread_t tid;
	int error;

	pthread_mutex_lock(&calculating);

	clock_gettime(CLOCK_REALTIME, &abs_time);
	abs_time.tv_sec += max_wait->tv_sec;
	abs_time.tv_nsec += max_wait->tv_nsec;

	pthread_create(&tid, NULL, expensiveCall, NULL); 
	error = pthread_cond_timedwait(&done, &calculating, &abs_time);

	if(error == ETIMEDOUT) {
		fprintf(stderr, "Timed Out\n");
	}
	if(!error) {
		printf("BIEN\n");
		pthread_mutex_unlock(&calculating);
	}
}

void  timeout(int maxTimeToWait) {
	struct timespec max_wait;
	memset(&max_wait,0,sizeof(max_wait));
	max_wait.tv_sec = maxTimeToWait;
	doOrTimeout(&max_wait);
}