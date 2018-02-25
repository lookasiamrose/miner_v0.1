#ifndef MINER_H
#define MINER_H

#include <QObject>
#include <QMap>
#include <QVector>

class Miner : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QString>* algoAliasMap;
    QVector<QString> algo_names;

    void initAlgoNames();
    void initSupportedAlgoAliases();
public:
    enum algos{ ALGO_NULL, ALGO_ARGON2, ALGO_AXIOM, ALGO_BASTION, ALGO_BLAKE, ALGO_BLAKECOIN, // ALGO_BLAKE2B,
                ALGO_BLAKE2S, ALGO_BMW, ALGO_C11, ALGO_CRYPTOLIGHT, ALGO_CRYPTONIGHT, ALGO_DECRED, ALGO_DEEP,
                ALGO_DMD_GR, ALGO_DROP, ALGO_FRESH, ALGO_GROESTL, ALGO_HEAVY, ALGO_HMQ1725, ALGO_HODL, ALGO_JHA,
                ALGO_KECCAK, ALGO_KECCAKC, ALGO_LBRY, ALGO_LUFFA, ALGO_LYRA2H, ALGO_LYRA2RE, ALGO_LYRA2REV2,
                ALGO_LYRA2Z, ALGO_LYRA2Z330, ALGO_M7M, ALGO_MYR_GR, ALGO_NEOSCRYPT, ALGO_NIST5, ALGO_PENTABLAKE,
                ALGO_PHI1612, ALGO_PLUCK, ALGO_POLYTIMOS, ALGO_QUARK, ALGO_QUBIT, ALGO_SCRYPT, ALGO_SCRYPTJANE,
                ALGO_SHA256D, ALGO_SHA256T, ALGO_SHAVITE3, ALGO_SKEIN, ALGO_SKEIN2, ALGO_SKUNK, ALGO_TIMETRAVEL,
                ALGO_TIMETRAVEL10, ALGO_TRIBUS, ALGO_VANILLA, ALGO_VELTOR, ALGO_WHIRLPOOL, ALGO_WHIRLPOOLX,
                ALGO_X11, ALGO_X11EVO, ALGO_X11GOST, ALGO_X13, ALGO_X13SM3, ALGO_X14, ALGO_X15, ALGO_X17,
                ALGO_XEVAN, ALGO_YESCRYPT, ALGO_YESCRYPTR8, ALGO_YESCRYPTR16, ALGO_ZR5, ALGO_COUNT};
public:
    explicit Miner(QObject *parent = 0);
    void mapoja_get_algo_alias( char** algo_or_alias );
    QString algo_name(enum algos arg);
    ~Miner();
signals:

public slots:
};

#endif // MINER_H
