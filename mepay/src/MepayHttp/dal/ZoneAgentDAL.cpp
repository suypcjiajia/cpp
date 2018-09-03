#include "ZoneAgentDAL.h"
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"

ZoneAgentDAL* ZoneAgentDAL::mInstance = NULL;
ZoneAgentDAL* ZoneAgentDAL::GetInstance()
{

    if (mInstance == NULL){
        mInstance = new ZoneAgentDAL;
    }
    return mInstance;
}

//创建表
int ZoneAgentDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+gZoneAgentTb+" ("+
            "sPrimaryKey VARCHAR(50) NOT NULL,"+
            "lUserId BIGINT  NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT default 0,"+
            "lUpdateId BIGINT default 0,"+
            "iSync INT default 0,"+
            "sAccount VARCHAR(50) UNIQUE NOT NULL,"+
            "sPhone VARCHAR(24) default '' ,"+
            "sPassWord VARCHAR(50) NOT NULL,"+
            "sName VARCHAR(50) default '',"+
            "sUnit VARCHAR(50) default '',"+
            "sID VARCHAR(50) NOT NULL,"+
            "sCity VARCHAR(2000) NOT NULL,"+
            "sRemark VARCHAR(200)  default '',"+
            "PRIMARY KEY(sPrimaryKey)"+
            ") ENGINE=INNODB  DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}
//添加
int ZoneAgentDAL::Insert( const ZoneAgentEntity& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql   = "insert into " +  gZoneAgentTb+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
            "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark)" +
            "VALUES( '"+entity.sPrimaryKey+"','" +
            tConvert.LongToStr(entity.lUserId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lUpdateId)+"','" +
            "0','" +
            entity.contentEntity.sAccount+"','" +
            entity.contentEntity.sPhone+"','" +
            entity.contentEntity.sPassWord+"','" +
            entity.contentEntity.sName+"','"+
            entity.contentEntity.sUnit+"','"+
            entity.contentEntity.sID+"','"+
            entity.contentEntity.sCity+"','"+
            entity.contentEntity.sRemark+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}
//删除
int ZoneAgentDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+gZoneAgentTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lUptTime) + "' "+
            ",iState = '333' "+
            ",iSync = '0' " ;
    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

    return 0;
}
//修改
int ZoneAgentDAL::Update(  const string& sPrimaryKey,const ZoneAgentEntity& entity )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "update "+gZoneAgentTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' "+
            ",iSync ='0'" ;
    if ( entity.contentEntity.sAccount != "-1nochange" ) {
        sSql += ",sAccount = '" + multi.RealEscapeString(entity.contentEntity.sAccount) + "' ";
    }
    if ( entity.contentEntity.sCity != "-1nochange" ) {
        sSql += ",sCity = '" + multi.RealEscapeString(entity.contentEntity.sCity) + "' ";
    }
    if ( entity.contentEntity.sID  != "-1nochange" ) {
        sSql += ",sID = '" + multi.RealEscapeString(entity.contentEntity.sID) + "' ";
    }
    if ( entity.contentEntity.sName  != "-1nochange" ) {
        sSql += ",sName = '" + multi.RealEscapeString(entity.contentEntity.sName) + "' ";
    }
    if ( entity.contentEntity.sPassWord != "-1nochange") {
        sSql += ",sPassWord = '" + multi.RealEscapeString(entity.contentEntity.sPassWord) + "' ";
    }
    if ( entity.contentEntity.sPhone != "-1nochange" ) {
        sSql += ",sPhone = '" + multi.RealEscapeString(entity.contentEntity.sPhone) + "' ";
    }
    if ( entity.contentEntity.sRemark != "-1nochange" ) {
        sSql += ",sRemark = '" + multi.RealEscapeString(entity.contentEntity.sRemark) + "' ";
    }
    if ( entity.contentEntity.sUnit != "-1nochange" ) {
        sSql += ",sUnit = '" + multi.RealEscapeString(entity.contentEntity.sUnit) + "' ";
    }

    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

//增加,批量
int ZoneAgentDAL::InsertBatch( const ZoneAgentList & pLst, const string& psSync ){
    // ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "replace into "+gZoneAgentTb+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
            "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark)"
            "VALUES";
    ZoneAgentList::const_iterator it = pLst.begin();
    for ( ; it != pLst.end() ; it++){
        const ZoneAgentEntity & entity = *it;
        sSql +="('" +entity.sPrimaryKey+"','" +
                tConvert.LongToStr(entity.lUserId)+"','" +
                tConvert.LongToStr(entity.lAddTime)+"','" +
                tConvert.LongToStr(entity.lUpTime)+"','" +
                tConvert.IntToStr(entity.iState)+"','" +
                tConvert.LongToStr(entity.lUpdateId)+"','" +
                psSync+"','" +
                entity.contentEntity.sAccount+"','" +
                entity.contentEntity.sPhone+"','" +
                entity.contentEntity.sPassWord+"','" +
                entity.contentEntity.sName+"','" +
                entity.contentEntity.sUnit+"','" +
                entity.contentEntity.sID+"','" +
                multi.RealEscapeString(entity.contentEntity.sCity)+"','" +
                multi.RealEscapeString(entity.contentEntity.sRemark)+
                "')";
        sSql += ",";
    }
    sSql = sSql.substr(0,sSql.length() -1);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

}

int ZoneAgentDAL::ModifyPwd(const string & psPrimaryKey,const string & psNewPwd,const long & plUpdateId){
    // ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+gZoneAgentTb+" set "+
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

//根据城市获取代理商
int ZoneAgentDAL::GetListByCity(const string & psCity,ZoneAgentList & pOutLst)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark"
                  " from "+gZoneAgentTb;
    sSql+=" where sCity like '%"+psCity+"%'";
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;

        while( NULL != (row =mysql_fetch_row( result )) ) {
            iRet = 0;
            //加载主键表
            ZoneAgentEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            iIndex++;
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPhone = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sName =row[iIndex++];
            entity.contentEntity.sUnit =  row[iIndex++] ;
            entity.contentEntity.sID =  row[iIndex++] ;
            entity.contentEntity.sCity = row[iIndex++] ;
            entity.contentEntity.sRemark =  row[iIndex++] ;
            pOutLst.push_back( entity );
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

int ZoneAgentDAL::GetEntityByKey(const string & psKey,int iType,ZoneAgentEntity  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark"
                  " from "+gZoneAgentTb;
    if( 1 == iType ){
        sSql+=" where iState!=333 and sPrimaryKey= '"+psKey+"'";
    }else if( 2 == iType){
        sSql+=" where iState!=333 and lUserId= '"+psKey+"'";
    }else if( 3 == iType){
        sSql+=" where iState!=333 and sPhone= '"+psKey+"'";
    }else if(  4 == iType){
        sSql+=" where iState!=333 and  sID='市代理' and sPhone!='11111111115' and sCity= '"+psKey+"'";
    }else if(  5 == iType){
        sSql+=" where iState!=333 and  sID='省代理' and sPhone!='11111111115' and sCity like '%"+psKey+"%'";
    }

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
            entity.sPrimaryKey = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            iIndex++;
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPhone = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sName =row[iIndex++];
            entity.contentEntity.sUnit =  row[iIndex++] ;
            entity.contentEntity.sID =  row[iIndex++] ;
            entity.contentEntity.sCity = row[iIndex++] ;
            entity.contentEntity.sRemark =  row[iIndex++] ;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

int ZoneAgentDAL::LikeKey(const string & psKey,ZoneAgentEntity  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark"
                  " from "+gZoneAgentTb;

    sSql+=" where iState!=333 and sPhone !='11111111115' and sCity like '%"+psKey+"'";


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
            entity.sPrimaryKey = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            iIndex++;
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPhone = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sName =row[iIndex++];
            entity.contentEntity.sUnit =  row[iIndex++] ;
            entity.contentEntity.sID =  row[iIndex++] ;
            entity.contentEntity.sCity = row[iIndex++] ;
            entity.contentEntity.sRemark =  row[iIndex++] ;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}


int ZoneAgentDAL::GetList(const ZoneAgentQueryEntity & pQueryEntity,int piIndex ,int piCount,ZoneAgentList & pOutLst, int & piTotalCount)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark"
                  " from "+gZoneAgentTb ;

    string sCond = " where iState!=333 and";
    if( !pQueryEntity.sAccount.empty()){
        sCond += " sAccount like '%" + pQueryEntity.sAccount + "%' and";
    }
    if( !pQueryEntity.sCity.empty()){
        sCond += " sCity like '%" + pQueryEntity.sCity + "%' and";
    }
    if( !pQueryEntity.sID.empty()){
        sCond += " sID like '%" + pQueryEntity.sID + "%' and";
    }
    if( !pQueryEntity.sName.empty()){
        sCond += " sName like '%" + pQueryEntity.sName + "%' and";
    }
    if( !pQueryEntity.sPhone.empty()){
        sCond += " sPhone like '%" + pQueryEntity.sPhone + "%' and";
    }
    if( !pQueryEntity.sRemark.empty()){
        sCond += " sRemark like '%" + pQueryEntity.sRemark + "%' and";
    }
    if( !pQueryEntity.sUnit.empty()){
        sCond += " sUnit like '%" + pQueryEntity.sUnit + "%' and";
    }
    if( pQueryEntity.lUserId > 0){
        sCond += " lUserId='" + tConvert.LongToStr( pQueryEntity.lUserId )+ "' and";
    }
    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;
    sSql += " order by lAddTime desc ";
    sSql += " limit " + tConvert.IntToStr(piIndex) + "," + tConvert.IntToStr(piCount);
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            //加载主键表
            ZoneAgentEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            iIndex++;
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPhone = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sName =row[iIndex++];
            entity.contentEntity.sUnit =  row[iIndex++] ;
            entity.contentEntity.sID =  row[iIndex++] ;
            entity.contentEntity.sCity = row[iIndex++] ;
            entity.contentEntity.sRemark =  row[iIndex++] ;
            pOutLst.push_back( entity );
        }
        sSql = "select count(*)  from " + gZoneAgentTb ;

       sSql  +=  sCond;

        GetQueryTotalCount(sSql,piTotalCount);
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}


int ZoneAgentDAL::GetZoneByPhone(string sPhone,ZoneAgentList & pOutLst)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPhone,sPassWord,sName,sUnit,sID,sCity,sRemark"
                  " from "+gZoneAgentTb ;

    string sCond = " where iState!=333 and sID='市代理' and sPhone!='11111111115' and sPhone=" + sPhone;

    sSql += sCond;

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {


            ZoneAgentEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            iIndex++;
            entity.contentEntity.sAccount = row[iIndex++];
            entity.contentEntity.sPhone = row[iIndex++];
            entity.contentEntity.sPassWord = row[iIndex++];
            entity.contentEntity.sName =row[iIndex++];
            entity.contentEntity.sUnit =  row[iIndex++] ;
            entity.contentEntity.sID =  row[iIndex++] ;
            entity.contentEntity.sCity = row[iIndex++] ;
            entity.contentEntity.sRemark =  row[iIndex++] ;
            pOutLst.push_back( entity );
        }

    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//获取区代理商用户ID
int ZoneAgentDAL::GetZoneAgentIdByCity(const string & psCity,long & plAgentId)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select lUserId"
                  " from "+gZoneAgentTb;
    sSql+=" where sCity='"+psCity+"' and sID='" + AGENTCITY + "'";
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
            plAgentId = atol( row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

//更换手机号
int ZoneAgentDAL::UpdatePhone(const string & psPhone, const string & psNewPhone)
{
    TConvert tConvert;
    long lCurT = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+gZoneAgentTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurT) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurT) + "' ";
    sSql += ",sPhone='" +  psNewPhone + "'" ;

    sSql += "where sPhone = '"+psPhone+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

}

int ZoneAgentDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(psQuery);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        piCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            piCount = atoi(row[0]);
        }
    }
    return iRet;
}
