#ifndef ADVDAL_H
#define ADVDAL_H
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
class AdvDAL
{
public:
    AdvDAL();

    int CreateTable();
    int Add( const ShopAdv & entity );
    int Del(long id);
    int IsExist( long lPlatformId,const string & sAdvLink);
    int IsExist( long id);
    int GetData(  long  lPlatformId ,int iPage,int iCount,int iVerify,int iState, ShopAdvLst & lst,int & iTotalCount);
    int UpVerify(long id, int iValue );

private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);
    const string msTableName = "adv";
    TMutex mMutex;//线程锁
};

#endif // ADVDAL_H
