#include "stdarg.h"
#include "stddef.h"
#include "ncurses.h"
#include <cstring>
#include <string>
#include "Philosopher.h"

using namespace std;

mutex mtx;
volatile bool *isRunning = new bool(true);
bool killTreads = false;
volatile int ForkManager::number = 0;
mutex ForkManager::mtx;
volatile bool *ForkManager::isRunning = isRunning;
volatile int ForkManager::managerOff = 0;
vector<Fork *> ForkManager::forkList = vector<Fork *>();
void keyboardFunc()
{
    int key = 0;
    do
    {
        key = getch();
        if (key == 'Q')
        {
            killTreads = true;
        }
        //std::terminate();
    } while (key != 27 && !killTreads);
    *isRunning = false;
}

int main(int argc, char **argv)
{
    int philosopherNumber = 5;
    string consoleHelp = "Skladnia urochominia programu:\nNazwa [tup rozwiązania] [liczba filozofow]\n typy rozwiązania:\n1 - za pomocą kelnera\n2 - prosta realizacja z hierarchia zasobow\n3 - hierarchia zasobów z wyzwalieneim zasobo przez jakis czs";
    if (argc > 2)
    {
        philosopherNumber = atoi(argv[2]); // ta linijka kodu czasem wywala "segmentation fault", kiedy wykorzystać "atoi(argv[1])", błęd pojawia się częściej
        if (philosopherNumber == 0)
        {
            cout << "Bład parametrów. 2-i parametr nie jest liczbą" << endl;
            cout << consoleHelp << endl;
            return 0;
        }
    }
    if (argc > 1)
    {
        ForkManager::managerOff = atoi(argv[1]); // ta linijka kodu czasem wywala "segmentation fault", kiedy wykorzystać "atoi(argv[1])", błęd pojawia się częściej
        if (ForkManager::managerOff == 0)
        {
            cout << "Bład parametrów. 1-y parametr nie jest liczbą" << endl;
            cout << consoleHelp << endl;
            return 0;
        }
    }
    system("export TERM=xterm-256color");
    srand(time(NULL));
    ForkManager::genForklist(philosopherNumber);
    ForkManager::isRunning = isRunning;
    vector<Philosopher *> philosophers;
    vector<thread *> threads;

    for (int i = 0; i < philosopherNumber; i++)
    {
        mtx.lock();
        philosophers.push_back(new Philosopher(i));
        threads.push_back(new thread([&philosophers, i]() {
            cout << "Philosopher number: " << i << " thread: " << this_thread::get_id() << " was started...";
            philosophers.at(i)->run();
        }));
        mtx.unlock();
    }
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "done" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    int row, col;
    initscr();
    noecho();
    thread keyboardThread(keyboardFunc);
    raw();
    string help1 = "use ESC to exit;";
    string help2 = "use Shift+Q to emergency stop;";
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    vector<string> phNumber;
    vector<string> phState;
    vector<string> phProgress;
    vector<string> forkIsBusy;
    vector<string> phFood;
    vector<string> phNumberFork;
    vector<string> forkDirt;
    int hOffset = 5;
    int vOffset = 3;
    int fps = 0;

    do
    {
        getmaxyx(stdscr, row, col);
        for (int i = 0; i < philosopherNumber; i++)
        {

            philosophers.at(i)->mtx.lock();
            phNumber.push_back(to_string(philosophers.at(i)->number));
            phState.push_back(philosophers.at(i)->state);
            phProgress.push_back(to_string(philosophers.at(i)->progress) + "%");
            phFood.push_back(to_string(philosophers.at(i)->food));
            philosophers.at(i)->mtx.unlock();

            ForkManager::forkList.at(i)->mtx.lock();
            forkIsBusy.push_back((!ForkManager::forkList.at(i)->isBusy) ? "true" : "false");
            phNumberFork.push_back(to_string(ForkManager::forkList.at(i)->nrPhilosopher));
            forkDirt.push_back(to_string(ForkManager::forkList.at(i)->dirt));
            ForkManager::forkList.at(i)->mtx.unlock();
        }
        if (fps >= INT32_MAX)
        {
            fps = 0;
        }
        fps++;
        clear();

        attron(COLOR_PAIR(1));
        for (int i = vOffset; i < row - vOffset; i++)
        {
            mvhline(i, 5, ' ', col - 10);
        }

        mvhline(10 + philosopherNumber, hOffset + 1, '-', col - vOffset - 9);
        if (fps % 40 > 10)
        {
            mvwaddstr(stdscr, 3, (col - help1.length()) / 2, help1.c_str());
            mvwaddstr(stdscr, 4, (col - help2.length()) / 2, help2.c_str());
        }
        mvwaddstr(stdscr, 6, hOffset + 2, "Philosopher");
        mvwaddstr(stdscr, 6, hOffset + 15, "State");
        mvwaddstr(stdscr, 6, hOffset + 35, "Progress");
        mvwaddstr(stdscr, 6, hOffset + 45, "Eaten food");
        for (int i = 0; i <= (philosopherNumber - 1) / 5; i++)
        {
            mvwaddstr(stdscr, 11 + philosopherNumber + i * 5, hOffset + 2, "Fork number:");
            mvwaddstr(stdscr, 12 + philosopherNumber + i * 5, hOffset + 2, "is free:");
            mvwaddstr(stdscr, 13 + philosopherNumber + i * 5, hOffset + 2, "philosopher:");
            mvwaddstr(stdscr, 14 + philosopherNumber + i * 5, hOffset + 2, "dirt:");
        }
        mvwaddstr(stdscr, row - 4, col - 15, to_string(fps).c_str());
        for (int i = 0; i < philosopherNumber; i++)
        {
            attron(COLOR_PAIR(1));
            mvwaddstr(stdscr, 8 + i, hOffset + 4, phNumber.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, hOffset + 15, phState.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, hOffset + 38, phProgress.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, hOffset + 50, phFood.at(i).c_str());
            mvwaddstr(stdscr, 11 + philosopherNumber + (i / 5) * 5, hOffset + 15 + i % 5 * 7, to_string(i).c_str());
            mvwaddstr(stdscr, 14 + philosopherNumber + (i / 5) * 5, hOffset + 15 + i % 5 * 7, forkDirt.at(i).c_str());
            if (!strcmp(forkIsBusy.at(i).c_str(), "false"))
            {
                attron(COLOR_PAIR(3));
            }
            else
            {
                attron(COLOR_PAIR(2));
            }
            mvwaddstr(stdscr, 12 + philosopherNumber + (i / 5) * 5, hOffset + 15 + i % 5 * 7, forkIsBusy[i].c_str());
            mvwaddstr(stdscr, 13 + philosopherNumber + (i / 5) * 5, hOffset + 15 + i % 5 * 7, phNumberFork[i].c_str());
        }

        attron(COLOR_PAIR(4));
        for (int i = 0; i < vOffset; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = row - vOffset; i < row; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = 0; i < hOffset; i++)
        {
            mvvline(vOffset, i, ' ', row - hOffset);
        }
        for (int i = col - hOffset; i < col; i++)
        {
            mvvline(vOffset, i, ' ', row - hOffset);
        }
        refresh();
        this_thread::sleep_for(chrono::milliseconds(50));
        phNumber.clear();
        phState.clear();
        phProgress.clear();
        forkIsBusy.clear();
        phFood.clear();
        phNumberFork.clear();
        forkDirt.clear();
    } while (*isRunning);

    //keyboardThread.~thread();
    clear();
    endwin();
    system("clear");
    system("export TERM=xterm-256color");
    keyboardThread.join();
    if (killTreads)
    {
        for (int i = 0; i < philosopherNumber; i++)
        {
            threads.at(i)->~thread();
        }
    }
    cout << "waiting for threads:" << endl;
    vector<thread *> joinPhilosophers;
    for (int i = 0; i < philosopherNumber; i++)
    {
        joinPhilosophers.push_back(new thread([&threads, i]() {
            cout << "waiting for " << threads.at(i)->get_id() << "(" << i << ")"
                 << "... ";
            threads.at(i)->join();
            cout << i << " successful" << endl;
        }));
    }
    for (int i = 0; i < philosopherNumber; i++)
    {
        joinPhilosophers.at(i)->join();
    }
    cout << "removing objects" << endl;
    for (int i = 0; i < philosopherNumber; i++)
    {
        delete ForkManager::forkList.at(i);
        delete philosophers.at(i);
        delete threads.at(i);
    }
    delete isRunning;
    cout << "close" << endl;
    return 0;
}
