
/*

 *Author: suyp
 *Description:
 *Others:
 * Day:
*/




#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "TBLL.h"
#include "MyAES.h"
#include "TMd5.h"
#include "com/PostsAes.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "MyUtil.h"
#include "CommunityBLL.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "GetShouYe" == psMethod){

        iRet = GetShouYe(pJson,pResponse);
    }
    else if( "GetPublish" == psMethod){

        iRet = GetPublish(pJson,pResponse);
    }
    else if( "SendPublish" == psMethod){

        iRet = SendPublish(pJson,pResponse);
    }
    else if( "Hudong" == psMethod){

        iRet = Hudong(pJson,pResponse);
    }
    else if( "SendComment" == psMethod){

        iRet = SendComment(pJson,pResponse);
    }
    else if( "ReplyComment" == psMethod){

        iRet = ReplyComment(pJson,pResponse);
    }
    else if( "GetComment" == psMethod){

        iRet = GetComment(pJson,pResponse);
    }
    else if( "ToMeHudong" == psMethod){

        iRet = ToMeHudong(pJson,pResponse);
    }
    else if( "ClearHudong" == psMethod){

        iRet = ClearHudong(pJson,pResponse);
    }
    else if( "GetMyPuslish" == psMethod){

        iRet = GetMyPuslish(pJson,pResponse);
    }
    else if( "GetCollection" == psMethod){

        iRet = GetCollection(pJson,pResponse);
    }
    else if( "MyInfo" == psMethod){

        iRet = MyInfo(pJson,pResponse);
    }
    else{
        RetJsonMsg::SetRetMsg2(-19,pResponse);
    }
    return iRet;
}

/*
Description:初始化
Input: null
Output: null
Return: 0 -success
*/
int CommunityBLL::Init()
{

    return 0;
}


/*
Description:分页获取首页圈子信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::GetShouYe( const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    TConvert tConvert;

    long userId = tConvert.Json2Long( pJson["userId"]);
    int page = tConvert.Json2Int( pJson["page"]);


    string friends = GetFriends(userId);

    CommunityPublishList publishs;int pagecount = 0;
    mCommunityPublishDAL.GetShouYe(userId,friends,page,publishs,pagecount);

    pResponse["result"]["info"].resize(0);
    Json::Value & jResult  = pResponse["result"]["info"];

    for(int iRow = 0; iRow < publishs.size(); ++iRow ){
        CommunityPublishEntiy & entity = publishs[iRow];

        jResult[iRow]["id"] = entity.id;
        jResult[iRow]["userId"] = tConvert.LongToStr(entity.userId);
        jResult[iRow]["nickName"] = entity.nickName;
        jResult[iRow]["headImage"] = entity.headImage;
        jResult[iRow]["addTime"] = entity.addTime;
        jResult[iRow]["fineCount"] = entity.fineCount;
        jResult[iRow]["markCount"] = entity.markCount;
        jResult[iRow]["collectCount"] = entity.collectCount;
        jResult[iRow]["content"] = entity.content;
        jResult[iRow]["pic"] = entity.pic;
        jResult[iRow]["src"] = "";


    }
    pResponse["result"]["total"] = pagecount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}



/*
Description:获取单条圈子信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::GetPublish( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    string id = tConvert.Json2String( pJson["id"]);

    CommunityPublishEntiy entity;
    int iRet = mCommunityPublishDAL.GetEntity(id,entity);

    Json::Value & jResult  = pResponse["result"];

    jResult["userId"] = tConvert.LongToStr(entity.userId);
    jResult["nickName"] = entity.nickName;
    jResult["headImage"] = entity.headImage;
    jResult["addTime"] = entity.addTime;
    jResult["fineCount"] = entity.fineCount;
    jResult["markCount"] = entity.markCount;
    jResult["collectCount"] = entity.collectCount;
    jResult["content"] = entity.content;
    jResult["pic"] = entity.pic;

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

/*
Description:发布一条圈子信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::SendPublish( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    string content = tConvert.Json2String( pJson["content"]);
    string pic = pJson["pic"].toStyledString();
    int authority = tConvert.Json2Int( pJson["authority"]);

    UserDataEntity userEntity;
    mUserDataDAL.GetEntityById(userId,userEntity);

    CommunityPublishEntiy entity;
    entity.userId = userId;
    entity.content = content;
    entity.pic = pic;
    entity.authority = authority;
    entity.addTime = tConvert.GetCurrentDayTime();
    entity.collectCount = 0;
    entity.fineCount = 0;
    entity.headImage = userEntity.sHeadImage;
    entity.id = MyUtil::MakeUnixtimeAnd6Rand();
    entity.markCount = 0;
    entity.nickName = userEntity.sNickName;

    int iRet = mCommunityPublishDAL.Insert(entity);

    pResponse["result"]["id"] = entity.id;
    pResponse["result"]["addTime"] = entity.addTime;
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}


/*
Description:圈子互动 （点赞，分享，收藏）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::Hudong( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    string id = tConvert.Json2String( pJson["id"]);
    int type = tConvert.Json2Int( pJson["type"]);


    UserDataEntity userEntity;
    mUserDataDAL.GetEntityById(userId,userEntity);

    CommunityRemarkEntiy entity;
    entity.userId = userId;
    entity.publishId = id;
    entity.type = type;
    entity.id = MyUtil::MakeUnixtimeAnd6Rand();
    entity.addTime = tConvert.GetCurrentDayTime();
    entity.comment = "";
    entity.headImage = userEntity.sHeadImage;
    entity.nickName = userEntity.sNickName;
    entity.toheadImage = "";
    entity.tonickName = "";
    entity.touserId = 0;

    iRet = mCommunityRemarkDAL.Insert(entity);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

/*
Description:发表评论
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::SendComment( const  Json::Value  & pJson ,Json::Value & pResponse)
{

    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    string id = tConvert.Json2String( pJson["id"]);
    string comment = tConvert.Json2String( pJson["comment"]);

    UserDataEntity userEntity;
    mUserDataDAL.GetEntityById(userId,userEntity);

    CommunityRemarkEntiy entity;
    entity.userId = userId;
    entity.publishId = id;
    entity.type = commuityremark_type_comment;
    entity.addTime = tConvert.GetCurrentDayTime();
    entity.comment = comment;
    entity.headImage = userEntity.sHeadImage;
    entity.id = MyUtil::MakeUnixtimeAnd6Rand();
    entity.nickName = userEntity.sNickName;
    entity.publishId = id;
    entity.toheadImage ="";
    entity.tonickName = "";
    entity.touserId = 0;

    iRet = mCommunityRemarkDAL.Insert(entity);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}

/*
Description:回复评论
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::ReplyComment( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    long touserId = tConvert.Json2Long( pJson["touserId"]);
    string id = tConvert.Json2String( pJson["id"]);
    string comment = tConvert.Json2String( pJson["comment"]);

    UserDataEntity userEntity;
    mUserDataDAL.GetEntityById(userId,userEntity);
    UserDataEntity touserEntity;
    mUserDataDAL.GetEntityById(touserId,touserEntity);

    CommunityRemarkEntiy entity;
    entity.userId = userId;
    entity.publishId = id;
    entity.type = commuityremark_type_reply;
    entity.addTime = tConvert.GetCurrentDayTime();
    entity.comment = comment;
    entity.headImage = userEntity.sHeadImage;
    entity.id = MyUtil::MakeUnixtimeAnd6Rand();
    entity.nickName = userEntity.sNickName;
    entity.publishId = id;
    entity.toheadImage =touserEntity.sHeadImage;
    entity.tonickName = touserEntity.sNickName;
    entity.touserId = touserId;

    iRet = mCommunityRemarkDAL.Insert(entity);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


/*
Description:分页获取圈子互动消息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::GetComment( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    string id = tConvert.Json2String( pJson["id"]);
    int page = tConvert.Json2Int( pJson["page"]);

    CommunityRemarkList lst;int pagecount = 0;
    mCommunityRemarkDAL.GetPage(id,page,lst,pagecount);

    pResponse["result"]["info"].resize(0);
    Json::Value & jResult  = pResponse["result"]["info"];

    for(int iRow = 0; iRow < lst.size(); ++iRow ){
        CommunityRemarkEntiy & entity = lst[iRow];

        jResult[iRow]["id"] = entity.id;
        jResult[iRow]["userId"] = tConvert.LongToStr(entity.userId);
        jResult[iRow]["nickName"] = entity.nickName;
        jResult[iRow]["headImage"] = entity.headImage;
        jResult[iRow]["addTime"] = entity.addTime;
        jResult[iRow]["content"] = entity.comment;
        jResult[iRow]["type"] = entity.type;

    }
    pResponse["result"]["total"] = pagecount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}

/*
Description:获取给我的互动消息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::ToMeHudong( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    int page = tConvert.Json2Int( pJson["page"]);
    CommunityRemarkList lst;int pagecount = 0;

    mCommunityRemarkDAL.ToMeHudong(userId,page,lst,pagecount);

    pResponse["result"]["info"].resize(0);
    Json::Value & jResult  = pResponse["result"]["info"];

    for(int iRow = 0; iRow < lst.size(); ++iRow ){
        CommunityRemarkEntiy & entity = lst[iRow];

        jResult[iRow]["userId"] = tConvert.LongToStr(entity.userId);
        jResult[iRow]["nickName"] = entity.nickName;
        jResult[iRow]["headImage"] = entity.headImage;
        jResult[iRow]["addTime"] = entity.addTime;
        jResult[iRow]["content"] = entity.comment;
        jResult[iRow]["type"] = entity.type;

    }
    pResponse["result"]["total"] = pagecount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}


/*
Description:清空给我的互动消息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::ClearHudong( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);

    iRet = mCommunityRemarkDAL.ClearHudong(userId);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


/*
Description:分页获取我发出的圈子信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::GetMyPuslish( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    int page = tConvert.Json2Int( pJson["page"]);
    string condition = tConvert.Json2String( pJson["condition"]);

    CommunityPublishList publishs;int pagecount = 0;
    mCommunityPublishDAL.GetMyPuslish(userId,page,condition,publishs,pagecount);

    pResponse["result"]["info"].resize(0);
    Json::Value & jResult  = pResponse["result"]["info"];

    for(int iRow = 0; iRow < publishs.size(); ++iRow ){
        CommunityPublishEntiy & entity = publishs[iRow];

        jResult[iRow]["id"] = entity.id;
        jResult[iRow]["userId"] = tConvert.LongToStr(entity.userId);
        jResult[iRow]["nickName"] = entity.nickName;
        jResult[iRow]["headImage"] = entity.headImage;
        jResult[iRow]["addTime"] = entity.addTime;
        jResult[iRow]["fineCount"] = entity.fineCount;
        jResult[iRow]["markCount"] = entity.markCount;
        jResult[iRow]["collectCount"] = entity.collectCount;
        jResult[iRow]["content"] = entity.content;
        jResult[iRow]["pic"] = entity.pic;
    }
    pResponse["result"]["total"] = pagecount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

/*
Description:分页获取收藏的圈子信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::GetCollection( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);
    int page = tConvert.Json2Int( pJson["page"]);

    CommunityPublishList publishs;int pagecount= 0;
    mCommunityPublishDAL.GetCollection(userId,page,publishs,pagecount);

    pResponse["result"]["info"].resize(0);
    Json::Value & jResult  = pResponse["result"]["info"];

    for(int iRow = 0; iRow < publishs.size(); ++iRow ){
        CommunityPublishEntiy & entity = publishs[iRow];

        jResult[iRow]["id"] = entity.id;
        jResult[iRow]["userId"] = tConvert.LongToStr(entity.userId);
        jResult[iRow]["nickName"] = entity.nickName;
        jResult[iRow]["headImage"] = entity.headImage;
        jResult[iRow]["addTime"] = entity.addTime;
        jResult[iRow]["fineCount"] = entity.fineCount;
        jResult[iRow]["markCount"] = entity.markCount;
        jResult[iRow]["collectCount"] = entity.collectCount;
        jResult[iRow]["content"] = entity.content;
        jResult[iRow]["pic"] = entity.pic;
        jResult[iRow]["src"] = "";


    }
    pResponse["result"]["total"] = pagecount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

/*
Description:获取我的圈子个人详情
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int CommunityBLL::MyInfo( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    long userId = tConvert.Json2Long( pJson["userId"]);

    pResponse["result"]["publishCount"] = 0;
    pResponse["result"]["fineCount"] = 0;
    pResponse["result"]["collectCount"] = 0;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}


string CommunityBLL::GetFriends(long userId)
{
    TConvert tConvert;
    UserRelationEntityList relationList;
    UserRelationQueryEntity query;
    query.lUserId = userId;
    query.lToUserId = query.lUserId;
    query.iType = -1;
    int iTotalCount = 0;
    query.lUpTime = -1;
    mUserRelationDAL.GetData( query,relationList,iTotalCount);

    string friends;
    for( UserRelationEntityList::iterator it = relationList.begin(); it != relationList.end(); it++){
        UserRelationEntity & f = *it;
        if( f.iType == enum_rel_friend )
        friends += tConvert.LongToStr( f.lUserId ) + "," +  tConvert.LongToStr(f.lToUserId) + ",";
    }
    if( !friends.empty())
        friends = friends.substr(0,friends.length() -1);

    return friends;

}
