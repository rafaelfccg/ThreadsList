
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMBERS_OF_THREAD 30

static char password[] = "1001000789"; 
pthread_cond_t found;
pthread_mutex_t mutexi;

//Com uma thread e senha "1001000789" tempo de executacao fica em torno de 2 mimutos
//Com 30  Threads fica: 2 segundos
void *search(void* tid) {
	long long st = (long long) (tid);

	long long i;
	long long sizeIntervals = 10000000000/NUMBERS_OF_THREAD;
	long long fim = sizeIntervals+st;
	for(i=st;i<fim;i++){
		// convert number to string
		char senha[15];
		sprintf(senha,"%010lld",i);
		//compare passwords
		if(strcmp(senha,password)==0){
			printf("Sua Senha é: %s\n",senha);
			pthread_cond_signal(&found);
			//kill all threads
			exit(0);
		}
	}
	
}
long long min(long long a, long long b){
	return a <b ? a : b;
}
int main(int argc, char *argv[]) {
	
	//instaciate "resources"
	pthread_t ids_threads[NUMBERS_OF_THREAD];
	pthread_cond_init(&found, NULL);
	pthread_mutex_init(&mutexi, NULL);
	int status, i;
	//determine size of interval to each thread (ceil to guarantee no missing value to search at the end)
	long long sizeIntervals = ceil(10000000000.0/NUMBERS_OF_THREAD);
	long long count = 0;
	printf("size %lld\n", sizeIntervals);
	for(i=0; i < NUMBERS_OF_THREAD; i++) {
		//create thread min to make sure I stay in the range
		status = pthread_create(&ids_threads[i], NULL, search, (void*)(min(count,10000000000)));
		//says where next thread begins
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

