/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户业务接口
 *Others:
*/


#include "UserDataBLL.h"
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
#include "com/TCurl.h"
#include "dal/UserStateDAL.h"
#include "cache/UserDataCache.h"
#include "common/userutil.h"
#include "com/Request.h"
#include "MyUtil.h"
#include "Token.h"
#include "KVlaueEntity.h"
#include <boost/regex.hpp>

#include "bll/FansBankBLL.h"
using namespace std;
using namespace boost;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;

    if("AddData" == psMethod ){
        //注册-用户注册
        iRet =AddData(pJson,pResponse);
    }else if("UpdateData" == psMethod ){
        //注册-修改普通资料
        iRet =Update(pJson,pResponse);
    }else if("UpdateAccount" == psMethod ){
        //注册-修改帐号
        iRet =UpdateAccount(pJson,pResponse);
    }else if("IsRegistered" == psMethod ){
        //查询-手机号是否被注册
        iRet =IsRegistered(pJson,pResponse);
    }else if("UserLogin" == psMethod ){
        //查询-手机号是否被注册
        iRet =UserLogin(pJson,pResponse);
    }else if("GetUserInfo" == psMethod || "GetUserInfoByID" == psMethod  || "GetUserInfoByPhone" == psMethod || "GetUserInfoByGciID" == psMethod ){
        //查询-用户信息（主键或手机号或用户ID）
        iRet =GetUserInfo(pJson,pResponse);
    }else if("MyPushMan" == psMethod ){
        //查询-根据用户获取我的推广者信息（手机号或用户ID）
        iRet =MyPushMan(pJson,pResponse);
    }else if("MyPushCount" == psMethod ){
        //查询-根据用户获取我的推送人数（手机号或用户ID）
        iRet =MyPushCount(pJson,pResponse);
    }else if("PlatformFans" == psMethod ){
        //平台粉丝
        iRet =PlatformFans(pJson,pResponse);
    }else if("Attention" == psMethod ){
        //更改我的关注
        iRet =Attention(pJson,pResponse);
    }else if("BandGciID" == psMethod ){
        //绑定GCI会员23241
        iRet =BandGciID(pJson,pResponse);
    }else if( "GetUserInfoList" == psMethod){
        iRet = GetUserInfoList(pJson,pResponse);
    }else if( "ForgetPassword" == psMethod){
        iRet = ForgetPassword(pJson,pResponse);
    }else if( "ModifyPwd" == psMethod){
        iRet = ModifyPwd(pJson,pResponse);
    }else if( "MyFSLevelPushCount" == psMethod){
        iRet = MyFSLevelPushCount(pJson,pResponse);
    }else if( "MyPushAndLoginedCount" == psMethod){
        iRet = MyPushAndLoginedCount(pJson,pResponse);
    }else if( "BandThirdId" == psMethod){
        iRet = BandThirdId(pJson,pResponse);
    }else if( "LoginByThird" == psMethod){
        iRet = LoginByThird(pJson,pResponse);
    }else if( "BindInfo" == psMethod){
        iRet = BindInfo(pJson,pResponse);
    }else if( "MyShops" == psMethod){
        iRet = MyShops(pJson,pResponse);
    }else if ( "UnBandThird" == psMethod){
        iRet = UnBandThird(pJson,pResponse);
    }else if( "SetGrade" == psMethod){
        iRet = SetGrade(pJson,pResponse);
    }else if( "GetVips" == psMethod){
        iRet = GetVips(pJson,pResponse);
    }else if( "GetGrade" == psMethod){
        iRet  = GetGrade(pJson, pResponse);
    }else if( "Pass" == psMethod){
        iRet  = Pass(pJson, pResponse);
    }else if( "GetRegisters" == psMethod){
        iRet  = GetRegisters(pJson, pResponse);
    }
    else if( "Auto" == psMethod){//不安全，屏蔽
        iRet  = Auto(pJson, pResponse);
    }
    else if( "GetPushUpChain" == psMethod){
        iRet  = GetPushUpChain(pJson, pResponse);
    }else if( "LoginByCode" == psMethod){
        iRet = LoginByCode(pJson,pResponse);
    }else if( "Sign" == psMethod){
        iRet = Sign(pJson,pResponse);
    }else if( "GetFans" == psMethod){
        iRet = GetFans(pJson,pResponse);
    }else if( "GetVideo" == psMethod){
        iRet = GetVideo(pJson,pResponse);
    }else if( "Perfect" == psMethod){
        iRet = Perfect(pJson,pResponse);
    }else if("RegByBind" == psMethod){
        iRet = RegByBind(pJson,pResponse);
    }else if("Make" == psMethod){
        iRet = Make(pJson,pResponse);
    }else if("PeragentLst" == psMethod){
        iRet = PeragentLst(pJson,pResponse);
    }else if("ShopLst" == psMethod){
        iRet = ShopLst(pJson,pResponse);
    }else if("GetMakedFans" == psMethod){
        iRet = GetMakedFans(pJson,pResponse);
    }else if("ThirdUser" == psMethod){
        iRet = ThirdUser(pJson,pResponse);
    }else if("BindPhone" == psMethod){
        iRet = BindPhone(pJson,pResponse);
    }else if("GetUnHotCount" == psMethod){
        iRet = GetUnhotCount(pJson,pResponse);
    }else if("GetUnHotInfoByDay" == psMethod){
        iRet = GetUnHotInfoByDay(pJson,pResponse);
    }else if("HotVerify" == psMethod){
        iRet = HotVerify(pJson,pResponse);
    }else if("Id2Phone" == psMethod){
        iRet = Id2Phone(pJson,pResponse);
    }else if("FansRecover" == psMethod){
        iRet = FansRecover(pJson,pResponse);
    }else if("GetVerifyList" == psMethod){
        iRet = GetVerifyList(pJson,pResponse);
    }else if("CheckDeivce" == psMethod){
        iRet = CheckDeivce(pJson,pResponse);
    }else if("UpdateDeivce" == psMethod){
        iRet = UpdateDeivce(pJson,pResponse);
    }
    else{
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}



/*
Description:初始化
Input: null
Output: null
Return: 0 -success
*/
int UserDataBLL::Init()
{

    return 0;
}

/*
Description:用户注册
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::AddData( const  Json::Value  & pJson, Json::Value & pResponse )
{
    int iRet = -1;

    if( pJson["sCode"].isNull() || pJson["sAccount"].isNull() || pJson["sPassword"].isNull() ||  pJson["sNickName"].isNull() || pJson["iSex"].isNull() || pJson["sProvince"].isNull()
            || pJson["sCity"].isNull() || pJson["sSignature"].isNull() || pJson["sEmail"].isNull() || pJson["sHeadImage"].isNull() || pJson["iPhoneAuth"].isNull()
            || pJson["lPushManId"].isNull() || pJson["lPlatformID"].isNull()
            ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert tConvert;

    //    if( pJson["bBind"].isNull() ){
    //        RetJsonMsg::SetRetMsg(-1,pResponse,"请绑定微信或支付宝来注册");
    //        return -1;
    //    }


    UserDataEntity entity;


    entity.sAccount = tConvert.Json2String(pJson["sAccount"]);
    //entity.sPassword = tConvert.Json2String(pJson["sPassword"]);
    entity.sPassword = Md5ToLower( "T" + MyUtil::MakeRandNum(6) );
    entity.sNickName = tConvert.Json2String(pJson["sNickName"]);
    entity.iSex = tConvert.Json2Int(pJson["iSex"]);
    entity.sProvince = tConvert.Json2String(pJson["sProvince"]);
    entity.sCity = tConvert.Json2String(pJson["sCity"]);
    entity.sSignature = tConvert.Json2String(pJson["sSignature"]);
    entity.sEmail = tConvert.Json2String(pJson["sEmail"]);
    entity.sHeadImage = tConvert.Json2String(pJson["sHeadImage"]);
    entity.iPhoneAuth = tConvert.Json2Int(pJson["iPhoneAuth"]);
    entity.lPushManId = tConvert.Json2Long(pJson["lPushManId"]);
    entity.lPlatformId = tConvert.Json2Long(pJson["lPlatformID"]);
    entity.lCurrentPlatformId = entity.lPlatformId;
    entity.sGciID = "" ;
    entity.sGciPwd = "" ;
    entity.sGciPhone = "" ;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.lUpdateId = entity.lAddTime;

    if( !MyUtil::IsRightPhone(entity.sAccount))
    {
        RetJsonMsg::SetRetMsg(-1,pResponse,"手机号不合法");
        return -1;
    }

    //    if( entity.sAccount.substr(0,2) == "17" && Request::Phone138(entity.sAccount) != 0){
    //        RetJsonMsg::SetRetMsg(-1,pResponse,"你可能用了一个假的手机号");
    //        return -1;
    //    }

    if(!pJson["sBirthDay"].isNull() ){
        entity.sBirthDay = tConvert.Json2String(pJson["sBirthDay"]);
    }else{
        entity.sBirthDay = "1900-01-01 00:00:00";
    }

    string sCode = tConvert.Json2String(pJson["sCode"]);

    if( entity.sPassword.length() != 32 || entity.lPlatformId == 7613354){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    if( entity.lPushManId < 39 || ( entity.lPushManId > 39 && entity.lPushManId < 50000)){
        //不能关注系统号（39除外）
        entity.lPushManId = -1;
    }




    UserDataEntity uTmp;
    if(! pJson["sQqNum"].isNull()){
        string sQqNum = tConvert.Json2String(pJson["sQqNum"]);

        mUserDataDAL.GetEntityByQqNum(sQqNum,uTmp);
        if( (uTmp.sAccount != entity.sAccount) &&  !uTmp.sQqNum.empty()){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "QQ号已被别的手机号绑定";
            return -1;
        }
        entity.sQqNum = sQqNum;
    }else  if(! pJson["sWeixinNum"].isNull()){
        string sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
        mUserDataDAL.GetEntityByWeixinNum(sWeixinNum,uTmp);
        if((uTmp.sAccount != entity.sAccount) &&  !uTmp.sWeixinNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "微信号已被别的手机号绑定";
            return -1;
        }
        entity.sWeixinNum = sWeixinNum;
    }else  if(! pJson["sAliNum"].isNull()){
        string sAliNum = tConvert.Json2String(pJson["sAliNum"]);
        mUserDataDAL.GetEntityBysAliNum(sAliNum,uTmp);
        if((uTmp.sAccount != entity.sAccount) &&  !uTmp.sAliNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "支付宝号已被别的手机号绑定";
            return -1;
        }
        entity.sAliNum = sAliNum;
    }


    //免短信注册（符合另一个规则）
    if( Md5ToLower(entity.sAccount + "ttdsu") != tConvert.Json2String(pJson["sCode"])){
        //条件合法性-手机验证码
        string sMsg;
        int ret = IsUserPhoneCode(tConvert.Json2String(pJson["sAccount"]),tConvert.Json2String(pJson["sCode"]),sMsg);
        if ( 0 != ret  ) {
            RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
            return ret;
        }
    }


    //条件合法性-帐号是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExistByAccont( tConvert.Json2String(pJson["sAccount"]),bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::AddData mUserDataDAL.IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (true == bIsExist) {
        UserDataEntity tmp;
        mUserDataDAL.GetEntityByAccount(entity.sAccount,tmp);
        pResponse["entity"]["lUserId"] = tConvert.LongToStr( tmp.lUserId );
        RetJsonMsg::SetRetMsg(-9,pResponse);
        return -1;
    }

    //条件合法性-平台是否存在
    long lPlatformManId = 0;
    int  iVerify = -1,iState = -1;
    iRet = GetPlatformByID( tConvert.LongToStr(entity.lPlatformId),lPlatformManId,iVerify);


    if( iRet == 0 ){

    }else{
        iRet = GetPlatformState(tConvert.LongToStr(entity.lPlatformId),  iState);
        if( iState == 333){
            entity.lPlatformId = 100012;
            lPlatformManId = 39;
        }else {
            RetJsonMsg::SetRetMsg(-101,pResponse);
            return -101;
        }
    }

    //条件合法性-推广人是否存在
    if ( entity.lPushManId> 0 ) {
        bIsExist = false;
        if ( 0 != this->mUserDataDAL.IsExistByUserId( tConvert.LongToStr(entity.lPushManId),bIsExist ) ){
            appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::AddData mUserDataDAL.IsExistByUserId error");
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -1;
        }
        if (false == bIsExist) {
            entity.lPushManId = lPlatformManId;
        }
    }else{
        entity.lPushManId = lPlatformManId;
    }


    //appendlog(TTDLogger::LOG_DEBUG,"UserDataBLL::AddData GetMaxUserId=%ld",this->mUserDataDAL.GetMaxUserId());
    //获取最大用户ID
    long lUserId = this->mUserDataDAL.GetMaxUserId() + 1;
    entity.lUserId = lUserId;
    entity.sPrimaryKey = tConvert.LongToStr(lUserId);
    if (-1 == lUserId) {
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    //操作数据库(针对可能并发重复时，多操作几次)
    for (int i = 0; i < 6; ++i) {
        if (5 == i) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -1;
        }

        if( !IsRightNick(entity.sNickName))
        {
            entity.sNickName = tConvert.LongToStr(entity.lUserId);
        }
        //appendlog(TTDLogger::LOG_DEBUG,"UserDataBLL::AddData i=%d",i);
        int iRet = this->mUserDataDAL.Insert(entity);
        if ( 0 == iRet ) {
            break;
        } else if ( 1062 == iRet ) {
            long lUserId = this->mUserDataDAL.GetMaxUserId()+1;
            if (-1 == lUserId) {
                RetJsonMsg::SetRetMsg(-4,pResponse);
                return -1;
            }
            entity.sPrimaryKey = tConvert.LongToStr(lUserId);
            entity.lUserId = lUserId;
        } else {
            appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::AddData error");
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -1;
        }
    }
    appendlog(TTDLogger::LOG_INFO,"UserDataBLL::AddData :%ld sAccount:%s lPushManId:%ld lPlatformId:%ld Code:%s",
              entity.lUserId, entity.sAccount.c_str(), entity.lPushManId,entity.lPlatformId,sCode.c_str());


    //GiveActivityReward(entity.sAccount,1);
    pResponse["entity"]["sPassword"] = entity.sPassword;
    pResponse["entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
    pResponse["entity"]["sNickName"] = entity.sNickName;


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

//删除
//int UserDataBLL::Delete( const  Json::Value  & pJson  ,Json::Value & pResponse)
//{
//    //数据合法性
//    if( pJson["sPrimaryKey"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -1;
//    }
//    TConvert tConvert;
//    //条件合法性-是否存在
//    bool bIsExist = false;
//    if ( 0 != this->mUserDataDAL.IsExist( tConvert.Json2String( pJson["sPrimaryKey"]), bIsExist ) ){
//        appendlog(TTDLogger::LOG_ERROR,"FriendResquestBLL::Delete mFriendResquestDAL.IsExist error");
//        RetJsonMsg::SetRetMsg(-4,pResponse);
//        return -1;
//    }
//    if (false == bIsExist) {
//        RetJsonMsg::SetRetMsg(-5,pResponse);
//        return -1;
//    }

//    //参数转换
//    string sPrimaryKey = tConvert.Json2String(pJson["sPrimaryKey"]);

//    //操作数据库
//    if ( 0 != this->mUserDataDAL.Delete(sPrimaryKey)) {
//        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Delete  error");
//        RetJsonMsg::SetRetMsg(-4,pResponse);
//        return -1;
//    }

//    RetJsonMsg::SetRetMsg(0,pResponse);
//    return 0;
//}

/*
Description:修改普通资料
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Update( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["sPrimaryKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    TConvert tConvert;

    bool bGive = tConvert.Json2Bool( pJson["bGive"]);
    //条件合法性-是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExist( tConvert.Json2String( pJson["sPrimaryKey"]), bIsExist ) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (false == bIsExist) {
        RetJsonMsg::SetRetMsg(-5,pResponse);
        return -1;
    }


    UserDataEntity entity;

    entity.sPrimaryKey = tConvert.Json2String(pJson["sPrimaryKey"]);

    if( pJson["sNickName"].isNull() ){
        entity.sNickName = "-1nochange";
    }else{
        entity.sNickName = tConvert.Json2String(pJson["sNickName"]);

        if( !IsRightNick(entity.sNickName))
        {
            entity.sNickName = "女同学";
        }
    }
    if( pJson["iSex"].isNull() ){
        entity.iSex = -1;
    }else{
        entity.iSex = tConvert.Json2Int(pJson["iSex"]);
    }
    if( pJson["sProvince"].isNull() ){
        entity.sProvince = "-1nochange";
    }else{
        entity.sProvince = tConvert.Json2String(pJson["sProvince"]);
    }
    if( pJson["sCity"].isNull() ){
        entity.sCity = "-1nochange";
    }else{
        entity.sCity = tConvert.Json2String(pJson["sCity"]);
    }
    if( pJson["sSignature"].isNull() ){
        entity.sSignature = "-1nochange";
    }else{
        entity.sSignature = tConvert.Json2String(pJson["sSignature"]);
    }
    if( pJson["sEmail"].isNull() ){
        entity.sEmail = "-1nochange";
    }else{
        entity.sEmail = tConvert.Json2String(pJson["sEmail"]);
    }
    if( pJson["sHeadImage"].isNull() ){
        entity.sHeadImage = "-1nochange";
    }else{
        entity.sHeadImage = tConvert.Json2String(pJson["sHeadImage"]);
    }
    if( pJson["iPhoneAuth"].isNull() ){
        entity.iPhoneAuth = -1;
    }else{
        entity.iPhoneAuth = tConvert.Json2Int(pJson["iPhoneAuth"]);
    }
    if( pJson["sBirthDay"].isNull() ){
        entity.sBirthDay = "-1nochange";
    }else{
        entity.sBirthDay = tConvert.Json2String(pJson["sBirthDay"]);

        boost::regex expr("(\\d{4})-(\\d{2})-(\\d{2})");

        if( !boost::regex_match(entity.sBirthDay, expr))//匹配1，不匹配0
        {
            RetJsonMsg::SetRetMsg(-1,pResponse,"日期格式不合法");
            return -1;
        }
    }


    if ( 0 != this->mUserDataDAL.Update(entity.sPrimaryKey,entity)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    // if( bGive ){
    appendlog(TTDLogger::LOG_INFO,"UserDataBLL::Update GiveActivityReward give true");
    string IdFromToken = tConvert.Json2String( pJson["IdFromToken"] );
    // if( IdFromToken == entity.sPrimaryKey){
    appendlog(TTDLogger::LOG_INFO,"UserDataBLL::Update GiveActivityReward equal");
    UserDataEntity e;
    mUserDataDAL.GetEntityById(tConvert.StrToLong( entity.sPrimaryKey ),e);
    if( IsUserInfoPerfect(e)){
        appendlog(TTDLogger::LOG_INFO,"UserDataBLL::Update GiveActivityReward lUserId:%ld",e.lUserId);
        GiveActivityReward(e.sAccount,2);
    }
    // }
    // }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}


/*
Description:忘记密码
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::ForgetPassword( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    int iRet = -1;
    //数据合法性
    if( pJson["sAccount"].isNull() || pJson["sCode"].isNull() || pJson["sNewPassword"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sCode =  tConvert.Json2String(pJson["sCode"]);
    string sNewPassword =  tConvert.Json2String(pJson["sNewPassword"]);

    //条件合法性
    UserDataEntity userEntity;
    iRet = mUserDataDAL.GetEntity( "-1nochange",sAccount, "-1nochange",userEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    //条件合法性-手机验证码
    string sMsg;
    int ret = IsUserPhoneCode(sAccount,sCode,sMsg);
    if (0 != ret ) {
        RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
        return ret;
    }

    //操作数据库
    if ( 0 != this->mUserDataDAL.ModifyPwd(tConvert.LongToStr(userEntity.lUserId),sNewPassword)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::ForgetPassword error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:修改密码
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::ModifyPwd( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    //数据合法性
    int iRet = -1;
    if( pJson["sPrimaryKey"].isNull() || pJson["sOldPwd"].isNull() || pJson["sNewPassword"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sPrimaryKey = tConvert.Json2String(pJson["sPrimaryKey"]);
    string sOldPwd =  tConvert.Json2String(pJson["sOldPwd"]);
    string sNewPassword =  tConvert.Json2String(pJson["sNewPassword"]);


    UserDataEntity userEntity;
    iRet = mUserDataDAL.GetEntity(sPrimaryKey, "-1nochange", "-1nochange",userEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    if( userEntity.sPassword != sOldPwd){
        RetJsonMsg::SetRetMsg(-6,pResponse);
        return -6;
    }

    //操作数据库
    if ( 0 != this->mUserDataDAL.ModifyPwd(sPrimaryKey,sNewPassword)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::ModifyPwd error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:手机号是否被注册了
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::IsRegistered( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    //数据合法性
    if( pJson["sAccount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);

    //条件合法性-记录是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExistByAccont( sAccount,
                                                  bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::IsRegistered mUserDataDAL.IsExistByAccont error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (true == bIsExist) {
        RetJsonMsg::SetRetMsg(-9,pResponse);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:修改帐号(手机号)
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::UpdateAccount( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["sAccount"].isNull() || pJson["sPassword"].isNull() || pJson["sNewAccount"].isNull()
            ||  pJson["sCode"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iRet = -1;

    TConvert tConvert;

    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sPassword =  tConvert.Json2String(pJson["sPassword"]);
    string sNewAccount =  tConvert.Json2String(pJson["sNewAccount"]);
    string sCode =  tConvert.Json2String(pJson["sCode"]);


    //免短信注册（符合另一个规则）
    if( Md5ToLower(sNewAccount + "gci") != tConvert.Json2String(pJson["sCode"])){
        //条件合法性-手机验证码
        string sMsg;
        int ret = IsUserPhoneCode(sAccount,sCode,sMsg);
        if (0 !=ret) {
            RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
            return ret;
        }
    }

    //密码
    UserDataEntity userEntity;
    iRet = mUserDataDAL.GetEntity( "-1nochange", sAccount,"-1nochange",userEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    if( userEntity.sPassword != sPassword){
        RetJsonMsg::SetRetMsg(-6,pResponse);
        return -6;
    }

    //条件合法性-记录是否存在-新帐号-操作数据库
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExistByAccont( tConvert.Json2String(pJson["sNewAccount"]),
                                                  bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::UpdateAccount mUserDataDAL.IsExistByAccont error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (true == bIsExist) {
        RetJsonMsg::SetRetMsg(-3,pResponse);
        return -1;
    }


    //操作数据库
    if ( 0 != this->mUserDataDAL.UpdateAccount(sAccount,sNewAccount)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}
/*
Description:修改GCI会员
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::BandGciID( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["sGciID"].isNull() || pJson["sGciPwd"].isNull() || pJson["sGciPhone"].isNull()
            ||  pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    //参数转换
    UserDataEntity entity;

    //赋值
    TConvert tConvert;
    entity.sPrimaryKey = tConvert.Json2String(pJson["lUserId"]);
    entity.lUserId = tConvert.Json2Long(pJson["lUserId"]);
    entity.sGciID = tConvert.Json2String(pJson["sGciID"]);
    entity.sGciPwd = tConvert.Json2String(pJson["sGciPwd"]);
    entity.sGciPhone = tConvert.Json2String(pJson["sGciPhone"]);

    //check data
    if( entity.sGciPhone.empty() || entity.sGciPwd.empty() || entity.sGciID.empty() || entity.lUserId <= 0){
        RetJsonMsg::SetRetMsg(-2,pResponse);
        return -2;
    }
    string sUrl = "http://180.131.36.204/index.php?s=/Api/Appapi/boundUser";
    //条件合法性-记录是否存在-主键-操作数据库
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExist(    entity.sPrimaryKey,
                                             bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::UpdateAccount mUserDataDAL.IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (false == bIsExist) {
        RetJsonMsg::SetRetMsg(-5,pResponse);
        return -1;
    }

    //    if( !entity.ContentEntity.sGciID.empty()){
    //        appendlog(TTDLogger::LOG_ERROR," IServiceIBLL::BandGciID_async already band GciID:%s UserId:%ld",tConvert.ws2s( psGciID).c_str(),lUserId);
    //        return 10008;//GCi的己绑定码
    //    }
    //    long lId =DetailBLL::GetInstance()->GetUserIdByGciID(tConvert.ws2s(psGciID));
    //    if( lId > 0){
    //        appendlog(TTDLogger::LOG_ERROR," IServiceIBLL::BandGciID_async already band GciID:%s UserId:%ld",tConvert.ws2s( psGciID).c_str(),lUserId);
    //        return 10009;//GCi的己绑定码
    //    }

    Json::Value jsonPost;
    jsonPost["username"] =entity.sGciID;
    jsonPost["userpwd"] = entity.sGciPwd;
    jsonPost["userphone"] = entity.sGciPhone ;
    jsonPost["userBoundid"] =  tConvert.LongToStr(entity.lUserId);

    string sBeforemd5 = "userBoundid="  +tConvert.LongToStr( entity.lUserId ) + "&username=" +    entity.sGciID
            +"&userphone=" +  entity.sGciPhone +"&userpwd=" + entity.sGciPwd + "&key=ebec28d12beee51f504a97f7ecb92eca";
    string sMd5 =  Md5ToLower(sBeforemd5 );
    jsonPost["sign"] = sMd5;
    string sPost = jsonPost.toStyledString();
    string sResponse;
    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    TCurl tCurl;
    if ( 0 == tCurl.PostJosn(sUrl,httpHeaderVector,sPost,sResponse)){
        Json::Value jsonRes ;
        Json::Reader reader;
        reader.parse(sResponse.c_str(),jsonRes);
        // bool bState =  tConvert.Json2Bool( jsonRes["status"] );
        int iCode =  tConvert.Json2Int( jsonRes["code"] );

        if(  iCode == 0  ){


            if ( 0 != this->mUserDataDAL.Update(entity.sPrimaryKey,entity)) {
                appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
                RetJsonMsg::SetRetMsg(-4,pResponse);
                return -1;
            }
            RetJsonMsg::SetRetMsg(0,pResponse);
            return 0;
        }else{
            RetJsonMsg::SetRetMsg(iCode,pResponse);
            return -1;
        }
    }
    RetJsonMsg::SetRetMsg(-1,pResponse);
    return -1;
}


/*
Description:绑定第三方账号
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::BandThirdId( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if(  pJson["sAccount"].isNull() || pJson["sPassword"].isNull() ||
         (pJson["sQqNum"].isNull() && pJson["sWeixinNum"].isNull() &&  pJson["sAliNum"].isNull()) ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    UserDataEntity entity;

    TConvert tConvert;int iRet;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sPassword = tConvert.Json2String(pJson["sPassword"]);



    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount,"-1nochange",entity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }



    //check password
    if( entity.sPassword !=  sPassword){
        RetJsonMsg::SetRetMsg(-6,pResponse);
        return -1;
    }




    UserDataEntity uTmp;
    if(! pJson["sQqNum"].isNull()){
        string sQqNum = tConvert.Json2String(pJson["sQqNum"]);

        mUserDataDAL.GetEntityByQqNum(sQqNum,uTmp);
        if( (uTmp.sAccount != sAccount) &&  !uTmp.sQqNum.empty()){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "已被其他帐号绑定";
            return -1;
        }
        mUserDataDAL.UpQqNum(sAccount,sQqNum);
    }
    if(! pJson["sWeixinNum"].isNull()){
        string sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
        mUserDataDAL.GetEntityByWeixinNum(sWeixinNum,uTmp);
        if((uTmp.sAccount != sAccount) &&  !uTmp.sWeixinNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "已被其他帐号绑定";
            return -1;
        }
        mUserDataDAL.UpWeixinNum(sAccount,sWeixinNum);
    }
    if(! pJson["sAliNum"].isNull()){
        string sAliNum = tConvert.Json2String(pJson["sAliNum"]);
        mUserDataDAL.GetEntityBysAliNum(sAliNum,uTmp);
        if((uTmp.sAccount != sAccount) &&  !uTmp.sAliNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "已被其他帐号绑定";
            return -1;
        }
        mUserDataDAL.UpAliNum(sAccount,sAliNum);
    }



    RetJsonMsg::SetRetMsg(0,pResponse);
    return -1;
}


/*
Description:解绑第三方账号
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::UnBandThird( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    // RetJsonMsg::SetRetMsg(-1,pResponse,"无法解绑");
    // return -1;

    if(  pJson["sAccount"].isNull() || (pJson["sQqNum"].isNull() && pJson["sWeixinNum"].isNull()
                                        && pJson["sAliNum"].isNull())){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;int iRet;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);

    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntityByAccount(sAccount,entity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }

    UserDataEntity uTmp;
    if(! pJson["sQqNum"].isNull()){
        string sQqNum = tConvert.Json2String(pJson["sQqNum"]);
        if( entity.sQqNum != sQqNum){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "解绑有误";
            return -1;
        }
        mUserDataDAL.UpQqNum(sAccount,"");
    }
    if(! pJson["sWeixinNum"].isNull()){
        string sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
        if((entity.sWeixinNum != sWeixinNum) &&  !uTmp.sWeixinNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "解绑有误";
            return -1;
        }
        mUserDataDAL.UpWeixinNum(sAccount,"");
    }
    if(! pJson["sAliNum"].isNull()){
        string sAliNum = tConvert.Json2String(pJson["sAliNum"]);
        if((entity.sAliNum != sAliNum) &&  !uTmp.sAliNum.empty() ){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            pResponse["sMsg"] = "解绑有误";
            return -1;
        }
        mUserDataDAL.UpAliNum(sAccount,"");
    }



    RetJsonMsg::SetRetMsg(0,pResponse);
    return -1;

}


/*
Description:第三方Id登录
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::LoginByThird( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet ;
    //数据合法性
    if( pJson["sQqNum"].isNull() && pJson["sWeixinNum"].isNull() && pJson["sAliNum"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sQqNum,sWeixinNum,sAliNum;
    if(!pJson["sQqNum"].isNull()){
        sQqNum = tConvert.Json2String(pJson["sQqNum"]);
    }else if(!pJson["sWeixinNum"].isNull()){
        sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
    }else if(!pJson["sAliNum"].isNull()){
        sAliNum = tConvert.Json2String(pJson["sAliNum"]);
    }

    UserDataEntity entity;

    if( ! sQqNum.empty() ){
        iRet = mUserDataDAL.GetEntityByQqNum(sQqNum,entity);

    }else if( ! sWeixinNum.empty()){
        iRet = mUserDataDAL.GetEntityByWeixinNum(sWeixinNum,entity);
    }else if(! sAliNum.empty() ){
        iRet = mUserDataDAL.GetEntityBysAliNum(sAliNum,entity);
    }

    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        if( iRet == -5)
            pResponse["sMsg"] = "第三方Id不存在";
        return iRet;
    }
    Data2Json(entity,pResponse);
    long lCurTime = tConvert.GetCurrentTime();
    pResponse["entity"]["lLoginTime"] = tConvert.LongToStr(lCurTime);
    pResponse["entity"]["sPassword"] = entity.sPassword;
    pResponse["entity"]["sToken"] = Token::MakeLoginToken(tConvert.LongToStr(entity.lUserId));


    UserStateDAL userStatedal;

    bool b = false;
    userStatedal.IsExistByUserId( tConvert.LongToStr( entity.lUserId),b);
    if( !b){
        Json::Value jWalletRespon;
        MoneyBagServiceAddByRegister(entity.sAccount,entity.sNickName,jWalletRespon);
    }

    userStatedal.SetLoginTime(entity.lUserId, lCurTime);


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:查询绑定信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::BindInfo( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet ;
    //数据合法性
    if( pJson["sQqNum"].isNull() && pJson["sWeixinNum"].isNull() && pJson["sAccount"].isNull()
            && pJson["sAliNum"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sQqNum,sWeixinNum,sAccount,sAliNum;
    if(!pJson["sQqNum"].isNull()){
        sQqNum = tConvert.Json2String(pJson["sQqNum"]);
    }else if(!pJson["sWeixinNum"].isNull()){
        sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
    }else if(!pJson["sAccount"].isNull()){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }else if(!pJson["sAliNum"].isNull()){
        sAliNum = tConvert.Json2String(pJson["sAliNum"]);
    }

    UserDataEntity entity;

    if( ! sQqNum.empty() ){
        iRet = mUserDataDAL.GetEntityByQqNum(sQqNum,entity);

    }else if( ! sWeixinNum.empty()){
        iRet = mUserDataDAL.GetEntityByWeixinNum(sWeixinNum,entity);
    }else if( ! sAccount.empty()){
        iRet = mUserDataDAL.GetEntityByAccount(sAccount,entity);
    }else if( ! sAliNum.empty()){
        iRet = mUserDataDAL.GetEntityBysAliNum(sAliNum,entity);
    }

    if( iRet != 0 && iRet != -5){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }
    pResponse["sAccount"] = entity.sAccount;
    pResponse["sQqNum"] = entity.sQqNum;
    pResponse["sWeixinNum"] = entity.sWeixinNum;
    pResponse["sAliNum"] = entity.sAliNum;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:获取我开的店
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::MyShops( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    //数据合法性
    if(  pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    pResponse["list"] = UserDataCache::GetInstance()->GetMyShops( tConvert.Json2Long(pJson["lUserId"]));

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}


/*
Description:变更用户等级
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::SetGrade( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet = -1;
    //数据合法性
    if(  pJson["sAccount"].isNull() || pJson["iLevel"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    int iLevel = tConvert.Json2Int( pJson["iLevel"] );

    UserDataEntity userDataEntity;
    userDataEntity.lUserId = -1;
    iRet = mUserDataDAL.GetEntityByAccount(sAccount,userDataEntity);

    if( (iRet == 0 ) && ( userDataEntity.lUserId > 0 )){

        if( 0 == mUserGradeDal.IsExist(userDataEntity.lUserId)){
            iRet = mUserGradeDal.UpdateLevel(userDataEntity.lUserId, iLevel);
        }else{
            iRet = mUserGradeDal.Add(userDataEntity.lUserId, iLevel);
        }
    }else{

    }


    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}

/*
Description:分批获取会员
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetVips( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    //数据合法性
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iPage = tConvert.Json2Int( pJson["iPage"] );
    int iCount = tConvert.Json2Int( pJson["iCount"] );


    UserGradeList lst;
    mUserGradeDal.GetVip( iPage , iCount, lst);

    UserDataList userDataLst;longs lUserIds;
    for( UserGradeList::iterator it = lst.begin(); it != lst.end(); it++){
        UserGradeEntity & entity = *it;
        lUserIds.push_back(entity.lUserId);
    }
    mUserDataDAL.GetUserInfoList(lUserIds , userDataLst  );

    int i = 0;
    for( UserGradeList::iterator it = lst.begin(); it != lst.end(); it++){
        UserGradeEntity & entity = *it;

        UserDataEntity  user;
        if(UserUtil::GetUserData( entity.lUserId, userDataLst,user) ){
            pResponse["list"][i]["sAccount"] = user.sAccount;
        }else{
            pResponse["list"][i]["sAccount"] = "";
        }
        pResponse["list"][i]["lUserId"] = tConvert.LongToStr( entity.lUserId);
        pResponse["list"][i]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
        pResponse["list"][i]["sAccount"] = user.sAccount;
        i++;
    }
    if( lst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:查询用户的等级
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetGrade( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet = -1;
    //数据合法性
    if(  pJson["sAccount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sAccount = tConvert.Json2String(pJson["sAccount"]);

    UserDataEntity userDataEntity;
    userDataEntity.lUserId = -1;
    iRet = mUserDataDAL.GetEntityByAccount(sAccount,userDataEntity);

    int iLevel;

    if( (iRet == 0 ) && ( userDataEntity.lUserId > 0 )){


        iRet = mUserGradeDal.GetLevel(userDataEntity.lUserId, iLevel);
        if( iRet == -5){
            iLevel = 0;
            iRet = 0;
        }
    }else{

    }

    pResponse["iLevel"] = iLevel;
    pResponse["sAccount "] = userDataEntity.sAccount;


    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}

/*
Description:判断用户密码
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Pass(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet;
    //数据合法性
    if( (pJson["sAccount"].isNull() && pJson["lUserId"].isNull()) || pJson["sPassword"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    UserDataEntity entity;
    if(!pJson["sAccount"].isNull()){
        string sAccount = tConvert.Json2String(pJson["sAccount"]);
        iRet = mUserDataDAL.GetEntityByAccount(sAccount,entity);
    }else if( !pJson["lUserId"].isNull()){
        long lUserId = tConvert.Json2Long( pJson["lUserId"]);
        iRet = mUserDataDAL.GetEntityById(lUserId,entity);
    }

    string sPassword = tConvert.Json2String(pJson["sPassword"]);


    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        if( iRet == -5)
            pResponse["sMsg"] = "用户不存在";
        return -1;
    }

    if( entity.sPassword != sPassword){
        RetJsonMsg::SetRetMsg(-6,pResponse);
        return -1;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}

/*
Description:获取注册量
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetRegisters(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet = 0;
    if( pJson["sYear"].isNull() && pJson["sMonth"].isNull() &&  pJson["sDay"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    CommonStructVector lst;
    vector<string> splits ;
    if( !pJson["sYear"].isNull()){

        string sYear = tConvert.Json2String(pJson["sYear"]);//"2015~2017"
        tConvert.Split(sYear,"~",splits);
        string sStart = splits[0];
        string sEnd = splits[1];
        iRet = mUserDataDAL.GetRegistersByYear(sStart,sEnd,lst);

    }else if(!pJson["sMonth"].isNull()){
        string sMonth = tConvert.Json2String(pJson["sMonth"]);//"2015-08~2017-04"

        tConvert.Split(sMonth,"~",splits);
        string sStart = splits[0];
        string sEnd = splits[1];

        iRet = mUserDataDAL.GetRegistersByMonth(sStart,sEnd,lst);
    }else if(!pJson["sDay"].isNull()){
        string sDay = tConvert.Json2String(pJson["sDay"]);//"2015-08-02~2017-04-01"

        tConvert.Split(sDay,"~",splits);
        string sStart = splits[0];
        string sEnd = splits[1];
        iRet = mUserDataDAL.GetRegistersByDay(sStart,sEnd,lst);
    }


    int i = 0;
    for( CommonStructVector::iterator it = lst.begin(); it != lst.end(); it++){
        CommonStruct & entity = *it;


        pResponse["list"][i]["sV1"] = entity.v1;
        pResponse["list"][i]["sV2"] =  entity.v2;
        i++;
    }
    if( lst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:获取每日待激活数量
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetUnhotCount(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet = 0;
    if(  pJson["sDay"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    CommonStructVector lst;


    iRet = mUserDataDAL.GetUnHotCount(lst);



    int i = 0;
    for( CommonStructVector::iterator it = lst.begin(); it != lst.end(); it++){
        CommonStruct & entity = *it;


        pResponse["list"][i]["sV1"] = entity.v1;
        pResponse["list"][i]["sV2"] =  entity.v2;
        i++;
    }
    if( lst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:如果不存在用户，则自动注册
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Auto(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["sAccount"].isNull() || pJson["lPushManId"].isNull() || pJson["lPlatformId"].isNull()
         || pJson["sOpenId"].isNull() ||  pJson["sAccessToken"].isNull()  |  pJson["src"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sOpenId = tConvert.Json2String(pJson["sOpenId"]);
    string sAccessToken = tConvert.Json2String(pJson["sAccessToken"]);
    string src = tConvert.Json2String(pJson["src"]);

    string unioid;
    if( src == "1"){
        unioid = GetWeiXinUserInfo(sOpenId,sAccessToken);
    }else {
        unioid = GetAlipayUserInfo(sAccessToken);
    }
    if( unioid.empty()){
        RetJsonMsg::SetRetMsg(-1,pResponse,"验证第三方信息失败");
        return -1;
    }




    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);
    if( iRet != 0){
        if( iRet == -5 ){

            Json::Value j,r;
            j["sCode"] = Md5ToLower(sAccount + "ttdsu");
            j["sAccount"] = sAccount;
            j["sPassword"] = Md5ToLower("T888888");
            j["sNickName"] = "";
            j["iSex"] = 1;
            j["sProvince"] = "";
            j["sCity"] = "";
            j["sSignature"] = "";
            j["sEmail"] = "";
            j["sHeadImage"] = "";
            j["iPhoneAuth"] = 1;
            j["lPushManId"] = pJson["lPushManId"];
            j["lPlatformID"] = pJson["lPlatformId"];
            j["bBind"] = true;

            if( src == "1"){
                j["sWeixinNum"] = unioid;
            }else {
                j["sAliNum"] = unioid;
            }
            iRet = AddData(j,r);

            if( iRet != 0){
                string sMsg = tConvert.Json2String ( r["sMsg"] );
                pResponse = r;
                RetJsonMsg::SetRetMsg(iRet,pResponse,sMsg);
                return iRet;
            }

        }else{
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return iRet;

        }
    }else{
        RetJsonMsg::SetRetMsg(-9,pResponse,"手机号已注册");
        return iRet;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:获取N层上级推广者
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetPushUpChain(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if( pJson["iLevel"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    if( pJson["sAccount"].isNull() && pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    int iLevel = tConvert.Json2Int(pJson["iLevel"]);
    long lUserId =-1;string sAccount;
    if( !pJson["lUserId"].isNull()  ){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }
    if( !pJson["sAccount"].isNull()  ){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }

    if( (iLevel <= 0 || iLevel >20) ){
        RetJsonMsg::SetRetMsg(-1,pResponse,"参数取值有误");
        return -1;
    }

    UserDataEntity entity;entity.lUserId = -1;
    if( lUserId > -1){
        iRet = mUserDataDAL.GetEntity(tConvert.LongToStr(lUserId),"-1nochange","-1nochange",entity);
    }else{
        iRet = mUserDataDAL.GetEntity("-1nochange",sAccount,"-1nochange",entity);
    }
    if( iRet != 0 ){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    int iCurLevel = 0;
    for( int i = 0 ; i < iLevel; i++){
        if( entity.lPushManId <=0){
            break;
        }
        entity.lUserId = -1;
        iRet = mUserDataDAL.GetEntity(tConvert.LongToStr(entity.lPushManId),"-1nochange","-1nochange",entity);
        if( iRet == 0 && entity.lUserId != -1){
            pResponse["list"][iCurLevel]["lUserId"] =tConvert.LongToStr( entity.lUserId);
            pResponse["list"][iCurLevel]["sAccount"] = entity.sAccount;
            pResponse["list"][iCurLevel]["sNickName"] = entity.sNickName;
            iCurLevel ++;

        }else{
            break;
        }
    }

    if( iCurLevel == 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:每日签到
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Sign(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;


    if( pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    if( 0 == mUserSignDAL.IsExist(lUserId,  tConvert.TimeToStr( tConvert.GetCurrentTime()/1000000 ).substr(0,10))){
        RetJsonMsg::SetRetMsg(1,pResponse);
        pResponse["sMsg"] = "今天已签到";
        return 1;
    }

    iRet = mUserSignDAL.Insert(lUserId,tConvert.GetCurrentTime());

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

/*
Description:更改我的关注
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Attention( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull() || pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    UserDataEntity entity;

    entity.sPrimaryKey = tConvert.Json2String(pJson["lUserId"]);
    entity.lUserId = tConvert.Json2Long(pJson["lUserId"]);
    entity.lCurrentPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);


    if( entity.lCurrentPlatformId <= 0 || entity.lUserId <= 0){
        RetJsonMsg::SetRetMsg(-2,pResponse);
        return -2;
    }

    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExist(   entity.sPrimaryKey,
                                            bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Attention mUserDataDAL.IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (false == bIsExist) {
        RetJsonMsg::SetRetMsg(-5,pResponse);
        return -1;
    }
    long lPlatformManId=0;
    int  iVerify;
    iRet = GetPlatformByID(tConvert.LongToStr( entity.lCurrentPlatformId),lPlatformManId,iVerify);
    if( iRet != 0 || lPlatformManId <=0){
        RetJsonMsg::SetRetMsg(-101,pResponse);
        return -101;
    }
    if ( 0 != this->mUserDataDAL.Update(entity.sPrimaryKey,entity)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Attention error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    appendlog(TTDLogger::LOG_INFO,"UserDataBLL::Attention sucessful lUserId:%ld lCurrentPlatformId:%ld",entity.lUserId,entity.lCurrentPlatformId);
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:平台粉丝
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::PlatformFans(const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    TConvert tConvert;
    //数据合法性
    if( pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    UserDataList UserDataLst;
    if ( 0 != this->mUserDataDAL.PlatformFans( lPlatformId ,UserDataLst)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::PlatformFans error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    int iCount = 0;
    while(iCount <  UserDataLst.size()){

        UserDataEntity & entity = UserDataLst[ iCount ];
        pResponse["list"][iCount ]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["list"][iCount ]["sNickName"] = entity.sNickName;
        pResponse["list"][iCount ]["sHeadImage"] =   entity.sHeadImage;

        iCount++;
    }
    if( UserDataLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:获取粉丝列表
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetFans( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;

    if( pJson["Id"].isNull() || pJson["iType"].isNull()  || pJson["iPage"].isNull()  || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long Id = tConvert.Json2Long(pJson["Id"]);
    int iType = tConvert.Json2Long(pJson["iType"]);
    int iPage = tConvert.Json2Long(pJson["iPage"]);
    int inum = tConvert.Json2Long(pJson["iCount"]);



    long lUserId = -1;
    string sNickName = "null",sTimeStart = "null",sTimeEnd = "null",sAccount = "null",sWeixinNum = "null",sAliNum= "null";

    string sHot;
    if( !pJson["sHot"].isNull()){
        sHot = tConvert.Json2String(pJson["sHot"]);
    }
    if( !pJson["lUserId"].isNull()){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }
    if( !pJson["sNickName"].isNull()){
        sNickName = tConvert.Json2String(pJson["sNickName"]);
    }
    if( !pJson["sTimeStart"].isNull()){
        sTimeStart = tConvert.Json2String(pJson["sTimeStart"]);
    }
    if( !pJson["sTimeEnd"].isNull()){
        sTimeEnd = tConvert.Json2String(pJson["sTimeEnd"]);
    }
    if( !pJson["sAccount"].isNull()){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }
    if( !pJson["sWeixinNum"].isNull()){
        sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
    }
    if( !pJson["sAliNum"].isNull()){
        sAliNum = tConvert.Json2String(pJson["sAliNum"]);
    }

    UserDataList UserDataLst;int iTotal = 0;
    if ( 0 != this->mUserDataDAL.GetFans(Id,iType,iPage,inum ,sHot,lUserId,sNickName,sAccount
                                         ,sTimeStart,sTimeEnd,sWeixinNum,sAliNum,UserDataLst,iTotal)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::PlatformFans error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    int iCount = 0;
    while(iCount <  UserDataLst.size()){

        UserDataEntity & entity = UserDataLst[ iCount ];
        pResponse["list"][iCount ]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["list"][iCount ]["sNickName"] = entity.sNickName;
        pResponse["list"][iCount ]["sHeadImage"] =   entity.sHeadImage;
        pResponse["list"][iCount ]["sAccount"] =   entity.sAccount;
        pResponse["list"][iCount ]["sAddTime"] =  tConvert.TimeToStr(entity.lAddTime/1000000 );
        pResponse["list"][iCount ]["sWeixinNum"] =   entity.sWeixinNum;
        pResponse["list"][iCount ]["sAliNum"] =   entity.sAliNum;
        string sSrc = "普通";
        if(entity.sAccount.at(0) == '2'){
            sSrc = "官方";
        }
        pResponse["list"][iCount ]["sSrc"] = sSrc;

        iCount++;
    }
    if( UserDataLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    pResponse["iTotal"] = iTotal;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:获取生产的粉丝列表
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetMakedFans( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;

    if(  pJson["iPage"].isNull()  || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    int iPage = tConvert.Json2Long(pJson["iPage"]);
    int inum = tConvert.Json2Long(pJson["iCount"]);

    long lUserId = -1, lPlatformId = -1, lPushManId = -1;
    string sNickName = "null",sTimeStart = "null",sTimeEnd = "null",sAccount = "null",sWeixinNum = "null",sAliNum= "null";

    if( !pJson["lUserId"].isNull()){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }
    if( !pJson["lPlatformId"].isNull()){
        lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    }
    if( !pJson["lPushManId"].isNull()){
        lPushManId = tConvert.Json2Long(pJson["lPushManId"]);
    }
    if( !pJson["sNickName"].isNull()){
        sNickName = tConvert.Json2String(pJson["sNickName"]);
    }
    if( !pJson["sTimeStart"].isNull()){
        sTimeStart = tConvert.Json2String(pJson["sTimeStart"]);
    }
    if( !pJson["sTimeEnd"].isNull()){
        sTimeEnd = tConvert.Json2String(pJson["sTimeEnd"]);
    }
    if( !pJson["sAccount"].isNull()){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }
    if( !pJson["sWeixinNum"].isNull()){
        sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
    }
    if( !pJson["sAliNum"].isNull()){
        sAliNum = tConvert.Json2String(pJson["sAliNum"]);
    }

    UserDataList UserDataLst;int iTotal = 0;
    if ( 0 != this->mUserDataDAL.GetMakedFans(iPage,inum ,lUserId,lPlatformId,lPushManId,sNickName,sAccount
                                              ,sTimeStart,sTimeEnd,sWeixinNum,sAliNum,UserDataLst,iTotal)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::GetMakedFans error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    int iCount = 0;
    while(iCount <  UserDataLst.size()){

        UserDataEntity & entity = UserDataLst[ iCount ];
        pResponse["list"][iCount ]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["list"][iCount ]["sNickName"] = entity.sNickName;
        pResponse["list"][iCount ]["sHeadImage"] =   entity.sHeadImage;
        pResponse["list"][iCount ]["sAccount"] =   entity.sAccount;
        pResponse["list"][iCount ]["sAddTime"] =  tConvert.TimeToStr(entity.lAddTime/1000000 );
        pResponse["list"][iCount ]["sWeixinNum"] =   entity.sWeixinNum;
        pResponse["list"][iCount ]["sAliNum"] =   entity.sAliNum;
        pResponse["list"][iCount ]["lPlatformId"] = tConvert.LongToStr( entity.lCurrentPlatformId );
        pResponse["list"][iCount ]["lPushManId"] = tConvert.LongToStr( entity.lPushManId );

        iCount++;
    }
    if( UserDataLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    pResponse["iTotal"] = iTotal;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

//获取我的视频
int UserDataBLL::GetVideo( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet = -1;
    //数据合法性
    if( pJson["userId"].isNull()  ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -1;
    }
    pResponse["result"].resize(0);

    long userId = tConvert.Json2Long(pJson["userId"]);


    UserDataEntity userDataEntity;
    mUserDataDAL.GetEntityById(userId,userDataEntity);

    string sType;
    Json::Value j = UserDataCache::GetInstance()->GetMyNearShops(userId);
    Json::Value j2 = UserDataCache::GetInstance()->GetMyShops(userId);
    if(j.size() > 0){
        sType += tConvert.IntToStr(enum_user_role_nearshop) + ",";
        sType += tConvert.IntToStr(enum_user_role_fansbank) + ",";
    }
    if( j2.size() > 0){
        sType += tConvert.IntToStr(enum_user_role_gongying)+ ",";
        sType += tConvert.IntToStr(enum_user_role_fansbank) + ",";
    }

    string sWalletUrl = "http://guanli.ttdomo.com/GetDataInterface/GetRoleByPhone.aspx";
    Json::Value jWalletPost,jWalletRespons;
    jWalletPost["sPhone"] = userDataEntity.sAccount;
    iRet = Request::WalletHttpPost(sWalletUrl,jWalletPost,jWalletRespons);

    bool bIsPerAgent  = false,bIsZoneAgent = false;
    if( iRet == 0){
        bIsPerAgent = tConvert.Json2Bool(  jWalletRespons["bIsPerAgent"] );
        bIsZoneAgent = tConvert.Json2Bool(  jWalletRespons["bIsZoneAgent"] );
    }

    if( bIsPerAgent){
        sType += tConvert.IntToStr(enum_user_role_peragent)+ ",";
    }
    if( bIsZoneAgent){
        sType += tConvert.IntToStr(enum_user_role_zoneagent)+ ",";
    }


    sType = sType.substr(0,sType.length() -1);

    UserVideoLst videos;
    mUserVideoDAL.GetData(sType,videos);
    for( int i = 0; i < videos.size() ; i++){
        UserVideoEntity & entity = videos[i];
        pResponse["result"][i]["content"] =   entity.content;
        pResponse["result"][i]["icon"] =   entity.icon;
        pResponse["result"][i]["title"] =   entity.title;
        pResponse["result"][i]["url"] =   entity.url;
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//判断用户资料是否完整
int UserDataBLL::Perfect( const  Json::Value  & pJson  ,Json::Value & pResponse )
{

    if( pJson["sAccount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;


    string sAccount;
    sAccount = tConvert.Json2String(pJson["sAccount"]);

    //获取用户内容
    UserDataEntity entity;
    iRet = this->mUserDataDAL.GetEntityByAccount(sAccount,entity);
    if ( 0 != iRet ){
        RetJsonMsg::SetRetMsg(iRet ,pResponse);

        if( iRet == -5){
            pResponse["sMsg"] = "目标用户不存在";
        }
        return -1;
    }
    RetJsonMsg::SetRetMsg(0 ,pResponse);
    bool bPerfect = IsUserInfoPerfect(entity);
    pResponse["bPerfect"]  = bPerfect;
    if(!bPerfect )
        pResponse["sTips"]  = "尊敬的用户,您还有一些基本资料没填写完善,完善资料可送50购物卡";
    else
        pResponse["sTips"]  = "恭喜你，已经完成了基本资料的补全，我们将会赠送50购物卡作为奖励哦";
    return 0;
}

//解冻
int UserDataBLL::Jiedong(const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    TConvert tConvert;
    int iRet = -1;


    string sAccount;
    sAccount = tConvert.Json2String(pJson["account"]);
    if( !MyUtil::IsRightPhone(sAccount))
    {
        RetJsonMsg::SetRetMsg2(-1,pResponse,"手机号不合法");
        return -1;
    }

    UserDataEntity userEntity;
    mUserDataDAL.GetEntityByAccount(sAccount,userEntity);
    if(userEntity.sAccount != sAccount){
        RetJsonMsg::SetRetMsg2(iRet,pResponse,"手机号未注册");
        return iRet;
    }
    if(userEntity.iState == 0){
        RetJsonMsg::SetRetMsg2(iRet,pResponse,"这个手机号没有被冻结");
        return iRet;
    }

    iRet = mUserDataDAL.Jeidong(sAccount);
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//生成假粉丝
int UserDataBLL::Make(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["iCount"].isNull() || pJson["sBegin"].isNull()
            ||  pJson["sTimeStart"].isNull() || pJson["sTimeEnd"].isNull()
            || pJson["lPlatformId"].isNull() || pJson["lPushManId"].isNull()
            || pJson["sCode"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iCount = tConvert.Json2Int(pJson["iCount"]);
    string sBegin = tConvert.Json2String(pJson["sBegin"]);
    string sTimeStart = tConvert.Json2String(pJson["sTimeStart"]);
    string sTimeEnd = tConvert.Json2String(pJson["sTimeEnd"]);
    if( sTimeStart.length() == 10){
        sTimeStart +=  " 00:00:00";
    }
    if( sTimeEnd.length() == 10){
        sTimeEnd +=  " 23:59:59";
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    long  lPushManId = tConvert.Json2Long(pJson["lPushManId"]);
    long lTimeStart = (long)tConvert.StrToTime(sTimeStart.c_str());
    long lTimeEnd =(long) tConvert.StrToTime(sTimeEnd.c_str());
    if( lTimeEnd<=lTimeStart){
        RetJsonMsg::SetRetMsg(-1,pResponse,"结束时间要大于开始时间");
        return -1;
    }
    string sCode = tConvert.Json2String(pJson["sCode"]);
    double dPrice = 2.5;
    if( !pJson["dPrice"].isNull() ){//指定粉丝价值
        dPrice = tConvert.Json2Double(pJson["dPrice"]);
        if( dPrice < 0 || dPrice > 100.0){
            RetJsonMsg::SetRetMsg(-1,pResponse,"指定粉丝价值必须大于0小于100");
            return -1;
        }
    }

    if( sCode != "better.9394"){
        RetJsonMsg::SetRetMsg(-1,pResponse,"密码错误");
        return -1;
    }
    if( sBegin.at(0) != '2' || sBegin.size() !=3){
        RetJsonMsg::SetRetMsg(-1,pResponse,"手机号开头错误");
        return -1;
    }
    if( iCount > 5000){
        RetJsonMsg::SetRetMsg(-1,pResponse,"一次数量不能大于5000");
        return -1;
    }

    //平台是否存在
    long lPlatformManId = 0;
    int  iVerify = -1,iState = -1;
    iRet = GetPlatformByID( tConvert.LongToStr(lPlatformId),lPlatformManId,iVerify);
    if( iRet == 0 ){

    }else{
        iRet = GetPlatformState(tConvert.LongToStr(lPlatformId),  iState);
        if( iState == 333){

            RetJsonMsg::SetRetMsg(-101,pResponse);
            return -101;
        }else {
            RetJsonMsg::SetRetMsg(-101,pResponse);
            return -101;
        }
    }

    //推广人是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExistByUserId( tConvert.LongToStr(lPushManId),bIsExist ) ){
        RetJsonMsg::SetRetMsg(-1,pResponse,"推广人不存在");
        return -1;
    }
    if (false == bIsExist) {
        RetJsonMsg::SetRetMsg(-1,pResponse,"推广人不存在");
        return -1;
    }



    for( int i= 0; i< iCount ; ){

        UserDataEntity entity;
        entity.iPhoneAuth = 0;
        entity.iSex = 0;
        entity.iState = 0;
        entity.iSync = 0;
        entity.lAddTime = ( lTimeStart + MyUtil::GetRand(lTimeEnd - lTimeStart))*1000000;
        entity.lUpTime = entity.lAddTime;
        entity.lUpdateId = entity.lAddTime;
        entity.lCurrentPlatformId= lPlatformId;
        entity.lPlatformId = lPlatformId;
        entity.lPushManId = lPushManId;
        entity.sNickName = "未填昵称";
        entity.sBirthDay = "1900-01-01";
        entity.sHeadImage = "";
        entity.sAccount = sBegin +  MyUtil::MakeRandNum(8);
        entity.sAliNum = "";
        entity.sCity = "";
        entity.sEmail = "";
        entity.sGciID = "";
        entity.sGciPhone = "";
        entity.sPassword = "0a69d8c89e305fe071e2057fae7d04b0";
        entity.sProvince = "";
        entity.sWeixinNum = "";
        entity.iHot = 1;
        long lUserId = this->mUserDataDAL.GetMaxUserId() + 1;
        entity.lUserId = lUserId;
        entity.sPrimaryKey = tConvert.LongToStr(lUserId);

        if(Make(entity) == 0){
            if( mFansBankUserValueDAL.IsExist(entity.lUserId,enum_kvalue_type_platform) != 0){
                mFansBankUserValueDAL.Add(entity.lUserId,dPrice,enum_kvalue_type_platform);
            }else{
                mFansBankUserValueDAL.UpValue(entity.lUserId,dPrice,enum_kvalue_type_platform);
            }
            if( mFansBankUserValueDAL.IsExist(entity.lUserId,enum_kvalue_type_push) != 0){
                mFansBankUserValueDAL.Add(entity.lUserId,dPrice,enum_kvalue_type_push);
            }else{
                mFansBankUserValueDAL.UpValue(entity.lUserId,dPrice,enum_kvalue_type_push);
            }
            i++;
        }else{

        }
    }
    FansBankBLL fb ;
    fb.UpdatePrice(lPlatformId,enum_kvalue_type_platform);
    fb.UpdatePrice(lPushManId,enum_kvalue_type_push);
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}

//获取个代列表(粉丝信息)
int UserDataBLL::PeragentLst(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    //数据合法性
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);


    string sUrl = "http://guanli.ttdomo.com/GetDataInterface/PersonalAgent/GetPerAgentList.aspx";
    Json::Value  jsonPost,  jsonRespons;
    jsonPost["iPageIndex"] = iPage;
    jsonPost["iPageSize"] = iCount;
    if(!pJson["sPhone"].isNull() ){
        jsonPost["sPhone"] = pJson["sPhone"];
    }
    if(!pJson["lUserId"].isNull() ){
        jsonPost["lUserId"] = pJson["lUserId"];
    }
    if(!pJson["sWorkZone"].isNull() ){
        jsonPost["sWorkZone"] = pJson["sWorkZone"];
    }
    if(Request::WalletHttpPost(sUrl,jsonPost,jsonRespons)==0){

        LongLongMap platformHotCountMap ,platformUnHotCountMap;
        mUserDataDAL.GetFansNumGroupBy(enum_kvalue_type_push,1,platformHotCountMap);
        mUserDataDAL.GetFansNumGroupBy(enum_kvalue_type_push,0,platformUnHotCountMap);

        pResponse["iTotal"]  = jsonRespons["count"];
        Json::Value jlistPerAgentList = jsonRespons["listPerAgentList"];
        Json::Value jLst;
        for( int i = 0;i < jlistPerAgentList.size(); i++){
            Json::Value jEntity  = jlistPerAgentList[i];

            jLst[i]["sCity"]  = jEntity["sWorkZone"];
            long lUserId = tConvert.Json2Long( jEntity["lUserId"]);
            jLst[i]["lUserId"]  = jEntity["lUserId"];

            UserDataEntity userData;
            mUserDataDAL.GetEntityById(lUserId,userData);

            int iHotCount = 0,iUnHotCount = 0;
            iHotCount = platformHotCountMap[tConvert.Json2Long(jLst[i]["lUserId"])];
            iUnHotCount = platformUnHotCountMap[tConvert.Json2Long(jLst[i]["lUserId"])];


            KValueEntity kvalue;
            mFansBankKValueDAL.GetEntity(lUserId,enum_kvalue_type_push, tConvert.GetCurrentDay(),kvalue);

            jLst[i]["sAccount"] = userData.sAccount;
            jLst[i]["sNickName"] = userData.sNickName;
            jLst[i]["iSex"] = userData.iSex;
            jLst[i]["iActive"] = iHotCount;
            jLst[i]["iUnActive"] = iUnHotCount;
            jLst[i]["iTrade"] = userData.iState;
            jLst[i]["dPrice"] = kvalue.price;



        }
        pResponse["list"] = jLst;
        RetJsonMsg::SetRetMsg(0,pResponse);
        return 0;

    }else{
        RetJsonMsg::SetRetMsg(-1,pResponse,"获取个代列表出错");
        return -1;
    }
}

//获取联盟商家列表(粉丝信息)
int UserDataBLL::ShopLst(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    //数据合法性
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);



    Json::Value  jsonPost,  jsonRespons;
    jsonPost["iIndex"] = (iPage -1)*iCount;
    jsonPost["iCount"] = iCount;
    if( !pJson["sShopName"].isNull() ){
        jsonPost["sShopName"] = pJson["sShopName"];
    }
    if( !pJson["sShopPhone"].isNull() ){
        jsonPost["sShopPhone"] = pJson["sShopPhone"];
    }
    if( !pJson["lPerAgentId"].isNull() ){
        jsonPost["lPerAgentId"] = pJson["lPerAgentId"];
    }
    if( !pJson["sPlatformId"].isNull() ){
        jsonPost["sPlatformId"] = pJson["sPlatformId"];
    }
    if( !pJson["lUserId"].isNull() ){
        jsonPost["lUserId"] = pJson["lUserId"];
    }
    if(Request::ShopGetLst(TBLL::GetInstance()->GetMepayIP(),jsonPost,jsonRespons)==0){

        LongLongMap platformHotCountMap ,platformUnHotCountMap;
        mUserDataDAL.GetFansNumGroupBy(enum_kvalue_type_platform,1,platformHotCountMap);
        mUserDataDAL.GetFansNumGroupBy(enum_kvalue_type_platform,0,platformUnHotCountMap);

        pResponse["iTotal"]  = jsonRespons["iTotal"];
        Json::Value jShopList = jsonRespons["list"];
        Json::Value jLst;
        for( int i = 0;i < jShopList.size(); i++){
            Json::Value jEntity  = jShopList[i];

            jLst[i]["lPlatformId"]  = jEntity["sPrimaryKey"];
            long lUserId = tConvert.Json2Long( jEntity["lUserId"]);
            jLst[i]["lUserId"]  = jEntity["lUserId"];

            UserDataEntity userData;
            mUserDataDAL.GetEntityById(lUserId,userData);

            int iHotCount = 0,iUnHotCount = 0;
            iHotCount = platformHotCountMap[tConvert.Json2Long(jLst[i]["lPlatformId"])];
            iUnHotCount = platformUnHotCountMap[tConvert.Json2Long(jLst[i]["lPlatformId"])];

            KValueEntity kvalue;
            mFansBankKValueDAL.GetEntity(tConvert.Json2Long( jLst[i]["lPlatformId"] ),enum_kvalue_type_platform, tConvert.GetCurrentDay(),kvalue);

            jLst[i]["sAccount"] = userData.sAccount;
            jLst[i]["sNickName"] = userData.sNickName;
            jLst[i]["iSex"] = userData.iSex;
            jLst[i]["iActive"] = iHotCount;
            jLst[i]["iUnActive"] = iUnHotCount;
            jLst[i]["iFansTrad"] = tConvert.Json2Int(jEntity["iFansTrad"]);

            jLst[i]["sShopName"] = jEntity["sShopName"];
            jLst[i]["iOffLineDisCount"] = jEntity["iOffLineDisCount"];
            jLst[i]["lPerAgentId"] = jEntity["lPerAgentId"];
            jLst[i]["dPrice"] = kvalue.price;



        }
        pResponse["list"] = jLst;
        RetJsonMsg::SetRetMsg(0,pResponse);
        return 0;

    }else{
        RetJsonMsg::SetRetMsg(-1,pResponse,"获取商家列表出错");
        return -1;
    }

}


//获取粉丝审核列表(某一天的)
int UserDataBLL::GetUnHotInfoByDay(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() || pJson["iType"].isNull()
            || pJson["sDay"].isNull() || pJson["iHot"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    string sDay = tConvert.Json2String(pJson["sDay"]);
    int iHot = tConvert.Json2Int(pJson["iHot"]);


    CommonStructVector lst;int total = 0;
    mUserDataDAL.GetUnhotGroupById(iType,iPage,iCount,sDay,iHot,lst,total);




    for( int i = 0;i < lst.size(); i++){
        CommonStruct entity  = lst[i];

        pResponse["list"][i]["id"] = entity.v1;
        pResponse["list"][i]["iCount"] = entity.v2;
        pResponse["list"][i]["iHot"] = entity.v3;
        pResponse["list"][i]["sNickName"] = "";
        pResponse["list"][i]["sAccount"] = "";
        pResponse["list"][i]["lUserId"] = "";
        pResponse["list"][i]["sHeadImage"] = "";

    }
    pResponse["total"] = total;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


//获取粉丝审核列表(所有日期)
int UserDataBLL::GetVerifyList(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() || pJson["iType"].isNull() || pJson["iHot"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    int iHot = tConvert.Json2Int(pJson["iHot"]);


    CommonStructVector lst;int total = 0;
    mUserDataDAL.GetVerifyList(iType,iPage,iCount,iHot,lst,total);




    for( int i = 0;i < lst.size(); i++){
        CommonStruct entity  = lst[i];

        pResponse["list"][i]["id"] = entity.v1;
        pResponse["list"][i]["iCount"] = entity.v2;
        pResponse["list"][i]["iHot"] = entity.v3;
        pResponse["list"][i]["sDay"] = entity.v4;
        pResponse["list"][i]["sNickName"] = "";
        pResponse["list"][i]["sAccount"] = "";
        pResponse["list"][i]["lUserId"] = "";
        pResponse["list"][i]["sHeadImage"] = "";

    }
    pResponse["total"] = total;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


//审核更改状态
int UserDataBLL::HotVerify(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["iType"].isNull() || pJson["sDay"].isNull() || pJson["id"].isNull() || pJson["iHot"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    string id = tConvert.Json2String(pJson["id"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    string sDay = tConvert.Json2String(pJson["sDay"]);
    int iHot = tConvert.Json2Int(pJson["iHot"]);

    mUserDataDAL.HotVerify(sDay,iType,id,iHot);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

//粉丝回收
int UserDataBLL::FansRecover(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["iType"].isNull() || pJson["sDay"].isNull() || pJson["id"].isNull() || pJson["sCode"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    //手机验证码
    string sMsg;
    int ret = IsUserPhoneCode("13149308930",tConvert.Json2String(pJson["sCode"]),sMsg);
    if ( 0 != ret  ) {
        RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
        return ret;
    }



    string id = tConvert.Json2String(pJson["id"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    string sDay = tConvert.Json2String(pJson["sDay"]);

    string selfid;//排除掉自己
    if( iType == enum_kvalue_type_platform){
        int iVerify; long mastid = -1;
        GetPlatformByID(id,mastid,iVerify);
        if( mastid > -1){
            selfid = tConvert.LongToStr(mastid);
        }else{
            RetJsonMsg::SetRetMsg(-1,pResponse,"获取商家资料失败");
            return -1;

        }
    }else{
        selfid = id;
    }

    iRet = mUserDataDAL.FansRecover(sDay,iType,id,selfid);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

/*
Description:获取一批用户数据
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetUserInfoList( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lUserIdLst"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    vector<long> UserIds;
    Json::Value jsonUserLst = pJson["lUserIdLst"];
    for( int i = 0 ; i <  jsonUserLst.size() ; i++ ){
        if( jsonUserLst[i]["lUserId"].isNull()  ){
            RetJsonMsg::SetRetMsg(-15,pResponse);
            return -1;
        }
        UserIds.push_back( tConvert.Json2Long(jsonUserLst[i]["lUserId"]));
    }
    UserDataList UserDataLst;
    iRet =  mUserDataDAL.GetUserInfoList(UserIds,UserDataLst);
    if( iRet != 0){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::GetUserInfoList error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }
    DataLst2Json(UserDataLst, pResponse);
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::Id2Phone( const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["ids"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    vector<long> UserIds;
    Json::Value ids = pJson["ids"];
    for( int i = 0 ; i <  ids.size() ; i++ ){

        UserIds.push_back( tConvert.Json2Long(ids[i]));
    }
    UserDataList UserDataLst;
    iRet =  mUserDataDAL.GetUserInfoList(UserIds,UserDataLst);
    string phones;
    for(UserDataList::iterator it = UserDataLst.begin(); it != UserDataLst.end(); it++ ){
        UserDataEntity en = *it;
        phones += en.sAccount + ",";
    }
    if( phones.size() > 0){
        phones = phones.substr(0,phones.size() -1);
    }
    pResponse["phones"] = phones;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:用户登陆
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::UserLogin( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;int iRet;

    if( pJson["sAccount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iLoginType = 0;
    string sPassWord, sCode;
    if( ! pJson["sPassword"].isNull()  ){
        iLoginType = 1;//密码登陆
        sPassWord = tConvert.Json2String(pJson["sPassword"] );

    }else if(! pJson["sCode"].isNull() ){
        iLoginType =2;//验证码登陆
        sCode = tConvert.Json2String(pJson["sCode"] );
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    string sAccount,sQqNum,sWeixinNum ;


    if(!pJson["sAccount"].isNull() ){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }

    if(!pJson["sQqNum"].isNull() ){
        sQqNum = tConvert.Json2String(pJson["sQqNum"]);
    }


    if(!pJson["sWeixinNum"].isNull() ){
        sWeixinNum = tConvert.Json2String(pJson["sWeixinNum"]);
    }


    UserDataEntity entity;


    if( 2 == iLoginType ){//验证码登陆
        //条件合法性-手机验证码
        string sMsg;
        int ret = IsUserPhoneCode(sAccount,tConvert.Json2String(pJson["sCode"]),sMsg);
        if ( 0!= ret ) {
            RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
            return ret;
        }

        iRet = mUserDataDAL.GetEntityByAccount(sAccount,entity);

        if( iRet != 0){
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return -1;
        }



    }else{




        if( !sAccount.empty()){

            iRet = mUserDataDAL.GetEntityByAccount(sAccount,entity);


        }else if( ! sQqNum.empty() ){
            iRet = mUserDataDAL.GetEntityByQqNum(sQqNum,entity);

        }else if( ! sWeixinNum.empty()){
            iRet = mUserDataDAL.GetEntityByWeixinNum(sWeixinNum,entity);
        }

        if( iRet != 0){
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            if( iRet == -5)
                pResponse["sMsg"] = "用户不存在";
            return -1;
        }



        if( entity.sPassword != sPassWord){
            RetJsonMsg::SetRetMsg(-6,pResponse);
            return -1;
        }
    }

    if( entity.iState != enum_user_state_pass){
        if( entity.iState == enum_user_state_colse){
            RetJsonMsg::SetRetMsg(-1,pResponse,err1);
            return -1;
        }

    }



    Data2Json(entity,pResponse);
    long lCurTime = tConvert.GetCurrentTime();
    pResponse["entity"]["lLoginTime"] = tConvert.LongToStr(lCurTime);
    pResponse["entity"]["sPassword"] = entity.sPassword;
    pResponse["entity"]["sToken"] = Token::MakeLoginToken(tConvert.LongToStr(entity.lUserId));



    UserStateDAL userStatedal;

    bool b = false;
    userStatedal.IsExistByUserId( tConvert.LongToStr( entity.lUserId),b);
    if( !b){
        Json::Value jWalletRespon;
        MoneyBagServiceAddByRegister(entity.sAccount,entity.sNickName,jWalletRespon);
    }

    userStatedal.SetLoginTime(entity.lUserId, lCurTime);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}


/*
Description:用户信息（主键或手机号或用户ID）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::GetUserInfo( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["lUserId"].isNull() && pJson["sAccount"].isNull() && pJson["sGciID"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;

    string sPrimaryKey = "-1nochange";
    string sAccount = "-1nochange";
    string sGciID = "-1nochange";
    if( ! pJson["lUserId"].isNull() ){
        sPrimaryKey = tConvert.Json2String(pJson["lUserId"]);
    }
    if( ! pJson["sAccount"].isNull() ){
        sAccount = tConvert.Json2String(pJson["sAccount"]);
    }
    if( ! pJson["sGciID"].isNull() ){
        sGciID = tConvert.Json2String(pJson["sGciID"]);
    }

    //获取用户内容
    UserDataEntity entity;
    iRet = this->mUserDataDAL.GetEntity( sPrimaryKey,sAccount,sGciID,entity);
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::GetUserInfo mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);

        if( iRet == -5){
            pResponse["sMsg"] = "目标用户不存在";
        }
        return -1;
    }
    Data2Json(entity,pResponse);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}
/*
Description:根据用户获取我的推广者信息（手机号或用户ID）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::MyPushMan( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    string sPrimaryKey = "-1nochange";
    string sAccount = "-1nochange";
    string sGciID = "-1nochange";
    TConvert tConvert;
    int iRet = -1;
    if( !pJson["lUserId"].isNull()  ){
        sPrimaryKey = tConvert.Json2String(pJson["lUserId"]);
    }else if( !pJson["sAccount"].isNull() ){
        sAccount = tConvert.Json2String( pJson["sAccount"]);
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    //获取用户内容
    UserDataEntity entity;
    iRet = this->mUserDataDAL.GetEntity( sPrimaryKey,sAccount,sGciID,entity);
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::MyPushMan mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);
        return -1;
    }
    pResponse["entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
    pResponse["entity"]["sAccount"] =  entity.sAccount ;

    sPrimaryKey = tConvert.LongToStr(entity.lPushManId );
    sAccount = "-1nochange";
    sGciID = "-1nochange";
    UserDataEntity PushManEntity;
    //获取推送者用户内容
    iRet = this->mUserDataDAL.GetEntity( sPrimaryKey,sAccount,sGciID,PushManEntity);
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::MyPushMan mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);
        return -1;
    }
    pResponse["entity"]["lPushUserId"] = tConvert.LongToStr( PushManEntity.lUserId );
    pResponse["entity"]["sPushAccount"] =  PushManEntity.sAccount ;

    //平台
    long lPlatformManId = 0;
    int  iVerify;
    iRet = GetPlatformByID(tConvert.LongToStr(entity.lCurrentPlatformId ),lPlatformManId,iVerify);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(-101,pResponse);
        return -101;
    }
    //获取平台主用户内容
    sPrimaryKey = tConvert.LongToStr(lPlatformManId );
    sAccount = "-1nochange";
    sGciID = "-1nochange";
    UserDataEntity PlatformManEntity;
    iRet = this->mUserDataDAL.GetEntity( sPrimaryKey,sAccount,sGciID,PlatformManEntity);
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::MyPushMan mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);
        return -1;
    }
    pResponse["entity"]["lPlatformUserId"] = tConvert.LongToStr( PlatformManEntity.lUserId );
    pResponse["entity"]["sPlatformAccount"] =  PlatformManEntity.sAccount ;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}
/*
Description:根据用户获取我的推送人数（手机号或用户ID）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::MyPushCount( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    long lUserId = 0;
    if( !pJson["lUserId"].isNull()  ){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }else if( !pJson["sAccount"].isNull() ){
        string sAccount = tConvert.Json2String( pJson["sAccount"]);
        UserDataEntity entity;
        this->mUserDataDAL.GetEntity( "-1nochange",sAccount,"-1nochange",entity);
        lUserId = entity.lUserId;
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    //根据用户获取我的推送人数
    long lCount = 0;
    if ( 0 != this->mUserDataDAL.GetMyPushCount( tConvert.LongToStr( lUserId ) ,lCount ) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::MyPushCount GetMyPushCount error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    pResponse["lCount"] = tConvert.LongToStr(lCount);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:根据用户获取我的推广和登陆过人数（手机号或用户ID）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::MyPushAndLoginedCount( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    long lUserId = 0;
    if( !pJson["lUserId"].isNull()  ){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }else if( !pJson["sAccount"].isNull() ){
        string sAccount = tConvert.Json2String( pJson["sAccount"]);
        UserDataEntity entity;
        this->mUserDataDAL.GetEntity( "-1nochange",sAccount,"-1nochange",entity);
        lUserId = entity.lUserId;
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lCount = 0,lSCount = 0;

    UserDataList mypushLst;
    if ( 0 != this->mUserDataDAL.GetMyPush(  lUserId  ,mypushLst ) ){
        appendlog(TTDLogger::LOG_ERROR,"GetMyPush  error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    lCount = mypushLst.size();

    vector<long> lUserIdLst;
    UserDataList::iterator it = mypushLst.begin();
    for( ; it != mypushLst.end() ; it++){
        UserDataEntity & mypushEntity = *it;
        lUserIdLst.push_back(mypushEntity.lUserId);

    }

    if ( 0 != this->mUserDataDAL.GetBatchUserPushCount( lUserIdLst ,lSCount ) ){
        appendlog(TTDLogger::LOG_ERROR,"GetBatchUserPushCount error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    int iHotCount = 0;
    if ( 0 != this->mUserDataDAL.GetWeixinBindNum( lUserId  ,enum_kvalue_type_push,iHotCount ) ){
        appendlog(TTDLogger::LOG_ERROR,"GetWeixinBindNum  error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    pResponse["lCount"] = tConvert.LongToStr(lCount);
    pResponse["lLoginedCount"] = tConvert.LongToStr(iHotCount);
    pResponse["lSCount"] = tConvert.LongToStr(lSCount);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:获取用户下两级推广者数量
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::MyFSLevelPushCount( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    long lUserId = 0;
    if( !pJson["lUserId"].isNull()  ){
        lUserId = tConvert.Json2Long(pJson["lUserId"]);
    }else if( !pJson["sAccount"].isNull() ){
        string sAccount = tConvert.Json2String( pJson["sAccount"]);
        UserDataEntity entity;
        this->mUserDataDAL.GetEntity( "-1nochange",sAccount,"-1nochange",entity);
        lUserId = entity.lUserId;
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    //我的1,2级推广人数
    long lFCount = 0, lScount = 0;
    //1,2级个代人数
    long lFPerCount = 0, lSPerCount = 0;

    UserDataList mypushLst;
    if ( 0 != this->mUserDataDAL.GetMyPush(  lUserId  ,mypushLst ) ){
        appendlog(TTDLogger::LOG_ERROR,"GetMyPush  error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    lFCount = mypushLst.size();
    UserDataList::iterator it = mypushLst.begin();
    for( ; it != mypushLst.end() ; it++){
        UserDataEntity & mypushEntity = *it;
        //是否个代
        iRet =  IsPersonAgent(mypushEntity.lUserId );
        if( iRet == 1){
            lFPerCount++;
        }

        UserDataList mySecPushLst;
        if ( 0 != this->mUserDataDAL.GetMyPush(  mypushEntity.lUserId  ,mySecPushLst ) ){
            appendlog(TTDLogger::LOG_ERROR,"GetMyPush error");
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -1;
        }
        UserDataList::iterator itS = mySecPushLst.begin();
        for( ; itS != mySecPushLst.end() ; itS++){
            UserDataEntity & mypushSEntity = *itS;
            //是否个代
            iRet =  IsPersonAgent(mypushSEntity.lUserId );
            if( iRet == 1){
                lSPerCount++;
            }
        }
        lScount += mySecPushLst.size();
    }



    pResponse["lFPushCount"] = tConvert.LongToStr(lFCount);
    pResponse["lSPushCount"] = tConvert.LongToStr(lScount);
    pResponse["lFPerCount"] = tConvert.LongToStr(lFPerCount);
    pResponse["lSPerCount"] = tConvert.LongToStr(lSPerCount);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

//验证短信验证码
int UserDataBLL::IsUserPhoneCode(const string& psPhone, const string& psCode,string & sMsg)
{
    int iRet = -1;
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["sPhone"]=psPhone;
    jsonValue["sCode"]=psCode;

    //URL
    string sUrl = "http://"+TBLL::GetInstance()->GetPhonesmsIP()+":23240/?sBusiness=phonesms&sMethod=UseCode";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    appendlog(TTDLogger::LOG_DEBUG,"UserDataBLL::IsUserPhoneCode sReturn=%s",sReturn.c_str());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    iRet = tConvert.Json2Int(jsonReturn["iRet"]);

    sMsg = tConvert.Json2String(jsonReturn["sMsg"]);

    return iRet;
}


/*注册送购物币
 *iActivityType 京东专题_注册送购物币 = 0 京东专题_分享送购物币 = 1 完善资料赠送购物币 = 2
*/
bool UserDataBLL::GiveActivityReward(const string& psPhone, int iActivityType)
{

    Json::Value jsonValue,jRespons;
    jsonValue["sPhone"]=psPhone;
    jsonValue["iActivityType"]=iActivityType;

    //URL
    string sUrl = "http://"+TBLL::GetInstance()->GetWalletIP()+"/GetDataInterface/Activity/GiveActivityReward.aspx";

    Request::WalletHttpPost(sUrl,jsonValue,jRespons);

    string s = jRespons.toStyledString();
    string s2 = jsonValue.toStyledString();

    appendlog(TTDLogger::LOG_INFO,"UserDataBLL::GiveActivityReward req:%s,respons:%s",s2.c_str(),s.c_str());

    return true;
}
//获取平台
int UserDataBLL::GetPlatformByID(const string& psPlatformId, long & plUserId,int & iVerify)
{

    TConvert tConvert;
    Json::Value jsonValue,jsonReturn;
    jsonValue["lPlatformId"]=psPlatformId;

    int iRet = Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jsonValue,jsonReturn);
    if( iRet == 0){
        plUserId = tConvert.Json2Int(jsonReturn["entity"]["lUserId"]);
        iVerify = tConvert.Json2Int(jsonReturn["entity"]["iVerify"]);
    }
    return iRet;
}

int UserDataBLL::GetPlatformState(const string& sPlatformId, int & iState)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue,jsonReturn;
    jsonValue["lPlatformId"]=sPlatformId;

    int iRet = Request::ShopGetState(TBLL::GetInstance()->GetMepayIP(),jsonValue,jsonReturn);

    if(iRet == 0){
        iState = tConvert.Json2Int(jsonReturn["iState"]);
    }
    return iRet;
}


//是否个代
int UserDataBLL::IsPersonAgent(const long & plUserId )
{
    //参数转换
    return false;

}

/*
Description:通过绑定来注册
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::RegByBind(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["sAccount"].isNull() || pJson["sCode"].isNull() || pJson["sOpenId"].isNull()
         ||  pJson["sAccessToken"].isNull()  |  pJson["src"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sOpenId = tConvert.Json2String(pJson["sOpenId"]);
    string sAccessToken = tConvert.Json2String(pJson["sAccessToken"]);
    string src = tConvert.Json2String(pJson["src"]);

    string unioid;
    unioid.clear();
    if( src == "1"){
        unioid = GetWeiXinUserInfo(sOpenId,sAccessToken);
    }else {
        unioid = GetAlipayUserInfo(sAccessToken);
    }
    if( unioid.empty()){
        appendlog(TTDLogger::LOG_INFO,"RegByBind 验证第三方信息失败");
        RetJsonMsg::SetRetMsg(-1,pResponse,"验证第三方信息失败");
        return -1;
    }




    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);
    if( iRet != 0){
        if( iRet == -5 ){

            long lPushManId = 39;
            long lPlatformID = 100012;
            if( !pJson["lPushManId"].isNull() ){
                lPushManId = tConvert.Json2Long(pJson["lPushManId"]);
            }
            if( !pJson["lPlatformID"].isNull() ){
                lPlatformID = tConvert.Json2Long(pJson["lPlatformID"]);
            }

            Json::Value j,r;
            j["sCode"] = pJson["sCode"];
            j["sAccount"] = sAccount;
            j["sPassword"] =  Md5ToLower( "T" + MyUtil::MakeRandNum(6) );
            j["sNickName"] = "";
            j["iSex"] = 2;//默认性别为未知
            j["sProvince"] = "";
            j["sCity"] = "";
            j["sSignature"] = "";
            j["sEmail"] = "";
            j["sHeadImage"] = "";
            j["iPhoneAuth"] = 1;
            j["lPushManId"] = tConvert.LongToStr( lPushManId );
            j["lPlatformID"] = tConvert.LongToStr( lPlatformID );
            j["bBind"] = true;
            if( src == "1"){
                j["sWeixinNum"] = unioid;
            }else {
                j["sAliNum"] = unioid;
            }
            iRet = AddData(j,pResponse);
            return iRet;


        }else{
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return iRet;

        }
    }else{

        RetJsonMsg::SetRetMsg(-9,pResponse,"手机号已注册");

        pResponse["entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );

        return iRet;

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}

/*
Description:绑定手机号(手机号不存在，会先注册)
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::BindPhone(const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    int iRet = -1;

    if(  pJson["sAccount"].isNull() || pJson["sCode"].isNull() || pJson["sOpenId"].isNull()
         ||  pJson["sAccessToken"].isNull()  ||   pJson["src"].isNull() || pJson["id"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sOpenId = tConvert.Json2String(pJson["sOpenId"]);
    string sAccessToken = tConvert.Json2String(pJson["sAccessToken"]);
    string src = tConvert.Json2String(pJson["src"]);
    string id = tConvert.Json2String(pJson["id"]);

    string unioid;
    unioid.clear();
    //    if( src == "1"){
    //        unioid = GetWeiXinUserInfo(sOpenId,sAccessToken);
    //    }else {
    //        unioid = GetAlipayUserInfo(sAccessToken);
    //    }
    unioid = id;
    //    if( unioid.empty()){
    //        appendlog(TTDLogger::LOG_INFO,"BindPhone 验证第三方信息失败");
    //        RetJsonMsg::SetRetMsg(-1,pResponse,"BindPhone验证第三方信息失败");
    //        return -1;
    //    }

    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntityByAccount(sAccount, entity);
    if( iRet != 0){
        if( iRet == -5 ){

            ThirdUserEntity en;
            if(mThirdUserDAL.GetById(unioid,en) != 0){//如果没有第三方信息，则默认的平台号和推广人
                en.platformId = 100012;
                en.pushManId = 39;
            }

            string sPwd = MyUtil::MakeRandNum(6);


            Json::Value j,r;
            j["sCode"] = pJson["sCode"];
            j["sAccount"] = sAccount;
            j["sPassword"] =  Md5ToLower( "T" + sPwd );
            j["sNickName"] = en.nickName;
            j["iSex"] = 2;//默认性别为未知
            j["sProvince"] = "";
            j["sCity"] = "";
            j["sSignature"] = "";
            j["sEmail"] = "";
            j["sHeadImage"] = en.headImage;
            j["iPhoneAuth"] = 1;
            j["lPushManId"] = tConvert.LongToStr( en.pushManId );
            j["lPlatformID"] = tConvert.LongToStr( en.platformId );
            j["bBind"] = true;
            if( src == "1"){
                j["sWeixinNum"] = unioid;
            }else {
                j["sAliNum"] = unioid;
            }
            iRet = AddData(j,pResponse);
            return iRet;


        }else{
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return iRet;

        }
    }else{
        if( (!entity.sWeixinNum.empty() && src == "1" ) || (!entity.sAliNum.empty() && src == "2" )){
            RetJsonMsg::SetRetMsg(-1,pResponse,"手机号已经绑定了第三方");
            return iRet;
        }
        if( src == "1"){
            iRet = mUserDataDAL.UpWeixinNum(sAccount,unioid);
        }else if( src == "2"){
            iRet = mUserDataDAL.UpAliNum(sAccount,unioid);
        }

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:第三方用户注册
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::ThirdUser(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = -1;

    if( pJson["openId"].isNull() || pJson["accessToken"].isNull() || pJson["src"].isNull()
            || pJson["pushManId"].isNull() || pJson["platformID"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sOpenId = tConvert.Json2String(pJson["openId"]);
    string sAccessToken = tConvert.Json2String(pJson["accessToken"]);
    string src = tConvert.Json2String(pJson["src"]);
    long pushManId = tConvert.Json2Long(pJson["pushManId"]);
    long platformID = tConvert.Json2Long(pJson["platformID"]);

    string unioid;Json::Value jReturn;
    unioid.clear();
    if( src == "1"){
        unioid = GetWeiXinUserInfo(sOpenId,sAccessToken,jReturn);
    }else {
        unioid = GetAlipayUserInfo(sAccessToken);
    }
    if( unioid.empty()){
        appendlog(TTDLogger::LOG_INFO,"ThirdUser 验证第三方信息失败");
        RetJsonMsg::SetRetMsg(-1,pResponse,"验证第三方信息失败");
        return -1;
    }
    ThirdUserEntity en;
    en.type = atoi(src.c_str());
    en.platformId = platformID;
    en.pushManId = pushManId;
    en.addTime = tConvert.GetCurrentDayTime();
    en.id = unioid;
    en.headImage = tConvert.Json2String( jReturn["headimgurl"] );
    en.nickName = tConvert.Json2String( jReturn["nickname"] );
    en.sex = tConvert.Json2Int( jReturn["sex"] );

    iRet = mThirdUserDAL.Insert(en);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:短信登录（如不存大帐号则自动注册）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::LoginByCode(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = -1;

    //    RetJsonMsg::SetRetMsg(-1,pResponse,"请用微信或密码登录");
    //    return -1;

    if(  pJson["sAccount"].isNull() || pJson["sCode"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);

    if( !MyUtil::IsRightPhone(sAccount))
    {
        RetJsonMsg::SetRetMsg(-1,pResponse,"手机号不合法");
        return -1;
    }

    //    if( sAccount.substr(0,2) == "17" && Request::Phone138(sAccount) != 0){
    //        RetJsonMsg::SetRetMsg(-15,pResponse,"你可能用了一个假的手机号");
    //        return -1;
    //    }


    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);
    if( iRet != 0){
        if( iRet == -5 ){

            long lPushManId = 39;
            long lPlatformID = 100012;
            if( !pJson["lPushManId"].isNull() ){
                lPushManId = tConvert.Json2Long(pJson["lPushManId"]);
            }
            if( !pJson["lPlatformID"].isNull() ){
                lPlatformID = tConvert.Json2Long(pJson["lPlatformID"]);
            }

            Json::Value j,r;
            j["sCode"] = pJson["sCode"];
            j["sAccount"] = sAccount;
            j["sPassword"] =  Md5ToLower( "T" + MyUtil::MakeRandNum(6) );
            j["sNickName"] = "";
            j["iSex"] = 2;//默认性别为未知
            j["sProvince"] = "";
            j["sCity"] = "";
            j["sSignature"] = "";
            j["sEmail"] = "";
            j["sHeadImage"] = "";
            j["iPhoneAuth"] = 1;
            j["lPushManId"] = tConvert.LongToStr( lPushManId );
            j["lPlatformID"] = tConvert.LongToStr( lPlatformID );
            iRet = AddData(j,r);

            mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);

        }
    }else{
        //条件合法性-手机验证码
        string sMsg;
        int ret = IsUserPhoneCode(sAccount,tConvert.Json2String(pJson["sCode"]),sMsg);
        if ( 0 != ret ) {
            RetJsonMsg::SetRetMsg(ret,pResponse,sMsg);
            return ret;
        }

        if( entity.iState == enum_user_state_colse){
            RetJsonMsg::SetRetMsg(-1,pResponse,err1);
            return -1;
        }


    }


    Data2Json(entity,pResponse);
    long lCurTime = tConvert.GetCurrentTime();
    pResponse["entity"]["lLoginTime"] = tConvert.LongToStr(lCurTime);
    pResponse["entity"]["sPassword"] = entity.sPassword;
    pResponse["entity"]["sToken"] = Token::MakeLoginToken(tConvert.LongToStr(entity.lUserId));


    UserStateDAL userStatedal;

    bool bLogin = true;
    if(!pJson["bLogin"].isNull()){
        bLogin = tConvert.Json2Bool(pJson["bLogin"]);
    }
    if( bLogin){
        bool b = false;
        userStatedal.IsExistByUserId( tConvert.LongToStr( entity.lUserId),b);
        if( !b){
            Json::Value jWalletRespon;
            MoneyBagServiceAddByRegister(entity.sAccount,entity.sNickName,jWalletRespon);
        }
        userStatedal.SetLoginTime(entity.lUserId, lCurTime);
    }


    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:CheckDeivce
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::CheckDeivce(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = 0;

    if(  pJson["sAccount"].isNull() || pJson["sDeviceId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sDeviceId = tConvert.Json2String(pJson["sDeviceId"]);



    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);

    UserDeviceEntity deviceEntity;
    mUserDeviceDAL.GetById(entity.lUserId,deviceEntity);

    if( deviceEntity.deviceId.empty()){
       iRet = 1;

    }else if( deviceEntity.deviceId == sDeviceId){
        iRet = 0;

    }else{
        iRet = 2;
    }



    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:UpdateDeivce
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserDataBLL::UpdateDeivce(const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    int iRet = 0;

    if(  pJson["sAccount"].isNull() || pJson["sDeviceId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    string sDeviceId = tConvert.Json2String(pJson["sDeviceId"]);



    UserDataEntity entity;
    iRet = mUserDataDAL.GetEntity("-1nochange",sAccount, "-1nochange",entity);

    if( iRet == 0){
       iRet = mUserDeviceDAL.replace(entity.lUserId,sDeviceId);
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


//设置默认值
//void UserDataBLL::SetDefaultValue( UserDataEntity& pEntity )
//{
//    pEntity.sAccount = "-1nochange";
//    pEntity.sPassword = "-1nochange";
//    pEntity.sNickName = "-1nochange";
//    pEntity.iSex = -1;
//    pEntity.sProvince = "-1nochange";
//    pEntity.sCity = "-1nochange";
//    pEntity.sSignature = "-1nochange";
//    pEntity.sEmail = "-1nochange";
//    pEntity.sHeadImage = "-1nochange";
//    pEntity.iPhoneAuth = -1;
//    pEntity.lPushManId = -1;
//    pEntity.lPlatformId = -1;
//    pEntity.lCurrentPlatformId = -1;
//    pEntity.sGciID = "-1nochange";
//    pEntity.sGciPwd = "-1nochange";
//    pEntity.sGciPhone = "-1nochange";
//}

void UserDataBLL::DataLst2Json(  UserDataList & pLst,Json::Value & pResponse ){
    int iCount = 0;
    TConvert tConvert;
    while(iCount <  pLst.size()){

        UserDataEntity & entity = pLst[ iCount ];
        pResponse["list"][iCount]["sPrimaryKey"] = entity.sPrimaryKey;
        pResponse["list"][iCount]["lUserId"] = tConvert.LongToStr(entity.lUserId);
        pResponse["list"][iCount]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iCount]["lUpTime"] = tConvert.LongToStr(entity.lUpTime);
        pResponse["list"][iCount]["lUpdateId"] = tConvert.LongToStr(entity.lUpdateId);
        pResponse["list"][iCount]["sAccount"] = entity.sAccount;
        pResponse["list"][iCount]["sNickName"] = entity.sNickName;
        pResponse["list"][iCount]["iSex"] = entity.iSex;
        pResponse["list"][iCount]["sProvince"] = entity.sProvince;
        pResponse["list"][iCount]["sCity"] = entity.sCity;
        pResponse["list"][iCount]["sSignature"] = entity.sSignature;
        pResponse["list"][iCount]["sEmail"] = entity.sEmail;
        pResponse["list"][iCount]["sHeadImage"] = entity.sHeadImage;
        pResponse["list"][iCount]["iPhoneAuth"] = entity.iPhoneAuth;
        pResponse["list"][iCount]["lPushManId"] = tConvert.LongToStr(entity.lPushManId);
        pResponse["list"][iCount]["lPlatformId"] = tConvert.LongToStr(entity.lPlatformId);
        pResponse["list"][iCount]["lCurrentPlatformId"] = tConvert.LongToStr(entity.lCurrentPlatformId);
        pResponse["list"][iCount]["sGciID"] = entity.sGciID;
        iCount++;
    }
    if( pLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

}

void UserDataBLL::Data2Json( const UserDataEntity & entity,Json::Value & pResponse)
{
    TConvert tConvert;
    pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
    pResponse["entity"]["lUserId"] = tConvert.LongToStr(entity.lUserId);
    pResponse["entity"]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
    pResponse["entity"]["lUpTime"] = tConvert.LongToStr(entity.lUpTime);
    pResponse["entity"]["lUpdateId"] = tConvert.LongToStr(entity.lUpdateId);
    pResponse["entity"]["sAccount"] = entity.sAccount;
    pResponse["entity"]["sNickName"] = entity.sNickName;
    pResponse["entity"]["iSex"] = entity.iSex;
    pResponse["entity"]["sProvince"] = entity.sProvince;
    pResponse["entity"]["sCity"] = entity.sCity;
    pResponse["entity"]["sSignature"] = entity.sSignature;
    pResponse["entity"]["sEmail"] = entity.sEmail;
    pResponse["entity"]["sHeadImage"] = entity.sHeadImage;
    pResponse["entity"]["iPhoneAuth"] = entity.iPhoneAuth;
    pResponse["entity"]["lPushManId"] = tConvert.LongToStr(entity.lPushManId);
    pResponse["entity"]["lPlatformId"] = tConvert.LongToStr(entity.lPlatformId);
    pResponse["entity"]["lCurrentPlatformId"] = tConvert.LongToStr(entity.lCurrentPlatformId);
    pResponse["entity"]["sGciID"] = entity.sGciID;
    string sBirthDay = entity.sBirthDay.substr(0,10);
    if( sBirthDay == "1900-01-01"){
        sBirthDay = "";
    }
    pResponse["entity"]["sBirthDay"] =  sBirthDay;

}


bool UserDataBLL::IsRightNick(const string & sNickName)
{
    if( sNickName.empty())
        return true;
    int iCount = 0;
    for(int i = 0 ; i < sNickName.size() ; i ++){
        if(sNickName[i] ==' '){
            iCount++;
        }
    }
    if( iCount == sNickName.size())
        return false;

    return true;
}


//判断用户资料是否完整
bool UserDataBLL::IsUserInfoPerfect(const UserDataEntity& entity)
{
    if( entity.sNickName.empty()){
        return false;
    }
    if (entity.sProvince.empty()){
        return false;
    }
    if( entity.sCity.empty()){
        return false;
    }
    if( entity.sHeadImage.empty()){
        return false;
    }
    if( entity.sBirthDay.empty() || entity.sBirthDay.substr(0,4) <= "1900"){
        return false;
    }
    if( entity.iSex > 1){
        return false;
    }
    return true;
}


// 获取微信用户unionid信息
string UserDataBLL::GetWeiXinUserInfo(string sOpenId,string sAccessToken)
{
    Json::Value jReturn;
    return GetWeiXinUserInfo(sOpenId,sAccessToken,jReturn);

}

// 获取微信用户unionid信息
string UserDataBLL::GetWeiXinUserInfo(string sOpenId,string sAccessToken,Json::Value & jReturn)
{

    string sUrl = "http://mepay.ttdomo.com/WeiXinPay/BusinessAPI/GetUserInfo.ashx";
    sUrl += "?sOpenId=" + sOpenId;
    sUrl += "&sAccessToken=" + sAccessToken;

    Json::Value jpost ;
    jpost["sOpenId"] = sOpenId;
    jpost["sAccessToken"] = sAccessToken;

    TCurl tCurl;string sReturn;

    tCurl.Post(sUrl,jpost.toStyledString(),sReturn);


    TConvert tConvert;;
    tConvert.Str2Json(sReturn,jReturn);
    appendlog(TTDLogger::LOG_INFO, "GetWeiXinUserInfo return:%s",jReturn.toStyledString().c_str());

    if( !jReturn["errcode"].isNull() ){
        return "";
    }

    string unionid =  tConvert.Json2String( jReturn["unionid"] );
    return unionid;

}


// 获取alipay用户unionid信息
string UserDataBLL::GetAlipayUserInfo(string sAccessToken)
{

    string sUrl = "http://mepay.ttdomo.com/Newalipay/BusinessAPI/GetUserInfo.ashx";
    sUrl += "?sAccessToken=" + sAccessToken;

    Json::Value jpost ;
    jpost["sAccessToken"] = sAccessToken;

    TCurl tCurl;string sReturn;

    tCurl.Post(sUrl,jpost.toStyledString(),sReturn);


    TConvert tConvert;Json::Value jReturn;
    tConvert.Str2Json(sReturn,jReturn);
    appendlog(TTDLogger::LOG_INFO, "GetAlipayUserInfo return:%s",jReturn.toStyledString().c_str());
    if( !jReturn["error_response"].isNull() ){
        return "";
    }

    string user_id =  tConvert.Json2String( jReturn["modUserInfo"]["UserId"] );
    return user_id;

}

//注册钱包帐号
int UserDataBLL::MoneyBagServiceAddByRegister(string phone,string name,Json::Value & jResponse)
{
    TConvert tConvert;int iRet = -1;
    long timestamp = tConvert.GetCurrentTime()/1000;
    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    Json::Value jPost;

    string sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Finance/MoneyBagService/addByRegister"
                                                                      "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
    jPost.clear();jResponse.clear();
    jPost["phone"] = phone;
    jPost["name"] = name;
    iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    return iRet;

}


int UserDataBLL::Make(UserDataEntity & entity)
{

    TConvert tConvert;
    //帐号是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExistByAccont( entity.sAccount,bIsExist) ){
        return -1;
    }
    if (true == bIsExist) {
        return -1;
    }

    int iRet = this->mUserDataDAL.Insert(entity);
    if ( 0 == iRet ) {
        return 0;
    }else{
        return -1;
    }


}
