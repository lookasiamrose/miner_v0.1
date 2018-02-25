#include "connectionmanager.h"
#include <QDebug>

ConnectionManager::ConnectionManager(Globals* globals, QObject *parent) : QObject(parent)
{
    version = "cpuminer-opt/3.7.9";

    globalsRef = globals;

    str = new StratumSocket(globals, this);

    //connectStratum("http://europe.cryptonight-hub.miningpoolhub.com:20596");
    //subscribeStratum();
    //authorizeStratum("Guillotine.lukasio8","2991105c");
    //disconnectStratum();
}
static int sockopt_keepalive_cb(void *userdata, curl_socket_t fd, curlsocktype purpose)
{
    int tcp_keepintvl = 50; //50s
    int tcp_keepidle = 50; //50s

    struct tcp_keepalive vals;
    vals.onoff = 1;
    vals.keepalivetime = tcp_keepidle * 1000;
    vals.keepaliveinterval = tcp_keepintvl * 1000;

    DWORD outputBytes;
    if (WSAIoctl(fd, SIO_KEEPALIVE_VALS, &vals, sizeof(vals), NULL, 0, &outputBytes, NULL, NULL))
    { //setting keepalive protocol values
        return 1;
    }

    return 0;
}
static curl_socket_t opensocket_grab_cb(void *clientp, curlsocktype purpose, struct curl_sockaddr *addr)
{ //default
    curl_socket_t *sock = (curl_socket_t*) clientp;
    *sock = socket(addr->family, addr->socktype, addr->protocol);
    return *sock;
}
bool ConnectionManager::connectStratum(QString urlArg)
{
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    int perform;

    curl = curl_easy_init();
    if(curl){
        /* timeout change from 300 (default) to 30
         * TCP_NODELAY - disable Nagle algorithm, allow smallest packets to be send
         * OPENSOCKETDATA - this function is the first param of the opensocketfunction
         * CONNECT_ONLY - just connect, nothing else - testing purpose
         *
         *
         */
        res = curl_easy_setopt(curl, CURLOPT_URL, urlArg.toStdString().c_str());
        qDebug()<<"CURLOPT_URL "<<res;
        res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
        qDebug()<<"CURLOPT_CONNECTTIMEOUT "<<res;
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorString);
        qDebug()<<"CURLOPT_ERRORBUFFER "<<res;
        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        qDebug()<<"CURLOPT_VERBOSE "<<res;
        res = curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
        qDebug()<<"CURLOPT_TCP_NODELAY "<<res;
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_keepalive_cb);
        qDebug()<<"CURLOPT_SOCKOPTFUNCTION "<<res;
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket_grab_cb);
        qDebug()<<"CURLOPT_OPENSOCKETFUNCTION "<<res;
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &socket);
        qDebug()<<"CURLOPT_OPENSOCKETDATA "<<res;
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1);
        qDebug()<<"CURLOPT_CONNECT_ONLY "<<res;

        perform = curl_easy_perform(curl);
        if(perform)
        {
            qDebug()<<"Stratum connection method failed: "<<curlErrorString;
            curl_easy_cleanup(curl);
            curl = NULL;
            return false;
        }
    }else
    {
        qDebug()<<"Stratum connection method failed: "<<"CURL initialization failed";
        return false;
    }
    return true;
}
void ConnectionManager::subscribeStratum()
{

}
bool ConnectionManager::authorizeStratum(QString login, QString pass)
{
    QString authorizeTemplate;

    authorizeTemplate = ("{\"method\":\"login\",\"params\":{\"login\":\"<login>\",\"pass\":\"<password>\",\"agent\":\"<software>\"},\"id\":1}");

    authorizeTemplate.replace("<login>",login);
    authorizeTemplate.replace("<password>", pass);
    authorizeTemplate.replace("<software>", version);

    LowerLevel_MyWinSock::send_string(socket,authorizeTemplate.toStdString());

    QString ret = QString::fromStdString(LowerLevel_MyWinSock::receive_string(socket,30));

    qDebug()<<ret;

    return true;
}
void ConnectionManager::disconnectStratum()
{
    if(curl)
    {
        curl_easy_cleanup(curl);
        curl = NULL;
        qDebug()<<"Stratum disconnected";
    }
}
size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out = (struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
bool ConnectionManager::LowerLevel_MyWinSock::send_string(curl_socket_t sock, std::string line)
{
    line.append("\n");
    int left = (int) line.size();
    size_t sentPointer = 0;
    int sessionSent;

    fd_set wd; //zestaw deskryptorów do zapisu
    while(left > 0)
    {
        struct timeval timeout = {0, 0}; //to erase

        FD_ZERO(&wd); //clear
        FD_SET(sock, &wd); //przypisz deskryptor sock od zestawu
        int res = select((int) (sock + 1), NULL, &wd, NULL, &timeout); //no of descriptors (sock+1), read des, write des, exception des SETS, timeout {s, us}
        if(res < 1)
        {
            qDebug()<<"ERROR: "<<"Winsock2::select failed in ConnectionManagerClass. WSAGetLastError():"<<WSAGetLastError();
            return false;
        }
        sessionSent = send(sock, &line.at(sentPointer), left, 0);
        if(sessionSent < 0)
        {
            if(WSAGetLastError() != WSAEWOULDBLOCK)
            {
                qDebug()<<"ERROR: "<<"Winsock2::send failed in ConnectionManagerClass. WSAGetLastError():"<<WSAGetLastError();
                return false;
            }
            sessionSent = 0;
        }

        sentPointer += sessionSent;
        left -= sessionSent;
    }


    return true;
}
std::string ConnectionManager::LowerLevel_MyWinSock::receive_string(curl_socket_t sock, int timeout)
{
    std::string line;

    struct timeval tv = {timeout, 0};
    fd_set rd;

    FD_ZERO(&rd);
    FD_SET(sock, &rd);
    int ret = select((int)(sock + 1), &rd, NULL, NULL, &tv) > 0;
    if(ret < 1)
    {
        qDebug()<<"ERROR: "<<"Winsock2::select failed in ConnectionManagerClass. WSAGetLastError():"<<WSAGetLastError();
        return std::string("");
    }
    //
    time_t rstart;
    time(&rstart);
    do {
        char s[2048];
        int n;

        memset(s, 0, 2048);
        n = recv(sock, s, 2044, 0);
        if (n < 1)
        {
            qDebug()<<"ERROR: "<<"Winsock2::recv failed in ConnectionManagerClass. WSAGetLastError():"<<WSAGetLastError();
            return std::string("");
        }
        line.append(s,strlen(s));

    }while(time(NULL) - rstart < 60 && (std::string::npos == line.find("\n")));
    //

    return line;
}






























