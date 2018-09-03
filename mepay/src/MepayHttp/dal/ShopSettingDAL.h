#ifndef SHOPSETTINGDAL_H
#define SHOPSETTINGDAL_H
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
class ShopSettingDAL
{
public:
    ShopSettingDAL();

    int Add( const ShopSetting& entity  );
    int IsExist( long lPlatformId);
    int UpdateCashBack(long lPlatformId, int  iCashBack);
    int UpdateCoinBack(long lPlatformId, int  iCoinBack);
    int GetEntity(long lPlatformId,ShopSetting  & entity);
    int UpdateStartTime(long lPlatformId ,long lStartTime);
    int UpdateRecommend(long lPlatformId, int  value);
    int UpdateRestrict(long lPlatformId, int  value);
    int UpdatePark(long lPlatformId, int  value);
    int UpdateWifi(long lPlatformId, int  value);
    int UpdateDelivery(long lPlatformId, int  value);
    int UpdateCheck(long lPlatformId, string value);
private:
    const string msTableName = "shopsetting";
    TMutex mMutex;//线程锁
};

#endif // SHOPSETTINGDAL_H
