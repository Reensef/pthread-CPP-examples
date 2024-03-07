## Example One
In this example, the file is read by two threads. Each thread reads its own half.

## Example Two
This is an example of using semaphores. A mutex differs from a semaphore in that you can specify how many threads can run at the same time when it is blocked.

## Example Three
This is an example of using a conditional variable. A special syntax is used for this.
```
pthread_mutex_lock(&mutex);
while (value < 10)
{
    pthread_cond_wait(&cond, &mutex);
}
pthread_mutex_unlock(&mutex);
```