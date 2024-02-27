#include <chrono>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <thread>

sem_t semaphore1;
sem_t semaphore2;

void *waiter(void *args)
{
    // Wait for the simaphore2 to give the signal
    sem_wait(&semaphore2);
    std::cout << "Waiter finished!\n";
}

void *signaler1(void *args)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // Send a signal of semaphore1
    sem_post(&semaphore1);
    std::cout << "Signaler1 finished!\n";
}

void *signaler2(void *args)
{
    // Wait for the simaphore1 to give the signal
    sem_wait(&semaphore1);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // Send a signal of semaphore2
    sem_post(&semaphore2);
    std::cout << "Signaler2 finished!\n";
}

int main(int argc, char *argv[])
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    sem_init(&semaphore1, 0, 0);
    sem_init(&semaphore2, 0, 0);

    pthread_create(&thread1, NULL, waiter, NULL);
    pthread_create(&thread2, NULL, signaler1, NULL);
    pthread_create(&thread3, NULL, signaler2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
}