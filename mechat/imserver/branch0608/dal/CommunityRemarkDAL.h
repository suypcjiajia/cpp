#ifndef CommunityRemarkDAL_H
#define CommunityRemarkDAL_H

#include <string>
#include <vector>
#include "dal/Basedal.h"
#include "CommunityRemarkEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class CommunityRemarkDAL:public BaseDal
{

public:
    CommunityRemarkDAL(){
        msTableName = "community_remark";
        msAllColumn = " id,publishId,userId,nickName,headImage,addTime,comment,type,touserId,tonickName,toheadImage ";
    }

    int Insert( CommunityRemarkEntiy& entity );


    int GetPage(string id,int page, CommunityRemarkList & lst,int & pagecount);

    int ToMeHudong(long userId,int page, CommunityRemarkList & lst,int & pagecount);

    int ClearHudong(long userId);


private:
    int Fetch( string sSql ,CommunityRemarkList & lst);
    int RowToEntity(MYSQL_ROW row ,CommunityRemarkEntiy & entity);

};

#endif // CommunityRemarkDAL_H
