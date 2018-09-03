#ifndef USERVIDEODAL_H
#define USERVIDEODAL_H

#include <string>
#include <vector>
#include "dal/Basedal.h"
#include "UserVideoEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:用户角色视频
 *Others:
*/
class UserVideoDAL:public BaseDal
{

public:
    UserVideoDAL(){
        msTableName = "user_video";
        msAllColumn = " addTime,type,url,title,icon,content ";
    }

    int CreateTable();

    int Insert( const UserVideoEntity & entity );

    int GetData( const string & sType ,UserVideoLst & lst);

private:

};

#endif // USERVIDEODAL_H
