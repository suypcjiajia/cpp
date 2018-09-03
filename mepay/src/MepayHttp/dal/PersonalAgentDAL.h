#ifndef PERSONALAGENT_DAL_H
#define PERSONALAGENT_DAL_H

#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "bll/ZoneAgnetBLL.h"
using namespace std;
//个人代理数据
class PersonalAgentDAL
{
    // 定义单键
public:
    static PersonalAgentDAL* GetInstance();

public:

    //增加
    int Insert( const PersonalAgentEntity& entity );
    //删除
    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
    //修改
    int Update( const string& psPrimaryKey,const PersonalAgentEntity& entity );
    //根据主键（用户ID）获取个代信息
    int GetEntityByKey(const string & psKey,int piType,PersonalAgentEntity  & entity);
    //根据条件获取多个代信息
    int GetData(const PersonalAgentQueryEntity & pQueryEntity,int piIndex ,int piCount,PersonalAgentList & pOutLst,  int & piTotalCount);

public:
    static const string msTableName ;
private:
    PersonalAgentDAL(){}
    int GetQueryTotalCount(const string & psQuery, int & piCount);
private:
    static PersonalAgentDAL* mInstance;
    TMutex mMutex;//线程锁


};

#endif // ZoneAgentDAL
