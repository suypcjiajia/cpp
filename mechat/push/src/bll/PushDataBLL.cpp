#include "PushDataBLL.h"
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
#include "com/RetJsonMsg.hpp"
#include "bll/ConnHashMapBLL.h"
#include "com/ios_push/original_ssl_client.h"
#include "CmdValue.h"
#include "NoticeEntity.h"
#include "ConnKeepLiveBLL.h"
using namespace std;

//http接口调用入口
int PushDataBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "PushData" == psMethod ){

        iRet = PushData(pJson,pResponse);
    } else if( "Inside" == psMethod ){

        iRet = Inside(pJson,pResponse);
    }else if ( "LogOut" == psMethod){
        LogOut(pJson,pResponse);
    }else if ( "PushInsides" == psMethod){
        PushInsides(pJson,pResponse);
    }
    else {
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}

int PushDataBLL::LogOut(Json::Value  pJson ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    ConnHashMapEntity connEntity;
    iRet = ConnHashMapBLL::GetInstance()->Find(lUserId,connEntity);
    if( 0 == iRet){
        ConnKeepLiveBLL::GetInstance()->del(connEntity.mHdl);
        try{
            connEntity.mServer->close(connEntity.mHdl,0,"");
        }catch(const websocketpp::exception& e){
            appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::LogOut close:%s",e.m_msg.c_str());

        }
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

void PushDataBLL::GetAllPushIPs(StringLst & all)
{
    string sLocal = TBLL::GetInstance()->GetLocalNetworkIP();
    string sOther = TBLL::GetInstance()->GetOtherPushAddr();
    TConvert tConvert;
    tConvert.Split(sOther,",",all);
    all.push_back(sLocal);
    return;
}


//分批。每一批用户所在的推送机要相同
void PushDataBLL::PutUserToBucket(IpManyUserMap & ipManyUserMap,const PushUserEntity & con)
{
    IpManyUserMapIter it = ipManyUserMap.find(con.sPushIp);
    if( it != ipManyUserMap.end() ){
        vector<PushUserEntity> & eles = it->second;
        eles.push_back(con);
    }else{
        vector<PushUserEntity>  eles;
        eles.push_back(con);
        ipManyUserMap[con.sPushIp] = eles;
    }

}

//推送数据(http对外接口)
int PushDataBLL::PushData( const  Json::Value& pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["iCmd"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;bool bAll = false;



    IpManyUserMap ipManyUserMap;
    if( !pJson["jUserIds"].isNull()){//一个或多个目标用户，要对目标用户按它们所在的推送机来分批。每一批用户所在的推送机要相同

        Json::Value jIds = pJson["jUserIds"];
        if( jIds.size() == 0)
            bAll = true;

        for( int i = 0 ; i< jIds.size(); i++){
            PushUserEntity con;
            long lUserId = tConvert.Json2Long(jIds[i]);
            mUserDAL.GetPushIp(lUserId ,con.sPushIp );
            con.lUserId = lUserId;
            if( !con.sPushIp.empty()){//这个目标用户已连接到某台推送机
                PutUserToBucket(ipManyUserMap,con);
            }
        }
        if(bAll){

            StringLst  all;
            GetAllPushIPs(all);
            for(StringLst::iterator it = all.begin(); it != all.end(); it++){
                PushUserEntity con;
                con.sPushIp = *it;
                PutUserToBucket(ipManyUserMap,con);
            }

        }

    }else if( !pJson["lUserId"].isNull()){//单用户,这个是为了兼容以前的

        long lUserId = tConvert.Json2Long(pJson["lUserId"]);
        PushUserEntity con;

        mUserDAL.GetPushIp(lUserId ,con.sPushIp );
        con.lUserId = lUserId;

        if( !con.sPushIp.empty()){//这个目标用户已连接到某台推送机
            PutUserToBucket(ipManyUserMap,con);
        }

    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    if( ipManyUserMap.size() > 0){
        for( IpManyUserMapIter it = ipManyUserMap.begin(); it != ipManyUserMap.end() ; it++){
            vector<PushUserEntity> & eles = it->second;

            const string & sIp = it->first;

            Json::Value  jsonValue = pJson;

            if(!bAll){
                if( !jsonValue["jUserIds"].isNull())
                    jsonValue.removeMember("jUserIds");


                for( int i = 0 ; i< eles.size(); i++){
                    PushUserEntity & ele = eles[i];
                    jsonValue["jUserIds"][i] = tConvert.LongToStr( ele.lUserId );

                }
            }else{
                jsonValue["jUserIds"];
                jsonValue["jUserIds"].resize(0);
            }


            if ( sIp == TBLL::GetInstance()->GetLocalNetworkIP() ) {
                iRet = this->PushInsides( jsonValue, pResponse );
            } else {
                //目标用户在另一台推送机上，要转发过去
                string sUrl = "http://"+sIp+":23243/?sBusiness=PushData&sMethod=PushInsides";
                PostsAes postsAes;
                string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
                appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::PushData 目标用户在另一台推送机上，要转发过去 sReturn=%s",sReturn.c_str());
            }
        }
    }else{

        RetJsonMsg::SetRetMsg(-1,pResponse);
        pResponse["sMsg"] = "目标用户还没有连接到推送机";
        return -1;

    }



    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}
//推送数据(http推送机与推送机通信)
int PushDataBLL::Inside(  Json::Value   pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["iPhoneType"].isNull() || pJson["sToken"].isNull()
            || pJson["iCmd"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;

    int iType = tConvert.Json2Int(pJson["iPhoneType"]);
    string sToken = tConvert.Json2String(pJson["sToken"]);

    pJson.removeMember("iPhoneType");
    pJson.removeMember("sToken");



    int iRet = ToSend(tConvert.Json2Long(pJson["lUserId"]),iType,sToken, pJson.toStyledString());
    if ( 0 == iRet) {
        RetJsonMsg::SetRetMsg(0,pResponse);
        return 0;
    }else if( 1 == iRet ){
        RetJsonMsg::SetRetMsg(1,pResponse);
        return -1;
    }else{
        appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::Inside ToSend  err");
        RetJsonMsg::SetRetMsg(-1,pResponse);
        return -1;
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


//推送数据(http推送机与推送机通信)目标是多用户
int PushDataBLL::PushInsides(  Json::Value   pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["jUserIds"].isNull() || pJson["iCmd"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;int iRet = -1;


    Json::Value jUserIds = pJson["jUserIds"];

    pJson.removeMember("jUserIds");

    if( jUserIds.size() <=0){//空的话，表示推送给所有在线用户


        OnlineUserHashMap connHashMap;

        ConnHashMapBLL::GetInstance()->Copy(connHashMap);
        OnlineUserHashMap::iterator it = connHashMap.begin();
        for(  ; it != connHashMap.end(); it ++){//全部在线用户
            ConnHashMapEntity & entity = (it->second);

            pJson["lUserId"] = tConvert.LongToStr( entity.lUserId);
            ToSend( entity.lUserId,entity.iPhoneType,entity.sToken, pJson.toStyledString());

        }

    }else{

        for(int i = 0; i < jUserIds.size(); i++){
            long lDestUserId = tConvert.Json2Long(jUserIds[i]);
            string sToken;int iPhoneType;
            mUserDAL.GetToken(lDestUserId,sToken);
            mUserDAL.GetType(lDestUserId,iPhoneType);

            pJson["lUserId"] = tConvert.LongToStr(lDestUserId);
            ToSend(lDestUserId,iPhoneType,sToken, pJson.toStyledString());
        }

    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}

int PushDataBLL::PushOffline(long plUserId)
{
    string sMsg;
    ConnHashMapEntity connHashMapEntity;
    int iRet =ConnHashMapBLL::GetInstance()->Find(plUserId,connHashMapEntity);
    if ( 0 == iRet ) {

        //发送数据 websocket
        PushDataBLL pushDataBLL;
        iRet = pushDataBLL.SendByWs(plUserId,connHashMapEntity.mServer,connHashMapEntity.mHdl,sMsg);

        if( iRet != 0 && connHashMapEntity.iPhoneType == 1 ){
            //发送数据 apple apns
            iRet = ApplePushNetServer(plUserId, connHashMapEntity.sToken,sMsg );
        }

    }

    return 0;
}

//向推送机注册
int PushDataBLL::Register(server* pServer, connection_hdl pHdl,const long lUserId,const int iPhoneType,const string& sToken,const string & sVer)
{

    appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::Register lUserId:%ld,iPhoneType:%d,sToken:%s, sVer:%s",lUserId,iPhoneType,sToken.c_str(),sVer.c_str());
    PushUserEntity entity;
    if ( 0 != this->mUserDAL.GetEntity( lUserId,entity ) ) {
        appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::Register mUserDAL.GetEntity error");
        return -1;
    }
    TConvert tConvert;
    if ( ! entity.sPushIp.empty() ) {
        appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::Register last toketn:%s",entity.sToken.c_str());
        if( sToken == entity.sToken ){//同台设备登录，不用推送下线

        }else{
            if ( entity.sPushIp == TBLL::GetInstance()->GetLocalNetworkIP() ) {
                //同台推送机，推送下线
                Json::Value jsonValue;
                Json::Value pResponse;
                jsonValue["iCmd"]= enum_cmd_offline;
                jsonValue["lUserId"]=tConvert.LongToStr(lUserId);
                jsonValue["iPhoneType"]= entity.iPhoneType;
                jsonValue["sToken"]=entity.sToken;

                appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::Register on the same machine  push offline");

                if( entity.iPhoneType == 1 ){//暂时屏蔽推送IOS下线

                }else
                    Inside(jsonValue,pResponse);

            } else {
                //向其它推送机上发送强行下线
                Json::Value jsonValue;
                jsonValue["iCmd"]=enum_cmd_offline;
                jsonValue["lUserId"]=tConvert.LongToStr(lUserId);
                jsonValue["iPhoneType"]= entity.iPhoneType;
                jsonValue["sToken"]=entity.sToken;
                appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::Register diffrent machine  push offline");
                //URL
                string sUrl = "http://"+entity.sPushIp+":23243/?sBusiness=PushData&sMethod=Inside";
                //获取HTTP请求
                PostsAes postsAes;
                string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
                appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::Register push offline sReturn=%s",sReturn.c_str());
            }
        }
    }


    //    ConnHashMapEntity toCloseConn;
    //    if(0 == ConnHashMapBLL::GetInstance()->Find(lUserId,toCloseConn)){
    //        try{
    //          toCloseConn.mServer->close(toCloseConn.mHdl,0,"");
    //        }catch(const websocketpp::exception& e){

    //        }
    //    }

    //记录用户链接(本地缓存)
    long lAddTime = tConvert.GetCurrentTime();
    ConnHashMapEntity connHashMapEntity;
    connHashMapEntity.lUserId = lUserId;
    connHashMapEntity.lAddTime = lAddTime;
    connHashMapEntity.iPhoneType = iPhoneType;
    connHashMapEntity.sToken = sToken;
    connHashMapEntity.mServer = pServer;
    connHashMapEntity.mHdl = pHdl;
    connHashMapEntity.iMsgCount = 0;
    connHashMapEntity.sVer = sVer;
    ConnHashMapBLL::GetInstance()->Add(lUserId,connHashMapEntity);

    //更新用户分布式缓存
    PushUserEntity pushIpEntity;
    pushIpEntity.lUserId = lUserId;
    pushIpEntity.sPushIp = TBLL::GetInstance()->GetLocalNetworkIP();
    pushIpEntity.lAddTime = lAddTime;
    pushIpEntity.iPhoneType = iPhoneType;
    pushIpEntity.sToken = sToken;
    if ( 0 != this->mUserDAL.Replace( pushIpEntity ) ) {
        appendlog(TTDLogger::LOG_ERROR,"PushIpBLL::Replace error");
        return -1;
    }

    return 0;
}

//下线
int PushDataBLL::OffLine(server* pServer, connection_hdl pHdl,const long lUserId)
{
    appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::OffLine lUserId:%ld",lUserId);
    ConnHashMapBLL::GetInstance()->Remove(lUserId);
    return 0;
}

//处理客户端还回
int PushDataBLL::OnRetForChat(const int iRet ,const string & sKey)
{
    if(  0 == iRet){
        //更新chat状态
        ImDAL dal;
        dal.UpdateChatState(sKey,1);//alread readed
    }else{

    }
    return 0;
}

//处理客户端还回
int PushDataBLL::OnRetForGroupChat(const int iRet ,const string & sKey,long lUserId)
{
    if(  0 == iRet){
        //更新chat状态
        ImDAL dal;
        dal.UpdateGroupChatReaded(sKey,lUserId);//alread readed
    }else{

    }
    return 0;
}

//处理客户端还回
int PushDataBLL::OnRetForPublicChat(const int iRet ,const string & sKey,long lUserId)
{
    if(  0 == iRet){
        //更新chat状态
        ImDAL dal;
        dal.UpdatePublicChatReaded(sKey,lUserId);//alread readed
    }else{

    }
    return 0;
}

//发送数据
int PushDataBLL::ToSend(const long plUserId,const int piPhoneType,const string& psToken,const string& psMsg)
{
    int iRet = -1;
    string sMsg = psMsg;
    ConnHashMapEntity connHashMapEntity;
    iRet =ConnHashMapBLL::GetInstance()->Find(plUserId,connHashMapEntity);
    if ( 0 == iRet ) {

        //发送数据 websocket
        PushDataBLL pushDataBLL;
        iRet = pushDataBLL.SendByWs(plUserId,connHashMapEntity.mServer,connHashMapEntity.mHdl,sMsg);

        if( iRet != 0 && piPhoneType == 1 ){
            //发送数据 apple apns
            iRet = ApplePushNetServer(plUserId, psToken,sMsg );
        }

    }

    return iRet;
}
//发送数据by websocket
int PushDataBLL::SendByWs( const long plUserId,server* pServer, connection_hdl pHdl, const string& psStrData )
{
    try {
        WsSendCOM wsSendCOM;
        wsSendCOM.WsSendByServer(pServer,pHdl,psStrData);
    } catch (const websocketpp::exception& e) {
        appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::SendByWs lUserId=%ld error:%s",plUserId, e.m_msg.c_str());
        return -1;
    }
    return 0;
}
//向苹果的APNS发送消息
int PushDataBLL::ApplePushNetServer(long lUserId, const string& psToken, const string& psMsg )
{
    return 0;

    appendlog(TTDLogger::LOG_DEBUG,"enter PushDataBLL::ApplePushNetServer Token:%s Msg:%s",psToken.c_str(), psMsg.c_str());

    //数据合法性
    Json::Value jsonValue;
    TConvert tConvert;
    if ( -1 == tConvert.Str2Json(psMsg, jsonValue)){
        return -1;
    }

    if(  jsonValue["iCmd"].isNull() ) {
        return -1;
    }

    if( psToken.find(" ") == string::npos){//IOS的token包括空格的
        return -1;
    }

    if(!IsTrouble(lUserId,jsonValue)){
        return 0;//消息免打扰
    }

    appendlog(TTDLogger::LOG_DEBUG,"PushDataBLL::ApplePushNetServer to Send");

    int iCmd = tConvert.Json2Int(jsonValue["iCmd"]);

    ConnHashMapEntity connEntity;
    if(ConnHashMapBLL::GetInstance()->Find(lUserId,connEntity) == 0){
        connEntity.iMsgCount++;
        ConnHashMapBLL::GetInstance()->SetMsgCount(lUserId,connEntity.iMsgCount);
    }else{
        connEntity.iMsgCount = 1;
    }



    string sAlert = "";
    Json::Value jSend;


    if ( enum_cmd_offline == iCmd){
        sAlert = "你的帐号已在其它地方登录,如不是你本人操作,请注意帐号安全";
    }else if ( enum_cmd_resqfri == iCmd){
        sAlert = "有新的好友申请";
    }else if ( enum_cmd_chat == iCmd || enum_cmd_group_chat == iCmd){
        ImDAL dal;
        UserDataEntity userEntity;
        long lSendUserId;
        if( enum_cmd_chat == iCmd){
            lSendUserId = tConvert.Json2Long(jsonValue["Entity"]["lUserId"]);
        }else if (enum_cmd_group_chat == iCmd){
            lSendUserId = tConvert.Json2Long(jsonValue["Entity"]["lSendId"]);
        }
        dal.GetUserInfo(lSendUserId,userEntity);
        sAlert = userEntity.sNickName;
        sAlert += ":";

        int iType = tConvert.Json2Int( jsonValue["Entity"]["iType"]);
        string sContent = tConvert.Json2String( jsonValue["Entity"]["sContent"]);
        if( 1 == iType){
            sAlert += sContent;
        }else if( 2 == iType){
            sAlert += "发了一张图片";
        }else if( 3 == iType){
            sAlert += "发了一条语音";
        }else if( 5 == iType){
            sAlert += "发了一个红包";
        }else if( 6 == iType){
            sAlert += "向你转了一笔帐,可在钱包帐单查看详情";
        }else if( 7 == iType){
            sAlert = "收到一条支付凭证,可在钱包帐单查看详情";
        }
        if( sAlert.length() > 1000){
            sAlert = sAlert.substr(0,1000);
        }

        if( enum_cmd_chat == iCmd){
            jSend["server"]["iCmd"] = enum_cmd_chat;
            jSend["server"]["lSendUserId"] = tConvert.LongToStr(lSendUserId);
        }else if(enum_cmd_group_chat == iCmd ){
            jSend["server"]["iCmd"] = enum_cmd_group_chat;
            jSend["server"]["lGroupId"] = jsonValue["Entity"]["lGroupId"];
        }
    }else if(enum_cmd_notice == iCmd){
        sAlert = "天天兑:";
        string sContent ;
        if(!jsonValue["Entity"]["sContent"]["sTitle"].isNull()){
           sContent = tConvert.Json2String( jsonValue["Entity"]["sContent"]["sTitle"]);
        }
        sAlert += sContent;
        jSend["server"]["iCmd"] = enum_cmd_notice;
        jSend["server"]["lSendUserId"] = tConvert.LongToStr(28);
    }else if(enum_cmd_public_chat == iCmd){


        sAlert = tConvert.Json2String(jsonValue["Entity"]["sNickName"]);
        sAlert += ":";

        int iType = tConvert.Json2Int( jsonValue["Entity"]["iType"]);
        string sContent = tConvert.Json2String( jsonValue["Entity"]["sContent"]);
        if( 1 == iType){
            sAlert += sContent;
        }else if( 2 == iType){
            sAlert += "发了一张图片";
        }else if( 3 == iType){
            sAlert += "发了一条语音";
        }else if( 5 == iType){
            sAlert += "发了一个红包";
        }
        if( sAlert.length() > 1000){
            sAlert = sAlert.substr(0,1000);
        }

    }else if( enum_cmd_get_money == iCmd){
        //{"aps":{"alert":"你账户收到100.00元付款","badge":1,"mutable-content":1,"sound": "HasBeenReceiving1.caf"}}
        sAlert = tConvert.Json2String( jsonValue["Entity"]["sContent"]);
        jSend["aps"]["mutable-content"] = 1;
        int iType = tConvert.Json2Int( jsonValue["Entity"]["iType"]);

        string sSount = "HasBeenReceiving.caf";
        if( iType == 2){
            sSount = "HasToAccount.caf";
        }
        jSend["aps"]["sound"] = sSount;

        jSend["server"]["iCmd"] = iCmd;
        jSend["server"]["iType"] = iType;

    }
    else {
        return -1;
    }

    jSend["aps"]["alert"] = sAlert;
    jSend["aps"]["badge"] = connEntity.iMsgCount;


    string sJsend = jSend.toStyledString();
    appendlog(TTDLogger::LOG_INFO,"PushDataBLL::ApplePushNetServer sToken=%s jSend=%s",psToken.c_str(),sJsend.c_str());

    try {
        //发送
        original_ssl_client ios_ssl_client;
        int iRet = ios_ssl_client.SslSendMsg(psToken,jSend.toStyledString());
        if (-1 == iRet){
            appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::ApplePushNetServer sToken=%s sAlert=%s",psToken.c_str(),sAlert.c_str());
            return -1;
        }
    } catch ( const Exception &e ) {
        appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::ApplePushNetServer sToken=%s sAlert=%s err:%s",psToken.c_str(),sAlert.c_str(),e.what());
        return -1;
    }catch(...){
        appendlog(TTDLogger::LOG_ERROR,"PushDataBLL::ApplePushNetServer sToken=%s sAlert=%s unknow err",psToken.c_str(),sAlert.c_str());
        return -1;

    }

    if( enum_cmd_chat == iCmd){
        return 1;
    }
    return 0;
}
//消息是否打扰
bool PushDataBLL::IsTrouble(long lUserId, const Json::Value & jMsg)
{
    TConvert tConvert;
    int iCmd = tConvert.Json2Int(jMsg["iCmd"]);
    if( enum_cmd_group_chat  == iCmd){
        long lGroupId = tConvert.Json2Long( jMsg["Entity"]["lGroupId"]);
        int iMsgMode;
        mImDAL.GetMsgMode(lUserId,lGroupId,iMsgMode);
        if( 1 == iMsgMode ){//消息免打扰
            return false;
        }
    }
    return true;

}




bool PushDataBLL::FiltCmd(long lUserId,int iCmd)
{
    ConnHashMapEntity connHashMapEntity;
    if ( 0 == ConnHashMapBLL::GetInstance()->Find(lUserId,connHashMapEntity) ) {


        if( connHashMapEntity.sVer == ""){
            return false;
        }

        //这里要改成可配置


        if( iCmd == enum_cmd_chat || iCmd == enum_cmd_agreefri || iCmd == enum_cmd_blackfri || iCmd == enum_cmd_cancelchat
                || iCmd == enum_cmd_delfri || iCmd == enum_cmd_resqfri || iCmd == enum_cmd_disagreefri)
        {
            if( connHashMapEntity.iPhoneType == 1 &&  connHashMapEntity.sVer < "2.0" ){
                return true;
            }else if(connHashMapEntity.iPhoneType == 2 &&  connHashMapEntity.sVer < "2.3" ){
                return true;
            }
        }
    }
    return false;

}
