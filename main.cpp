#include "stdarg.h"
#include "stddef.h"
#include "ncurses.h"
#include <cstring>
#include "Philosopher.h"

using namespace std;

mutex mtx;
volatile bool *isRunning = new bool(true);
bool killTreads = false;
volatile int ForkManager::number = 0;
mutex ForkManager::mtx;
volatile bool *ForkManager::isRunning = isRunning;
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

// static void test()
// {
//     for (int i = 0; i < 10000; i++)
//     {
//         cout << this_thread::get_id() << endl;
//     }
// }

int main(int argc, char **argv)
{
    system("export TERM=xterm-256color");
    srand(time(NULL));
    int philosopherNumber = 6;
    if (argc > 1)
    {
        string tmp = argv[1];
        philosopherNumber = stoi(tmp); // ta linijka kodu czasem wywala "segmentation fault", kiedy wykorzystać "atoi(argv[1])", błęd pojawia się częściej
        cout << "this error ,perhaps, because of \"stoi()\" function" << endl;
        cout << "ten błąd z powowdu funkcji \"stoi()\"" << endl;
    }
    ForkManager::genForklist(philosopherNumber);
    ForkManager::isRunning = isRunning;
    vector<Philosopher *> philosophers;
    vector<thread *> threads;

    for (int i = 0; i < philosopherNumber; i++)
    {
        mtx.lock();
        cout << "start1-" << i << endl;
        philosophers.push_back(new Philosopher(i));
        cout << "start2-" << i << endl;
        //this_thread::sleep_for(chrono::milliseconds(1000));
        cout << "start3-" << i << endl;
        threads.push_back(new thread([&philosophers, i]() {
            cout << "start4-" << i << endl;
            philosophers.at(i)->run();
        }));
        cout << "start5-" << i << endl;
        mtx.unlock();
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "done" << endl;
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
    int vOffset = 5;
    int hOffset = 3;
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
            forkIsBusy.push_back((ForkManager::forkList.at(i)->isBusy) ? "true" : "false");
            ForkManager::forkList.at(i)->mtx.unlock();
        }
        if (fps >= INT32_MAX)
        {
            fps = 0;
        }
        fps++;
        clear();

        attron(COLOR_PAIR(1));
        for (int i = hOffset; i < row - hOffset; i++)
        {
            mvhline(i, 5, ' ', col - 10);
        }

        mvwaddstr(stdscr, 3, (col - help1.length()) / 2, help1.c_str());
        mvwaddstr(stdscr, 4, (col - help2.length()) / 2, help2.c_str());
        mvwaddstr(stdscr, 6, vOffset + 1, "Philosopher");
        mvwaddstr(stdscr, 6, vOffset + 15, "State");
        mvwaddstr(stdscr, 6, vOffset + 35, "Progress");
        mvwaddstr(stdscr, 6, vOffset + 45, "Food");
        for (int i = 0; i <= (philosopherNumber - 1) / 5; i++)
        {
            mvwaddstr(stdscr, 9 + philosopherNumber + i * 2, vOffset + 1, "Fork number");
            mvwaddstr(stdscr, 10 + philosopherNumber + i * 2, vOffset + 1, "is busy");
        }
        mvwaddstr(stdscr, row - 4, col - 15, to_string(fps).c_str());

        for (int i = 0; i < philosopherNumber; i++)
        {
            attron(COLOR_PAIR(1));
            mvwaddstr(stdscr, 8 + i, vOffset + 3, phNumber.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, vOffset + 15, phState.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, vOffset + 38, phProgress.at(i).c_str());
            mvwaddstr(stdscr, 8 + i, vOffset + 45, phFood.at(i).c_str());
            if (!strcmp(forkIsBusy.at(i).c_str(), "false"))
            {
                attron(COLOR_PAIR(2));
            }
            else
            {
                attron(COLOR_PAIR(3));
            }
            mvwaddstr(stdscr, 9 + philosopherNumber + (i / 5) * 2, vOffset + 15 + i % 5 * 7, to_string(i).c_str());
            mvwaddstr(stdscr, 10 + philosopherNumber + (i / 5) * 2, vOffset + 15 + i % 5 * 7, forkIsBusy[i].c_str());
        }
        attron(COLOR_PAIR(4));
        for (int i = 0; i < hOffset; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = row - hOffset; i < row; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = 0; i < vOffset; i++)
        {
            mvvline(hOffset, i, ' ', row - hOffset);
        }
        for (int i = col - vOffset; i < col; i++)
        {
            mvvline(hOffset, i, ' ', row - hOffset);
        }
        refresh();
        this_thread::sleep_for(chrono::milliseconds(50));
        phNumber.clear();
        phState.clear();
        phProgress.clear();
        forkIsBusy.clear();
        phFood.clear();
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
    for (int i = 0; i < philosopherNumber; i++)
    {
        cout << "waiting for " << threads.at(i)->get_id() << "... ";
        threads.at(i)->join();
        cout << "successful" << endl;
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
