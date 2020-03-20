#include <mutex>
#include <iostream>
#include <condition_variable>
using namespace std;
class Fork
{
public:
    int number;
    bool isBusy;
    int nrPhilosopher;
    int dirt;
    mutex mtx;
    mutex cvMtx;
    unique_lock<mutex> *ulm;
    condition_variable cv;

    Fork();
    Fork(int);
};