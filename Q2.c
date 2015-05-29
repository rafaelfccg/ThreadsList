#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBERS_OF_THREAD 30


pthread_cond_t found;
pthread_mutex_t mutexi;
pthread_barrier_t barreira;
int resultados =0;

struct Equipe
{
	int a;
	int b;
	Equipe(int a, int b):a(a),b(b){}
};


int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

void* doIt(void* team){
	Equipe* e = (Equipe*) team;
	int result = gcd(e->a,e->b);
	resultados +=result;
	pthread_barrier_wait(&barreira);
}

int main(int argc, char *argv[]) {

	int num_equipes, num_membros;
	scanf("%d%d",num_equipes,num_membros);
	Equipe e[num_equipes];
	pthread_t ids_threads[num_equipes];
	pthread_barrier_init(&barreira, NULL, num_equipes+1);
	int i=0 ,j=0;
	for (i = 0; i < num_membros; ++i)
	{
		for (j = 0; j < num_equipes; ++j)
		{	
			int a,b;
			scanf("%d%d",a,b);
			e[j] = Equipe(a,b);
		}
		for (j = 0; j < num_equipes; ++j)
		{	
			pthread_create(&ids_threads[j],NULL,doIt,(void*)(&e[j]));
		}
		pthread_barrier_wait(&barreira);
		printf("Resultado turno %d:%d\n",(i+1),resultados);
		resultados = 0;
	}
	
}

