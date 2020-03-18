#include "Fork.h"

Fork::Fork() {}
Fork::Fork(int nr)
{
    this->number = nr;
    this->isBusy = false;
    this->nrPhilosopher = -1;
}