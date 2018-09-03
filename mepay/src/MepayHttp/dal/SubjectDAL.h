#ifndef SUBJECTDAL_H
#define SUBJECTDAL_H

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

class SubjectDAL
{
public:
    SubjectDAL();
    //初始化表-没有自动创建
    int CreateTable();
    int replace( const Subject& entity  );
    int GetAll(SubjectLst  & subjects);
private:
    const string msTableName = "subject";
    TMutex mMutex;//线程锁
};

#endif // SUBJECTDAL_H
