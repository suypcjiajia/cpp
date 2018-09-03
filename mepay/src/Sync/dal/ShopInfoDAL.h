#ifndef PRIMARY_KEY_DAL_H
#define PRIMARY_KEY_DAL_H

#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "define.h"
#include "UserDataEntity.h"
#include "TypeDef.h"

using namespace std;

class ShopInfoDAL
{
    // 定义单键
public:  static ShopInfoDAL* GetInstance();
private: static ShopInfoDAL* mInstance;
private: TMutex mMutex;//线程锁
public:

    int GetList(   UserDataList & plstOut);
    long GetMasterId( long lPlatformId);
    int Update(   long lUserId,long lPushId);


    //查询数据
    int GetList( int piIndex, int piCount, ShopInfoList & plstOut);

    //获取所有区域
    long GetZones( StringLst & zones);
    //获取区域下的所有平台Id
    long GetPlatformIdsByZone( string  zone, LongLst & platformIds);
    //获取区域下的粉丝粉量
    long GetCountByZone( string day, LongLst & platformIds);

    int UpdateWeixinNum(   const string & s);


};

#endif // PRIMARY_KEY_DAL_H
