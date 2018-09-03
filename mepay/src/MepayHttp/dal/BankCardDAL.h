#ifndef BANKCARD_DAL_H
#define BANKCARD_DAL_H

/*
 * 银行卡
 * */

#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "bll/ZoneAgnetBLL.h"
using namespace std;

class BankCardDAL
{
    // 定义单键
public:
    static BankCardDAL* GetInstance();
private:
    static BankCardDAL* mInstance;
    BankCardDAL(){}
private:
    TMutex mMutex;//线程锁
public:
    //创建表
    int CreateTable();

    //增加
    int Insert( const BankCardEntity& entity );
    //删除
    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
    //修改
    int Update( const string& psPrimaryKey,const BankCardEntity& entity );
    int GetList(const BankCardQueryEntity & pQueryEntity,int piIndex ,int piCount,BankCardList & pOutLst, int & piTotalCount);
    int GetEntity(const string & ,BankCardEntity &  entity);
    //是否存在
    //0 -yes -1 no
    int IsExist(const string & psPrimaryKey);

private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);

};

#endif // BANKCARD_DAL_H
