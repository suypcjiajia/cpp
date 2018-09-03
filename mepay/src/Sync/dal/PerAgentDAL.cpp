#include "PerAgentDAL.h"




PerAgentDAL::PerAgentDAL()
{

}



int PerAgentDAL::Replace( const PerAgent & entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "replace into " +  msTableName+"(" + mAllColumn  + ")" +
            " VALUES( '"+ mlti.RealEscapeString( entity.addTime )+"','" +
            mlti.RealEscapeString(entity.upTime)+"','" +
            tConvert.IntToStr(entity.state)+"','" +
            mlti.RealEscapeString(entity.city)+"','" +
            mlti.RealEscapeString( entity.pushZone)+"','" +
            mlti.RealEscapeString(entity.pushPhone) + "','" +
            tConvert.LongToStr(entity.pushId) + "','" +
            tConvert.IntToStr(entity.activationState) + "','" +
            mlti.RealEscapeString( entity.activationTime)+"','" +
            mlti.RealEscapeString(entity.dealTime) + "','" +
            tConvert.IntToStr(entity.trialDays) + "','" +
            mlti.RealEscapeString(entity.peragentPhone) + "','" +
            tConvert.LongToStr(entity.userId) + "','" +
            mlti.RealEscapeString(entity.orderNo) + "','" +
            tConvert.DoubleToStr(entity.totalMoney) + "','" +
            tConvert.DoubleToStr(entity.payMoney)+"','" +
            tConvert.IntToStr(entity.payCount)+"','" +
            mlti.RealEscapeString(entity.peragentName)+"','" +
            tConvert.IntToStr(entity.extendType)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}



