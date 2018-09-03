#ifndef BASEDAL_H
#define BASEDAL_H

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TypeDef.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

class BaseDal
{
public:
    BaseDal();

    int AutoCommit(bool bOpen);
    int RollBack();
    int Commit();
    int BatchBaseQuery(const StringLst batchSql);

protected:
    int BaseQuery(const string& sSql);    
    int BaseQueryResult(const string & sSql,MYSQL_RES ** res);
    void FreeResult(MYSQL_RES * res);




    int GetQueryTotalCount(const string & psQuery, int & piCount);
    int GetCount(const string & sWhere, int & iCount);
    int GetCountAterGroup(const string & sWhere, int & iCount);
    int GetSum(const string & column,const string & sWhere, int & iCount);
    //是否存在
    //0 -yes -1 no
    int IsExist(const string & sWhere);

protected:
    string msAllColumn;//表格字段名: column1,column2,******
    string msTableName;//表格名

    TConvert mtConvert;
    TMultiMysqlDAL mMulti;
};

#endif // BASEDAL_H
