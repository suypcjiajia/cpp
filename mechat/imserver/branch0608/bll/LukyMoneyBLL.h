#ifndef LUKYMONEYBLL_H
#define LUKYMONEYBLL_H
#include "bll/PrivateChatBLL.h"
#include "dal/PrivateChatDAL.h"
#include "dal/OrderNumDAL.h"
#include "TypeDef.h"
/*
 *红包(调用钱包服务)
 *
 *
 */


/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装调用钱包
 *Others:
*/

class LukyMoneyBLL
{
public:
    LukyMoneyBLL();


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
    Description:收到的红包查询
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int TransferReceive(Json::Value jsonReq,Json::Value & jsonRespons);


    /*
    Description:发出的红包查询
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int TransferGive(Json::Value jsonReq,Json::Value & jsonRespons);



public:


    //红包,转账
    //lUserId : 发送者
    //lRevUserLst : 一批接收者， 一个或多个
    //sOrderNo : 聊天记录的sKey
    int LukyTransfer(long lUserId, const LongLst & lRevUserLst,const string & sPayPassword,
                   int iTotalNum,Json::Value jsonTotalAmount ,int iCurrency,int iCoinType,int iType,const string & sOrderNo ,
                   Json::Value & jsonRespons);


    //红包,转账 确认
    //sOrderNo : 聊天记录的sKey
    int TransferConfirm(long lUserId, const string & sOrderNo,Json::Value & jsonRespons);

    //红包信息查询
    //sOrderNo : 聊天记录的sKey
    int TransferQuery(const string & sOrderNo,Json::Value & jsonRespons);


private:


    //下单
    int TransferOrder(const string & sOrderNo, string & sPayPhone,const vector<string> & sToPhoneLst, int iCurrency,int iCoinType, int iType,int iTotalNum,Json::Value jsonTotalAmount,
                      const string & sRemark , string & sTransferID,Json::Value & jsonRespons);

    //支付
    int TransferPay(const string & sTransferID, const string & sPayPhone, const string & sPayPassword,Json::Value & jsonRespons);

    int Requst(const string & sUrl, const Json::Value & jsonPost,Json::Value & jsonRespons);

    void Replace(Json::Value & jsonRespons);
private:
    const string sAesKey = "!@#$%^&*()QWERTY";

    OrderNumDAL mOrderNumDal;

};

#endif // LUKYMONEYTIMEOUTBLL_H
