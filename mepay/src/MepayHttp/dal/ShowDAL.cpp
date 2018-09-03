#include "ShowDAL.h"





int ShowDAL::Add( const ShowPro & entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "insert into " +  msTableName+"( lPlatformId,lAddTime,sPicLink,sName,dPrice,iVerify,iState,id,dCoin,iCoinType,sContent,sExpireTime,sCanot,sUtime,sLaw,sDetailPics)" +
            "VALUES( '"+tConvert.LongToStr( entity.lPlatformId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            mlti.RealEscapeString(entity.sPicLink)+"','" +
            mlti.RealEscapeString(entity.sName)+"','" +
            tConvert.DoubleToStr(entity.dPrice)+"','" +
            tConvert.IntToStr(entity.iVerify)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.id)+"','" +
            tConvert.DoubleToStr( entity.dCoin)+"','" +
            tConvert.IntToStr(entity.iCoinType)+"','" +
            mlti.RealEscapeString(entity.sContent)+"','" +
            mlti.RealEscapeString(entity.sExpireTime)+"','" +
            mlti.RealEscapeString(entity.sCanot)+"','" +
            mlti.RealEscapeString(entity.sUtime)+"','" +
            mlti.RealEscapeString(entity.sLaw)+"','" +
            mlti.RealEscapeString(entity.sDetailPics)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int ShowDAL::UpVerify(long id, int iValue )
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iVerify =" + tConvert.IntToStr(iValue) ;
    sSql += " where id='"+ tConvert.LongToStr(id) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShowDAL::Update(long id, string sPicLink,string sName ,double dPrice, double coin,
                    int coinType, string canot,string content, string detailPics,string expireTime, string utime
                    ,string law)
{
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql = "update "+ msTableName + " set iVerify=0,lAddTime=" + tConvert.LongToStr( tConvert.GetCurrentTime() );
    string sSet;
    if( !sPicLink.empty()){
        sSet += ",sPicLink ='" + mlti.RealEscapeString(sPicLink) + "'" ;
    }
    if( !sPicLink.empty()){
        sSet += ",sName ='" + mlti.RealEscapeString(sName)  + "'";
    }
    if( dPrice >= 0){
        sSet += ",dPrice ='" + tConvert.DoubleToStr(dPrice) + "'";
    }
    if( coin >= 0){
        sSet += ",dCoin ='" + tConvert.DoubleToStr(coin) + "'";
    }
    if( coinType >= 0){
        sSet += ",iCoinType ='" + tConvert.IntToStr(coinType) + "'";
    }

    if( !canot.empty()){
        sSet += ",sCanot ='" + mlti.RealEscapeString(canot) + "'" ;
    }
    if( !content.empty()){
        sSet += ",sContent ='" + mlti.RealEscapeString(content) + "'" ;
    }
    if( !detailPics.empty()){
        sSet += ",sDetailPics ='" + mlti.RealEscapeString(detailPics) + "'" ;
    }
    if( !expireTime.empty()){
        sSet += ",sExpireTime ='" + mlti.RealEscapeString(expireTime) + "'" ;
    }
    if( !utime.empty()){
        sSet += ",sUtime ='" + mlti.RealEscapeString(utime) + "'" ;
    }
    if( !law.empty()){
        sSet += ",sLaw ='" + mlti.RealEscapeString(law) + "'" ;
    }

    sSql += sSet;

    sSql += " where id='"+ tConvert.LongToStr(id) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


string ShowDAL::GetSqlIncreemSallNum(long id ,int iSallNum)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set iSallNum=iSallNum+" + tConvert.IntToStr( iSallNum );
    sSql += " where id='"+ tConvert.LongToStr(id) + "'";
    return sSql;
}

int ShowDAL::IncreemSallNum(long id ,int iSallNum)
{
    string sSql = GetSqlIncreemSallNum(id,iSallNum);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int ShowDAL::Del(long id)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;
    string sSql = "update " + msTableName + " set iState=333  where id=" + tConvert.LongToStr(id);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int ShowDAL::IsExist( long id)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where id="+ tConvert.LongToStr(id);

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

int ShowDAL::GetData(const QueryShowPro & query,int iPage, int iCount,ShowProLst & lst,int & iTotalCount)
{

    int iRet = -1;
    MYSQL_RES* result;

    string sSql = "select a.id,a.lPlatformId,a.sName,a.sPicLink,a.dPrice,a.dCoin,a.iCoinType,b.sShopName from " + msTableName + " as a inner join shopdal as b ";


    string sCondtion = " where a.lPlatformId=b.sPrimaryKey and a.iVerify=1 and a.iState=1 and";
    if(! query.sPlatformId.empty()){
        sCondtion += " a.lPlatformId like '%" + mMulti.RealEscapeString(query.sPlatformId) + "%' and";
    }
    if(! query.sShopName.empty()){
        sCondtion += " b.sShopName like '%" + mMulti.RealEscapeString(query.sShopName) + "%' and";
    }
    sCondtion = sCondtion.substr(0,sCondtion.length() -3);

    sSql += sCondtion;

    sSql += " limit " + mtConvert.IntToStr( (iPage-1)*iCount) + "," + mtConvert.IntToStr(iCount);

    //appendlog(TTDLogger::LOG_DEBUG, "xxxx:%s",sSql.c_str());

    BaseQueryResult(sSql,&result);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {
            ShowPro  entity;
            int iIndex = 0;

            entity.id =  atol(row[iIndex++]);
            entity.lPlatformId =  atol(row[iIndex++]);            
            entity.sName = row[iIndex++];
            entity.sPicLink = row[iIndex++];
            entity.dPrice = atof(row[iIndex++] );
            entity.dCoin = atof(row[iIndex++] );
            entity.iCoinType = atoi(row[iIndex++] );
            entity.sShopName = row[iIndex++] ;


            lst.push_back(entity);

        }
    }

    string sCountSql = "select count(*) from "+ msTableName + " as a inner join shopdal as b ";
    sCountSql += sCondtion;
    GetQueryTotalCount(sCountSql,iTotalCount);

    // 释放内存
    FreeResult( result );
    return iRet;
}

//查询数据
int ShowDAL::GetData(  long  lPlatformId ,int iPage,int iCount, int iVerify,int iState,string sName,ShowProLst & lst, int & iTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL multimysql;

    int iIndex = (iPage -1 )*iCount;

    string sCondition;

    string sSql = "select " + msAllColumn +  " from "+msTableName ;
    if( -1 != iVerify){
      sCondition += " iVerify=" + tConvert.IntToStr(iVerify) + " and";
    }
    if( -1 != iState){
      sCondition += " iState=" + tConvert.IntToStr(iState) + " and";
    }else if (  -1 == iState){
      sCondition += " iState!=" + tConvert.IntToStr(333) + " and";
    }
    if( -1 != lPlatformId){
        sCondition += " lPlatformId=" + tConvert.LongToStr(lPlatformId) + " and";
    }
    if( "" != sName){
        sCondition += " sName like '%" + multimysql.RealEscapeString(sName) + "%' and";
    }

    if( !sCondition.empty()){
        sCondition = sCondition.substr(0,sCondition.length() - 3);
    }
    if( !sCondition.empty()){
        sCondition = " where " + sCondition;
        sSql +=  sCondition;
    }
    sSql += " order by lAddTime desc  limit " +  tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    BaseQueryResult(sSql,&result);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {
            ShowPro  entity;
            RowToEntity(row,entity);

            lst.push_back(entity);

        }
    }

    GetCount(sCondition,iTotalCount);

    // 释放内存
    FreeResult( result );
    return iRet;

}

int ShowDAL::GetEntity(long id ,ShowPro & entity)
{
    int iRet = -1;
    string sSql = "select " + msAllColumn  +  " from "+msTableName  + " where id=" + mtConvert.LongToStr(id);

    MYSQL_RES* result;

    MYSQL_ROW row = NULL;
    BaseQueryResult(sSql, &result);

    if( result != NULL){

        iRet = ERRNOTDATA;



        if ( NULL != (row = mysql_fetch_row(result) )) {

            iRet = 0;

            RowToEntity(row,entity);
        }
    }

    FreeResult( result );
    return iRet;

}

//首页商品轮播
int ShowDAL::GetLoop(string town,string area,ShowProLst & shows)
{
    int iRet = -1;
    string sSql = "select a.id,a.lPlatformId,a.sPicLink,a.sName,a.dPrice,a.dCoin,a.iCoinType  from "
            +msTableName  + " as a inner join "
            + gShopTb +  " as b";
    string sCondition = " where a.lPlatformId=b.sPrimaryKey and a.iState=1 and a.iVerify=1 and b.iShowNearby=1 and left(now(),10)<=a.sExpireTime and b.sAddress like'%" + town + "%" + area + "%'";

    sSql += sCondition;
    sSql += " limit 10";

    MYSQL_RES* result;

    MYSQL_ROW row = NULL;
    BaseQueryResult(sSql, &result);

    if( result != NULL){



        while ( NULL != (row = mysql_fetch_row(result) )) {

            iRet = 0;
            int iIndex = 0;
            ShowPro show;
            show.id = atol(row[iIndex++]);
            show.lPlatformId = atol(row[iIndex++]);
            show.sPicLink = row[iIndex++];
            show.sName = row[iIndex++];
            show.dPrice = atof(row[iIndex++]);
            show.dCoin = atof(row[iIndex++]);
            show.iCoinType = atoi(row[iIndex++]);

            shows.push_back(show);
        }
    }

    FreeResult( result );
    return iRet;

}

//商品分区分类分页
int ShowDAL::GetPageByType(string town,string area,double gpsLat,double gpsLon,int page, string type,int sort,ShowProLst & shows)
{
    int iRet = -1;

    char buf[256] = {0};
    if( gpsLat > 0 && gpsLon > 0){

        //根据经纬度计算距离
        sprintf(buf,",( 6378140 * acos( cos( radians(%f) ) * cos( radians( b.dGpsLat ) ) "
                    "* cos( radians( b.dGpsLon ) - radians(%f) ) + sin( radians(%f) ) * sin( radians( b.dGpsLat ) ) ) ) "
                    "as distance  ",gpsLat,gpsLon,gpsLat);
    }
    string sBuf = buf;

    string sSql = "select a.id,a.lPlatformId,a.sPicLink,a.sName,a.dPrice,a.dCoin,a.iCoinType " + sBuf + " from "
            +msTableName  + " as a inner join "
            + gShopTb +  " as b";
    string sCondition = " where a.lPlatformId=b.sPrimaryKey and a.iState=1 and a.iVerify=1 and b.iShowNearby=1 and left(now(),10)<=a.sExpireTime and b.sAddress like '%" + town + "%" + area  + "%'";
    if( !type.empty()){
        sCondition +=  " and b.sShopType='" + type + "'";
    }

    sSql += sCondition;

    if( sort == 1){
      sSql += " order by distance ";
    }else if( sort == 2){
      sSql += " order by iSallNum desc ";
    }

    string sIndex = mtConvert.IntToStr( (page -1)*10 );
    sSql += " limit " + sIndex + ",10";

    MYSQL_RES* result;

    MYSQL_ROW row = NULL;
    BaseQueryResult(sSql, &result);

    if( result != NULL){



        while ( NULL != (row = mysql_fetch_row(result) )) {

            iRet = 0;
            int iIndex = 0;
            ShowPro show;
            show.id = atol(row[iIndex++]);
            show.lPlatformId = atol(row[iIndex++]);
            show.sPicLink = row[iIndex++];
            show.sName = row[iIndex++];
            show.dPrice = atof(row[iIndex++]);
            show.dCoin = atof(row[iIndex++]);
            show.iCoinType = atoi(row[iIndex++]);

            shows.push_back(show);
        }
    }

    FreeResult( result );
    return iRet;

}

//商品分类数量
int ShowDAL::TypeInfo(string town,string area,StringLongStdMap & typeinfo)
{
    int iRet = -1;
    string sSql = "select b.sShopType,count(*) as iCount from "
            +msTableName  + " as a inner join "
            + gShopTb +  " as b";
    string sCondition = " where a.lPlatformId=b.sPrimaryKey and a.iState=1 and a.iVerify=1 and b.iShowNearby=1 and left(now(),10)<=a.sExpireTime and b.sAddress like '%" + town  + "%" + area + "%'";

    sSql += sCondition;

    sSql += " group by b.sShopType order by iCount desc";


    MYSQL_RES* result;

    MYSQL_ROW row = NULL;
    BaseQueryResult(sSql, &result);

    if( result != NULL){



        while ( NULL != (row = mysql_fetch_row(result) )) {

            iRet = 0;
            typeinfo[row[0]] = atoi( row[1] );

        }
    }

    FreeResult( result );
    return iRet;

}

void ShowDAL::RowToEntity(MYSQL_ROW row , ShowPro & entity)
{
    int iIndex = 0;

    entity.id =  atol(row[iIndex++]);
    entity.lPlatformId =  atol(row[iIndex++]);
    entity.lAddTime = atol(row[iIndex++]);
    entity.sPicLink = row[iIndex++];
    entity.sName = row[iIndex++];
    entity.dPrice = atof(row[iIndex++] );
    entity.dCoin = atof(row[iIndex++] );
    entity.iCoinType = atoi(row[iIndex++] );
    entity.iVerify = atoi(row[iIndex++] );
    entity.iState = atoi(row[iIndex++] );

    entity.sContent = row[iIndex++];
    entity.sExpireTime = row[iIndex++];
    entity.sCanot = row[iIndex++];
    entity.sUtime = row[iIndex++];
    entity.sLaw = row[iIndex++];
    entity.sDetailPics = row[iIndex++];
    entity.sSpecification = row[iIndex++];

}

