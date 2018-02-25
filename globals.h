#ifndef GLOBALS_H
#define GLOBALS_H

#include <QByteArray>
#include <QString>

class Globals
{
public:
    Globals();
    class Work {
    public:
        quint32 data[48];
        quint32 target[8];
        QString jobId;
    };
    Work work;
    QByteArray rpc2Blob;
    quint32 rpc2Target;
    QString rpc2JobId;
    double stratumDiff;

    quint8 noOfThreads;
    double* threadsHashrates;
};

#endif // GLOBALS_H
