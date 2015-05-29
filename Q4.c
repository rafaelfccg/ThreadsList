
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMBERS_OF_THREAD 30


typedef struct elem{
    int value;
    struct elem *prox;
}Elem;

typedef struct blockingQueue {
    unsigned sizeBuffer,statusBuffer;
    Elem *head,*last;
    pthread_cond_t prodCond;
    pthread_cond_t consCond;
    pthread_mutex_t prodMut;
    pthread_mutex_t consMut;
    
}BlockingQueue;


BlockingQueue *newBlockingQueue(unsigned inSizeBuffer)
{
    
    BlockingQueue *queue = malloc(sizeof(BlockingQueue));
    pthread_cond_init(&queue->prodCond, NULL);
    pthread_cond_init(&queue->consCond, NULL);
    pthread_mutex_init(&queue->prodMut, NULL);
    pthread_mutex_init(&queue->consMut, NULL);
    
    queue->head = NULL;
    queue->last = NULL;
    queue->statusBuffer =0;
    queue->sizeBuffer = inSizeBuffer;
    return queue;
}

void putBlockingQueue(BlockingQueue *Q,int newValue)
{
    Elem *elem = malloc(sizeof(Elem));
    elem->value = newValue;
    elem->prox = NULL;
    //lock produtores
    pthread_mutex_lock(&Q->prodMut);
    //printf("Vou adicionar (PUSH %d) %d\n",newValue,Q->statusBuffer);
     if(Q->statusBuffer >= Q->sizeBuffer)
     {
            printf("Esperando consumidor \n");
            pthread_cond_wait(&Q->consCond, &Q->prodMut);
            printf("Produtor Continuara \n");

    }
    if(Q->head == NULL && Q->last==NULL)
    {
        Q->head = elem;
        Q->last = elem;
    }
    else{
       
        Q->last->prox = elem;
        Q->last = elem;
    }
    Q->statusBuffer++;

   
    pthread_mutex_unlock(&Q->prodMut);
    pthread_cond_signal(&Q->prodCond);
}

int takeBlockingQueue(BlockingQueue*Q)
{   pthread_mutex_lock(&Q->consMut);
     if(Q->head == NULL)
    {
        printf("Esperando produtor \n");
        pthread_cond_wait(&Q->prodCond, &Q->consMut);
       if(Q->head == NULL)printf("FUUUU");
  
        printf("Consumidor continuara\n");
    }

    int value = value = Q->head->value;
    Elem *el = Q->head;
  
    //printf("VOU REMOVER (POP %d) %d\n", value,Q->statusBuffer);
    if(Q->head==Q->last){
    	free(el);
    	Q->head = NULL;
    	Q->last = NULL;
    }else{
		Q->head = Q->head->prox;
		//el->prox = NULL;
		free(el);
    }
    
    Q->statusBuffer--;
    pthread_mutex_unlock(&Q->consMut);
    pthread_cond_signal(&Q->consCond);
    return value;
}

void *thread1(void* bla){
    BlockingQueue *queue = (BlockingQueue*) bla;
    
    putBlockingQueue(queue, 1);
    putBlockingQueue(queue, 2);
    putBlockingQueue(queue, 3);
    //printf("Adicionei 3 \n");
    sleep(2);
    putBlockingQueue(queue, 4);
    putBlockingQueue(queue, 5);
    putBlockingQueue(queue, 6);
    putBlockingQueue(queue, 7);
    putBlockingQueue(queue, 8);
    putBlockingQueue(queue, 9);
    putBlockingQueue(queue, 10);
    putBlockingQueue(queue, 11);
    putBlockingQueue(queue, 12);
    
   
    
    return NULL;
}

void *thread2(void* bla){
    BlockingQueue *queue = (BlockingQueue*) bla;
    //sleep(2);
     sleep(1);
    takeBlockingQueue(queue);//1
    takeBlockingQueue(queue);//2
    takeBlockingQueue(queue);//3 
    takeBlockingQueue(queue);//4
    takeBlockingQueue(queue);//5
    takeBlockingQueue(queue);//6
     sleep(1);
    takeBlockingQueue(queue);//7
    takeBlockingQueue(queue);//8
    takeBlockingQueue(queue);//9
    takeBlockingQueue(queue);//10
    takeBlockingQueue(queue);//11
    takeBlockingQueue(queue);//12
    sleep(1);
    //takeBlockingQueue(queue);
    //takeBlockingQueue(queue);
    //takeBlockingQueue(queue);
    
    
    
    return NULL;
}
int main(int argc, char *argv[]) {
    
    BlockingQueue *queue = newBlockingQueue(10);
    
    pthread_t cons;
    pthread_t prod;
    
    pthread_create(&prod, NULL, thread1, queue);
    pthread_create(&cons, NULL, thread2, queue);
    
    pthread_join(cons, NULL);
    pthread_join(prod, NULL);
    
    
}

