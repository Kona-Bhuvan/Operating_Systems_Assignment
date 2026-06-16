// Name : Kona Bhuvaneswar Reddy
// Roll No: B240214CS
// Class : CS02
// Operating Systems Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

typedef struct
{
    long long N;
    long long start;
    long long end;
    long long *shared_sum;
    pthread_mutex_t *lock;
} ThreadData;

void *factors(void *param)
{
    ThreadData *data = (ThreadData *)param;
    long long local_sum = 0;

    for (long long i = data->start; i <= data->end; i++)
    {
        if (data->N % i == 0)
        {
            local_sum += i;

            long long paired_factor = data->N / i;
            if (paired_factor != data->N && paired_factor != i)
            {
                local_sum += paired_factor;
            }
        }
    }

    pthread_mutex_lock(data->lock);
    *(data->shared_sum) += local_sum;
    pthread_mutex_unlock(data->lock);

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Error: Invalid number of arguments!\n");
        printf("Usage: %s <N> <P>\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);
    int P = atoi(argv[2]);
    
    if(P < 1){
    	printf("Invalid number of Threads!\n");
    	return 1;
    }

    if (N <= 1)
    {
        printf("false\n");
        return 0;
    }

    pthread_t threads[P]; // thread identifier
    pthread_attr_t attr;  // set of thread attributes
    ThreadData thread_data[P];
    pthread_mutex_t lock;
    long long total_sum = 0;

    pthread_attr_init(&attr);
    pthread_mutex_init(&lock, NULL);

    long long limit = (long long)sqrt(N);
    long long range_per_thread = limit / P;

    for (int i = 0; i < P; i++)
    {
        thread_data[i].N = N;
        thread_data[i].shared_sum = &total_sum;
        thread_data[i].lock = &lock;

        thread_data[i].start = i * range_per_thread + 1;
        thread_data[i].end = (i == P - 1) ? limit : (i + 1) * range_per_thread;

        pthread_create(&threads[i], &attr, factors, &thread_data[i]);
    }

    for (int i = 0; i < P; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&lock);

    if (total_sum == N)
        printf("true\n");
    else
        printf("false\n");

    return 0;
}
