#include "ShopInfoDAL.h"
#include "dal/TMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
#include "stdio.h"
ShopInfoDAL* ShopInfoDAL::mInstance = NULL;
ShopInfoDAL* ShopInfoDAL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new ShopInfoDAL;
    }
    return mInstance;
}

//添加
int ShopInfoDAL::Insert( const ShopInfoEntity& entity  )
{
    ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "insert into "+gShopTb+"( sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                                         "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                                         "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                                         "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                                         "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,sOtherCerPhoto,sPushArea,iCoinType,iProrate)"
                                         "VALUES( '"+ (entity.sPrimaryKey)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lUpdateId)+"','" +
            "0','" +
            tConvert.LongToStr(entity.contentEntity.lUserId)+"','" +
            multi.RealEscapeString(entity.contentEntity.sAccount)+"','" +
            multi.RealEscapeString(entity.contentEntity.sPassWord)+"','" +
            multi.RealEscapeString(entity.contentEntity.sShopName)+"','" +
            multi.RealEscapeString(entity.contentEntity.sCompanyName)+"','" +
            multi.RealEscapeString(entity.contentEntity.sShopPhone)+"','" +
            multi.RealEscapeString(entity.contentEntity.sBusiness)+"','" +
            multi.RealEscapeString(entity.contentEntity.sShopType)+"','" +
            tConvert.IntToStr(entity.contentEntity.iOffLineDisCount)+"','" +
            multi.RealEscapeString( (entity.contentEntity.sLicensePhoto))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sIdPhoto))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sShopFrontPhoto))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sShopDetailPhoto))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sAddress))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sCtiy))+"','" +
            tConvert.DoubleToStr(entity.contentEntity.dGpsLat)+"','" +
            tConvert.DoubleToStr(entity.contentEntity.dGpsLon)+"','" +
            multi.RealEscapeString( (entity.contentEntity.sHours))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sPosNum))+"','" +
            tConvert.LongToStr(entity.contentEntity.lPerAgentId)+"','" +
            tConvert.IntToStr(entity.contentEntity.iAwardingPoints)+"','" +
            tConvert.LongToStr(entity.contentEntity.lServiceUserId)+"','" +
            multi.RealEscapeString( (entity.contentEntity.sSevvicePhone)) +"','" +
            tConvert.IntToStr(entity.contentEntity.iAvgConsume)+"','" +
            tConvert.IntToStr(entity.contentEntity.iShowNearby)+"','" +
            tConvert.IntToStr(entity.contentEntity.iVerify)+"','" +
            multi.RealEscapeString( (entity.contentEntity.sOtherCerPhoto))+"','" +
            multi.RealEscapeString( (entity.contentEntity.sPushArea))+"','" +
            tConvert.IntToStr(entity.contentEntity.iCoinType)+"','" +
            tConvert.IntToStr(entity.contentEntity.iProrate)+
            "')";

    appendlog(TTDLogger::LOG_DEBUG,"ShopInfoDAL::Insert sql:%s",sSql.c_str());

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}
//删除
int ShopInfoDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
{
    ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lUptTime) + "' "+
            ",iState = '333' "+
            ",iSync = '0' ";
    sSql += " where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//修改
int ShopInfoDAL::Update(  const string& sPrimaryKey,const ShopInfoEntity& pEntity )
{
    TConvert tConvert;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(pEntity.lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(pEntity.lUpTime) + "' "+
            ",iSync = '0'" ;

    if( pEntity.iState != -1){
        sSql += ",iState='" + tConvert.IntToStr(pEntity.iState) + "' ";
    }
    if( pEntity.contentEntity.lUserId != -1){
        sSql += ",lUserId = '" + tConvert.IntToStr(pEntity.contentEntity.lUserId) + "' ";
    }
    if (pEntity.contentEntity.iVerify != -1) {
        sSql += ",iVerify = '" + tConvert.IntToStr(pEntity.contentEntity.iVerify) + "' ";
    }
    if (pEntity.contentEntity.iAuth != -1) {
        sSql += ",iAuth = '" + tConvert.IntToStr(pEntity.contentEntity.iAuth) + "' ";
    }
    if ( !pEntity.contentEntity.sAddress.empty()) {
        sSql += ",sAddress = '" + mlti.RealEscapeString( pEntity.contentEntity.sAddress) + "' ";
    }
    if (pEntity.contentEntity.iAwardingPoints != -1) {
        sSql += ",iAwardingPoints = '" + tConvert.IntToStr(pEntity.contentEntity.iAwardingPoints) + "' ";
    }
    if( pEntity.contentEntity.dGpsLat > 0){
        sSql += ",dGpsLat = '" + tConvert.DoubleToStr(pEntity.contentEntity.dGpsLat) + "' ";
    }
    if( pEntity.contentEntity.dGpsLon > 0){
        sSql += ",dGpsLon = '" + tConvert.DoubleToStr(pEntity.contentEntity.dGpsLon) + "' ";
    }
    if( pEntity.contentEntity.iOffLineDisCount >= 0){
        sSql += ",iOffLineDisCount = '" + tConvert.IntToStr(pEntity.contentEntity.iOffLineDisCount) + "' ";
    }
    if( pEntity.contentEntity.lServiceUserId > 0){
        sSql += ",lServiceUserId = '" + tConvert.LongToStr(pEntity.contentEntity.lServiceUserId) + "' ";
    }
    if( !pEntity.contentEntity.sBusiness.empty()){
        sSql += ",sBusiness = '" + mlti.RealEscapeString( (pEntity.contentEntity.sBusiness)) + "' ";
    }
    if( !pEntity.contentEntity.sCompanyName.empty()){
        sSql += ",sCompanyName = '" + mlti.RealEscapeString( (pEntity.contentEntity.sCompanyName)) + "' ";
    }

    if( pEntity.contentEntity.lPerAgentId > 0 ){
        sSql += ",lPerAgentId = '" + tConvert.LongToStr( pEntity.contentEntity.lPerAgentId) + "' ";
    }
    if( !pEntity.contentEntity.sHours.empty()){
        sSql += ",sHours = '" + mlti.RealEscapeString( (pEntity.contentEntity.sHours)) + "' ";
    }
    if( !pEntity.contentEntity.sIdPhoto.empty()){
        sSql += ",sIdPhoto = '" + mlti.RealEscapeString( (pEntity.contentEntity.sIdPhoto)) + "' ";
    }
    if( !pEntity.contentEntity.sLicensePhoto.empty()){
        sSql += ",sLicensePhoto = '" + mlti.RealEscapeString( (pEntity.contentEntity.sLicensePhoto)) + "' ";
    }
    if( !pEntity.contentEntity.sPosNum.empty()){
        sSql += ",sPosNum = '" + mlti.RealEscapeString( (pEntity.contentEntity.sPosNum)) + "' ";
    }
    if( !pEntity.contentEntity.sSevvicePhone.empty()){
        sSql += ",sSevvicePhone = '" + mlti.RealEscapeString( (pEntity.contentEntity.sSevvicePhone)) + "' ";
    }
    if( !pEntity.contentEntity.sShopDetailPhoto.empty()){
        sSql += ",sShopDetailPhoto = '" + mlti.RealEscapeString( (pEntity.contentEntity.sShopDetailPhoto)) + "' ";
    }
    if( !pEntity.contentEntity.sShopFrontPhoto.empty()){
        sSql += ",sShopFrontPhoto = '" + mlti.RealEscapeString( (pEntity.contentEntity.sShopFrontPhoto)) + "' ";
    }
    if( !pEntity.contentEntity.sShopName.empty()){
        sSql += ",sShopName = '" + mlti.RealEscapeString( (pEntity.contentEntity.sShopName)) + "' ";
    }
    if( !pEntity.contentEntity.sShopPhone.empty()){
        sSql += ",sShopPhone = '" + mlti.RealEscapeString( (pEntity.contentEntity.sShopPhone)) + "' ";
    }
    if( !pEntity.contentEntity.sShopType.empty()){
        sSql += ",sShopType = '" + mlti.RealEscapeString( (pEntity.contentEntity.sShopType)) + "' ";
    }
    if( pEntity.contentEntity.iAvgConsume >= 0){
        sSql += ",iAvgConsume = '" + tConvert.IntToStr(pEntity.contentEntity.iAvgConsume) + "' ";
    }
    if( pEntity.contentEntity.iShowNearby != -1){
        sSql += ",iShowNearby = '" + tConvert.IntToStr(pEntity.contentEntity.iShowNearby) + "' ";
    }
    if( pEntity.contentEntity.iProrate != -1){
        sSql += ",iProrate = '" + tConvert.IntToStr(pEntity.contentEntity.iProrate) + "' ";
    }
    if( !pEntity.contentEntity.sOtherCerPhoto.empty()){
        sSql += ",sOtherCerPhoto = '" + mlti.RealEscapeString( (pEntity.contentEntity.sOtherCerPhoto)) + "' ";
    }
    if( !pEntity.contentEntity.sTradeMark.empty()){
        sSql += ",sTradeMark = '" + mlti.RealEscapeString( (pEntity.contentEntity.sTradeMark)) + "' ";
    }
    if( pEntity.contentEntity.iCoinType >= 0){
        sSql += ",iCoinType ='" + tConvert.IntToStr( (pEntity.contentEntity.iCoinType)) + "' ";
    }
    sSql += " where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//特殊促销配置
int ShopInfoDAL::PromotionConfig(  const string& sPlatformId,int iCoinPersend, int iPromotionDiscount )
{
    TConvert tConvert;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "update "+gShopTb+" set iCoinPercent=" + tConvert.IntToStr(iCoinPersend);
    sSql += ",iPromotionDiscount=" + tConvert.IntToStr(iPromotionDiscount);
    string sCondition = " where sPrimaryKey='" + mlti.RealEscapeString(sPlatformId) + "'";
    sSql += sCondition;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//获取属于我授权商店(收款记录)
int ShopInfoDAL::MyImpowerShop(long userId,ShopInfoList & lst)
{
    string sSql = "SELECT sd.sPrimaryKey, sd.sAddress, sd.sShopName, sd.sShopFrontPhoto, sd.sShopPhone, sd.sAccount FROM shopdal AS sd "
                  " LEFT JOIN user_power AS up ON sd.sPrimaryKey=up.idPlatform WHERE up.power=1 and up.userId=" + mtConvert.LongToStr(userId)
                   + "AND sd.sCity!='美支付供应商'";


    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                int iIndex = 0;
                ShopInfoEntity entity;
                entity.sPrimaryKey = row[iIndex++] ;
                entity.contentEntity.sAddress =  row[iIndex++] ;
                entity.contentEntity.sShopName = row[iIndex++] ;
                entity.contentEntity.sShopFrontPhoto = row[iIndex++] ;
                entity.contentEntity.sShopPhone = row[iIndex++] ;
                entity.contentEntity.sAccount = row[iIndex++] ;

                lst.push_back(entity);


        }
    }

    con->FreeResult( result );

    return iRet;
}

//查询数据
int ShopInfoDAL::GetList( const ShopQueryEntity & pQueryEntity , int piIndex, int piCount, ShopInfoList & plstOut,  int & piTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sRemark,sOtherCerPhoto,sTradeMark,iCoinType,iCoinPercent,iPromotionDiscount ";


    if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        char buf[256] = {0};
        //根据经纬度计算距离
        sprintf(buf,",( 6378140 * acos( cos( radians(%f) ) * cos( radians( dGpsLat ) ) "
                    "* cos( radians( dGpsLon ) - radians(%f) ) + sin( radians(%f) ) * sin( radians( dGpsLat ) ) ) ) "
                    "as distance  ",pQueryEntity.dGpsLat,pQueryEntity.dGpsLon,pQueryEntity.dGpsLat);
        sSql += buf;
    }
    sSql +=  " from "+gShopTb;

    string sCond = " where iState=0 and";
    if( ! pQueryEntity.sCtiy.empty()){
        if( pQueryEntity.sCtiy == "citys"){
            sCond += " sCity!='美支付供应商' and";
        }else{
            sCond += " sCity like '%" + multi.RealEscapeString(pQueryEntity.sCtiy) + "%' and";
        }
    }
    if( ! pQueryEntity.sPlatformId.empty()){
        sCond += " sPrimaryKey like '%" + multi.RealEscapeString( pQueryEntity.sPlatformId ) + "%' and";
    }
    if( ! pQueryEntity.sShopName.empty()){
        sCond += " sShopName like '%" + multi.RealEscapeString(pQueryEntity.sShopName) + "%' and";
    }
    if( ! pQueryEntity.sShopType.empty()){
        sCond += " sShopType= '" + multi.RealEscapeString(pQueryEntity.sShopType) + "' and";
    }
    if(  pQueryEntity.iVerify >= 0){
       sCond += " iVerify= '" + tConvert.IntToStr( pQueryEntity.iVerify ) + "' and";
    }
    if(  pQueryEntity.iAuth >= 0){
       sCond += " iAuth= '" + tConvert.IntToStr( pQueryEntity.iAuth ) + "' and";
    }
    if( ! pQueryEntity.sShopPhone.empty()){
        sCond += " sShopPhone= '" +  multi.RealEscapeString(pQueryEntity.sShopPhone) + "' and";
    }
    if(  pQueryEntity.lPerAgentId > 0 ){
        sCond += " lPerAgentId= '" + tConvert.LongToStr( pQueryEntity.lPerAgentId )+ "' and";
    }
    if(  pQueryEntity.iApplyDiscount >= 0){
        sCond +=  " left(sRemark,2)>'00' and";
    }
    if(  pQueryEntity.iShowNearby >= 0){
        sCond +=  " iShowNearby='" +  tConvert.IntToStr( pQueryEntity.iShowNearby ) + "' and";
    }

    if(  pQueryEntity.lStartTime >= 0){
        sCond +=  " lAddTime>'" +  tConvert.LongToStr( pQueryEntity.lStartTime ) + "' and";
    }

    if(  pQueryEntity.lEndTime >= 0){
        sCond +=  " lAddTime<'" +  tConvert.LongToStr( pQueryEntity.lEndTime ) + "' and";
    }

    if(  pQueryEntity.iEnumDiscount > 0){
        int iDisCountStart = 0,iDisCountEnd = 0;
        if( pQueryEntity.iEnumDiscount == 1){
            iDisCountStart = 100 ;
            iDisCountEnd =  90;
        }else if( pQueryEntity.iEnumDiscount == 2){
            iDisCountStart = 90;
            iDisCountEnd =  80;
        }else if( pQueryEntity.iEnumDiscount == 3){
            iDisCountStart = 80;
            iDisCountEnd =  70;
        }else{
            iDisCountStart = 70;
            iDisCountEnd = 0;
        }
        sCond +=  " iOffLineDisCount>=" +  tConvert.LongToStr( iDisCountEnd ) + " and iOffLineDisCount<"  + tConvert.LongToStr( iDisCountStart ) + " and";
    }

    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;

    if( pQueryEntity.bStar){
        sSql += " order by iScore desc ";
    }else  if (pQueryEntity.iEnumDiscount == 0){
        sSql += " order by iOffLineDisCount asc";

    }else if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        sSql += " order by distance";
    }else{
        sSql += " order by  lAddTime desc";
    }
    sSql+=" limit "+tConvert.IntToStr(piIndex)+","+tConvert.IntToStr(piCount);
   // appendlog(TTDLogger::LOG_DEBUG, "xxxx:%s",sSql.c_str());

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            string sRemark = row[iIndex++];
            entity.contentEntity.sOtherCerPhoto = row[iIndex++];
            entity.contentEntity.sTradeMark = row[iIndex++];
            entity.contentEntity.iCoinType =  atoi(row[iIndex++]);
            entity.contentEntity.iCoinPercent =  atoi(row[iIndex++]);
            entity.contentEntity.iPromotionDiscount =  atoi(row[iIndex++]);
            if( pQueryEntity.iApplyDiscount >=0){
                //查询申请折扣变更的商家列表时，折扣信息不是当前折扣                
                if( sRemark.size() >= 2){
                    entity.contentEntity.iOffLineDisCount =  atoi(sRemark.substr(0,2).c_str());
                }
            }
            if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
                entity.contentEntity.lDistance = atol( row[iIndex++] );
            }

            plstOut.push_back(entity);

        }
        sSql = "select count(*)  from " + gShopTb ;
        sSql  +=  sCond;

        GetQueryTotalCount(sSql,piTotalCount);
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


//查询数据
int ShopInfoDAL::GetData( const ShopQueryEntity & pQueryEntity , int piIndex, int piCount, ShopInfoList & plstOut,  int & piTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sRemark,sOtherCerPhoto,sTradeMark,iCoinType,iCoinPercent,iPromotionDiscount ";


    if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        char buf[256] = {0};
        //根据经纬度计算距离
        sprintf(buf,",( 6378140 * acos( cos( radians(%f) ) * cos( radians( dGpsLat ) ) "
                    "* cos( radians( dGpsLon ) - radians(%f) ) + sin( radians(%f) ) * sin( radians( dGpsLat ) ) ) ) "
                    "as distance  ",pQueryEntity.dGpsLat,pQueryEntity.dGpsLon,pQueryEntity.dGpsLat);
        sSql += buf;
    }
    sSql +=  " from "+gShopTb;

    string sCond = " where iState!=333  and";
    if( ! pQueryEntity.sCtiy.empty()){
        if( pQueryEntity.sCtiy == "citys"){
            sCond += " sCity!='美支付供应商' and";
        }else{
            sCond += " sCity like '%" + multi.RealEscapeString(pQueryEntity.sCtiy) + "%' and";
        }
    }
    if( ! pQueryEntity.sPlatformId.empty()){
        sCond += " sPrimaryKey like '%" + multi.RealEscapeString( pQueryEntity.sPlatformId ) + "%' and";
    }
    if( ! pQueryEntity.sShopName.empty()){
        sCond += " sShopName like '%" + multi.RealEscapeString(pQueryEntity.sShopName) + "%' and";
    }
    if( ! pQueryEntity.sShopType.empty()){
        sCond += " sShopType= '" + multi.RealEscapeString(pQueryEntity.sShopType) + "' and";
    }
    if(  pQueryEntity.iVerify >= 0){
       sCond += " iVerify= '" + tConvert.IntToStr( pQueryEntity.iVerify ) + "' and";
    }
    if(  pQueryEntity.iAuth >= 0){
       sCond += " iAuth= '" + tConvert.IntToStr( pQueryEntity.iAuth ) + "' and";
    }
    if( ! pQueryEntity.sShopPhone.empty()){
        sCond += " sShopPhone= '" +  multi.RealEscapeString(pQueryEntity.sShopPhone) + "' and";
    }
    if(  pQueryEntity.lPerAgentId > 0 ){
        sCond += " lPerAgentId= '" + tConvert.LongToStr( pQueryEntity.lPerAgentId )+ "' and";
    }
    if(  pQueryEntity.iApplyDiscount >= 0){
        sCond +=  " left(sRemark,2)>'00' and";
    }
    if(  pQueryEntity.iShowNearby >= 0){
        sCond +=  " iShowNearby='" +  tConvert.IntToStr( pQueryEntity.iShowNearby ) + "' and";
    }

    if(  pQueryEntity.lStartTime >= 0){
        sCond +=  " lAddTime>'" +  tConvert.LongToStr( pQueryEntity.lStartTime ) + "' and";
    }

    if(  pQueryEntity.lEndTime >= 0){
        sCond +=  " lAddTime<'" +  tConvert.LongToStr( pQueryEntity.lEndTime ) + "' and";
    }
    if(  pQueryEntity.lUserId >= 0){
        sCond +=  " lUserId like '%" +  tConvert.LongToStr( pQueryEntity.lUserId ) + "%' and";
    }

    if(  pQueryEntity.iEnumDiscount > 0){
        int iDisCountStart = 0,iDisCountEnd = 0;
        if( pQueryEntity.iEnumDiscount == 1){
            iDisCountStart = 100 ;
            iDisCountEnd =  90;
        }else if( pQueryEntity.iEnumDiscount == 2){
            iDisCountStart = 90;
            iDisCountEnd =  80;
        }else if( pQueryEntity.iEnumDiscount == 3){
            iDisCountStart = 80;
            iDisCountEnd =  70;
        }else{
            iDisCountStart = 70;
            iDisCountEnd = 0;
        }
        sCond +=  " iOffLineDisCount>=" +  tConvert.LongToStr( iDisCountEnd ) + " and iOffLineDisCount<"  + tConvert.LongToStr( iDisCountStart ) + " and";
    }

    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;

    if( pQueryEntity.bStar){
        sSql += " order by iScore desc ";
    }else  if (pQueryEntity.iEnumDiscount == 0){
        sSql += " order by iOffLineDisCount asc";

    }else if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        sSql += " order by distance";
    }else{
        sSql += " order by  lAddTime desc";
    }
    sSql+=" limit "+tConvert.IntToStr(piIndex)+","+tConvert.IntToStr(piCount);
   // appendlog(TTDLogger::LOG_DEBUG, "xxxx:%s",sSql.c_str());

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            string sRemark = row[iIndex++];
            entity.contentEntity.sOtherCerPhoto = row[iIndex++];
            entity.contentEntity.sTradeMark = row[iIndex++];
            entity.contentEntity.iCoinType =  atoi(row[iIndex++]);
            entity.contentEntity.iCoinPercent =  atoi(row[iIndex++]);
            entity.contentEntity.iPromotionDiscount =  atoi(row[iIndex++]);
            if( pQueryEntity.iApplyDiscount >=0){
                //查询申请折扣变更的商家列表时，折扣信息不是当前折扣
                if( sRemark.size() >= 2){
                    entity.contentEntity.iOffLineDisCount =  atoi(sRemark.substr(0,2).c_str());
                }
            }
            if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
                entity.contentEntity.lDistance = atol( row[iIndex++] );
            }

            plstOut.push_back(entity);

        }
        sSql = "select count(*)  from " + gShopTb ;
        sSql  +=  sCond;

        GetQueryTotalCount(sSql,piTotalCount);
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


//查询数据
int ShopInfoDAL::GetMyShop( const long & plUserId , int iType,ShopInfoList & plstOut)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;


    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sOtherCerPhoto,iCoinType"
                  " from "+gShopTb + "  where  iState!=333 and iVerify=1 and lUserId=" + tConvert.LongToStr(plUserId);
    if( iType == 1){
      sSql += " and sCity!='美支付供应商'";
    }

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            entity.contentEntity.sOtherCerPhoto =   (row[iIndex++]);
            entity.contentEntity.iCoinType =  atoi(row[iIndex++]);

            plstOut.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}



int ShopInfoDAL::SearchShowPro()
{

}

//获取开的店
int ShopInfoDAL::Shops( const long & plUserId ,int iType, ShopInfoList & plstOut)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    string sType;
    if( iType == 1){
        sType = "lUserId=";
    }else{
        sType = "lPerAgentId=";
    }

    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sOtherCerPhoto"
                  " from "+gShopTb + "  where  iState!=333  and " + sType + tConvert.LongToStr(plUserId);

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            entity.contentEntity.sOtherCerPhoto =   (row[iIndex++]);

            plstOut.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}



//获取个代开的供应商
int ShopInfoDAL::OpenByPerAgent( const long & plUserId , ShopInfoList & plstOut)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    string sType;

    sType = "lPerAgentId=";


    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sOtherCerPhoto"
                  " from "+gShopTb + "  where  iState!=333 and sCity='美支付供应商' and lPerAgentId="  + tConvert.LongToStr(plUserId);

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            entity.contentEntity.sOtherCerPhoto =   (row[iIndex++]);

            plstOut.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//根据平台号或帐号或用户ID获取商家信息
int ShopInfoDAL::GetEntity(const string & pCond,int piType,ShopInfoEntity  & entity)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sRemark,sOtherCerPhoto,sTradeMark,iCoinType,iCoinPercent,iPromotionDiscount "
                  " from "+gShopTb ;

    if( piType == 1){
        sSql += " where iState!=333 and  sPrimaryKey='"+pCond+"'";
    }else if( piType == 2){
        sSql += " where iState!=333 and sAccount='"+pCond+"'";
    }else if (piType == 4){
        sSql += " where iState!=333 and sPosNum like '%"+pCond+"%'";
    }else if( piType == 5){
        sSql += " where iState!=333 and sShopName='"+pCond+"'";
    }

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            iRet = 0;
            //加载主键表
            int iIndex = 0;
            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId = tConvert.StrToLong(row[iIndex++]);
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            entity.contentEntity.sRemark =  row[iIndex++];
            entity.contentEntity.sOtherCerPhoto =   (row[iIndex++]);
            entity.contentEntity.sTradeMark =   (row[iIndex++]);
            entity.contentEntity.iCoinType =  atoi(row[iIndex++]);
            entity.contentEntity.iCoinPercent =  atoi(row[iIndex++]);
            entity.contentEntity.iPromotionDiscount = atoi(row[iIndex++]);
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}



int ShopInfoDAL::GetPlatformIdByShopName(string sShopName, long & lPlatformId)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL mult;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey"
                  " from "+gShopTb  + " where sShopName='" + mult.RealEscapeString( sShopName ) + "'";

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    //appendlog(TTDLogger::LOG_DEBUG,"xxxxxx:%s",sSql.c_str());


    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            iRet = 0;
            //加载主键表
            int iIndex = 0;
            lPlatformId = atol (row[iIndex++]);
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}


int ShopInfoDAL::CityShopCount(long lEndTime,CommonStructVector & out)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result = NULL;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sCity,count(*)  from shopdal where lAddTime<=" + tConvert.LongToStr(lEndTime) + " group by sCity";

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            CommonStruct entity;

            entity.v1 =  row[iIndex++];
            entity.v2 =  row[iIndex++];

            out.push_back(entity);

        }

    }

    if( result != NULL)
        con->FreeResult( result );

    return iRet;

}


//更新密码
int ShopInfoDAL::ModifyPwd(const string & psPrimaryKey,const string & psNewPwd,const long & plUpdateId){

    TConvert tConvert;

    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId='" + tConvert.LongToStr(plUpdateId) + "'"+
            ",lUpTime='" + tConvert.LongToStr(plUpdateId) + "'" +
            ",sPassWord='" + psNewPwd + "'";
    sSql += " where sPrimaryKey ='"+psPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

int ShopInfoDAL::UpRemark( const long plPlatformId,const string & psRemark,const long & plUpdateId){

    TConvert tConvert;
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId='" + tConvert.LongToStr(plUpdateId) + "'"+
            ",lUpTime='" + tConvert.LongToStr(plUpdateId) + "'" +
            ",sRemark='" + psRemark + "'";
    sSql += " where sPrimaryKey ='"+tConvert.LongToStr( plPlatformId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}
//更新折扣
int ShopInfoDAL::UpDiscount( const long plPlatformId,const int & piDiscount, const string & psRemark,const long & plUpdateId){

    TConvert tConvert;
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId='" + tConvert.LongToStr(plUpdateId) + "'"+
            ",lUpTime='" + tConvert.LongToStr(plUpdateId) + "'" +
            ",iOffLineDisCount='" + tConvert.IntToStr( piDiscount ) + "'" +
            ",sRemark='" + psRemark + "'";
    sSql += " where sPrimaryKey ='"+tConvert.LongToStr( plPlatformId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//更新商家平均分
int ShopInfoDAL::UpAvgScore(long lPlatformId,int avg)
{
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId='" + tConvert.LongToStr(lCurTime) + "'"+
            ",lUpTime='" + tConvert.LongToStr(lCurTime) + "'" +
            ",iScore='" + tConvert.IntToStr( avg ) + "'" ;
    sSql += " where sPrimaryKey ='"+tConvert.LongToStr( lPlatformId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//获取最大平台号
int ShopInfoDAL::GetMaxPlatformId(long & plPlatformId)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey from " + gShopTb  + " order by cast(sPrimaryKey as unsigned) desc limit 0,1" ;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            plPlatformId = atol( row[0]);
            iRet = 0;
        }else{
            plPlatformId = 100001;
            iRet = 0;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int ShopInfoDAL::GetState(long lPlatfromId, int & iState)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select iState from " + gShopTb  + " where sPrimaryKey=" + tConvert.LongToStr(lPlatfromId) ;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            iState = atoi( row[0]);
            iRet = 0;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int ShopInfoDAL::GetCashBackShops(int iPage, int iCount, CashBackShopLst & lst)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    if(iPage <=0 )
        iPage = 1;
    if( iCount <= 0)
        iCount =20;
    int iIndex = (iPage-1 ) * iCount;
    string sSql = "select a.sPrimaryKey,a.sShopPhone,b.lStartTime from shopdal as a ";
    sSql += "inner join shopsetting as b on a.sPrimaryKey=b.lPlatformId and a.iVerify=1 and b.iCashBack=1 ";
    sSql += "limit " +  tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;
            CashBackShop cashBackShop;
            cashBackShop.lPlatformId = atol(row[iIndex++]);
            cashBackShop.sShopPhone = row[iIndex++];
            cashBackShop.lStartTime = atol(row[iIndex++]);


            lst.push_back(cashBackShop);

        }

    }

    // 释放内存
    con->FreeResult( result );
    return iRet;
}

int ShopInfoDAL::GetEmptyGpsZone(ShopInfoList & lst)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,dGpsLat,dGpsLon from " + gShopTb  + " where sGpsZone='' and dGpsLat>0.0 and dGpsLon>0.0 limit 100 " ;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        while ( NULL != (row = mysql_fetch_row( result )) ) {
            ShopInfoEntity entity;
            int iIndex  =0;
            entity.sPrimaryKey = row[iIndex++];
            entity.contentEntity.dGpsLat = atof( row[iIndex++] );
            entity.contentEntity.dGpsLon = atof(  row[iIndex++] );
            lst.push_back(entity);

        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;


}

//推荐的商家优先显示，接下来是符合所在地址并按距离排序显示
int ShopInfoDAL::RecommendShops(const ShopQueryEntity & pQueryEntity,int iPage, int iCount, ShopInfoList & lst,int & iTotalCount)
{
    TMultiMysqlDAL multi;
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    if(iPage <=0 )
        iPage = 1;
    if( iCount <= 0)
        iCount =20;
    int iIndex = (iPage-1 ) * iCount;




    string sSql = "select a.sPrimaryKey,a.sShopPhone,a.sShopName,a.sBusiness,a.sShopType,"
                  "a.iOffLineDisCount,a.sShopFrontPhoto,a.sShopDetailPhoto,a.sAddress,a.sCity,a.lUserId,a.sAccount"
                  ",a.sPassWord,a.sCompanyName,a.sIdPhoto,a.sHours,a.lPerAgentId,a.sSevvicePhone,a.sPosNum,a.iAvgConsume"
                  ",a.iShowNearby,a.iVerify,a.iAuth,a.iProrate,a.sRemark,a.sOtherCerPhoto,a.lAddTime,a.lServiceUserId"
                  ",a.dGpsLat,a.dGpsLon,a.sLicensePhoto,a.iAwardingPoints,a.iCoinType ,a.iCoinPercent,a.iPromotionDiscount ";

    if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        char buf[256] = {0};
        //根据经纬度计算距离
        sprintf(buf,",( 6378140 * acos( cos( radians(%f) ) * cos( radians( a.dGpsLat ) ) "
                    "* cos( radians( a.dGpsLon ) - radians(%f) ) + sin( radians(%f) ) * sin( radians( a.dGpsLat ) ) ) ) "
                    "as distance  ",pQueryEntity.dGpsLat,pQueryEntity.dGpsLon,pQueryEntity.dGpsLat);
        sSql += buf;
    }

    sSql += " from shopdal as a ";
    sSql += "left join shopsetting as b ";
    string sCond = "on a.sPrimaryKey=b.lPlatformId  and a.iVerify=1 ";


    sCond += " where a.iState=0 and";
    if( ! pQueryEntity.sCtiy.empty()){
        if( pQueryEntity.sCtiy == "citys"){
            sCond += " sCity!='美支付供应商' and";
        }else{
            if( pQueryEntity.bCityZone){
                sCond += " sCity like '%" + multi.RealEscapeString(pQueryEntity.sCtiy) + "%' and";
            }else{

                sCond += " ((sCity!='美支付供应商' and sAddress like '%" + multi.RealEscapeString(pQueryEntity.sTown) + "%" +  multi.RealEscapeString(pQueryEntity.sArea) +  "%') or";
                sCond += " (sCity!='美支付供应商' and sGpsZone like '%" + multi.RealEscapeString(pQueryEntity.sTown) + "%" +  multi.RealEscapeString(pQueryEntity.sArea) +  "%')) and";
            }
        }
    }
    if( ! pQueryEntity.sShopName.empty()){
        sCond += " sShopName like '%" + multi.RealEscapeString(pQueryEntity.sShopName) + "%' and";
    }
    if( ! pQueryEntity.sShopType.empty()){
        sCond += " sShopType= '" + multi.RealEscapeString(pQueryEntity.sShopType) + "' and";
    }

    if(  pQueryEntity.iVerify >= 0){
       sCond += " iVerify= '" + tConvert.IntToStr( pQueryEntity.iVerify ) + "' and";
    }

    if(  pQueryEntity.iShowNearby >= 0){
        sCond +=  " iShowNearby='" +  tConvert.IntToStr( pQueryEntity.iShowNearby ) + "' and";
    }
    if(  pQueryEntity.iAuth >= 0){
       sCond += " iAuth= '" + tConvert.IntToStr( pQueryEntity.iAuth ) + "' and";
    }



    if(  pQueryEntity.iEnumDiscount > 0){
        int iDisCountStart = 0,iDisCountEnd = 0;
        if( pQueryEntity.iEnumDiscount == 4){
            iDisCountStart = 0;iDisCountEnd = 50;
        }else if( pQueryEntity.iEnumDiscount == 3){
            iDisCountStart = 51;iDisCountEnd = 70;
        }else if( pQueryEntity.iEnumDiscount == 2){
            iDisCountStart = 71;iDisCountEnd = 90;
        }else{
            iDisCountStart = 91;iDisCountEnd = 100;
        }
        sCond +=  " iOffLineDisCount>=" +  tConvert.LongToStr( iDisCountStart ) + " and iOffLineDisCount<"  + tConvert.LongToStr( iDisCountEnd ) + " and";
    }

    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;

    sSql += " order by b.iRecommend desc ";

    if( pQueryEntity.bStar){
        sSql += ",iScore desc ";
    }else  if (pQueryEntity.iEnumDiscount == 0){
        sSql += ",iOffLineDisCount asc";
    }

    if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
        sSql += " ,distance";
    }

    sSql += " limit " +  tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }


    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;
            ShopInfoEntity entity;
            entity.sPrimaryKey = row[iIndex++];
            entity.contentEntity.sShopPhone = row[iIndex++];
            entity.contentEntity.sShopName = row[iIndex++];
            entity.contentEntity.sBusiness = row[iIndex++];
            entity.contentEntity.sShopType = row[iIndex++];
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto = row[iIndex++];
            if( entity.contentEntity.sShopFrontPhoto.empty() || entity.contentEntity.sShopFrontPhoto == "http://tymplusback.oss-cn-shenzhen.aliyuncs.com/636090371131543966Length81501.jpg"){
                entity.contentEntity.sShopFrontPhoto = "http://testuserhead.oss-cn-shenzhen.aliyuncs.com/dianpu_icon%403x.png";
            }
            entity.contentEntity.sShopDetailPhoto = row[iIndex++];
            entity.contentEntity.sAddress = row[iIndex++];
            entity.contentEntity.sCtiy = row[iIndex++];
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sCompanyName = row[iIndex++];
            entity.contentEntity.sIdPhoto = row[iIndex++];
            entity.contentEntity.sHours = row[iIndex++];
            entity.contentEntity.lPerAgentId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone = row[iIndex++];
            entity.contentEntity.sPosNum = row[iIndex++];
            entity.contentEntity.iAvgConsume = atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby = atoi(row[iIndex++]);
            entity.contentEntity.iVerify = atoi(row[iIndex++]);
            entity.contentEntity.iAuth = atoi(row[iIndex++]);
            entity.contentEntity.iProrate = atoi(row[iIndex++]);
            entity.contentEntity.sRemark = row[iIndex++];
            entity.contentEntity.sOtherCerPhoto = row[iIndex++];
            entity.lAddTime = atol(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sLicensePhoto = row[iIndex++];
            entity.contentEntity.iAwardingPoints = atoi(row[iIndex++]);
            entity.contentEntity.iCoinType = atoi(row[iIndex++]);
            entity.contentEntity.iCoinPercent = atoi(row[iIndex++]);
            entity.contentEntity.iPromotionDiscount = atoi(row[iIndex++]);
            if( pQueryEntity.dGpsLat > 0 && pQueryEntity.dGpsLon > 0){
                entity.contentEntity.lDistance = atol( row[iIndex++] );
            }


            lst.push_back(entity);

        }

        sSql = "select count(*)";
        sSql += " from shopdal as a ";
        sSql += "left join shopsetting as b ";
        sSql  +=  sCond;
        appendlog(TTDLogger::LOG_DEBUG,"dddddd:%s",sSql.c_str());

        GetQueryTotalCount(sSql,iTotalCount);

    }

    // 释放内存
    con->FreeResult( result );
    return iRet;
}

//获取一个地区下的最大帐号
int ShopInfoDAL::GetMaxCityAccount( const string &  psCity, string & psAccount)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sAccount"
                  " from "+gShopTb  + " where binary sAccount like '" + psCity + "%' order by lAddTime desc limit 0,1" ;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            psAccount = row[0];
            iRet = 0;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}
//更换手机号
int ShopInfoDAL::UpdatePhone(long lPlatfromId, const string & psNewPhone, long lNewUserId)
{
    TConvert tConvert;
    long lCurT = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurT) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurT) + "' ";
    sSql += ",sShopPhone='" +  psNewPhone + "'" ;
    sSql += ",lUserId=" + tConvert.LongToStr(lNewUserId);

    sSql += " where sPrimaryKey = '"+tConvert.LongToStr(lPlatfromId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

}


int ShopInfoDAL::UpdateGpsZone(const string & sPlatfromId, const string & sZone)
{
    TConvert tConvert;
    long lCurT = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+gShopTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurT) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurT) + "' ";
    sSql += ",sGpsZone='" +  sZone + "'" ;

    sSql += " where sPrimaryKey = '"+ BaseDal::mMulti.RealEscapeString(sPlatfromId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

}
