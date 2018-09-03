#ifndef PerAgentDAL_H
#define PerAgentDAL_H
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

struct PerAgent{
    string addTime   ;
    string upTime  ;
    int state ;
    string  city ;
    string  pushZone ;
    string  pushPhone ;
    long pushId ;
    int activationState ;
    string  activationTime ;
    string  dealTime ;
    int trialDays ;
    string peragentPhone ;
    long userId ;
    string  orderNo ;
    double totalMoney ;
    double payMoney ;
    int payCount ;
    string  peragentName ;
    int extendType ;
};

class PerAgentDAL
{
public:
    PerAgentDAL();

    int Replace( const PerAgent & entity );

private:

    const string msTableName = "peragent";
    TMutex mMutex;//线程锁
    string mAllColumn =  " addTime,  upTime ,state,city,pushZone,pushPhone,pushId,activationState,activationTime,"
                         "dealTime,trialDays,peragentPhone,userId,orderNo,totalMoney,payMoney,payCount,peragentName,extendType";
};

#endif
