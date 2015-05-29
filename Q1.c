
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBERS_OF_THREAD 30

static char password[] = "1001000789"; 
pthread_cond_t found;
pthread_mutex_t mutexi;


void *search(void* tid) {
	long long st = (long long) (tid);

	long long i;
	long long sizeIntervals = 10000000000/NUMBERS_OF_THREAD;
	long long fim = sizeIntervals+st;
	for(i=st;i<fim;i++){
		char senha[15];
		sprintf(senha,"%010lld",i);
		//printf("%s\n",senha);
		if(strcmp(senha,password)==0){
			printf("Sua Senha é: %s\n",senha);
			pthread_cond_signal(&found);
			exit(0);
		}
	}
	
}

int main(int argc, char *argv[]) {
	

	pthread_t ids_threads[NUMBERS_OF_THREAD];
	pthread_cond_init(&found, NULL);
	pthread_mutex_init(&mutexi, NULL);
	int status, i;
	long long sizeIntervals = 10000000000/NUMBERS_OF_THREAD;
	long long count = 0;
	printf("size %lld\n", sizeIntervals);
	for(i=0; i < NUMBERS_OF_THREAD; i++) {
		//printf("Main here - created Thread (i=%d)\nStart at %lld- end at %lld\n", i,count,count+sizeIntervals);
		status = pthread_create(&ids_threads[i], NULL, search, (void*)count);
		count+= sizeIntervals;
		if(status != 0) {
			printf("Error ao criar Thread - %d, error code = %d", i, status);
			exit(-1);
		}
	}
	pthread_mutex_lock(&mutexi);
	pthread_cond_wait(&found, &mutexi);
	exit(0);
	
}

