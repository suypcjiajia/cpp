#include "ShopSettingDAL.h"




//“返折扣购物币”开关,默认关
//“折扣返现”开关,默认关
//“返现生效时间”，

ShopSettingDAL::ShopSettingDAL()
{

}



int ShopSettingDAL::Add( const ShopSetting& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql   = "insert into " +  msTableName+"( lPlatformId,lAddTime,iCashBack,iCoinBack,lStartTime)" +
            "VALUES( '"+tConvert.LongToStr( entity.lPlatformId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.IntToStr(entity.iCashBack)+"','" +
            tConvert.IntToStr(entity.iCoinBack)+"','" +
            tConvert.LongToStr( entity.lStartTime)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int ShopSettingDAL::IsExist( long lPlatformId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lPlatformId='"+ tConvert.LongToStr(lPlatformId)   +"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        iRet = -4;
        return iRet;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        iRet = 0; //exist
    }else if( NULL != result ){
        iRet = 1;//no exist
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}



int ShopSettingDAL::UpdateCashBack(long lPlatformId, int  iCashBack )
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iCashBack =" + tConvert.IntToStr(iCashBack) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateStartTime(long lPlatformId ,long lStartTime)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " lStartTime = " + tConvert.LongToStr(lStartTime);
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateCoinBack(long lPlatformId, int  iCoinBack)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iCoinBack =" + tConvert.IntToStr(iCoinBack) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateRecommend(long lPlatformId, int  value)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iRecommend =" + tConvert.IntToStr(value) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateRestrict(long lPlatformId, int  value)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iRestrict =" + tConvert.IntToStr(value) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int ShopSettingDAL::UpdatePark(long lPlatformId, int  value)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iPark =" + tConvert.IntToStr(value) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateWifi(long lPlatformId, int  value)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iWifi =" + tConvert.IntToStr(value) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int ShopSettingDAL::UpdateDelivery(long lPlatformId, int  value)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iDelivery =" + tConvert.IntToStr(value) ;
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::UpdateCheck(long lPlatformId, string value)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " sCheckPwd ='" + multi.RealEscapeString(value)  + "'";
    sSql += " where lPlatformId='"+ tConvert.LongToStr(lPlatformId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShopSettingDAL::GetEntity(long lPlatformId,ShopSetting  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TConvert tConvert;
    // SQL
    string sSql = "select lPlatformId,lAddTime,iCashBack,iCoinBack,lStartTime,iRecommend,iRestrict"
                  ",iPark,iWifi,iDelivery ,sCheckPwd from "+msTableName;
    sSql+=" where  lPlatformId= '"+tConvert.LongToStr(lPlatformId)+"'";
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row =mysql_fetch_row( result );
        if( NULL != row  ) {
            iRet = 0;
            int iIndex = 0;
            entity.lPlatformId = atol(row[iIndex++]);
            entity.lAddTime = atol( row[iIndex++] );
            entity.iCashBack = atoi( row[iIndex++]);
            entity.iCoinBack = atoi(row[iIndex++]);
            entity.lStartTime = atol( row[iIndex++] );
            entity.iRecommend = atoi(row[iIndex++]);
            entity.iRestrict = atoi(row[iIndex++]);
            entity.iPark = atoi(row[iIndex++]);
            entity.iWifi = atoi(row[iIndex++]);
            entity.iDelivery = atoi(row[iIndex++]);
            entity.sCheckPwd = row[iIndex++];

        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}
