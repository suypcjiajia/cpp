#ifndef CONFIGDAL_H
#define CONFIGDAL_H

#include <string>
#include <vector>
#include "TypeDef.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:配置信息表
 *Others:
*/
class ConfigDAL
{
private:
    const string msTableName = "fansbank_config";
public:

    int GetSyncUser(long & lValue);

    int SetSyncUser( long lValue);

    int GetSyncTimeTrade(long & lValue);

    int SetSyncTimeTrade(long lValue);

    int GetDeValueDay(string & sDay);

    int SetDeValueDay( string sDay);

    int GetConsumeSyncTime(string & sTime);

    int SetConsumeSyncTime( string sTime);


private:

};

#endif // CONFIGDAL_H
