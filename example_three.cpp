#include <iostream>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int value = 0;

void *thread_function(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (value < 10)
    {
        printf("Value is %d\n", value);
        value++;
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread, NULL, &thread_function, NULL);

    /*
    A thread blocks access to shared data using a mutex.
    The thread checks the condition inside the while.

    If the condition is not satisfied, the thread calls pthread_cond_wait(),
    which releases the mutex and blocks the current thread until it receives
    signal from another thread using pthread_cond_signal().

    When another thread modifies data and sends a signal,
    the blocked thread continues execution and rechecks
    condition in while.
    */

    pthread_mutex_lock(&mutex);
    while (value < 10)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Main thread finished\n");
    pthread_join(thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
