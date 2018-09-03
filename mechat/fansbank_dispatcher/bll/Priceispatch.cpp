#include<thread>

#include <unistd.h>
#include "Priceispatch.h"
#include "TBLL.h"
#include "com/Request.h"
#include "MyUtil.h"
PriceDispatch::PriceDispatch()
{

}

void PriceDispatch::Run()
{


    thread t2(SyncUserThread,(void*)this);
   // thread t3(HotThread,(void*)this);
   // thread t4(ConsumHotThread,(void*)this);

    thread t5(TimerThread,(void*)this);


//     pthread_t tid1;
//    pthread_create( &tid1, NULL, MakeKvalueThread, (void*)this );
//        pthread_t tid2;
//        pthread_create( &tid2, NULL, SyncUserThread, (void*)this );
//        pthread_t tid3;
//        pthread_create( &tid3, NULL, HotThread, (void*)this );
//        pthread_t tid4;
//        pthread_create( &tid4, NULL, ConsumHotThread, (void*)this );


    while(true){
        mChageIdMap.clear();

        CaluSign();
        CaluTrade();
        DeIncreemValue();
        UpdateKValue();

        sleep(60);
    }

}

//把注册的用户同步到用户价值表
void* PriceDispatch::SyncUserThread(void*ptr)
{

    PriceDispatch * p = (PriceDispatch*)(ptr);
    p->RealSyncUser();

}



//把注册的用户同步到用户价值表
void PriceDispatch::RealSyncUser()
{

    long lLastId = 0;
    mConfigDAL.GetSyncUser(lLastId);
    while(true){
        longs ls;
        mUserDataDAL.Get100User(lLastId,ls);
        for( int i = 0; i < ls.size(); i++){
            long lUserId = ls[i];

            if( mUserValueDAL.IsExist(lUserId,enum_kvalue_type_platform) == 1){
                mUserValueDAL.Add(lUserId,4.95 ,enum_kvalue_type_platform);
            }

            if( mUserValueDAL.IsExist(lUserId,enum_kvalue_type_push) == 1){
                mUserValueDAL.Add(lUserId,4.95 ,enum_kvalue_type_push);
            }
            lLastId = lUserId;

        }
        mConfigDAL.SetSyncUser(lLastId);
        sleep(5);
    }

}

//处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
void* PriceDispatch::HotThread(void*ptr)
{

    PriceDispatch * p = (PriceDispatch*)(ptr);
    p->RealHot();

}

//处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
void* PriceDispatch::ConsumHotThread(void*ptr)
{

    PriceDispatch * p = (PriceDispatch*)(ptr);
    p->RealConsumHot();

}

//处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
void PriceDispatch::RealHot()
{

    while(true){
        LetBindWeixin2Hot();
        LetPerfectInfo2Hot();

        sleep(20);

    }

}

//处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
void PriceDispatch::RealConsumHot()
{

    while(true){

        sleep(1);

        if( !LetConsume2Hot() ){
            sleep(60*5);
        }

    }

}

//让已绑定微信的粉丝变成可交易状态
void PriceDispatch::LetBindWeixin2Hot()
{
    mUserDataDAL.LetBindWeixinHot();
}

//完善粉丝资料变成可交易状态
void PriceDispatch::LetPerfectInfo2Hot()
{
    mUserDataDAL.LetPerfectInfo2Hot();

}

//从钱包服务批量获取消费手机号，把这批手机粉丝设置成可交易状态
bool PriceDispatch::LetConsume2Hot()
{
    //内网  appKey=8b0d24f77338b9e8e4f66e1c86666d79b66829b6  appSecret=df75dc0ca66690b0

    //外网  appKey:'d38cb571ee4936e44dbf4b36d9f43f3537dc6c76',appSecret:'558aa11af8e9c747'

    string respons;
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;
    bool bRet = false;

    string sPutTime;
    mConfigDAL.GetConsumeSyncTime(sPutTime);
    int pageCount = 100;

    Json::Value paramJSON ;
    paramJSON["pageIndex"] =  "1";
    paramJSON["pageSize"]  =  tConvert.IntToStr(pageCount);
    paramJSON["addtimeStartTime"] =  sPutTime;
    paramJSON["_otherType"]= "乐兑商城联盟商家";
    paramJSON["iState"]= "交易过的";
    paramJSON["recordOrderby"]= "id";

    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    string url = "http://" + TBLL::GetInstance()->GetWalletIP() + "/Api/Finance/TabPayOrderService/getListByConsumePhone"
                                                                  "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);

    Request::WalletHttpPost2(url,APP_SECRET,tConvert.LongToStr(timestamp),paramJSON.toStyledString(),respons);
    Json::Value jRepons;
    tConvert.Str2Json(respons,jRepons);

    appendlog(TTDLogger::LOG_INFO,"url:%s respons:%s",url.c_str(),respons.c_str());

    if( tConvert.Json2String( jRepons["state"] ) == "Success" ){
        Json::Value jResult = jRepons["result"];

        string sLastTime;
        string sWhere = "sAccount in(";
        for( int i = 0; i < jResult.size(); i++ ){
            Json::Value jen = jResult[i];
            string phone = tConvert.Json2String( jen["phone"] );
            string addtime = tConvert.Json2String( jen["addtime"] );
            sWhere +=  "'" + phone + "',";

            if( sLastTime < addtime){
                sLastTime = addtime;
            }


        }
        if( jResult.size() > 0){
            sWhere = sWhere.substr(0,sWhere.length() -1);
            sWhere += ")";
            mUserDataDAL.LetConsume2Hot(sWhere);
            mConfigDAL.SetConsumeSyncTime(sLastTime);


        }

        static int total = 0;
        if( jResult.size() > 1)
            total += jResult.size();


        if( jResult.size() >= pageCount){
            bRet = true;
        }
    }
    return bRet;


}


void* PriceDispatch::TimerThread(void*ptr)
{
    TConvert tConvert;
    string sCur;

    while( true){

        sleep(1);

        string sDayTime = tConvert.GetCurrentDayTime();
        string sTime =sDayTime.substr(11,2);
        string sDay = sDayTime.substr(0,10);
        if( sTime == "01" && sCur != sDay){//每天凌晨1点执行一次

            sCur = sDay;

            thread * t1 = new thread(MakeKvalueThread,(void*)ptr);

        }

    }

}

//每天产生大于10个粉丝的行情
void* PriceDispatch::MakeKvalueThread(void*ptr)
{
    PriceDispatch * p = (PriceDispatch*)(ptr);
    p->RealMakevalue();
}

void PriceDispatch::RealMakevalue()
{
    longs ls;
    mUserDataDAL.Get100FansPlatfrom(ls);
    enum_kvalue_type eType = enum_kvalue_type_platform;

    MakeKvalue(ls,eType);

    ls.clear();;
    mUserDataDAL.Get100FansPush(ls);
    eType = enum_kvalue_type_push;

    MakeKvalue(ls,eType);



}


void PriceDispatch::MakeKvalue(longs & ls, enum_kvalue_type eType)
{


    for( longs::iterator it = ls.begin(); it != ls.end(); it++){
        long & Id = *it;


        double dAvg = 1.0;
        string sName,sShopPhone;

        if( eType == enum_kvalue_type_platform){
            mUserValueDAL.GetPlatformAvg(sShopPhone,Id,eType,dAvg);
            sName = GetShopName(Id);
        }
        else{
            mUserValueDAL.GetPushAvg( Id,eType,dAvg);
            UserDataEntity userData;
            mUserDataDAL.GetEntityById(Id,userData);
            sName = userData.sNickName;

        }

        string sCurDay = mtConvert.GetCurrentDay();

        KValueEntity kvalueEntity;
        if(mKValueDAL.IsExist(Id,eType,sCurDay) != 0){

            KValueEntity yk;
            mKValueDAL.GetEntity(Id,eType,mtConvert.GetYesterDay(),yk);
            if( yk.close <= 0){
                yk.close = GetRandByPrice(dAvg);
            }

            kvalueEntity.price = dAvg;
            GetRandByPrice(dAvg,kvalueEntity.open,kvalueEntity.close,kvalueEntity.hight,kvalueEntity.low);
            kvalueEntity.turnOver = 0.0;
            kvalueEntity.turnRate = 0.0;
            kvalueEntity.Id = Id;
            kvalueEntity.iType = eType;
            kvalueEntity.sDay = sCurDay;
            kvalueEntity.sName = sName;
            kvalueEntity.yclose = yk.close;
            mKValueDAL.Add(kvalueEntity);

        }
    }

}


//贬值处理
int PriceDispatch::DeIncreemValue()
{
    string sDay;
    mConfigDAL.GetDeValueDay(sDay);
    string sCurDay = mtConvert.GetCurrentDay();
    if(sCurDay != sDay){//确保每天只执行一次
        mUserValueDAL.DeIncreemVeryUser();
        mUserValueDAL.IncreemVeryUser();
        mConfigDAL.SetDeValueDay(sCurDay);
    }
    mUserValueDAL.SetLowest();
    return 0;
}

//产生每天活跃用户价值
int PriceDispatch::CaluSign()
{
    UserSignLst userSignLst;
    mUserSignDAL.GetSigns(userSignLst);

    longs lUserIds;

    for( int i = 0; i < userSignLst.size(); i++){
        UserSignEntity & entity = userSignLst[i];

        if( mUserValueDAL.IsExist(entity.lUserId,enum_kvalue_type_platform) == 1){
            mUserValueDAL.Add(entity.lUserId,1.0 + 0.1,enum_kvalue_type_platform);//每个用户保底1元,签到用户每天加0.1元
        }else{
            mUserValueDAL.IncreemValue(entity.lUserId,0.1,enum_kvalue_type_platform);//签到用户每天加0.1元
        }

        if( mUserValueDAL.IsExist(entity.lUserId,enum_kvalue_type_push) == 1){
            mUserValueDAL.Add(entity.lUserId,1.0 + 0.1,enum_kvalue_type_push);//每个用户保底1元,签到用户每天加0.1元
        }else{
            mUserValueDAL.IncreemValue(entity.lUserId,0.1,enum_kvalue_type_push);//签到用户每天加0.1元
        }

        lUserIds.push_back(entity.lUserId);

        mChageIdMap[entity.lUserId] = 0;
    }

    //设置签到为已计算状态
    mUserSignDAL.SetState(lUserIds);

    return 0;

}


//计算用户在联盟商家交易额，产生交易用户价值
int PriceDispatch::CaluTrade()
{
    TConvert tConvert;
    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +  "/GetDataInterface/Consume/ReportForm/GetUserConsumeList.aspx";

    long lSyncTimeTrade = 0;
    if( 0 != mConfigDAL.GetSyncTimeTrade(lSyncTimeTrade)){//获取上一次的同步时间
        return 0;
    }



    Json::Value jPost,jResponse;

    jPost["sStartTime"] = tConvert.TimeToStr(lSyncTimeTrade);

    //调用桌斌的联盟商家用户交易量
    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);
    appendlog(TTDLogger::LOG_INFO, "PriceDispatch::CaluTrade get trade param:%s from wallet:%s",jPost.toStyledString().c_str(),jResponse.toStyledString().c_str());

    if( iRet == 0){


        Json::Value jlistUserConsume =  jResponse["listUserConsume"];
        long lLastTime = mtConvert.StrToTime( mtConvert.Json2String( jResponse["sLastTime"] ).c_str() );
        mConfigDAL.SetSyncTimeTrade(lLastTime);

        for( int i = 0 ; i  < jlistUserConsume.size() ; i++){
            string sPhone = tConvert.Json2String( jlistUserConsume[i]["sPhone"]);
            double dMoney = tConvert.Json2Double( jlistUserConsume[i]["dMoney"]);
            long lPayId = 0,lCurrentPlatformId = 0;
            mUserDataDAL.GetUserIdByPhone(sPhone,lPayId,lCurrentPlatformId);
            if(  0 == lPayId ){//桌斌给的手机号，还没注册，过滤掉
                continue;
            }
            mChageIdMap[lPayId] = 0;


            string sYesterDayStart = mtConvert.GetYesterDay() + " 00:00:00";
            string sYesterDayEnd = mtConvert.GetYesterDay() + " 23:59:59";
            Json::Value jRes;
            double dRecTotalMoney = 0.0;
            if( 0 == GetMerRecInfo(lCurrentPlatformId,sYesterDayStart,sYesterDayEnd,jRes)){
                dRecTotalMoney = mtConvert.Json2Double( jRes["dRecTotalMoney"] );
            }





            enum_kvalue_type eType = enum_kvalue_type_platform;
            if( mUserValueDAL.IsExist(lPayId,eType) == 1){
                mUserValueDAL.Add(lPayId, 1.0 + GetTradeValue(dRecTotalMoney,dMoney,eType),eType);//每个用户保底1元,用户交易量*0.002
            }else{
                mUserValueDAL.IncreemValue(lPayId,GetTradeValue(dRecTotalMoney,dMoney,eType),eType);//用户交易量*0.002
            }

            eType = enum_kvalue_type_push;
            if( mUserValueDAL.IsExist(lPayId,eType) == 1){
                mUserValueDAL.Add(lPayId, 1.0 + GetTradeValue(dRecTotalMoney,dMoney,eType),eType);//每个用户保底1元,用户交易量*0.002
            }else{
                mUserValueDAL.IncreemValue(lPayId,GetTradeValue(dRecTotalMoney,dMoney,eType),eType);//用户交易量*0.002
            }

        }

    }

}


//更新日K线数据
int PriceDispatch::UpdateKValue()
{

    UserDataList users;
    mUserDataDAL.GetUserInfoList(mChageIdMap,users);

    mPlatformMap.clear();mPushMap.clear();

    for( UserDataList::iterator it = users.begin(); it != users.end(); it++){
        UserDataEntity & entity = *it;
        mPlatformMap[entity.lCurrentPlatformId] = 0;
        mPushMap[entity.lPushManId] = 0;
    }

    //更新平台日K线数据
    RealUpdateKValue(mPlatformMap,enum_kvalue_type_platform);
    //更新推广日K线数据
    RealUpdateKValue(mPushMap,enum_kvalue_type_push);


}


//更新日K线数据
int PriceDispatch::RealUpdateKValue(LongLongMap & IdMap, enum_kvalue_type eType){

    TConvert tConvert;

    string sCurDay = tConvert.TimeToStr( tConvert.GetCurrentTime()/1000000).substr(0,10);

    for( LongLongMap::iterator it = IdMap.begin(); it != IdMap.end(); it++){
        long Id = it->first;
        double dAvg = 1.0;

        string sName, sShopPhone;

        if( eType == enum_kvalue_type_platform){

            Json::Value  jRes;
            GetShopEntity(Id,jRes);
            sName = mtConvert.Json2String(  jRes["entity"]["sShopName"] );
            sShopPhone = mtConvert.Json2String(  jRes["entity"]["sShopPhone"] );

            mUserValueDAL.GetPlatformAvg(sShopPhone,Id,enum_kvalue_type_platform,dAvg);

        }
        else{
            mUserValueDAL.GetPushAvg( Id,enum_kvalue_type_push,dAvg);
            UserDataEntity userData;
            mUserDataDAL.GetEntityById(Id,userData);
            sName = userData.sNickName;

        }

        KValueEntity kvalueEntity;
        if(mKValueDAL.IsExist(Id,eType,sCurDay) == 0){

            mKValueDAL.GetEntity(Id,eType,sCurDay,kvalueEntity);
            //更新实时价格
            mKValueDAL.UpdatePrice(Id,eType,sCurDay ,dAvg);
            //让收盘价等于实时价格
            mKValueDAL.UpdateClose(Id,eType,sCurDay ,dAvg);
            if( kvalueEntity.hight < dAvg){
                //更新最高价
                mKValueDAL.UpdateHigh(Id,eType,sCurDay ,dAvg);
            }
            if( kvalueEntity.low > dAvg){
                //更新最低价
                mKValueDAL.UpdateLow(Id,eType,sCurDay ,dAvg);
            }

        }else{

            KValueEntity yk;
            mKValueDAL.GetEntity(Id,eType,tConvert.GetYesterDay(),yk);
            if( yk.close <= 0){
                yk.close = GetRandByPrice(dAvg);
            }

            kvalueEntity.price = dAvg;
            GetRandByPrice(dAvg,kvalueEntity.open,kvalueEntity.close,kvalueEntity.hight,kvalueEntity.low);
            kvalueEntity.turnOver = 0.0;
            kvalueEntity.turnRate = 0.0;
            kvalueEntity.Id = Id;
            kvalueEntity.iType = eType;
            kvalueEntity.sDay = sCurDay;
            kvalueEntity.sName = sName;
            kvalueEntity.yclose = yk.close;
            mKValueDAL.Add(kvalueEntity);

        }
    }
}


double PriceDispatch::GetRandByPrice(double dPrice)
{
    return (MyUtil::GetRand(101) + 50)*dPrice/100 + 1.0;
}

void PriceDispatch::GetRandByPrice(double dPrice, double & dOpen, double & dClose, double & dHight, double & dLow)
{
    double d[5];
    d[0] = dPrice;
    for( int i = 1; i < 5;i++){
        d[i] = GetRandByPrice(dPrice);
    }
    dOpen = d[1];
    dClose = d[2];
    dHight = dPrice;
    dLow = dPrice;

    for( int i = 0; i < 5; i++){
        if( d[i] > dHight){
            dHight = d[i];
        }
        if( d[i] < dLow){
            dLow = d[i];
        }
    }

}



//获取商家收款信息,调用桌斌
int PriceDispatch::GetMerRecInfo( long lPlatformId ,string sStartTime, string sEndTime,Json::Value & jResponse)
{
    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +"/GetDataInterface/Consume/ReportForm/GetMerRecInfo.aspx";

    Json::Value jPost;
    jPost["lPlatformId"] = mtConvert.LongToStr( lPlatformId );
    jPost["sStartTime"] =sStartTime;
    jPost["sEndTime"] =sEndTime;

    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);
    string s = jResponse.toStyledString();

    return iRet;
}


string PriceDispatch::GetShopName(long Id)
{
    Json::Value jPost,jRes;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return mtConvert.Json2String(  jRes["entity"]["sShopName"] );
}

int PriceDispatch::GetShopEntity(long Id,Json::Value & jRes)
{
    Json::Value jPost;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return 0;
}

double PriceDispatch::GetTradeValue(double dRecTotalMoney,double dTrade,int iType)
{

    if( iType == enum_kvalue_type_platform){
        return GetNearPoint(dRecTotalMoney)*dTrade;
    }else{
        return dTrade*0.002;
    }

}

//根据实体店流水，得到价值比例
double PriceDispatch::GetNearPoint(double dRecTotalMoney)
{
    //    实体店的昨日流水<=200元	0.20%
    //    实体店的昨日流水<=300元	0.30%
    //    实体店的昨日流水<=400元	0.40%
    //    实体店的昨日流水<=500元	0.50%
    //    实体店的昨日流水<=600元	0.60%
    //    实体店的昨日流水<=700元	0.70%
    //    实体店的昨日流水<=800元	0.80%
    //    实体店的昨日流水<=900元	0.90%
    //    实体店的昨日流水>=1000元	1%


    int iBase  = dRecTotalMoney/1000;
    double dPoin = 0.001;
    double dNearPoin = iBase*dPoin;
    if( dNearPoin < 0.002){
        dNearPoin = 0.002;
    }
    return dNearPoin;

    //    if( dRecTotalMoney <= 200){
    //        return 0.002;
    //    }else if( dRecTotalMoney <=300){
    //        return 0.003;
    //    }else if( dRecTotalMoney <=400){
    //        return 0.004;
    //    }else if( dRecTotalMoney <=500){
    //        return 0.005;
    //    }else if( dRecTotalMoney <=600){
    //        return 0.006;
    //    }else if( dRecTotalMoney <=700){
    //        return 0.007;
    //    }else if( dRecTotalMoney <=800){
    //        return 0.008;
    //    }else if( dRecTotalMoney <=900){
    //        return 0.009;
    //    }else if( dRecTotalMoney  > 900){
    //        return 0.01;

}
