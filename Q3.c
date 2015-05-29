#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int n;
int marker = 1;
pthread_mutex_t getNextM;
pthread_mutex_t product[100000];
int count[100000];
int p;

int getNext(){
	int next;
	printf("getNext");
	
	next = marker;
	marker++;
	if(marker <=n){
		return next;
	}else
		return -1;
}	


void* counter(void * bla){
	printf("CRIOU VEI ");
	int id=  (int) bla;
	char fileName[50];
	FILE * currentFile;
	
	pthread_mutex_lock(&getNextM);
	int next = getNext();
	pthread_mutex_unlock(&getNextM);
	printf("%dGot activity%d\n",id,next);
	if(next == -1) return NULL;
	printf("%dGot activity%d\n",id,next);

	sprintf(fileName,"%d.in",next);
	printf("%s\n",fileName);
	currentFile = fopen(fileName,"r");
	
	while(next!=-1){
		printf("%dGot file %s\n",id,fileName);
		
		int productNumber;
		while(fscanf(currentFile,"%d",&productNumber)!=EOF){
			pthread_mutex_lock(&product[productNumber]);
			count[productNumber]++;
			printf("%dFound Product:%d , counter :%d\n",id,productNumber, count[productNumber]);
			pthread_mutex_unlock(&product[productNumber]);
		}
		fclose(currentFile);
		pthread_mutex_lock(&getNextM);
		next = getNext();
		pthread_mutex_unlock(&getNextM);
		if(next==-1) return NULL;
		sprintf(fileName,"%d.in",next);
		currentFile = fopen(fileName,"r");
	
	}
	
}

int main(){
	int t;
	printf("Read number of files:\n");
	scanf("%d",&n);
	printf("Read number of threads:\n");
	scanf("%d",&t);
	printf("Read number of products:\n");
	scanf("%d",&p);
	pthread_t threads[t];
	int i = 0;
	printf("Create mutexs:\n");
	pthread_mutex_init(&getNextM,NULL);
	for(i =0; i<p; i++){
		pthread_mutex_init(&product[i],NULL);
		count[i] = 0;
	}
	printf("Initiate Threads\n");
	for(i =0; i<t; i++){
		printf("Creating thread %d\n",i);
		int status = pthread_create(&threads[i], NULL,counter,(void *) i);
		if(status != 0) {
			printf("Error ao criar Thread - %d, error code = %d", i, status);
			exit(-1);
		}
	}	
	printf("Join Threads\n");
	for(i =0; i<t; i++){
		pthread_join(&threads[i], NULL);
	}
	printf("Print Counter\n");
	for(i =0; i<n; i++){
		printf("Product %d : %d\n",i,count[i]);
	}
}




