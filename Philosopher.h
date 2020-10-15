#include <math.h>
#include "ForkManager.h"
using namespace std;

class Philosopher
{
public:
    volatile int number;
    string state;
    int progress;
    int food;
    mutex mtx;
    Fork *leftFork;
    Fork *rightFork;

    Philosopher();
    Philosopher(const Philosopher &);
    Philosopher(int);

    void run();
    void eat();
    void think();
    void takeFork();
    void takeForks();
    void takeForks2();
    void takeForks3();
    void releaseFork();
    void releaseForks();
    void workSimulation(int);
};