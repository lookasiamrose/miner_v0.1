#include <QtTest/QtTest>
#include "../miner.h"
#include "../connectionmanager.h"

class MinerUnitTests: public QObject
{
    Q_OBJECT
private:
    Miner* miner;
    ConnectionManager* connManager;
public:
    MinerUnitTests(){
        miner = new Miner(this);

    }
private slots:
    void unitTest_mapoja_get_algo_alias();
    void unitTest_mapoja_algo_name();
    void unitTest_curl_ftp_demo();
};
void MinerUnitTests::unitTest_curl_ftp_demo()
{
    CURL* curl;
    CURLcode res;
      struct FtpFile ftpfile = {
        "new.txt", /* name to store the file as if successful */
        NULL
      };

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if(curl) {
        /*
         * You better replace the URL with one that works!
         */
        curl_easy_setopt( curl, CURLOPT_USERPWD, "f1324_test:Kuchenka1994" );
        curl_easy_setopt(curl, CURLOPT_URL,
                         "ftp://s14.mydevil.net/test.txt");
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

        /* Switch on full protocol/debug output */
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        if(CURLE_OK != res) {
          /* we failed */
          fprintf(stderr, "curl told us %d\n", res);
        }

        QCOMPARE(res, CURLE_OK);
    }else{
        QVERIFY(curl);
    }

      if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    curl_global_cleanup();
}
void MinerUnitTests::unitTest_mapoja_get_algo_alias()
{
    char * temp  = strdup("bitcore");
    miner->mapoja_get_algo_alias(&temp);
    QCOMPARE(QString(temp), QString("timetravel10"));

    temp  = strdup("zoin");
    miner->mapoja_get_algo_alias(&temp);
    QCOMPARE(QString(temp), QString("lyra2z330"));
}
void MinerUnitTests::unitTest_mapoja_algo_name()
{
    QCOMPARE(miner->algo_name(Miner::ALGO_HEAVY), QString("heavy"));
}
QTEST_MAIN(MinerUnitTests)
#include "minerunittests.moc"
