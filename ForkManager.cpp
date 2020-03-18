#include "ForkManager.h"

ForkManager::ForkManager()
{
}

bool ForkManager::canEat(int lFork)
{
    int rFork = lFork + 1;
    if (lFork >= number - 1)
    {
        rFork = 0;
    }
    forkList.at(lFork)->mtx.lock();
    forkList.at(rFork)->mtx.lock();
    if (!forkList.at(lFork)->isBusy && !forkList.at(rFork)->isBusy)
    {
        forkList.at(lFork)->isBusy = true;
        forkList.at(lFork)->nrPhilosopher = lFork;
        forkList.at(rFork)->isBusy = true;
        forkList.at(rFork)->nrPhilosopher = lFork;
        forkList.at(lFork)->mtx.unlock();
        forkList.at(rFork)->mtx.unlock();
        return true;
    }
    forkList.at(lFork)->mtx.unlock();
    forkList.at(rFork)->mtx.unlock();
    return false;
}

void ForkManager::genForklist(int n)
{
    if (n == 1)
    {
        n = 2;
    }
    number = n;
    for (int i = 0; i < n; i++)
    {
        forkList.push_back(new Fork(i));
    }
}

void ForkManager::getFork(int lFork, Fork *leftFork, Fork *rightFork)
{
    int rFork = lFork + 1;
    if (lFork >= number - 1)
    {
        rFork = 0;
    }
    forkList.at(lFork)->mtx.lock();
    forkList.at(rFork)->mtx.lock();
    if (forkList.at(lFork)->nrPhilosopher == lFork && forkList.at(rFork)->nrPhilosopher == lFork)
    {
        leftFork = forkList.at(lFork);
        rightFork = forkList.at(rFork);
        forkList.at(lFork)->mtx.unlock();
        forkList.at(rFork)->mtx.unlock();
        return;
    }
    forkList.at(lFork)->mtx.unlock();
    forkList.at(rFork)->mtx.unlock();
    return;
}

void ForkManager::releaseFork(int lFork, Fork *leftFork, Fork *rightFork)
{
    int rFork = lFork + 1;
    if (lFork >= number - 1)
    {
        rFork = 0;
    }
    forkList.at(lFork)->mtx.lock();
    forkList.at(rFork)->mtx.lock();
    if (forkList.at(lFork)->nrPhilosopher == lFork && forkList.at(rFork)->nrPhilosopher == lFork)
    {
        leftFork = NULL;
        rightFork = NULL;
        forkList.at(lFork)->isBusy = false;
        forkList.at(lFork)->nrPhilosopher = -1;
        forkList.at(rFork)->isBusy = false;
        forkList.at(rFork)->nrPhilosopher = -1;
        forkList.at(lFork)->mtx.unlock();
        forkList.at(rFork)->mtx.unlock();
        return;
    }
    forkList.at(lFork)->mtx.unlock();
    forkList.at(rFork)->mtx.unlock();
    return;
}
int ForkManager::checkForks()
{
    int n = 0;
    for (int i = 0; i < number; i++)
    {
        forkList.at(i)->mtx.lock();
        if (forkList.at(i)->isBusy)
        {
            n++;
        }
        forkList.at(i)->mtx.unlock();
    }

    return n;
}

void ForkManager::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}