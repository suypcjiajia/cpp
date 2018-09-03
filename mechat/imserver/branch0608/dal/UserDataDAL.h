//用户数据访问层
//封装sql


#ifndef UserDataDAL_H
#define UserDataDAL_H

#include <string>
#include <vector>
#include "UserDataEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/



class UserDataDAL:public BaseDal
{

public:
    UserDataDAL();
    //创建表
    int CreateTable();
    //增加
    int Insert( UserDataEntity& pEntity );
    //删除
    int Delete( const string& psPrimaryKey );
    //修改
    int Update( const string& psPrimaryKey,const UserDataEntity& pEntity );
    int ModifyPwd(const string& psPrimaryKey, const string & sNewPwd);
    //修改帐号
    int UpdateAccount( const string & psAccount, const string & psNewAccount);

    //获取最大用户ID
    long GetMaxUserId();
    //记录是否存在-主键
    int IsExist( const string& psPrimaryKey, bool& pbIsBool );
    //记录是否存在-帐号
    int IsExistByAccont( const string& psAccount, bool& pbIsBool );
    //记录是否存在-帐号密码
    int IsExistByAccontPassword( const string& psAccount,const string& psPassword, bool& pbIsBool );
    //记录是否存在-主键帐号密码
    int IsExistByPrimaryKeyAccontPassword( const string& psPrimaryKey, const string& psAccount,const string& psPassword, bool& pbIsBool );
    //记录是否存在-用户ID
    int IsExistByUserId( const string& psUserId, bool& pbIsBool );



    //登录
    int IsLogin( const string& psAccont, const string& psPassword, bool& pbIsBool );

    //查询一行数据
    //ret : -5 -数据不存在  0 -成功
    int GetEntity( const string& psPrimaryKey,const string& psAccount,const string& psGciID, UserDataEntity& pEntity );


    //ret : -5 -数据不存在  0 -成功
    int GetEntityById(long lUserId,UserDataEntity& pEntity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntityByAccount(const string& sAccount,UserDataEntity& pEntity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntityByQqNum(const string& sQqNum,UserDataEntity& pEntity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntityByWeixinNum(const string& sWeixinNum,UserDataEntity& pEntity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntityBysAliNum(const string& sAliNum,UserDataEntity& pEntity);

    //根据用户获取我的推送人数
    int GetMyPushCount( const string& psPushManId, long& plCount );
    //根据用户获取我的推送且登陆过的人数
    int GetMyPushAndLoginedCount( const string& psPushManId, long& plCount );
    //根据用户获取all的推送人数
    int GetBatchUserPushCount(const vector<long>  & plPushManId, long& plCount );
    //获取粉丝人数（根据平台号或推广人）
    int GetFansNum(long Id, int iType,int & iCount);
    //获取可交易（激活）粉丝人数（根据平台号或推广人）
    int GetWeixinBindNum(long Id, int iType,int & iCount);
    //获取粉丝人数GroupBy
    int GetFansNumGroupBy(int iType,int iHot,LongLongMap & longlongMap);
    //获取粉丝审核列表(某一天的)
    int GetUnhotGroupById(int iType,int iPage, int iCount,string sDay,int iHot,CommonStructVector & lst, int & total);
    //获取粉丝审核列表(所有日期>=2018-05-01)
    int GetVerifyList(int iType,int iPage, int iCount,int iHot,CommonStructVector & lst, int & total);


    //获取推广粉丝
    int GetMyPush(const long & plUserId, vector<UserDataEntity>& pList );
    //获取平台粉丝
    int PlatformFans( const long &  plPlatformId, vector<UserDataEntity>& pList  );
    //分页获取粉丝列表(可带查询条件)
    int GetFans(  long Id,int iType,int iPage, int iCount,string sHot,long lUserId,string sNickName
                  , string sAccount, string sTimeStart,string sTimeEnd,
                  string sWeixinNum, string sAliNum,UserDataList & pList , int & iTotal );
    //分页获取生产的粉丝列表(可带查询条件)
    int GetMakedFans(  int iPage, int iCount,long lUserId,long lPlatformId,long lPushManId,string sNickName
                               , string sAccount, string sTimeStart,string sTimeEnd,
                               string sWeixinNum, string sAliNum,UserDataList & pList , int & iTotal);
    //有消费过的粉丝人数
    int GetConsumFansNum(long Id, int iType,int & iCount);
    //有消费过的粉丝
    int GetConsumFans(string sSelPhone,long Id, int iType,int iIndex,int iNum,longs & lUserIds);

    //随机获取一批粉丝
    int GetRandFans(string sSelPhone,long Id, int iType,int iNum,longs & lUserIds);
    //获取一批粉丝
    int GetFansv2(string sSelPhone,long Id, int iType,int iNum,longs & lUserIds);

    //获取一批用户数据
    int GetUserInfoList( const vector<long> &  plUserIdLst, vector<UserDataEntity>& pList  );



    int UpQqNum(const string sAccount  , const string & sQqNum);
    int UpWeixinNum(const string sAccount  , const string & sQqWeixinNum);
    int UpAliNum(const string sAccount  , const string & num);
    int GetRegistersByMonth(string sStart, string sEnd,CommonStructVector & vct);
    int GetRegistersByDay(string sStart, string sEnd,CommonStructVector & vct);
    int GetRegistersByYear(string sStart, string sEnd,CommonStructVector & vct);

    int GetUnHotCount(CommonStructVector & vct);
    int HotVerify(string sDay, int iType , string id, int iHot);
    int FansRecover(string sDay, int iType , string id, string selfid);

    //获取手机号对应的用户Id
    int GetUserIdByPhone(const string & sPhone, long & lUserId);

    //获取推广粉丝数
    int GetFansByPeragentId(long lPeragnetId, int & iCount);
    //获取推广粉丝数 and hot=1
    int GetHotFansCountByPeragent(long lPeragnetId, int & iCount);
    //获取平台的关注粉丝数
    int GetFansByPlatformId(long lPlatformId, int & iCount);
    //获取平台的关注女性粉丝数
    int GetFemaleFansByPlatformId(long lPlatformId, int & iCount);
    //获取平台的关注男性粉丝数
    int GetMaleFansByPlatformId(long lPlatformId, int & iCount);

    //获取关注的成员列表
    int GetAttentionByPlatformId(long lPlatformId,int iIndex ,int iCount,UserDataList & userDataLst);

    //转移推广关系
    int MovePushMan(long lOrgId, long lNewId, string sSelledUser);
    //转移推广关系 sql
    string GetSqlMovePushMan(long lOrgId, long lNewId, string sSelledUser);

    //转移关注关系
    int MoveAttention(long lOrgId, long lNewId, string sSelledUser);
    //转移关注关系 sql
    string GetSqlMoveAttention(long lOrgId, long lNewId,  string sSelledUser);

    //解冻
    int Jeidong(string sAccount);


private:
    int ExcutEntity(const string  & sQql, UserDataEntity& pEntity);

    void RowToEntity(MYSQL_ROW row ,UserDataEntity & entity);
    void ResultToLst(MYSQL_RES* result ,vector<UserDataEntity>& lst);
    string msAll;

};

#endif // UserDataDAL_H
