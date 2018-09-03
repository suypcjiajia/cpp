#ifndef BANKDAL_H
#define BANKDAL_H

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

class BankDAL
{
public:
    BankDAL();
    //初始化表-没有自动创建
    int CreateTable();
    int replace( const BankEntity& entity  );
    int GetEntity(long lPlatformId,BankEntity  & entity);
private:
    const string msTableName = "bank";
    TMutex mMutex;//线程锁
};

#endif // BANKDAL_H
