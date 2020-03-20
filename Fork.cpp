#include "Fork.h"

Fork::Fork() {}
Fork::Fork(int nr)
{
    this->number = nr;
    this->isBusy = false;
    this->nrPhilosopher = -1;
    this->dirt = 0;
    ulm = new unique_lock<mutex>(cvMtx);
}