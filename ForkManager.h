#include <chrono>
#include <vector>
#include <thread>
#include "Fork.h"
using namespace std;

class ForkManager
{
public:
    static volatile int number;
    static vector<Fork *> forkList;
    static volatile bool *isRunning;
    static mutex mtx;

    ForkManager();

    static bool canEat(int);
    static void genForklist(int);
    static void getFork(int, Fork *, Fork *);
    static void releaseFork(int, Fork *, Fork *);
    static int checkForks();
    static void workSimulation(int);
};
