#include "Philosopher.h"

Philosopher::Philosopher(int n)
{
    this->number = n;
    string state;
    this->progress = 0;
    this->food = 0;
    this->leftFork = NULL;
    this->rightFork = NULL;
}

Philosopher::Philosopher() {}

void Philosopher::run()
{
    while (*ForkManager::isRunning)
    {
        think();
        if (!*ForkManager::isRunning)
        {
            return;
        }
        takeFork();
        if (!*ForkManager::isRunning)
        {
            return;
        }
        eat();
        if (!*ForkManager::isRunning)
        {
            return;
        }
        releaseFork();
    }
}

void Philosopher::think()
{
    mtx.lock();
    state = "thinking";
    mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void Philosopher::takeFork()
{
    mtx.lock();
    state = "taking Forks";
    mtx.unlock();
    while (!ForkManager::canEat(number) && *ForkManager::isRunning)
    {
        //cout << number << " can;t eat" << endl;
        workSimulation(rand() % 10 + 15);
    }
    //cout << number << " eat+++" << endl;
    ForkManager::getFork(number, leftFork, rightFork);
}

void Philosopher::eat()
{
    mtx.lock();
    state = "eating";
    mtx.unlock();
    leftFork->mtx.lock();
    leftFork->dirt += rand() % 10;
    leftFork->mtx.unlock();
    rightFork->mtx.lock();
    rightFork->dirt += rand() % 10;
    rightFork->mtx.unlock();
    workSimulation(rand() % 10 + 15);
    mtx.lock();
    if (food >= INT32_MAX)
    {
        food = 0;
    }
    food++;
    mtx.unlock();
}

void Philosopher::releaseFork()
{
    this->mtx.lock();
    state = "release Fork";
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
    ForkManager::releaseFork(number, leftFork, rightFork);
}

void Philosopher::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
        this->mtx.lock();
        progress = float(i * 100) / times;
        this->mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    this->mtx.lock();
    progress = 0;
    this->mtx.unlock();
}
