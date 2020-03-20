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
    if (forkList.at(lFork)->dirt >= 100000)
    {
        forkList.at(lFork)->dirt = 0;
    }
    if (forkList.at(rFork)->dirt >= 100000)
    {
        forkList.at(rFork)->dirt = 0;
    }
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

void ForkManager::getFork(int lFork, Fork *&leftFork, Fork *&rightFork)
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

void ForkManager::getFork(int nFork, Fork *&fork, int phNumber)
{
    forkList.at(nFork)->cv.wait(*(forkList.at(nFork)->ulm), [&nFork]() {
        return !forkList.at(nFork)->isBusy;
    });
    forkList.at(nFork)->mtx.lock();
    forkList.at(nFork)->nrPhilosopher = phNumber;
    forkList.at(nFork)->isBusy = true;
    fork = forkList.at(nFork);
    forkList.at(nFork)->mtx.unlock();
    return;
}

void ForkManager::releaseFork(int lFork, Fork *&leftFork, Fork *&rightFork)
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

void ForkManager::releaseFork(Fork *&leftFork)
{
    if (leftFork == NULL)
    {
        return;
    }
    leftFork->mtx.lock();
    leftFork->isBusy = false;
    leftFork->nrPhilosopher = -1;
    leftFork->mtx.unlock();
    leftFork->cv.notify_one();
    leftFork = NULL;
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