#include "stratumsocket.h"
#include <QDataStream>

void StratumSocket::socketInit()
{
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
      connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
      connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
      connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(errorOccured(QAbstractSocket::SocketError)));
}

StratumSocket::StratumSocket(Globals* globals, QObject *parent) : QObject(parent) , outputStream(qDebug())
{
    globalsRef = globals;

    state = States::INITIALIZATION;
    address = QString("europe.cryptonight-hub.miningpoolhub.com");
    port = QString("20596");
    timeout = QString("30");

    //address = QString("127.0.0.1");
    //port = QString("21");

    socket = new QTcpSocket(this);

    socketInit();
    socket->connectToHost(address, port.toInt());

    readyRead();
}
void StratumSocket::errorOccured(QAbstractSocket::SocketError socketError)
{
    outputStream<<"StratumSocket: error occured: "<<socketError;
}
bool StratumSocket::sendString(QString line)
{
    socket->write(line.toLatin1());

    if(!socket->flush())
    {
        outputStream<<"StratumSocket: couldn't send the line.";
        return false;
    }

    return true;
}
QString StratumSocket::receiveString()
{
    return true;
}
void StratumSocket::connected()
{
    if(state == States::INITIALIZATION)
    {
        QString authorizeTemplate;
        authorizeTemplate = ("{\"method\":\"login\",\"params\":{\"login\":\"<login>\",\"pass\":\"<password>\",\"agent\":\"<software>\"},\"id\":1}\n");

        authorizeTemplate.replace("<login>","Guillotine.lukasio8");
        authorizeTemplate.replace("<password>", "2991105c");
        authorizeTemplate.replace("<software>", "cpuminer-opt/3.7.9");
        state = States::AUTHORIZATION;
        sendString(authorizeTemplate);
    }
}
void StratumSocket::disconnected()
{
    outputStream<<"StratumSocket: disconnected";
    state = States::INITIALIZATION;

    while(true)
    {
        socket->connectToHost(address, port.toInt());

        if(!socket->waitForConnected(timeout.toInt()))
        {
            outputStream<<"StratumSocket: timeout (5s): retrying...";
        }else
        {
            outputStream<<"StratumSocket: reconnected successfully.";
            break;
        }
    }
}
bool StratumSocket::checkBlobLength(QString blobString)
{
    int blobLength = blobString.size();
    if (blobLength % 2 != 0 || ((blobLength / 2) < 40 && blobLength != 0) || (blobLength / 2) > 128) {
       return false;
    }
    return true;
}
QByteArray StratumSocket::hex2bit_deprecated(QString blobString)
{
    QStringList hexs;
    for(int i=0; i<blobString.size(); i+=2)
    {
        QString temp;
        temp.append(blobString.at(i));
        temp.append(blobString.at(i+1));
        hexs.append(temp);
    }
    bool ok;
    QByteArray binBlob( nullptr, blobString.size());
    int i = 0;
    foreach (QString hexString, hexs) {
        binBlob.insert(i++,hexString.toInt(&ok, 16));
        if(!ok)
        {
            outputStream<<"blob conversion from hex to bin failed";
            break;
        }
    }
    return binBlob;
}
QByteArray StratumSocket::hex2bin(QString hexString)
{
    QByteArray byteArrayOfHexString = QByteArray::fromHex(hexString.toLatin1());

    return byteArrayOfHexString;
}
QByteArray StratumSocket::jsonToBinary(QJsonObject job, int bytes)
{
    QJsonValue targetJson = job.value("target");
    QString targetString = targetJson.toString();
    targetString = targetString.left(bytes*2);

    return hex2bin(targetString);
}

void StratumSocket::readyRead()
{
    QByteArray jsonData = socket->readAll();


    if(jsonData.endsWith('\n'))
    {
        jsonData.remove(jsonData.length()-1,1);
    }else{
        outputStream<<"StratumSocket: incorrect data received.";
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject main = document.object();

    if(state == States::AUTHORIZATION)
    {
        /*jsonData.clear();
        jsonData.push_back("{\"id\":1,\"jsonrpc\":\"2.0\",\"error\":null,\"result\":{\"id\":\"530657290760427\",\"job\":{\"blob\":\"0101c69aa3d4057406b9accbbd9b87e03d4bddb42a3927a6aa4277010a72de7ad470a2aa242f4e00000000346705e37636d4dc246edf1a95890253e6d73fc0b337b3f98ad4b8ab6e8dbc4303\",\"job_id\":\"870838806172832\",\"target\":\"6ea30100\"},\"status\":\"OK\"}}\n");*/
        QJsonObject result = main.value("result").toObject();
        QJsonObject job = result.value("job").toObject();

        QJsonValue job_id = job.value("job_id");

        QJsonValue blob = job.value("blob");
        QString blobString = blob.toString();

        outputStream<<"is blob okay? "<<checkBlobLength(blobString);
        outputStream<<"local blob: "<<blobString;

        QByteArray blobBin = hex2bin(blobString);
        globalsRef->rpc2Blob = blobBin;
        outputStream<<"local blobBinary: "<<blobBin.toHex();

        quint32 target;
        QByteArray targetBin = jsonToBinary(job, 4);
        outputStream<<"local targetHex: "<<targetBin.toHex();
        QDataStream targetStream(targetBin);
        targetStream>>target;
        outputStream<<"local target: "<<target;

        if(globalsRef->rpc2Target != target)
        {
            double hashrate = 0.0;
            for (int i = 0; i < globalsRef->noOfThreads; i++)
                hashrate += globalsRef->threadsHashrates[i];

            double diff = trunc( ( ((double)0xffffffff) / target ) ); //remove .xx

            globalsRef->rpc2Target = target;
            globalsRef->stratumDiff = diff;
            outputStream<<"global stratumDiff: "<<globalsRef->stratumDiff;
        }

        globalsRef->rpc2JobId = job_id.toString();
        outputStream<<"global job_id: "<<globalsRef->rpc2JobId;

        memcpy(globalsRef->work.data, globalsRef->rpc2Blob, globalsRef->rpc2Blob.size());
        memset(globalsRef->work.target, 0xff, sizeof(globalsRef->work.target));
        globalsRef->work.target[7] = globalsRef->rpc2Target;
        outputStream<<"global work.data: "<<globalsRef->work.data;
        outputStream<<"global work.target: "<<globalsRef->work.target;

        globalsRef->work.jobId = globalsRef->rpc2JobId;

        state = States::EXTRANONCE;
        sendString("{\"id\": 3, \"method\": \"mining.extranonce.subscribe\", \"params\": []}");
    }else if(state == States::EXTRANONCE)
    {
        QJsonValue id = main.value("id");
        int idInt = id.toInt();

        if(idInt != 3){
            outputStream<<"extranonce is supported!";
        }else{
            outputStream<<"std method for unsupported extranonce";
        }
    }

}

/*
struct work {
    uint32_t data[48];
    uint32_t target[8];

    double targetdiff;
    double shareratio;
    double sharediff;

    int height;
    char *txs;
    char *workid;

    char *job_id;
    size_t xnonce2_len;
    unsigned char *xnonce2;
        uint32_t nonces[8];
        bool     nfound[8];
};
struct stratum_job {
    char *job_id;
    unsigned char prevhash[32];
        unsigned char claim[32]; // lbry
    size_t coinbase_size;
    unsigned char *coinbase;
    unsigned char *xnonce2;
    int merkle_count;
    unsigned char **merkle;
    unsigned char version[4];
    unsigned char nbits[4];
    unsigned char ntime[4];
    bool clean;
    double diff;
};
*/




















































