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
    static volatile int managerOff;
    static mutex mtx;

    ForkManager();

    static bool canEat(int);
    static void genForklist(int);
    static void getFork(int, Fork *&, Fork *&);
    static void getFork(int, Fork *&, int);
    static void getFork2(int, Fork *&, int);
    static void getFork3(int, Fork *&, int);
    static int getForkNumber(int);
    static void releaseFork(int, Fork *&, Fork *&);
    static void releaseFork(Fork *&);
    static int checkForks();
    static void workSimulation(int);
};
