#ifndef ZONEAGENET_DAL_H
#define ZONEAGENET_DAL_H

#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "bll/ZoneAgnetBLL.h"
using namespace std;

class ZoneAgentDAL
{
    // 定义单键
public:
    static ZoneAgentDAL* GetInstance();
private:
    static ZoneAgentDAL* mInstance;
    ZoneAgentDAL(){}
private:
    TMutex mMutex;//线程锁
public:
    //创建表
    int CreateTable();

    //增加
    int Insert( const ZoneAgentEntity& entity );
    //删除
    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
    //修改
    int Update( const string& psPrimaryKey,const ZoneAgentEntity& entity );

    int ModifyPwd(const string & psPrimaryKey,const string & psNewPwd,const long & plUpdateId);

    //增加,批量
    int InsertBatch( const ZoneAgentList & pLst, const string& psSync );
    int GetList(const ZoneAgentQueryEntity & pQueryEntity,int piIndex ,int piCount,ZoneAgentList & pOutLst, int & piTotalCount);
    int GetEntityByKey(const string & psKey,int iType,ZoneAgentEntity &  entity);
    int LikeKey(const string & psKey,ZoneAgentEntity  & entity);
    //根据城市获取代理商
    int GetListByCity(const string & psCity,ZoneAgentList & pOutLst);
    //获取区代理商用户ID
    int GetZoneAgentIdByCity(const string & psCity,long & plAgentId);
    //更换手机号
    int UpdatePhone(const string & psPhone, const string & psNewPhone);

    int GetZoneByPhone(string sPhone,ZoneAgentList & pOutLst);
private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);
};

#endif // ZoneAgentDAL
