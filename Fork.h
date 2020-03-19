#include <mutex>
#include <iostream>
using namespace std;
class Fork
{
public:
    int number;
    bool isBusy;
    int nrPhilosopher;
    int dirt;
    mutex mtx;

    Fork();
    Fork(int);
};