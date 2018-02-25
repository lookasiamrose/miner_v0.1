#include "mainwindow.h"
#include "globals.h"
#include "miner.h"
#include "connectionmanager.h"

#include <QApplication>
#include <QDebug>

#include <Windows.h>

int main(int argc, char *argv[])
{
    Globals globals;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    globals.noOfThreads = sysInfo.dwNumberOfProcessors;

    globals.threadsHashrates = (double *) calloc( globals.noOfThreads, sizeof(double));
    //own








    //Qt initialize

    QApplication a(argc, argv);
    MainWindow w;

    Miner* miner = new Miner();
    ConnectionManager* connManager = new ConnectionManager(&globals);

    w.show();

    return a.exec();
}
