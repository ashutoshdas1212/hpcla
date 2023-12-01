#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_ENTRANCES 5

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
int entrance_counters[NUM_ENTRANCES] = {0};
int total_customers = 0;
int closing_time = 0;

void *entrance_thread(void *arg)
{
    int entrance_id = *((int *)arg);

    while (!closing_time)
    {
        // Simulate customers entering the store
        usleep(1000000); // Sleep for 1 second
        pthread_mutex_lock(&counter_mutex);
        entrance_counters[entrance_id]++;
        total_customers++;
        pthread_mutex_unlock(&counter_mutex);
    }

    pthread_exit(NULL);
}

void *management_thread(void *arg)
{
    while (!closing_time)
    {
        // Simulate management queries
        usleep(5000000); // Sleep for 5 seconds
        pthread_mutex_lock(&counter_mutex);
        printf("Management Query: Total customers so far: %d\n", total_customers);
        pthread_mutex_unlock(&counter_mutex);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t entrance_threads[NUM_ENTRANCES];
    pthread_t management_thread_id;

    int entrance_ids[NUM_ENTRANCES];

    // Create threads for entrances
    for (int i = 0; i < NUM_ENTRANCES; i++)
    {
        entrance_ids[i] = i;
        pthread_create(&entrance_threads[i], NULL, entrance_thread, (void *)&entrance_ids[i]);
    }

    // Create thread for management queries
    pthread_create(&management_thread_id, NULL, management_thread, NULL);

    // Simulate the store operating for a day
    sleep(30); // You can adjust the simulation time as needed

    // Set the closing time flag
    closing_time = 1;

    // Wait for all entrance threads to finish
    for (int i = 0; i < NUM_ENTRANCES; i++)
    {
        pthread_join(entrance_threads[i], NULL);
    }

    // Wait for the management thread to finish
    pthread_join(management_thread_id, NULL);

    // Calculate total customers at the end of the day
    printf("Total customers entered the store today: %d\n", total_customers);

    return 0;
}

gcc - pthread chatgp.c - o chatgp
