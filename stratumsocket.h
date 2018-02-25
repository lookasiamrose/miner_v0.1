#ifndef STRATUMSOCKET_H
#define STRATUMSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <globals.h>

class StratumSocket : public QObject
{
    Q_OBJECT
protected:
    enum class States{ INITIALIZATION, AUTHORIZATION, SUBSCRIPTION, WORK, EXTRANONCE};
private:
    QTcpSocket* socket;
    QDebug outputStream;
    Globals* globalsRef;

    QString address;
    QString port;
    QString timeout;
    States state;
public:
    explicit StratumSocket(Globals* globals, QObject *parent = 0);
    bool sendString(QString line);
    QString receiveString();
    QByteArray hex2bit_deprecated(QString blobString);
    QByteArray hex2bin(QString blobString);
    QByteArray jsonToBinary(QJsonObject job, int bytes);
signals:

public slots:

protected:
    void socketInit();
    bool checkBlobLength(QString blobString);
private slots:
    void connected();
    void disconnected();
    void readyRead();
    void errorOccured(QAbstractSocket::SocketError socketError);
};

#endif // STRATUMSOCKET_H
