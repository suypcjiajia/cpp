#ifndef OfflineOrderDAL_H
#define OfflineOrderDAL_H
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

struct OfflineOrder{
    string orderNo ;
    double money ;
    string payType ;
    int payState ;
    long platformId ;
    string shopName ;
    string shopPhone ;
    string zone ;
    long payUserId ;
    string payNickName ;
    string payPhone ;
    string payHeadImage ;
    string addTime ;
    string upTime ;
    string remark ;
    double notDividedMoney ;
    int offLineDisCount ;
    int recType ;
    double coin ;
    int coinType ;
    int isFans ;
    double unPushDivided ;
    double unPlatformDivided ;
};

class OfflineOrderDAL
{
public:
    OfflineOrderDAL();

    int Replace( const OfflineOrder & entity );

private:

    const string msTableName = "offline_order";
    TMutex mMutex;//线程锁
    string mAllColumn =  " orderNo,  money , payType , payState , platformId , shopName , shopPhone , zone , payUserId , "
                         "payNickName, payPhone , payHeadImage , addTime , upTime , remark , "
                         "notDividedMoney, offLineDisCount , recType , coin , coinType , isFans , unPushDivided , unPlatformDivided";
};

#endif
