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
    switch (ForkManager::managerOff)
    {
    case 0:
    case 1:
    {
        while (*ForkManager::isRunning)
        {
            think();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            takeFork();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            eat();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            releaseFork();
        }
        break;
    }
    case 2:
    {
        while (*ForkManager::isRunning)
        {
            think();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            takeForks();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            eat();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            releaseForks();
        }
        break;
    }
    case 3:
    {
        while (*ForkManager::isRunning)
        {
            think();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            takeForks2();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            eat();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            releaseForks();
        }
        break;
    }
    case 4:
    {
        while (*ForkManager::isRunning)
        {
            think();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            takeForks3();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            eat();
            if (!*ForkManager::isRunning)
            {
                break;
            }
            releaseForks();
        }
        break;
    }
    default:
        break;
    }
    ForkManager::releaseFork(leftFork);
    ForkManager::releaseFork(rightFork);
}

void Philosopher::think()
{
    mtx.lock();
    state = "thinking";
    mtx.unlock();
    workSimulation(rand() % 15 + 20);
}

void Philosopher::takeFork()
{
    mtx.lock();
    state = "taking Forks";
    mtx.unlock();
    while (!ForkManager::canEat(number) && *ForkManager::isRunning)
    {
        //cout << number << " can;t eat" << endl;
        workSimulation(rand() % 15 + 20);
    }
    //cout << number << " eat+++" << endl;
    ForkManager::getFork(number, leftFork, rightFork);
}

void Philosopher::takeForks()
{
    if (number < ForkManager::number - 1)
    {

        mtx.lock();
        state = "taking left Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting left Fork";
        mtx.unlock();
        ForkManager::getFork(number, leftFork, number);
        mtx.lock();
        state = "taking right Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting right Fork";
        mtx.unlock();
        ForkManager::getFork(number + 1, rightFork, number);
    }
    else
    {
        mtx.lock();
        state = "taking right Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting right Fork";
        mtx.unlock();
        ForkManager::getFork(0, rightFork, number);
        mtx.lock();
        state = "taking left Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting left Fork";
        mtx.unlock();
        ForkManager::getFork(number, leftFork, number);
    }
}

void Philosopher::takeForks3()
{
    if (number < ForkManager::number - 1)
    {

        mtx.lock();
        state = "taking left Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting left Fork";
        mtx.unlock();
        ForkManager::getFork3(number, leftFork, number);
        mtx.lock();
        state = "taking right Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting right Fork";
        mtx.unlock();
        ForkManager::getFork3(number + 1, rightFork, number);
    }
    else
    {
        mtx.lock();
        state = "taking right Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting right Fork";
        mtx.unlock();
        ForkManager::getFork3(0, rightFork, number);
        mtx.lock();
        state = "taking left Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        mtx.lock();
        state = "waiting left Fork";
        mtx.unlock();
        ForkManager::getFork3(number, leftFork, number);
    }
}

void Philosopher::takeForks2()
{
    if (number < ForkManager::number - 1)
    {
        while (leftFork == NULL || rightFork == NULL)
        {

            while (leftFork == NULL)
            {
                mtx.lock();
                state = "taking left Fork";
                mtx.unlock();
                workSimulation(rand() % 15 + 20);
                mtx.lock();
                state = "waiting left Fork";
                mtx.unlock();
                ForkManager::getFork2(number, leftFork, number);
            }
            while (rightFork == NULL)
            {
                mtx.lock();
                state = "taking right Fork";
                mtx.unlock();
                workSimulation(rand() % 15 + 20);
                mtx.lock();
                state = "waiting right Fork";
                mtx.unlock();
                ForkManager::getFork2(number + 1, rightFork, number);
                if (rightFork == NULL)
                {
                    mtx.lock();
                    state = "release left Fork";
                    mtx.unlock();
                    workSimulation(20);
                    ForkManager::releaseFork(leftFork);
                }
            }
        }
    }
    else
    {
        while (leftFork == NULL || rightFork == NULL)
        {

            while (rightFork == NULL)
            {
                mtx.lock();
                state = "taking right Fork";
                mtx.unlock();
                workSimulation(rand() % 15 + 20);
                mtx.lock();
                state = "waiting right Fork";
                mtx.unlock();
                ForkManager::getFork2(0, rightFork, number);
            }
            while (leftFork == NULL)
            {
                mtx.lock();
                state = "taking left Fork";
                mtx.unlock();
                workSimulation(rand() % 15 + 20);
                mtx.lock();
                state = "waiting left Fork";
                mtx.unlock();
                ForkManager::getFork2(number, leftFork, number);
                if (leftFork == NULL)
                {
                    mtx.lock();
                    state = "release right Fork";
                    mtx.unlock();
                    workSimulation(20);
                    ForkManager::releaseFork(rightFork);
                }
            }
        }
    }
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
    workSimulation(rand() % 15 + 20);
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
    workSimulation(rand() % 15 + 20);
    ForkManager::releaseFork(number, leftFork, rightFork);
}

void Philosopher::releaseForks()
{
    if (number < ForkManager::number - 1)
    {
        mtx.lock();
        state = "release right Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        ForkManager::releaseFork(rightFork);
        mtx.lock();
        state = "release left Fork";
        mtx.unlock();
        //workSimulation(rand() % 15 + 20);
        ForkManager::releaseFork(leftFork);
    }
    else
    {
        mtx.lock();
        state = "release left Fork";
        mtx.unlock();
        workSimulation(rand() % 15 + 20);
        ForkManager::releaseFork(leftFork);
        mtx.lock();
        state = "release right Fork";
        mtx.unlock();
        //workSimulation(rand() % 15 + 20);
        ForkManager::releaseFork(rightFork);
    }
}

void Philosopher::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(5));
        this->mtx.lock();
        progress = float(i * 100) / times;
        this->mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(5));
    }
    this->mtx.lock();
    progress = 0;
    this->mtx.unlock();
}
