#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

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
int sums[consumers];

void *producer(void *arg)
{
    int thread_index = *(int *)arg;
    free(arg); //ai, these two lines allow the thread to print its id

    int unique[max] = {0}; //array to keep track of unique numbers in the thread
    int numbers[per_producer]; //array to store numbers
    unsigned int seed = time(NULL) ^ thread_index; //ai

    for (int i = 0; i < per_producer; )
    {
        int random_num = rand_r(&seed) % max;
        if (!unique[random_num])
        {
            unique[random_num] = 1; //ai caught a bug here
            numbers[i] = random_num;
            i++; //increase i here so that if a number is duplicated, it'll loop that again until there is a free number
        }
    }
    

    for(int i = 0; i < per_producer; i++) //writing to pipe
    {
        sem_wait(&write_s);
        pthread_mutex_lock(&write_m);   //these two lines 'open' the pipe for writing
        write(pipefd[1], &numbers[i], sizeof(int)); //writes to the pipe
        pthread_mutex_unlock(&write_m);
        sem_post(&write_s);             //these two lines close the pipe
    }
    

    printf("Thread complete ID: %d\n", thread_index);
    return 0;

}

void *consumer(void *arg)
{
    int thread_index = *(int *)arg;
    free(arg);

    int number;
    int total = 0;

    for(int i = 0; i < per_consumer; i++) //writing to pipe
    {
        sem_wait(&read_s);
        pthread_mutex_lock(&read_m); //these two lines 'open' the pipe for reading
        read(pipefd[0], &number, sizeof(int)); //reads the pipe
        pthread_mutex_unlock(&read_m);
        sem_post(&read_s); //these two lines close the pipe

        total += number;
    }
    sums[thread_index] = total;
    return 0;
}

double average()
{
    double total_average = 0.0;
    for (int i = 0; i < consumers; i++)
    {
        total_average += sums[i];
    }
    return total_average / consumers;
}


int main(int argc, char *argv[])
{
    pthread_t t_producers[producers];
    pthread_t t_consumers[consumers];

    if(pipe(pipefd) == -1) //catches if the pipe fails
    {
        perror("pipe");
        return 1;
    }
    if (pthread_mutex_init(&write_m, NULL) != 0 || pthread_mutex_init(&read_m, NULL) != 0) //ai. catches if the mutex fails
    {
        perror("pthread_mutex_init");
        return 1;
    }
    if(sem_init(&write_s, 0, 1) != 0 || sem_init(&read_s, 0, 1) != 0) //ai. catches if the semaphore fails
    {
        perror("sem_init");
        return 1;
    }
    srand(time(NULL)); //ai. Added because it was producing the same average every run

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("Fork failed");
        exit(0);
    }
    else if(pid > 0) //parent
    {
        close(pipefd[0]);
        for (int i = 0; i < producers; i++) //ai
        {                                   //generates 10 producers
            int *idx = malloc(sizeof(int)); //this is what is used for thread ID
            *idx = i;
            pthread_create(&t_producers[i], NULL, producer, idx);
        }
        for(int i = 0; i < producers; i++)
        {
            pthread_join(t_producers[i], NULL);
        }
        int status;
        waitpid(pid, &status, 0);
    }
    else if (pid == 0) //child
    {   

        freopen("average.txt", "w", stdout);
        close(pipefd[1]);
        for (int i = 0; i < consumers; i++) //ai
        {                                   //generates 10 consumers
            int *idx = malloc(sizeof(int)); //this is what is used for thread ID
            *idx = i;
            pthread_create(&t_consumers[i], NULL, consumer, idx);
        }
        for(int i = 0; i < consumers; i++)
        {
            pthread_join(t_consumers[i], NULL);
        }
        printf("Average of all consumer sums: %.2f\n", average());
        exit(0); //ends child process
    }


}