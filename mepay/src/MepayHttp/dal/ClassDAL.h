#ifndef CLASSDAL_H
#define CLASSDAL_H

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

class ClassDAL
{
public:
    ClassDAL();
    int CreateTable();
    int replace( const NearCalss& entity  );
    int GetAll(string sZone ,NearCalssLst  & subjects);
    int GetAllCalss(NearCalssLst  & subjects);
private:
    const string msTableName = "class";
    TMutex mMutex;//线程锁
};

#endif // CLASSDAL_H
