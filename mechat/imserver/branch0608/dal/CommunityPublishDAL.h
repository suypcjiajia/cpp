#ifndef CommunityPublishDAL_H
#define CommunityPublishDAL_H

#include <string>
#include <vector>
#include <dal/Basedal.h>
#include <inc/CommunityPublishEntiy.h>
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:创建圈子发布信息
 *Others:
*/
class CommunityPublishDAL:public BaseDal
{

public:
    CommunityPublishDAL(){
        msTableName = "community_publish";
        msAllColumn = " id,userId,nickName,headImage,addTime,fineCount,markCount,collectCount,content,pic,authority ";
    }


    int Insert(const CommunityPublishEntiy& entity );
    int GetEntity(string id,CommunityPublishEntiy& entity);
    //分页获取首页圈子信息
    int GetShouYe(long userId,string friends,int page, CommunityPublishList & lst, int & pagecount);

    //分页获取我发出的圈子信息
    int GetMyPuslish(long userId,int page,string condition,CommunityPublishList & lst,int & pagecount );

    //分页获取收藏的圈子信息
    int GetCollection(long userId,int page,CommunityPublishList & lst ,int & pagecount);


private:
    int Fetch( string sSql ,CommunityPublishList & lst);
    int RowToEntity(MYSQL_ROW row ,CommunityPublishEntiy & entity);
};

#endif // CommunityPublishDAL_H
