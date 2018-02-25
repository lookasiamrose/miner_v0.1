#include "miner.h"
#include <string>
#include <QDebug>

Miner::Miner(QObject *parent) : QObject(parent)
{
    algoAliasMap = new QMap<QString, QString>();
    initSupportedAlgoAliases();

    initAlgoNames();

}
Miner::~Miner()
{
    delete algoAliasMap;
}
void Miner::initSupportedAlgoAliases()
{
    if(algoAliasMap == nullptr) algoAliasMap = new QMap<QString, QString>();
    //insert supported algos
    algoAliasMap->insert("bitcore",           "timetravel10" );
    algoAliasMap->insert("bitzeny",           "yescryptr8"   );
    algoAliasMap->insert("blake256r8",        "blakecoin"    );
    algoAliasMap->insert("blake256r8vnl",     "vanilla"      );
    algoAliasMap->insert("blake256r14",       "blake"        );
    algoAliasMap->insert("blake256r14dcr",    "decred"       );
    algoAliasMap->insert("cryptonote",        "cryptonight"  );
    algoAliasMap->insert("cryptonight-light", "cryptolight"  );
    algoAliasMap->insert("diamond",           "dmd-gr"       );
    algoAliasMap->insert("droplp",            "drop"         );
    algoAliasMap->insert("espers",            "hmq1725"      );
    algoAliasMap->insert("flax",              "c11"          );
    algoAliasMap->insert("hsr",               "x13sm3"       );
    algoAliasMap->insert("jackpot",           "jha"          );
    algoAliasMap->insert("jane",              "scryptjane"   );
    algoAliasMap->insert("lyra2",             "lyra2re"      );
    algoAliasMap->insert("lyra2v2",           "lyra2rev2"    );
    algoAliasMap->insert("lyra2zoin",         "lyra2z330"    );
    algoAliasMap->insert("myriad",            "myr-gr"       );
    algoAliasMap->insert("neo",               "neoscrypt"    );
    algoAliasMap->insert("phi",               "phi1612"      );
  //  algos->insert("sia",               "blake2b"      );
    algoAliasMap->insert("sib",               "x11gost"      );
    algoAliasMap->insert("timetravel8",       "timetravel"   );
    algoAliasMap->insert("ziftr",             "zr5"          );
    algoAliasMap->insert("yenten",            "yescryptr16"  );
    algoAliasMap->insert("yescryptr8k",       "yescrypt"     );
    algoAliasMap->insert("zcoin",             "lyra2z"       );
    algoAliasMap->insert("zoin",              "lyra2z330"    );
}
void Miner::mapoja_get_algo_alias( char** algo_or_alias )
{
   if(algoAliasMap->contains(QString(*algo_or_alias))){
        *algo_or_alias = strdup( algoAliasMap->value(QString(*algo_or_alias)).toStdString().c_str() );
        return;
    }
}
QString Miner::algo_name(enum algos arg)
{
    return algo_names.at(arg);
}
void Miner::initAlgoNames()
{
    algo_names.insert( ALGO_NULL, NULL);
    algo_names.insert( ALGO_ARGON2, "argon2");
    algo_names.insert( ALGO_AXIOM, "axiom");
    algo_names.insert( ALGO_BASTION, "bastion");
    algo_names.insert( ALGO_BLAKE, "blake");
    algo_names.insert( ALGO_BLAKECOIN, "blakecoin");
    //algo_names.insert( ALGO_BLAKE2B, "blake2b");
    algo_names.insert( ALGO_BLAKE2S, "blake2s");
    algo_names.insert( ALGO_BMW, "bmw");
    algo_names.insert( ALGO_C11, "c11");
    algo_names.insert( ALGO_CRYPTOLIGHT, "cryptolight");
    algo_names.insert( ALGO_CRYPTONIGHT, "cryptonight");
    algo_names.insert( ALGO_DECRED, "decred");
    algo_names.insert( ALGO_DEEP, "deep");
    algo_names.insert( ALGO_DMD_GR, "dmd-gr");
    algo_names.insert( ALGO_DROP, "drop");
    algo_names.insert( ALGO_FRESH, "fresh");
    algo_names.insert( ALGO_GROESTL, "groestl");
    algo_names.insert( ALGO_HEAVY, "heavy");
    algo_names.insert( ALGO_HMQ1725, "hmq1725");
    algo_names.insert( ALGO_HODL, "hodl");
    algo_names.insert( ALGO_JHA, "jha");
    algo_names.insert( ALGO_KECCAK, "keccak");
    algo_names.insert( ALGO_KECCAKC, "keccakc");
    algo_names.insert( ALGO_LBRY, "lbry");
    algo_names.insert( ALGO_LUFFA, "luffa");
    algo_names.insert( ALGO_LYRA2H, "lyra2h");
    algo_names.insert( ALGO_LYRA2RE, "lyra2re");
    algo_names.insert( ALGO_LYRA2REV2, "lyra2rev2");
    algo_names.insert( ALGO_LYRA2Z, "lyra2z");
    algo_names.insert( ALGO_LYRA2Z330, "lyra2z330");
    algo_names.insert( ALGO_M7M, "m7m");
    algo_names.insert( ALGO_MYR_GR, "myr-gr");
    algo_names.insert( ALGO_NEOSCRYPT, "neoscrypt");
    algo_names.insert( ALGO_NIST5, "nist5");
    algo_names.insert( ALGO_PENTABLAKE, "pentablake");
    algo_names.insert( ALGO_PHI1612, "phi1612");
    algo_names.insert( ALGO_PLUCK, "pluck");
    algo_names.insert( ALGO_POLYTIMOS, "polytimos");
    algo_names.insert( ALGO_QUARK, "quark");
    algo_names.insert( ALGO_QUBIT, "qubit");
    algo_names.insert( ALGO_SCRYPT, "scrypt");
    algo_names.insert( ALGO_SCRYPTJANE, "scryptjane");
    algo_names.insert( ALGO_SHA256D, "sha256d");
    algo_names.insert( ALGO_SHA256T, "sha256t");
    algo_names.insert( ALGO_SHAVITE3, "shavite3");
    algo_names.insert( ALGO_SKEIN, "skein");
    algo_names.insert( ALGO_SKEIN2, "skein2");
    algo_names.insert( ALGO_SKUNK, "skunk");
    algo_names.insert( ALGO_TIMETRAVEL, "timetravel");
    algo_names.insert( ALGO_TIMETRAVEL10, "timetravel10");
    algo_names.insert( ALGO_TRIBUS, "tribus");
    algo_names.insert( ALGO_VANILLA, "vanilla");
    algo_names.insert( ALGO_VELTOR, "veltor");
    algo_names.insert( ALGO_WHIRLPOOL, "whirlpool");
    algo_names.insert( ALGO_WHIRLPOOLX, "whirlpoolx");
    algo_names.insert( ALGO_X11, "x11");
    algo_names.insert( ALGO_X11EVO, "x11evo");
    algo_names.insert( ALGO_X11GOST, "x11gost");
    algo_names.insert( ALGO_X13, "x13");
    algo_names.insert( ALGO_X13SM3, "x13sm3");
    algo_names.insert( ALGO_X14, "x14");
    algo_names.insert( ALGO_X15, "x15");
    algo_names.insert( ALGO_X17, "x17");
    algo_names.insert( ALGO_XEVAN, "xevan");
    algo_names.insert( ALGO_YESCRYPT, "yescrypt");
    algo_names.insert( ALGO_YESCRYPTR8, "yescryptr8");
    algo_names.insert( ALGO_YESCRYPTR16, "yescryptr16");
    algo_names.insert( ALGO_ZR5, "zr5");
    algo_names.insert( ALGO_COUNT, "\0");
}
