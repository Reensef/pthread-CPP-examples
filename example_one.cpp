#include <pthread.h>
#include <fstream>
#include <iostream>
#include <string>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct FileParams
{
    const char *filename;
    const std::streampos startPos;
    const std::streampos endPos;
};

void *readFile(void *fileParams)
{
    FileParams *params = (FileParams *)(fileParams);
    std::ifstream file(params->filename);

    file.seekg(params->startPos, std::ios::beg);

    std::string line;
    while (std::getline(file, line))
    {
        pthread_mutex_lock(&mutex);
        std::cout << "Thread " << pthread_self() << ": " << line << std::endl;
        pthread_mutex_unlock(&mutex);

        if (file.tellg() > params->endPos)
            break;
    }

    file.close();
    delete params;
    pthread_exit(NULL);
}

int main()
{
    const char *filename = "example.txt";
    std::ifstream file(filename, std::ios::binary);
    std::string line;

    long countLine = 1;
    while (std::getline(file, line))
    {
        countLine++;
    }
    file.clear();
    file.seekg(0);

    std::streampos firstEndPos = 0;
    std::streampos secondStartPos = 0;
    std::streampos secondEndPos = 0;

    long currLine = 0;
    while (std::getline(file, line))
    {
        if (currLine == countLine / 2 - 1)
        {
            secondStartPos = file.tellg();
        }
        else if (currLine == countLine / 2)
        {
            firstEndPos = file.tellg();
            break;
        }
        currLine++;
    }
    file.seekg(0, std::ios::end);
    secondEndPos = file.tellg();

    file.close();

    FileParams *firstFileParam = new FileParams{filename, 0, firstEndPos};
    FileParams *secondFileParam = new FileParams{filename, secondStartPos, secondEndPos};
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, readFile, (void *)firstFileParam) != 0)
    {
        std::cerr << "Error creating thread 1" << std::endl;
        return 1;
    }

    if (pthread_create(&thread2, NULL, readFile, (void *)secondFileParam) != 0)
    {
        std::cerr << "Error creating thread 2" << std::endl;
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}