#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define producers 10
#define consumers 20
#define per_producer 500
#define per_consumer 250
#define max 1000

int pipefd[2]; //pipefd[0] is read and pipefd[1] is write
pthread_mutex_t write_m;
pthread_mutex_t read_m;
sem_t write_s;
sem_t read_s;

void producer(arg)
{
    int thread_index = *(int *)arg;
    free(arg); //ai, these two lines allow the thread to print its id

    int unique[max] = {0}; //a array to keep track of unique numbers in the thread
    int numbers[per_producer] //array to store numbers
    int i = 0;

    while (i < per_producer)
    {
        int random_num = (rand() % (max + 1));
        if (!used[random_num])
        {
            unique[i] = 1;
            numbers[i] = random_num;
            i++; //increase i here so that if a number is duplicated, it'll loop that again until there is a free number
        }
    }

    for(int i = 0; i < per_producer; i++) //writing to pipe
    {
        sem_wait(&write_s);
        pthread_mutex_lock(&write_m); //these two lines 'open' the pipe
        write(pipefd[1], &numbers[i], sizeof(int)); //writes to the pipe
        pthread_mutex_unlock(&write_m);
        sem_post(&write_s);
    }

    printf("Thread complete ID: %d", thread_index);

}


int main(int argc, char *argv[])
{
    pthread_t t_producers[producers];

    if(pipe(pipefd) == -1) //catches if the pipe fails
    {
        perror("pipe");
        return 1;
    }
    if (pthread_mutex_init(&lock, NULL) != 0) //catches if the mutex fails
    {
        perror("pthread_mutex_init");
        return 1;
    }
    if(sem_init(&write_sem, 0, 1) != 0) //catches if the semaphore fails
    {
        perror("sem_init");
        return 1;
    }

    pid_t pid = fork();
    if(pid > 0) //parent
    {
        for (int i = 0; i < producers; i++) //ai
        {                                   //generates 10 producers
            int *idx = malloc(sizeof(int));
            *idx = 1;
            pthread_create(&producers[i], NULL, producer, idx);
        }
        for(int i = 0; i < producers; i++)
        {
            pthread_join(producers[i], NULL);
        }
    }
    else if (pid == 0) //child
    {

        exit(0); //ends child process
    }

}