#ifndef GROUPMEMBERBLL_H
#define GROUPMEMBERBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/GroupMemberDAL.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群成员业务
 *Others:
*/
class GroupMemberBLL
{
public:
    /*
    Description:初始化
    Input: null
    Output: null
    Return: 0 -success
    */
    int Init();
    /*
    Description:分配业务接口
    Input: @psMethod 方法名 @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse );



    /*
    Description:成为群的成员 加入方式 二维码或好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int JoinIn( const Json::Value& pJson, Json::Value& pResponse );



    /*
    Description:主动退出群
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int JoinOut( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:群主踢人
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int KickOut( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取我加入的群
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyGroup( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:一个群的所有成员
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MemberById( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:一批群的成员
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MemberBylotsId( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:更改我在群中的名字
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetNickName( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:更改我的群配置
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ModifyMySetting( const Json::Value& pJson, Json::Value& pResponse );


public:
    int AddMember(long lGroupId, long lMemberId, int iJoin);

    //推送入群消息
    //lUserId :推送到目的ID
    //lMemberId : 谁加入了
    //sNickName : lMemberId的昵称
    //lMasterId : 群主
    //sInviteNick : 邀请者的昵称
    int PushJoinInCmd(long lUserId, long lMemberId,string sNickName,long lGroupId,long lMasterId, string sGroupName,string sInviteNick);

    //推送退群消息
    //lUserId :推送到目的ID
    //lMemberId : 谁退出了
    //sNickName : lMemberId的昵称
    int PushJoinOutCmd( long lUserId, long lMemberId,string sNickName,long lGroupId);

    //推送群主踢人消息
    //lUserId :推送到目的ID
    //lMemberId : 谁被踢了
    //sNickName : lMemberId的昵称
    int PushKickOutCmd( long lUserId, long lMemberId,string sNickName,long lGroupId);

    //推送变更群信息
    //lUserId :推送到目的ID
    //lModifyId : 变更者的ID
    //sNickName : 变更者的昵称
    int PushSetGroupCmd( long lUserId ,long lModifyId,string sNickName,long lGroupId ,string sGroupName);

    //推送群成员变更昵称
    //lUserId :推送到目的ID
    //lMemberId : 谁退出了
    //sNickName : lMemberId的昵称
    int PushMemberNameCmd( long lUserId ,long lMemberId,long lGroupId ,string sNickName);


    void Append(GroupMemberList & L, const GroupMemberList & R);


    //生成群的头像(成员头像列表)
    string MakeGroupHead(long lGroupId);
    bool IsMember(long lGroupId, long lMemberId);


private:    

    GroupMemberDAL mGroupMemberDAL;

};

#endif // GROUPMEMBERBLL_H
