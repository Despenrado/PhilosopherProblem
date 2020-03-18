#include <mutex>
#include <iostream>
using namespace std;
class Fork
{
public:
    int number;
    bool isBusy;
    int nrPhilosopher;
    mutex mtx;

    Fork();
    Fork(int);
};