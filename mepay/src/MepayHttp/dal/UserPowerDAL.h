#ifndef USERPOWERDAL_H
#define USERPOWERDAL_H
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
#include "dal/Basedal.h"
class UserPowerDAL : public BaseDal
{
public:
    UserPowerDAL(){
        msTableName = "user_power";
        msAllColumn = " power,idPlatform,userId,account,nickName,headImage ";
    }

    int Add( const UserPower & entity );
    int IsExist( long idPlatform,long userId,int power);
    //查询数据
    int GetData(long idPlatform, int iPower, int iPage,int iCount,long userId,string account, string nickName,UserPowerLst & lst,int & iTotalCount);

    int Del(long idPlatform,long userId,int power);

    int GetEntity(long idPlatform,long userId,int power,UserPower & entity);



private:

    void RowToEntity(MYSQL_ROW row , UserPower & entity);

    TMutex mMutex;//线程锁
};

#endif // USERPOWERDAL_H
