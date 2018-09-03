#include "OfflineOrderDAL.h"




OfflineOrderDAL::OfflineOrderDAL()
{

}



int OfflineOrderDAL::Replace( const OfflineOrder & entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "replace into " +  msTableName+"(" + mAllColumn  + ")" +
            " VALUES( '"+ mlti.RealEscapeString( entity.orderNo )+"','" +
            tConvert.DoubleToStr(entity.money)+"','" +
            mlti.RealEscapeString(entity.payType)+"','" +
            tConvert.IntToStr(entity.payState)+"','" +
            tConvert.LongToStr( entity.platformId)+"','" +
            mlti.RealEscapeString(entity.shopName) + "','" +
            mlti.RealEscapeString(entity.shopPhone) + "','" +
            mlti.RealEscapeString(entity.zone) + "','" +
            tConvert.LongToStr( entity.payUserId)+"','" +
            mlti.RealEscapeString(entity.payNickName) + "','" +
            mlti.RealEscapeString(entity.payPhone) + "','" +
            mlti.RealEscapeString(entity.payHeadImage) + "','" +
            mlti.RealEscapeString(entity.addTime) + "','" +
            mlti.RealEscapeString(entity.upTime) + "','" +
            mlti.RealEscapeString(entity.remark) + "','" +
            tConvert.DoubleToStr(entity.notDividedMoney)+"','" +
            tConvert.IntToStr(entity.offLineDisCount)+"','" +
            tConvert.IntToStr(entity.recType)+"','" +
            tConvert.DoubleToStr(entity.coin)+"','" +
            tConvert.IntToStr(entity.coinType)+"','" +
            tConvert.IntToStr(entity.isFans)+"','" +
            tConvert.DoubleToStr(entity.unPushDivided)+"','" +
            tConvert.DoubleToStr(entity.unPlatformDivided) +
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}



