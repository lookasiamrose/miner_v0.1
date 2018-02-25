#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <stdio.h>
#include <WinSock2.h>
#include <mstcpip.h> //include after winsock2.h
#include "curl/curl.h"
#include "globals.h"
#include "stratumsocket.h"

class ConnectionManager : public QObject
{
    Q_OBJECT
private:
    Globals* globalsRef;
    StratumSocket* str;
    class LowerLevel_MyWinSock
    {
    public:
        std::string sockBuffer;

        static bool send_string(curl_socket_t sock, std::string line);
        static std::string receive_string(curl_socket_t sock, int timeout);
    };
private:
    CURL* curl;
    char* curlErrorString[CURL_ERROR_SIZE]; //at least CURL_ERROR_SIZE
    curl_socket_t socket;
public:
    explicit ConnectionManager(Globals* globals, QObject *parent = 0);

    bool connectStratum(QString urlArg);
    void subscribeStratum();
    bool authorizeStratum(QString login, QString pass);
    void disconnectStratum();
public:
    QString version;
signals:

public slots:
};

static int sockopt_keepalive_cb(void *userdata, curl_socket_t fd, curlsocktype purpose);
static curl_socket_t opensocket_grab_cb(void *clientp, curlsocktype purpose, struct curl_sockaddr *addr);


struct FtpFile {
  const char *filename;
  FILE *stream;
};
size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);

#endif // CONNECTIONMANAGER_H
