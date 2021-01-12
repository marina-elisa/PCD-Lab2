#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define THREADS  5

int SOMA = 0;
int respond = 0;
int request = 0;

void *process(void* pId)
{
    int id = (int)pId + 1;

    printf("entrando thread %d\n", id);
    fflush(NULL);
    while(1)
    {
        //printf("while thread %d\n", id);fflush(NULL);
        while(respond != id)
        {
            //printf("thread %d request %d respond %d\n", id, request, respond);fflush(NULL);
            sleep(rand()%2);
            request = id;
            //printf("request=%d\n", request);fflush(NULL);
        }
        //SC
        printf("\n************************\nthread id=%d secao critica\n*************************\n", id); fflush(NULL);
        int local = SOMA;
        SOMA = local + 1;
        printf("\n**********************\nthread id=%d -> SOMA=%d\n**********************\n", id, SOMA); fflush(NULL);
        //printf("\n\nContinua?\n\n");
        //scanf("%i",&i); fflush(NULL);
        sleep(rand()%2);
        respond = 0;
    }
}

void *control(void* pId)
{
    int id = (int)pId + 1;

    printf("entrando server %d\n", id);
    while(1)
    {
        //printf("Server respond=%d\n", respond);
        fflush(NULL);
        //printf("Server request=%d\n", request);
        //fflush(NULL);
        if(request != 0)
        {
            respond = request;
            //printf("Dentro do if Server request=%d\n", request);fflush(NULL);
            while(respond != 0)
            {
                //printf("Dentro do Server while (respond!=0) respond=%d request=%d\n", respond,request);fflush(NULL);
                sleep(rand()%2);
           }
            //printf("encerrou Server while (respond!=0) respond=%d\n", respond);fflush(NULL);
            request = 0;
        }
    }
}

int main()
{
    pthread_t t[THREADS];
    pthread_t server;
    void* status;
    srand(time(NULL));

    pthread_create(&server, NULL, control, THREADS);
    for(int i=0; i<THREADS; i++)
        pthread_create(&t[i], NULL, process,(void *)i);

    pthread_join(server, &status);
    for(int ind=0; ind<THREADS; ind++)
        pthread_join(t[ind], &status);
}
