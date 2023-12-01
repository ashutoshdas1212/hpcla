#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ENTRANCES 5

// Shared variables
int total_customers = 0;
int store_open = 1;

// Mutex to protect shared variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to increment the total customer count
void *increment_customer_count(void *arg)
{
    while (store_open)
    {
        pthread_mutex_lock(&mutex);
        total_customers++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Function to handle queries from the management
void *handle_queries(void *arg)
{
    while (1)
    {
        if (!store_open)
        {
            break;
        }

        int choice;
        printf("Do you want to know the current customer count? (1) or close the store? (2): ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            pthread_mutex_lock(&mutex);
            printf("Current customer count: %d\n", total_customers);
            pthread_mutex_unlock(&mutex);
        }
        else if (choice == 2)
        {
            store_open = 0;
            break;
        }
        else
        {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }
    return NULL;
}

int main()
{
    // Create threads for each entrance
    pthread_t entrance_threads[NUM_ENTRANCES];
    for (int i = 0; i < NUM_ENTRANCES; i++)
    {
        pthread_create(&entrance_threads[i], NULL, increment_customer_count, NULL);
    }

    // Create a thread to handle queries from the management
    pthread_t query_thread;
    pthread_create(&query_thread, NULL, handle_queries, NULL);

    // Wait for all entrance threads to finish
    for (int i = 0; i < NUM_ENTRANCES; i++)
    {
        pthread_join(entrance_threads[i], NULL);
    }

    // Wait for the query thread to finish
    pthread_join(query_thread, NULL);

    // Print the total number of customers who entered the store
    printf("Total customers: %d\n", total_customers);

    return 0;
}
