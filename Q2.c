#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_cond_t found;
pthread_mutex_t mutexi;
pthread_barrier_t barreira;
int resultados;
int num_equipes, num_membros;
int print =0;
//assumi um numero maximo de 100 membros por equipe

int equipes[100][100][2];
//mdc
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

void* doIt(void* team){
	//cast
	int *equipi = (int*)team;
	int equipe = (*equipi);
	int i =0;
	for(i =0;i<num_membros;i++){
		//calculate
		int result = gcd(equipes[equipe][i][0],equipes[equipe][i][1]);
		pthread_mutex_lock(&mutexi);
		//lock and add to results	
		resultados +=result;
		pthread_mutex_unlock(&mutexi);
		//wait others
		pthread_barrier_wait(&barreira);
		// lock to print
		//printf("Equipe %d vai dar printLock",equipe);
		pthread_mutex_lock(&mutexi);
			//printf("Equipe %d entrou no lock e vai verificar p",equipe);
		if(print==0){
			printf("Resultado turno %d: %d\n",(i+1),resultados);
			resultados=0;
			print = 1;
		}
		//unlock
		pthread_mutex_unlock(&mutexi);
		//wait everyone check print
		pthread_barrier_wait(&barreira);
		//set print false to next iteration
		print = 0;
	}
}

int main(int argc, char *argv[]) {
	//le entrada
	scanf("%d%d",&num_equipes,&num_membros);
	// inicializa variaveis
	pthread_t ids_threads[num_equipes];
	int ids[num_equipes];
	pthread_barrier_init(&barreira, NULL, num_equipes);
	int i=0 ,j=0;
	resultados = 0;
	//le mais
	for (i = 0; i < num_equipes; ++i)
	{
		ids[i] =i;
		for (j = 0; j < num_membros; ++j)
		{	
			int a,b;
			scanf("%d%d",&a,&b);
			//equipes[j] = {a,b};
			equipes[i][j][0] = a;
			equipes[i][j][1] = b;
		}
		
	}
	// cria threads
	for (i = 0; i < num_equipes; ++i){
		//printf("Roda %d\n",i);
		int status = pthread_create(&ids_threads[i],NULL,doIt,(void*)(&ids[i]));
		if(status != 0) {
			printf("Error ao criar Thread - %d, error code = %d", i, status);
			exit(-1);
		}
	}
	// da join para esperar acabar
	for (i = 0; i < num_equipes; ++i){
		pthread_join(ids_threads[i],NULL);
	}
	
}

