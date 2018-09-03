#ifndef KEYSECRETDAL_H
#define KEYSECRETDAL_H
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
#include "define.h"

class KeySecretDAL
{
public:
    KeySecretDAL();

    int Add( const string & sAppKey, const string & sAppSecret , long lPlatformId);
    int Replace(const string & sAppKey, const string & sAppSecret,long lPlatformId);
    int IsExist( const string & sAppKey);
    int UpSecret(string sAccount );
    int Get(const string & sAppKey,string & sAppSecret, long & lPlatformId);

private:

    const string msTableName = "keysecret";
    TMutex mMutex;//线程锁
};

#endif // KEYSECRETDAL_H
