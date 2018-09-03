/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装调用钱包
 *Others:
*/


#include "LukyMoneyBLL.h"
#include "log/Logger.h"
#include "TConvert.h"
#include "com/RetJsonMsg.hpp"
#include "com/PushDataCOM.h"
#include "bll/TBLL.h"
#include "dal/OrderNumDAL.h"
#include "com/TCurl.h"
#include "MyAES.h"
#include "dal/UserDataDAL.h"



LukyMoneyBLL::LukyMoneyBLL()
{

}



/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int LukyMoneyBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "TransferReceive" == psMethod){
        iRet = TransferReceive(pJson,pResponse);
    }else if( "TransferGive" == psMethod){
        iRet = TransferGive(pJson,pResponse);
    }
    else {
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
int LukyMoneyBLL::Init()
{

    return 0;
}


//购物币红包
//sOrderNo : 聊天记录的sKey
//int LukyMoneyBLL::LkShopCoin(long lUserId, const LongLst & lRevUserLst,const string & sPayPassword,int iTotalNum,double fTotalAmount ,const string & sOrderNo ,
//               Json::Value & jsonRespons)
//{
//    int iRet = -1;string sTransferID ;
//    TConvert tConvert;

//    UserDataDAL uDataDal;UserDataEntity uDataEntity;
//    iRet = uDataDal.GetEntity( tConvert.LongToStr(lUserId) , "-1nochange","-1nochange",uDataEntity);
//    if( iRet != 0 ){
//        return iRet;
//    }
//    StringLst sRevPhoneLst;
//    UserDataList uDataLst;
//    uDataDal.GetUserInfoList(lRevUserLst,uDataLst);
//    for( UserDataList::iterator it = uDataLst.begin(); it != uDataLst.end() ; it++ ){
//        UserDataEntity & uToDataEntity = *it;
//        sRevPhoneLst.push_back(uToDataEntity.sAccount);
//    }


//    iRet = TransferOrder(sOrderNo,uDataEntity.sAccount, sRevPhoneLst,3,iTotalNum, fTotalAmount, "", sTransferID,jsonRespons);

//    if( iRet == 0){
//        jsonRespons.clear();
//        iRet = TransferPay( sTransferID,uDataEntity.sAccount, sPayPassword,jsonRespons);

//        if( 0 == iRet){
//            mOrderNumDal.AddData(sOrderNo, sTransferID);
//        }

//    }
//    string sRespon = jsonRespons.toStyledString();

//    Replace(jsonRespons);
//    return iRet ;


//}

//购物币红包
//lUserId : 发送者
//lRevUserLst : 一批接收者， 一个或多个
//sOrderNo : 聊天记录的sKey
int LukyMoneyBLL::LukyTransfer(long lUserId, const LongLst & lRevUserLst,
                             const string & sPayPassword,int iTotalNum,
                             Json::Value jsonTotalAmount ,int iCurrency,int iCoinType,int iType,const string & sOrderNo ,
               Json::Value & jsonRespons)
{
    int iRet = -1;string sTransferID ;
    TConvert tConvert;

    UserDataDAL uDataDal;UserDataEntity uDataEntity;
    iRet = uDataDal.GetEntity( tConvert.LongToStr(lUserId) , "-1nochange","-1nochange",uDataEntity);
    if( iRet != 0 ){
        RetJsonMsg::SetRetMsg(iRet,jsonRespons);
        return iRet;
    }
    StringLst sRevPhoneLst;
    UserDataList uDataLst;
    uDataDal.GetUserInfoList(lRevUserLst,uDataLst);
    for( UserDataList::iterator it = uDataLst.begin(); it != uDataLst.end() ; it++ ){
        UserDataEntity & uToDataEntity = *it;
        sRevPhoneLst.push_back(uToDataEntity.sAccount);
    }


    iRet = TransferOrder(sOrderNo,uDataEntity.sAccount, sRevPhoneLst,iCurrency,iCoinType,iType,iTotalNum, jsonTotalAmount, "", sTransferID,jsonRespons);

    if( iRet == 0){
        jsonRespons.clear();
        iRet = TransferPay( sTransferID,uDataEntity.sAccount, sPayPassword,jsonRespons);

        if( 0 == iRet){
            mOrderNumDal.AddData(sOrderNo, sTransferID);
        }

    }

    Replace(jsonRespons);
    return iRet ;


}



//红包,转账 确认
int LukyMoneyBLL::TransferConfirm(long lUserId, const string & sOrderNo,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;

    UserDataDAL uDataDal;UserDataEntity uDataEntity;
    iRet = uDataDal.GetEntity( tConvert.LongToStr(lUserId) , "-1nochange","-1nochange",uDataEntity);
    if( iRet != 0 ){
        RetJsonMsg::SetRetMsg(iRet,jsonRespons);
        return iRet;
    }
    string sTransferID;
    iRet = mOrderNumDal.GetTransferID(sOrderNo,sTransferID);
    if( 0 != iRet ){
        RetJsonMsg::SetRetMsg(iRet,jsonRespons);
        return iRet;
    }

    Json::Value jsonPost;
    jsonPost["sTransferID"] =  sTransferID ;
    jsonPost["sReceivePhone"] = uDataEntity.sAccount ;

    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferConfirm.aspx";
    iRet = Requst(sUrl,jsonPost,jsonRespons);
    Replace(jsonRespons);

    return iRet;

}


//红包信息查询
//sOrderNo : 聊天记录的sKey
int LukyMoneyBLL::TransferQuery(const string & sOrderNo,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;

    Json::Value jsonPost;
    jsonPost["sOrderNo"] = sOrderNo ;

    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferQuery.aspx";
    iRet = Requst(sUrl,jsonPost,jsonRespons);
    Replace(jsonRespons);

    if( iRet == 0){

        string sPayPhone = tConvert.Json2String(jsonRespons["sPayPhone"]);
        UserDataDAL uDataDal;UserDataEntity uDataEntity;
        uDataDal.GetEntity("-1nochange",sPayPhone,"-1nochange",uDataEntity);
        jsonRespons["lUserId"] = tConvert.LongToStr(uDataEntity.lUserId);
        jsonRespons["sNickName"] = uDataEntity.sNickName;
        jsonRespons["sHeadImage"] = uDataEntity.sHeadImage;


        jsonRespons.removeMember("iPayStatus");
        jsonRespons.removeMember("sOrderNo");
        jsonRespons.removeMember("sPayPhone");

        //sReceiveRecord
        Json::Value & jsonReceives = jsonRespons["sReceiveRecord"];

        for( int i = 0 ; i < jsonReceives.size(); i++){
            Json::Value & jsonPhone = jsonReceives[i]["sReceivePhone"];
            UserDataDAL uDataDal;UserDataEntity uDataEntity;
            uDataDal.GetEntity("-1nochange",tConvert.Json2String(jsonPhone),"-1nochange",uDataEntity);
            jsonReceives[i].removeMember("sReceivePhone");

            jsonReceives[i]["lUserId"] = tConvert.LongToStr(uDataEntity.lUserId);
            jsonReceives[i]["sNickName"] = uDataEntity.sNickName;
            jsonReceives[i]["sHeadImage"] = uDataEntity.sHeadImage;

        }

    }

    return iRet ;

}

/*
Description:收到的红包查询
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int LukyMoneyBLL::TransferReceive(Json::Value jsonReq,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferReceive.aspx";
    iRet = Requst(sUrl,jsonReq,jsonRespons);

    if( iRet == 0){
        Replace(jsonRespons);


        Json::Value & jList = jsonRespons["jList"];

        for( int i = 0 ; i < jList.size(); i++){
            Json::Value & sPayPhone = jList[i]["sPayPhone"];
            UserDataDAL uDataDal;UserDataEntity uDataEntity;
            uDataDal.GetEntity("-1nochange",tConvert.Json2String(sPayPhone),"-1nochange",uDataEntity);
            jList[i].removeMember("sPayPhone");

            jList[i]["lUserId"] = tConvert.LongToStr(uDataEntity.lUserId);
            jList[i]["sNickName"] = uDataEntity.sNickName;
            jList[i]["sHeadImage"] = uDataEntity.sHeadImage;

        }

    }

    return iRet ;

}


/*
Description:发出的红包查询
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int LukyMoneyBLL::TransferGive(Json::Value jsonReq,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferGive.aspx";
    iRet = Requst(sUrl,jsonReq,jsonRespons);

    if( iRet == 0){
        Replace(jsonRespons);

    }

    return iRet ;

}


//红包下单
//int LukyMoneyBLL::TransferOrder(const string & sOrderNo, string & sPayPhone, const vector<string> & sToPhoneLst,int iCurrency, int iTotalNum,double fTotalAmount,
//                                const string & sRemark , string & sTransferID,Json::Value & jsonRespons)
//{

//    TConvert tConvert;int iRet = -1;

//    Json::Value jsonPost;
//    jsonPost["sPayPhone"] = sPayPhone;
//    jsonPost["iCurrency"] =  iCurrency ;
//    jsonPost["iType"] =  1 ;//类型：1红包
//    jsonPost["iTotalNum"] =  iTotalNum ;
//    jsonPost["mTotalAmount"] =  fTotalAmount ;
//    jsonPost["sRemark"] =  sRemark ;
//    jsonPost["sOrderNo"] = sOrderNo ;
//    vector<string>::const_iterator it = sToPhoneLst.begin();
//    for( int i = 0; it != sToPhoneLst.end() ; it++){
//       jsonPost["jReceivePhoneList"][i] = *it ;
//       i++;
//    }
//    string sPost = jsonPost.toStyledString();
//    //appendlog(TTDLogger::LOG_DEBUG, "xxxxxx%s",sPost.c_str());


//    iRet = Requst("http://mepay.tymplus.com/GetDataInterface/MoneyBag/Transfer/TransferOrder.aspx",jsonPost,jsonRespons);
//    if( 0 == iRet ){
//        sTransferID = tConvert.Json2String( jsonRespons["sTransferID"] );
//    }
//    string sRes = jsonRespons.toStyledString();

//    return iRet ;

//}


//红包下单
int LukyMoneyBLL::TransferOrder(const string & sOrderNo, string & sPayPhone,
                                const vector<string> & sToPhoneLst, int iCurrency,int iCoinType, int iType,
                                int iTotalNum,Json::Value jsonTotalAmount,
                  const string & sRemark , string & sTransferID,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;

    Json::Value jsonPost;
    jsonPost["sPayPhone"] = sPayPhone;
    jsonPost["iCurrency"] =  iCurrency ;
    jsonPost["iSubCurrency"] =  iCoinType ;
    jsonPost["iType"] =  iType ;//类型：1红包
    jsonPost["iTotalNum"] =  iTotalNum ;
    jsonPost["mTotalAmount"] =  jsonTotalAmount ;
    jsonPost["sRemark"] =  sRemark ;
    jsonPost["sOrderNo"] = sOrderNo ;
    vector<string>::const_iterator it = sToPhoneLst.begin();
    for( int i = 0; it != sToPhoneLst.end() ; it++){
       jsonPost["jReceivePhoneList"][i] = *it ;
       i++;
    }


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferOrder.aspx";
    iRet = Requst(sUrl,jsonPost,jsonRespons);
    if( 0 == iRet ){
        sTransferID = tConvert.Json2String( jsonRespons["sTransferID"] );
    }

    return iRet ;

}

//红包支付
int LukyMoneyBLL::TransferPay(const string & sTransferID, const string & sPayPhone, const string & sPayPassword,Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;

    Json::Value jsonPost;
    jsonPost["sPayPhone"] = sPayPhone;
    jsonPost["sTransferID"] =  sTransferID ;
    jsonPost["sPayPassword"] = sPayPassword ;

    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/MoneyBag/Transfer/TransferPay.aspx";
    iRet = Requst(sUrl,jsonPost,jsonRespons);

    return iRet;


}






int LukyMoneyBLL::Requst(const string & sUrl, const Json::Value & jsonPost, Json::Value & jsonRespons)
{
    TConvert tConvert;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),sAesKey.size());
    string sAesPostData = myAES.Encrypt(jsonPost.toStyledString(),4);
    string sRealPostData = "sCondition=" + sAesPostData;


    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    string sAesReturn;
    TCurl tCurl;

    int iRet =tCurl.Posts(sUrl,httpHeaderVector,sRealPostData,sAesReturn,NULL);

    if( 0 == iRet ){
        string sReturn =  myAES.Decrypt(sAesReturn,4);
        Json::Reader reader;
        if (!reader.parse(sReturn, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["iRet"] );
    }
    return iRet ;
}

void LukyMoneyBLL::Replace(Json::Value & jsonRespons)
{
    TConvert tConvert;
    string err =  tConvert.Json2String( jsonRespons["err"]);
    if( err.empty()){
        err = "服务器异常";
    }
    jsonRespons.removeMember("err");
    jsonRespons["sMsg"] = err;
    if(jsonRespons["iRet"].isNull() ){
        jsonRespons["iRet"] = -1000000;
    }
}
